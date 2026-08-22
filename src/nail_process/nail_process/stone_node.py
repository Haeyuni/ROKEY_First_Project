"""stone_node — 스톤 픽업·정위치 압착·(옵션) 위치 검증 (NIS §6.7 M05).

**담당자 메모(NIS): "일정 지연 시 1순위 축소 대상"** — 이 노드가 다른
B계층 노드보다 구현 난이도가 높은 이유는, 소비 인터페이스가
`/skill/pick_place` · `/skill/probe_point` · `/skill/move_to` 세 개로
제한돼 있어(§6.7 인터페이스 표) `ContactPath`/`LateralContact` 같은
힘-경로 추종 스킬을 못 쓰기 때문이다. "압착력으로 압착 유지"를 이 세
primitive만으로 구현하려면 다음과 같은 조합이 필요하다:

1. `MoveTo` — 목표 위 접근 높이로 이동 + yaw 정렬 (자세 설정)
2. `ProbePoint(max_force_n=press_force_n)` — 힘 제어 하강으로 접촉·압착력
   도달을 확인한다. **문제**: `ProbePoint`는 끝나면 항상 후퇴한다
   (SDS §5.1) — "누른 채로 버티기"가 안 된다.
3. 그래서 `ProbePoint`가 돌려준 `point.contact_depth_mm`(접근 높이부터
   압착력 도달까지 실제 내려간 거리)으로 압착 깊이를 역산해, **같은
   깊이로 `MoveTo`를 한 번 더 내려보낸다.** 이번엔 위치제어지만 깊이 자체는
   힘제어로 검증된 값이라 안전하다. 여기서 `press_duration_s`만큼 그냥
   대기하면 실제로 눌려 있는 상태를 유지할 수 있다.
4. 그리퍼를 열려면(그냥 "지금 위치에서 열어라"가 없다) `PickPlace`를 다시
   써야 한다 — `robot_skill_node._target_task_pose()`가 `target_key`를
   targets.yaml에 없으면 **TF 프레임 이름으로 취급**하는 폴백을 이미 갖고
   있다(로봇스킬 노드 §5.1, 랙 슬롯용으로 만들어진 경로). 그래서 지금 누르고
   있는 자세를 TF로 한 번 브로드캐스트하고, 그 프레임을 `target_key`로
   `PickPlace(PLACE, approach_height_mm=approach_height_mm)`를 부르면
   "제자리에서 그리퍼 열고 후퇴"가 된다. `approach_height_mm`만큼 한 번
   들었다 다시 내려오는 왕복이 끼지만(같은 프레임이라 접근점=목표점),
   무해하고 위치도 그대로 보존된다.

이 설계는 §6.7 의 서술("압착력으로 압착 유지 → 그리퍼 Open")을 문서에
없는 새 스킬 없이 기존 세 인터페이스만으로 재현하려는 시도다. 스톤이
아주 작아 힘 제어 없는 순수 위치 하강은 손톱/스톤을 깰 위험이 있어
피했다.

**검증(`verify_enabled`, 기본 false)**: §6.7 스스로 "시간이 없으면 검증
없이 부착만 구현하고 `position_error=-1`을 반환하라"고 명시한다. `true`인
경우에도 이 구현은 정밀한 스톤 윤곽 검출이 아니라, 목표점 주변 원형으로
`verify_probe_count`점을 얕게 눌러 **압착 시 확인한 높이(hold_z) 근처에서
접촉하는 점만 "스톤 위"로 분류**하는 높이-대역 휴리스틱이다 —
scan_node 급의 군집화는 하지 않는다.
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
from nail_msgs.action import ProbePoint as ProbePointAction
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, StiffnessPoint, ToolState, \
    ValidationResult
from nail_msgs.srv import ValidatePrecondition

from nail_perception.geometry2d import centroid

# robot_skill_node 의 ABORT_* 표기(ABORT_LOW_STIFFNESS 등)와 맞춘다.
SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_PRECOND_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_GRIP_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_STONE_MISS: ErrorCode.SEV_ABORT,
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

        # IDS 부록: /validation/result 는 RELIABLE, depth 20 (inspection_node 와 공유)
        result_qos = QoSProfile(depth=20, reliability=ReliabilityPolicy.RELIABLE,
                                 durability=DurabilityPolicy.VOLATILE)
        self._result_pub = self.create_publisher(ValidationResult, '/validation/result',
                                                   result_qos)
        self._tf_broadcaster = TransformBroadcaster(self)

        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._pick_place_client = ActionClient(self, PickPlace, '/skill/pick_place',
                                                callback_group=self._cb_client)
        self._probe_client = ActionClient(self, ProbePointAction, '/skill/probe_point',
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
        d('safety_status_timeout_s', 0.2)
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        d('press_force_n', 1.5)
        d('press_duration_s', 2.0)
        d('position_tolerance_mm', 1.0)
        d('max_retry', 2)
        d('verify_enabled', False)
        d('verify_probe_count', 4)
        d('stone_pickup_frame', 'stone_tray')
        d('approach_height_mm', 15.0)
        # NIS 표에 없는 구현 보조값 — 위 docstring 의 압착/검증 설계에 필요하다.
        d('stone_grip_width_mm', 3.0)          # PICK 목표 파지 폭 (작은 스톤)
        d('press_search_margin_mm', 3.0)       # ProbePoint max_depth = approach_height + 이 값
        d('press_hold_speed_ratio', 0.1)       # 압착 깊이로 재하강할 때 속도(0~1)
        d('approach_speed_ratio', 0.3)         # 접근 이동 속도(0~1) — PickPlace 내부값과 동일
        d('verify_probe_radius_mm', 1.5)       # 검증 프로빙 원 반경
        d('verify_probe_max_force_n', 1.0)     # 검증 프로빙 힘 상한(스톤 건드리지 않게 약하게)
        d('verify_probe_depth_mm', 1.0)        # 검증 프로빙 최대 깊이
        d('stone_height_tolerance_mm', 0.3)    # 이 안에 있으면 "스톤 위" 로 분류

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
                          approach_height_mm, timeout_s, our_goal_handle, feedback_cb=None):
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

    # --- ProbePoint 클라이언트 헬퍼 (scan_node/inspection_node 와 동일 패턴) --------
    def _call_probe_point(self, x_mm, y_mm, z_mm, approach_height_mm, max_depth_mm, max_force_n,
                           timeout_s, our_goal_handle):
        """반환: (StiffnessPoint|None, error_code|None|'CANCELLED')."""
        if not self._probe_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_COMM_LOST

        goal = ProbePointAction.Goal()
        goal.target = Point(x=x_mm / 1000.0, y=y_mm / 1000.0, z=z_mm / 1000.0)
        goal.frame_id = 'nail_local_frame'
        goal.approach_height_mm = approach_height_mm
        goal.max_depth_mm = max_depth_mm
        goal.max_force_n = max_force_n
        goal.measure_release = False
        goal.source_tag = StiffnessPoint.SRC_VERIFY

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        self._probe_client.send_goal_async(goal).add_done_callback(on_goal_response)
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
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_TIMEOUT
        self._active_goal_handle = None

        result = state['result'].result
        if not result.base.success:
            return result.point, result.base.error.code
        return result.point, None

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

        press_force = self._val(goal.press_force_n, 'press_force_n')
        press_duration = self._val(goal.press_duration_s, 'press_duration_s')
        position_tolerance = self._val(goal.position_tolerance_mm, 'position_tolerance_mm')
        max_retry = int(self._val(goal.max_retry, 'max_retry'))
        # bool 필드는 "설정 안 함"과 False 를 구분 못 한다 — coating_node.use_compliance 와
        # 동일하게 goal 값을 그대로 신뢰한다.
        verify_enabled = goal.verify_enabled
        verify_probe_count = int(self._val(goal.verify_probe_count, 'verify_probe_count'))
        approach_height = self._val(goal.approach_height_mm, 'approach_height_mm')
        pickup_key = self.get_parameter('stone_pickup_frame').value
        grip_width = self.get_parameter('stone_grip_width_mm').value
        press_margin = self.get_parameter('press_search_margin_mm').value
        hold_speed = self.get_parameter('press_hold_speed_ratio').value
        approach_speed = self.get_parameter('approach_speed_ratio').value

        # PlaceStone.action 에는 target_position 의 기준 프레임을 밝히는
        # frame_id 필드가 없다 — sanding/coating/curing 이 하위 스킬 호출에
        # 공통으로 쓰는 'nail_local_frame' 기준이라고 가정한다.
        tx_mm = goal.target_position.x * 1000.0
        ty_mm = goal.target_position.y * 1000.0
        tz_mm = goal.target_position.z * 1000.0
        quat = _grip_orientation(goal.target_yaw_deg)

        def feedback(step, pct):
            fb = PlaceStone.Feedback()
            fb.step = step
            fb.percent = pct
            goal_handle.publish_feedback(fb)

        timeout_s = self.get_parameter('node_timeout_s').value
        retry_count = 0
        abort_code, abort_detail = None, ''
        actual_position = Point(x=0.0, y=0.0, z=0.0)
        position_error_mm = -1.0

        # 그리퍼가 지금 스톤을 물고 있는지 추적한다 — E_NO_CONTACT 재시도는
        # 아직 스톤을 놓지 않았으므로 PICK 을 다시 부르면 안 된다(이미 물고
        # 있는데 또 집으라는 명령이 되어 위험하다). 검증 실패로 인한 재배치는
        # 이미 PLACE 로 내려놓아 그리퍼가 비어 있으므로 새 스톤을 다시 집는다.
        stone_in_hand = False

        while True:
            if goal_handle.is_cancel_requested:
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if not self._safe_to_move():
                abort_code, abort_detail = ErrorCode.E_SAFETY_BLOCKED, 'safe_to_move=false'
                break

            # --- 0단계: PICK (아직 스톤을 안 물고 있을 때만) -----------------------
            if not stone_in_hand:
                feedback(0, 0.0)
                pick_result, err = self._call_pick_place(
                    PickPlace.Goal.MODE_PICK, pickup_key, grip_width, True, approach_height,
                    timeout_s, goal_handle)
                if err == 'CANCELLED':
                    abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                    break
                if err is not None:
                    detail = pick_result.base.error.detail if pick_result is not None else ''
                    abort_code, abort_detail = ErrorCode.E_GRIP_FAILED, \
                        f'PICK 실패({pickup_key}): {err} {detail}'.strip()
                    break
                stone_in_hand = True

            # --- 1단계: 목표 위 접근 + yaw 정렬 -----------------------------------
            feedback(1, 15.0)
            approach_pose = Pose()
            approach_pose.position.x = tx_mm / 1000.0
            approach_pose.position.y = ty_mm / 1000.0
            approach_pose.position.z = (tz_mm + approach_height) / 1000.0
            approach_pose.orientation.x, approach_pose.orientation.y, \
                approach_pose.orientation.z, approach_pose.orientation.w = quat
            mv_result, err = self._call_move_to(approach_pose, approach_speed, timeout_s,
                                                 goal_handle)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err is not None:
                detail = mv_result.base.error.detail if mv_result is not None else ''
                abort_code, abort_detail = err, f'접근 이동 실패: {detail or err}'
                break

            # --- 2단계: 힘 제어 하강 — 접촉 확인 + 압착력 도달 -----------------------
            feedback(2, 30.0)
            point, err = self._call_probe_point(
                tx_mm, ty_mm, tz_mm, approach_height, approach_height + press_margin,
                press_force, timeout_s, goal_handle)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err == ErrorCode.E_NO_CONTACT:
                self.get_logger().warn(
                    f'[{err}] stone: 목표 위치에서 표면 미검출 — 위치 오차로 간주, 재시도 대상')
                retry_count += 1
                if retry_count > max_retry:
                    abort_code, abort_detail = ErrorCode.E_STONE_MISS, \
                        f'{max_retry}회 재시도 후에도 목표 위치에서 접촉 실패'
                    break
                feedback(4, 0.0)
                continue
            if err is not None:
                # 접촉은 됐는데 실패(측면 힘 초과 등) — 위치 문제가 아니라 힘/자세
                # 문제로 보고 즉시 중단한다 (§6.7 에러표의 E_OVERFORCE 로 매핑).
                abort_code, abort_detail = ErrorCode.E_OVERFORCE, \
                    f'압착 중 ProbePoint 실패({err}) — 힘/자세 이상'
                break

            # --- 3단계: 같은 깊이로 재하강 — 물리적으로 압착 유지 -------------------
            feedback(3, 55.0)
            hold_z_mm = tz_mm + approach_height - point.contact_depth_mm
            hold_pose = Pose()
            hold_pose.position.x = tx_mm / 1000.0
            hold_pose.position.y = ty_mm / 1000.0
            hold_pose.position.z = hold_z_mm / 1000.0
            hold_pose.orientation.x, hold_pose.orientation.y, \
                hold_pose.orientation.z, hold_pose.orientation.w = quat
            mv_result, err = self._call_move_to(hold_pose, hold_speed, timeout_s, goal_handle)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err is not None:
                detail = mv_result.base.error.detail if mv_result is not None else ''
                abort_code, abort_detail = err, f'압착 유지 재하강 실패: {detail or err}'
                break

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
            feedback(3, 75.0)
            self._broadcast_hold_frame(tx_mm, ty_mm, hold_z_mm, quat)
            place_result, err = self._call_pick_place(
                PickPlace.Goal.MODE_PLACE, _HOLD_FRAME, 0.0, False, approach_height,
                timeout_s, goal_handle)
            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err is not None:
                detail = place_result.base.error.detail if place_result is not None else ''
                abort_code, abort_detail = ErrorCode.E_GRIP_FAILED, \
                    f'PLACE 실패: {err} {detail}'.strip()
                break
            stone_in_hand = False

            # --- 5단계: 검증 (옵션) -----------------------------------------------
            if not verify_enabled:
                position_error_mm = -1.0
                actual_position = Point(x=0.0, y=0.0, z=0.0)
                break

            feedback(5, 90.0)
            on_stone_xy = []
            radius = self.get_parameter('verify_probe_radius_mm').value
            v_force = self.get_parameter('verify_probe_max_force_n').value
            v_depth = self.get_parameter('verify_probe_depth_mm').value
            height_tol = self.get_parameter('stone_height_tolerance_mm').value
            for i in range(verify_probe_count):
                if goal_handle.is_cancel_requested or not self._safe_to_move():
                    break
                theta = 2.0 * math.pi * i / max(1, verify_probe_count)
                vx = tx_mm + radius * math.cos(theta)
                vy = ty_mm + radius * math.sin(theta)
                vpoint, verr = self._call_probe_point(
                    vx, vy, tz_mm, approach_height, approach_height + v_depth, v_force,
                    timeout_s, goal_handle)
                label = f'stone_{i}'
                if verr is not None or vpoint is None:
                    vr = self._make_validation_result(
                        goal.session_id, label, Point(x=vx / 1000.0, y=vy / 1000.0, z=0.0),
                        0.0, 0.0, height_tol, ValidationResult.RESULT_SKIP)
                else:
                    contact_z = tz_mm + approach_height - vpoint.contact_depth_mm
                    on_stone = abs(contact_z - hold_z_mm) <= height_tol
                    if on_stone:
                        on_stone_xy.append((vx, vy))
                    grading = ValidationResult.RESULT_PASS if on_stone else \
                        ValidationResult.RESULT_FAIL
                    vr = self._make_validation_result(
                        goal.session_id, label, vpoint.position, vpoint.release_force_n,
                        vpoint.stiffness_n_per_mm, height_tol, grading)
                self._result_pub.publish(vr)

            if on_stone_xy:
                cx, cy = centroid(on_stone_xy)
                actual_position = Point(x=cx / 1000.0, y=cy / 1000.0, z=hold_z_mm / 1000.0)
                position_error_mm = math.hypot(cx - tx_mm, cy - ty_mm)
            else:
                actual_position = Point(x=0.0, y=0.0, z=0.0)
                position_error_mm = float('inf')
                self.get_logger().warn(
                    'stone: 검증 프로빙에서 스톤 위 점을 하나도 못 찾음 — 재시도 대상')

            if position_error_mm <= position_tolerance:
                break

            retry_count += 1
            if retry_count > max_retry:
                abort_code, abort_detail = ErrorCode.E_STONE_MISS, \
                    (f'검증 위치 오차 {position_error_mm:.2f}mm > '
                     f'position_tolerance_mm({position_tolerance}), '
                     f'{max_retry}회 재배치 후에도 미달')
                break
            self.get_logger().warn(
                f'stone: 위치 오차 {position_error_mm:.2f}mm 초과 — 재배치 {retry_count}/{max_retry}')
            continue

        if abort_code == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, abort_detail,
                                              started_at)
            result.retry_count = retry_count
            return result
        if abort_code is not None:
            self._log_abort(abort_code, abort_detail)
            goal_handle.abort()
            result.base = self._result_base(False, abort_code, abort_detail, started_at)
            result.abort_reason = _abort_reason(abort_code)
            result.retry_count = retry_count
            return result

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        result.actual_position = actual_position
        result.position_error_mm = position_error_mm
        result.retry_count = retry_count
        return result

    def _make_validation_result(self, session_id, label, position, release_force_n,
                                 stiffness_n_per_mm, threshold_n, grading):
        vr = ValidationResult()
        vr.session_id = session_id
        vr.layer_index = 0
        vr.point_label = label
        vr.position = position
        vr.release_force_n = release_force_n
        vr.stiffness_n_per_mm = stiffness_n_per_mm
        vr.threshold_n = threshold_n
        vr.result = grading
        vr.measured_at = self.get_clock().now().to_msg()
        return vr

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
