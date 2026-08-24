"""stone_node — 스톤 픽업·정위치 압착 (NIS §6.7 M05).

**담당자 메모(NIS): "일정 지연 시 1순위 축소 대상"** — 실제로 v0.3 에서
한 번 축소됐다. 이 노드가 쓰던 `/skill/probe_point`(ProbePoint)가 폐지되면서
(scan_node/inspection_node 제거와 함께) 힘 제어 하강과 부착 위치 검증이
같이 사라졌다.

## 지금의 압착 방식 — 티칭된 높이로 위치 제어 하강

예전에는 `ProbePoint` 로 힘 제어 하강해 "실제로 표면에 닿은 깊이"를 재고
그 깊이로 다시 내려가 압착을 유지했다. 힘 센서로 표면을 찾았기 때문에
손톱 높이를 몰라도 됐다. 지금은 그 반대다 — **손톱 표면 높이를 알고 있다는
전제**로 바꿨다. `nail_local_frame` 의 z=0 이 손톱 표면이라고 티칭돼 있으므로
(`nail_bringup/config/static_frames.yaml`), 목표 z 로 그냥 내려가면 된다.

  1. `MoveTo` — 목표 위 `approach_height_mm` 지점으로 이동 + yaw 정렬
  2. `MoveTo` — 목표 z + `press_offset_mm` 까지 저속 하강 (압착)
  3. `press_duration_s` 만큼 그 자리에서 대기
  4. `PickPlace(PLACE)` — 지금 자세를 TF 로 브로드캐스트하고 그 프레임을
     `target_key` 로 넘겨 "제자리에서 그리퍼 열고 후퇴"시킨다
     (`robot_skill_node._target_task_pose()` 의 TF 폴백을 이용)

⚠️ **이 방식은 힘으로 멈추지 않는다.** `nail_local_frame` 의 z 가 실제보다
낮게 티칭돼 있으면 그만큼 손톱을 눌러버린다. `press_offset_mm` 을 처음엔
넉넉히(양수, 즉 표면보다 위) 잡고 실기에서 눈으로 보며 줄일 것.
`press_force_n` 은 이제 아무 데도 쓰이지 않아 액션에서 제거됐다.

⚠️ **부착 성공 여부를 확인하지 않는다.** 검증은 목표점 주변을 얕게 눌러보는
ProbePoint 휴리스틱이었고 함께 사라졌다 — `verify_enabled` / `max_retry` /
`position_tolerance_mm` / `position_error_mm` / `E_STONE_MISS` 가 전부
제거됐다. 스톤이 제대로 붙었는지는 **사람이 눈으로 확인**해야 한다.
"""
import math
import threading
import time

from geometry_msgs.msg import Point, Pose, TransformStamped
import rclpy
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from tf2_ros import TransformBroadcaster

from nail_msgs.action import MoveTo, PickPlace, PlaceStone
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, ToolState
from nail_msgs.srv import ValidatePrecondition

# robot_skill_node 의 ABORT_* 표기(ABORT_LOW_STIFFNESS 등)와 맞춘다.
SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_PRECOND_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_GRIP_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_OVERFORCE: ErrorCode.SEV_ABORT,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
}

_HOLD_FRAME = 'stone_hold_frame'  # PickPlace(PLACE) TF 폴백용 임시 프레임 이름


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


def _abort_reason(code):
    return 'ABORT_' + (code[2:] if code.startswith('E_') else code)


def _grip_orientation(yaw_deg):
    """면-아래(curing_node `_FACE_DOWN_QUAT`, 로컬 X축 180°) 자세에 nail_local_frame
    Z축 기준 yaw 를 외재적으로 합성한 쿼터니언. (x,y,z,w) = (cos(θ/2), sin(θ/2), 0, 0)
    — 회전행렬로 직접 검산한 값이다(각주 참고). curing_node 와 동일하게, 이 축의
    라이브 자세를 되읽는 인터페이스가 없어 실기 정렬 결과와 다를 수 있다.
    """
    half = math.radians(yaw_deg) / 2.0
    return (math.cos(half), math.sin(half), 0.0, 0.0)


class StoneNode(Node):

    def __init__(self):
        super().__init__('stone_node')
        self._declare_parameters()

        self._latest_safety = None
        self._last_safety_rx_monotonic = None
        safety_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                                 durability=DurabilityPolicy.TRANSIENT_LOCAL)

        self._cb_action = MutuallyExclusiveCallbackGroup()
        self._cb_client = MutuallyExclusiveCallbackGroup()

        self.create_subscription(SafetyState, self.get_parameter('safety_topic').value,
                                  self._on_safety_status, safety_qos,
                                  callback_group=self._cb_client)

        self._tf_broadcaster = TransformBroadcaster(self)

        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._pick_place_client = ActionClient(self, PickPlace, '/skill/pick_place',
                                                callback_group=self._cb_client)
        self._move_client = ActionClient(self, MoveTo, '/skill/move_to',
                                          callback_group=self._cb_client)
        self._active_goal_handle = None

        self._stone_server = ActionServer(
            self, PlaceStone, '/process/place_stone',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('stone_node ready (NIS §6.7 M05)')

    # --- 파라미터 (NIS §6.7 표 + 구현상 필요한 추가값) ----------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 1.0)
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        d('press_duration_s', 2.0)
        d('stone_pickup_frame', 'stone_tray')
        d('approach_height_mm', 15.0)
        # NIS 표에 없는 구현 보조값 — 위 docstring 의 압착 설계에 필요하다.
        # 압착 목표 z = target_position.z + 이 값 (nail_local_frame, mm).
        # 힘으로 멈추지 않으므로 이 값이 곧 "얼마나 세게 누르는가"다. 양수면
        # 표면보다 위(덜 누름), 음수면 표면 아래(더 누름). 스톤 두께만큼
        # 띄우는 것이 출발점 — 실기에서 눈으로 보며 줄일 것.
        d('press_offset_mm', 1.0)
        d('stone_grip_width_mm', 3.0)          # PICK 목표 파지 폭 (작은 스톤)
        # 핀셋을 "툴로서" RG2 가 쥐고 있는 폭(tool_rack.yaml 의 tweezers.
        # expected_grip_width_mm 와 맞출 것). 스톤을 놓을 때 이 폭까지만
        # 벌린다 — 완전개방(gripper_open_width_mm)까지 벌리면 핀셋 손잡이
        # 자체를 놓쳐버린다.
        d('tweezers_grip_width_mm', 20.0)
        d('press_hold_speed_ratio', 0.1)       # 압착 하강 속도(0~1). 힘 감시가 없으니 느리게.
        d('approach_speed_ratio', 0.3)         # 접근 이동 속도(0~1) — PickPlace 내부값과 동일

    # --- 안전 -----------------------------------------------------------------
    def _on_safety_status(self, msg):
        self._latest_safety = msg
        self._last_safety_rx_monotonic = time.monotonic()

    def _safe_to_move(self):
        timeout_s = self.get_parameter('safety_status_timeout_s').value
        return (self._latest_safety is not None
                and self._latest_safety.safe_to_move
                and self._last_safety_rx_monotonic is not None
                and time.monotonic() - self._last_safety_rx_monotonic <= timeout_s)

    def _on_cancel(self, goal_handle):
        if self._active_goal_handle is not None:
            self._active_goal_handle.cancel_goal_async()
        return CancelResponse.ACCEPT

    def _call_validate_precondition(self, session_id, timeout_s=5.0):
        if not self._validate_client.wait_for_service(timeout_sec=timeout_s):
            return False, ['ValidatePrecondition 서비스 연결 실패']
        req = ValidatePrecondition.Request()
        req.stage = ValidatePrecondition.Request.STAGE_STONE
        req.session_id = session_id
        req.required_tool = ToolState.TWEEZERS
        future = self._validate_client.call_async(req)
        deadline = time.monotonic() + timeout_s
        while not future.done():
            if time.monotonic() > deadline:
                return False, ['ValidatePrecondition 응답 타임아웃']
            time.sleep(0.02)
        resp = future.result()
        if resp is None:
            return False, ['ValidatePrecondition 응답 없음']
        return resp.ok, list(resp.blocking_reasons)

    # --- goal 수락 (§3.1 ②③④) --------------------------------------------------
    def _on_goal(self, goal_request):
        if not goal_request.session_id:
            self.get_logger().warn('PlaceStone REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('PlaceStone REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'PlaceStone REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # --- PickPlace 클라이언트 헬퍼 ------------------------------------------------
    def _call_pick_place(self, mode, target_key, expected_width_mm, verify_grip,
                          approach_height_mm, timeout_s, our_goal_handle, feedback_cb=None,
                          already_holding=False):
        """반환: (PickPlace.Result|None, error_code|None|'CANCELLED')."""
        if not self._pick_place_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_COMM_LOST

        goal = PickPlace.Goal()
        goal.mode = mode
        goal.target_key = target_key
        goal.frame_id = 'nail_local_frame'
        goal.approach_height_mm = approach_height_mm
        goal.expected_width_mm = expected_width_mm
        goal.grip_width_mm = expected_width_mm
        goal.verify_grip = verify_grip
        goal.already_holding = already_holding

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        send_future = self._pick_place_client.send_goal_async(
            goal, feedback_callback=feedback_cb)
        send_future.add_done_callback(on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return None, ErrorCode.E_TIMEOUT

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED
        self._active_goal_handle = gh

        result_done = threading.Event()

        def on_result(fut):
            state['result'] = fut.result()
            result_done.set()

        gh.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        while not result_done.wait(timeout=0.1):
            if our_goal_handle.is_cancel_requested:
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, 'CANCELLED'
            if not self._safe_to_move():
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_SAFETY_BLOCKED
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_TIMEOUT
        self._active_goal_handle = None

        result = state['result'].result
        if not result.base.success:
            return result, result.base.error.code
        return result, None

    # --- MoveTo 클라이언트 헬퍼 (curing_node 와 동일 패턴) --------------------------
    def _call_move_to(self, pose, speed_ratio, timeout_s, our_goal_handle):
        """반환: (MoveTo.Result|None, error_code|None|'CANCELLED')."""
        goal = MoveTo.Goal()
        goal.target = pose
        goal.frame_id = 'nail_local_frame'
        goal.linear = True
        goal.speed_ratio = speed_ratio
        goal.accel_ratio = speed_ratio
        goal.timeout_s = timeout_s

        if not self._move_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_COMM_LOST

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        self._move_client.send_goal_async(goal).add_done_callback(on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return None, ErrorCode.E_TIMEOUT

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED
        self._active_goal_handle = gh

        result_done = threading.Event()

        def on_result(fut):
            state['result'] = fut.result()
            result_done.set()

        gh.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        while not result_done.wait(timeout=0.1):
            if our_goal_handle.is_cancel_requested:
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, 'CANCELLED'
            if not self._safe_to_move():
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_SAFETY_BLOCKED
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_TIMEOUT
        self._active_goal_handle = None

        result = state['result'].result
        if not result.base.success:
            return result, result.base.error.code
        return result, None

    def _broadcast_hold_frame(self, x_mm, y_mm, z_mm, quat):
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'nail_local_frame'
        t.child_frame_id = _HOLD_FRAME
        t.transform.translation.x = x_mm / 1000.0
        t.transform.translation.y = y_mm / 1000.0
        t.transform.translation.z = z_mm / 1000.0
        t.transform.rotation.x, t.transform.rotation.y, \
            t.transform.rotation.z, t.transform.rotation.w = quat
        self._tf_broadcaster.sendTransform(t)

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = PlaceStone.Result()

        press_duration = self._val(goal.press_duration_s, 'press_duration_s')
        approach_height = self._val(goal.approach_height_mm, 'approach_height_mm')
        pickup_key = self.get_parameter('stone_pickup_frame').value
        grip_width = self.get_parameter('stone_grip_width_mm').value
        press_offset = self.get_parameter('press_offset_mm').value
        hold_speed = self.get_parameter('press_hold_speed_ratio').value
        approach_speed = self.get_parameter('approach_speed_ratio').value

        # PlaceStone.action 에는 target_position 의 기준 프레임을 밝히는
        # frame_id 필드가 없다 — sanding/coating/curing 이 하위 스킬 호출에
        # 공통으로 쓰는 'nail_local_frame' 기준이라고 가정한다.
        tx_mm = goal.target_position.x * 1000.0
        ty_mm = goal.target_position.y * 1000.0
        tz_mm = goal.target_position.z * 1000.0
        quat = _grip_orientation(goal.target_yaw_deg)
        # 압착 높이. 힘으로 멈추지 않으므로 이 숫자가 곧 압착 깊이다
        # (모듈 docstring 경고 참고).
        hold_z_mm = tz_mm + press_offset

        def feedback(step, pct):
            fb = PlaceStone.Feedback()
            fb.step = step
            fb.percent = pct
            goal_handle.publish_feedback(fb)

        def pose_at(z_mm):
            pose = Pose()
            pose.position.x = tx_mm / 1000.0
            pose.position.y = ty_mm / 1000.0
            pose.position.z = z_mm / 1000.0
            pose.orientation.x, pose.orientation.y, \
                pose.orientation.z, pose.orientation.w = quat
            return pose

        timeout_s = self.get_parameter('node_timeout_s').value
        abort_code, abort_detail = None, ''

        # 재시도 루프가 없다 — 접촉/부착을 확인할 센서가 없어서 "실패했으니 다시"
        # 를 판정할 근거 자체가 없다. 한 번 시도하고 결과를 사람에게 넘긴다.
        while True:
            if goal_handle.is_cancel_requested:
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if not self._safe_to_move():
                abort_code, abort_detail = ErrorCode.E_SAFETY_BLOCKED, 'safe_to_move=false'
                break

            # --- 0단계: PICK — 트레이에서 스톤 집기 ---------------------------------
            feedback(0, 0.0)
            pick_result, err = self._call_pick_place(
                PickPlace.Goal.MODE_PICK, pickup_key, grip_width, True, approach_height,
                timeout_s, goal_handle, already_holding=True)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err is not None:
                detail = pick_result.base.error.detail if pick_result is not None else ''
                abort_code, abort_detail = ErrorCode.E_GRIP_FAILED, \
                    f'PICK 실패({pickup_key}): {err} {detail}'.strip()
                break

            # --- 1단계: 목표 위 접근 + yaw 정렬 -----------------------------------
            feedback(1, 25.0)
            mv_result, err = self._call_move_to(pose_at(tz_mm + approach_height),
                                                 approach_speed, timeout_s, goal_handle)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err is not None:
                detail = mv_result.base.error.detail if mv_result is not None else ''
                abort_code, abort_detail = err, f'접근 이동 실패: {detail or err}'
                break

            # --- 2단계: 압착 높이로 저속 하강 --------------------------------------
            feedback(2, 50.0)
            mv_result, err = self._call_move_to(pose_at(hold_z_mm), hold_speed, timeout_s,
                                                 goal_handle)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err is not None:
                detail = mv_result.base.error.detail if mv_result is not None else ''
                abort_code, abort_detail = err, f'압착 하강 실패(z={hold_z_mm:.2f}mm): {detail or err}'
                break

            # --- 3단계: press_duration_s 만큼 눌러 유지 -----------------------------
            dwell_deadline = time.monotonic() + press_duration
            while time.monotonic() < dwell_deadline:
                if goal_handle.is_cancel_requested:
                    abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                    break
                if not self._safe_to_move():
                    abort_code, abort_detail = ErrorCode.E_SAFETY_BLOCKED, 'safe_to_move=false'
                    break
                time.sleep(0.05)
            if abort_code is not None:
                break

            # --- 4단계: 그리퍼 열기 + 이탈 (PickPlace TF 폴백, docstring 참고) ------
            # already_holding=True: 완전개방이 아니라 tweezers_grip_width_mm
            # 까지만 벌린다 — 스톤만 놓고 핀셋 손잡이는 계속 쥔 채 유지.
            feedback(3, 75.0)
            self._broadcast_hold_frame(tx_mm, ty_mm, hold_z_mm, quat)
            release_width = self.get_parameter('tweezers_grip_width_mm').value
            place_result, err = self._call_pick_place(
                PickPlace.Goal.MODE_PLACE, _HOLD_FRAME, release_width, False, approach_height,
                timeout_s, goal_handle, already_holding=True)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err is not None:
                detail = place_result.base.error.detail if place_result is not None else ''
                abort_code, abort_detail = ErrorCode.E_GRIP_FAILED, \
                    f'PLACE 실패: {err} {detail}'.strip()
                break

            feedback(4, 100.0)
            break

        if abort_code == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, abort_detail,
                                              started_at)
            return result
        if abort_code is not None:
            self._log_abort(abort_code, abort_detail)
            goal_handle.abort()
            result.base = self._result_base(False, abort_code, abort_detail, started_at)
            result.abort_reason = _abort_reason(abort_code)
            return result

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        # 명령값 그대로 — 부착 위치를 되읽는 수단이 없다 (docstring 경고).
        result.actual_position = Point(x=tx_mm / 1000.0, y=ty_mm / 1000.0,
                                        z=hold_z_mm / 1000.0)
        return result

    # --- 공통 ------------------------------------------------------------------
    def _result_base(self, success, code, detail, started_at):
        base = ResultBase()
        base.success = success
        base.error.code = code
        base.error.severity = _severity_for(code)
        base.error.detail = detail
        base.duration_s = max(0.0, time.monotonic() - started_at)
        base.completed_at = self.get_clock().now().to_msg()
        return base

    def _log_abort(self, code, detail):
        self.get_logger().error(f'[{code}] stone_node: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = StoneNode()
    executor = MultiThreadedExecutor(num_threads=4)
    executor.add_node(node)
    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
