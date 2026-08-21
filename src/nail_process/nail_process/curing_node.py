"""curing_node — UV 국소 조사 (NIS §6.5 M04, v0.2 permit 폐지 ★).

**UV 램프는 상시 ON 이고 소프트웨어로 끌 수 없다** (SDS §9.3). 이 노드는
"언제 켜는가"를 다루지 않는다 — "얼마나 오래 그 자리에 머무는가"로 조사량을
만든다. 접촉이 필요 없으므로 `ContactPath` 가 아니라 `/skill/move_to` 만
쓴다.

**`E_SAFETY_BLOCKED` 시 유일한 대응은 물리적 이탈이다.** permit 이 없으므로
"소등"이라는 선택지 자체가 없다 — 이 노드가 안전 결함에 반응하는 유일한
방법은 대기 위치로 물러나는 것뿐이고, 그래서 모든 종료 경로(성공/실패/취소/
타임아웃)에서 반드시 대기 위치 이탈을 시도한 뒤 `parked` 를 채운다.
`parked=false` 로 끝나는 경로가 있다면 그것 자체가 안전 결함이다.
"""
import math
import threading
import time

from geometry_msgs.msg import Pose
import rclpy
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import CureUV, MoveTo
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, ToolState
from nail_msgs.srv import GetStiffnessMap, ValidatePrecondition

from nail_perception.geometry2d import centroid, polygon_area, raster_fill

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_NO_SCAN: ErrorCode.SEV_ABORT,
    ErrorCode.E_PRECOND_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


# 램프가 nail_local_frame 의 -Z(표면) 를 향하도록 고정하는 자세 — 로컬 X축
# 기준 180도 회전. 이 축의 라이브 자세를 되읽는 인터페이스가 없어(§ 하단
# 주석) 실기 정렬 결과와 다를 수 있다.
_FACE_DOWN_QUAT = (1.0, 0.0, 0.0, 0.0)  # (x, y, z, w)


class CuringNode(Node):

    def __init__(self):
        super().__init__('curing_node')
        self._declare_parameters()

        self._latest_safety = None
        safety_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                                 durability=DurabilityPolicy.TRANSIENT_LOCAL)

        self._cb_action = MutuallyExclusiveCallbackGroup()
        self._cb_client = MutuallyExclusiveCallbackGroup()

        self.create_subscription(SafetyState, self.get_parameter('safety_topic').value,
                                  self._on_safety_status, safety_qos,
                                  callback_group=self._cb_client)

        self._get_map_client = self.create_client(
            GetStiffnessMap, '/scan/get_map', callback_group=self._cb_client)
        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._move_client = ActionClient(self, MoveTo, '/skill/move_to',
                                          callback_group=self._cb_client)
        self._move_goal_handle = None

        self._cure_server = ActionServer(
            self, CureUV, '/process/cure',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('curing_node ready — UV 상시 ON, 소프트웨어 소등 없음 (SDS §9.3)')

    # --- 파라미터 (NIS §6.5 표) -------------------------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        d('uv_always_on', True)  # v0.2 고정. false 미지원 — 값을 바꿔도 동작 안 변함
        d('standoff_mm', 15.0)
        d('standoff_tolerance_mm', 3.0)
        d('exposure_s', 30.0)
        d('path_speed_mms', 3.0)
        d('dwell_points', 5)
        d('dwell_s_per_point', 6.0)
        d('rework_exposure_scale', 1.5)
        d('entry_direction', 'from_side')
        d('park_distance_mm', 120.0)
        d('max_duration_s', 120.0)
        # MoveTo 는 speed_ratio(0~1) 를 받는다 — robot_skill_node 의
        # move_max_speed_mms 를 여기서 알 방법이 없어(별도 프로세스·파라미터)
        # path_speed_mms -> ratio 환산에 쓸 가정값을 로컬로 둔다. 실제
        # move_max_speed_mms 와 다르면 조사 이송 속도가 어긋난다.
        d('assumed_move_max_speed_mms', 100.0)

    # --- 안전 -----------------------------------------------------------------
    def _on_safety_status(self, msg):
        self._latest_safety = msg

    def _safe_to_move(self):
        return self._latest_safety is not None and self._latest_safety.safe_to_move

    def _on_cancel(self, goal_handle):
        if self._move_goal_handle is not None:
            self._move_goal_handle.cancel_goal_async()
        return CancelResponse.ACCEPT

    # --- 서비스 폴링 헬퍼 --------------------------------------------------------
    def _call_get_map(self, session_id, timeout_s=5.0):
        if not self._get_map_client.wait_for_service(timeout_sec=timeout_s):
            return False, False, None
        req = GetStiffnessMap.Request()
        req.session_id = session_id
        future = self._get_map_client.call_async(req)
        deadline = time.monotonic() + timeout_s
        while not future.done():
            if time.monotonic() > deadline:
                return False, False, None
            time.sleep(0.02)
        resp = future.result()
        if resp is None or not resp.found:
            return False, False, None
        return True, resp.map.valid, resp.map

    def _call_validate_precondition(self, session_id, timeout_s=5.0):
        if not self._validate_client.wait_for_service(timeout_sec=timeout_s):
            return False, ['ValidatePrecondition 서비스 연결 실패']
        req = ValidatePrecondition.Request()
        req.stage = ValidatePrecondition.Request.STAGE_CURE
        req.session_id = session_id
        req.required_tool = ToolState.UV
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
            self.get_logger().warn('CureUV REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('CureUV REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        found, valid, _map = self._call_get_map(goal_request.session_id)
        if not found or not valid:
            self.get_logger().warn(f'CureUV REJECT: E_NO_SCAN (found={found}, valid={valid})')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'CureUV REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # --- 체류 지점 산출 ----------------------------------------------------------
    def _generate_dwell_points(self, boundary_xy, n_points):
        """전체 영역 커버 — 대략 균등 간격으로 n_points 개를 뽑는다."""
        area = polygon_area(boundary_xy)
        if area <= 1e-6 or n_points <= 0:
            return [centroid(boundary_xy)]
        approx_pitch = math.sqrt(area / n_points)
        candidates = raster_fill(boundary_xy, max(0.5, approx_pitch), 0.0)
        if not candidates:
            return [centroid(boundary_xy)]
        if len(candidates) <= n_points:
            return candidates
        stride = len(candidates) / n_points
        return [candidates[int(i * stride)] for i in range(n_points)]

    def _entry_offset_xy(self, entry_direction):
        # NIS 는 방향 부호 규약을 명시하지 않는다 — 손끝 반대편(측면)에서
        # 접근한다는 개념만 있고, 정확한 각도는 미확정 상태다 (완전 차단은
        # 애초에 불가능하다고 문서 스스로 인정). 단순하고 일관된 값 하나로
        # 고정한다.
        return (1.0, 0.0)

    def _pose_at(self, xy_mm, z_mm):
        pose = Pose()
        pose.position.x = xy_mm[0] / 1000.0
        pose.position.y = xy_mm[1] / 1000.0
        pose.position.z = z_mm / 1000.0
        pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w = \
            _FACE_DOWN_QUAT
        return pose

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = CureUV.Result()

        standoff = self._val(goal.standoff_mm, 'standoff_mm')
        dwell_points_n = int(self._val(goal.dwell_points, 'dwell_points'))
        dwell_s = self._val(goal.dwell_s_per_point, 'dwell_s_per_point') * \
            (goal.exposure_scale if goal.exposure_scale > 0.0 else 1.0)
        path_speed = self._val(goal.path_speed_mms, 'path_speed_mms')
        park_distance = self._val(goal.park_distance_mm, 'park_distance_mm')
        max_duration = self._val(goal.max_duration_s, 'max_duration_s')
        speed_ratio = min(1.0, max(0.05, path_speed /
                                    self.get_parameter('assumed_move_max_speed_mms').value))

        found, valid, stiffness_map = self._call_get_map(goal.session_id)
        if not found or not valid:
            detail = f'GetStiffnessMap: found={found} valid={valid}'
            self._log_abort(ErrorCode.E_NO_SCAN, detail)
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_NO_SCAN, detail, started_at)
            return result

        boundary_xy = [(pt.x, pt.y) for pt in stiffness_map.region.boundary_polygon]
        if len(boundary_xy) < 3:
            detail = 'boundary_polygon 점 3개 미만 — 경계 미확정'
            self._log_abort(ErrorCode.E_NO_SCAN, detail)
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_NO_SCAN, detail, started_at)
            return result

        # target_regions: StiffnessPoint/BoundaryRegion 계열과 동일하게 mm 규약
        # (inspection_node 의 fail_points 도 이 계열에서 나온다).
        if len(goal.target_regions) > 0:
            dwell_xy = [(pt.x, pt.y) for pt in goal.target_regions]
        else:
            dwell_xy = self._generate_dwell_points(boundary_xy, dwell_points_n)

        center = centroid(boundary_xy)
        ex, ey = self._entry_offset_xy(self.get_parameter('entry_direction').value)
        park_xy = (center[0] + ex * park_distance, center[1] + ey * park_distance)
        park_pose = self._pose_at(park_xy, standoff)

        def feedback(pct, dwell_idx, elapsed_dwell_s, standoff_now):
            fb = CureUV.Feedback()
            fb.percent = pct
            fb.current_dwell_index = dwell_idx
            fb.elapsed_dwell_s = elapsed_dwell_s
            fb.current_standoff_mm = standoff_now
            goal_handle.publish_feedback(fb)

        deadline = time.monotonic() + max_duration
        actual_exposure_s = 0.0
        dwell_completed = 0
        abort_code = None
        abort_detail = ''

        # 1) 대기 위치로 먼저 이동 (측면 진입 — entry_direction)
        reason, mv_result = self._move(park_pose, speed_ratio, goal_handle,
                                        max(1.0, deadline - time.monotonic()))
        if reason != 'ok':
            abort_code, abort_detail = self._reason_to_code(reason, mv_result)

        # 2) 체류 지점 순회
        if abort_code is None:
            n = len(dwell_xy)
            for idx, xy in enumerate(dwell_xy):
                if time.monotonic() > deadline:
                    abort_code, abort_detail = ErrorCode.E_TIMEOUT, \
                        f'max_duration_s({max_duration}) 초과'
                    break
                target_pose = self._pose_at(xy, standoff)
                reason, mv_result = self._move(target_pose, speed_ratio, goal_handle,
                                                max(1.0, deadline - time.monotonic()))
                if reason != 'ok':
                    abort_code, abort_detail = self._reason_to_code(reason, mv_result)
                    break

                dwell_elapsed = 0.0
                tick = 0.1
                while dwell_elapsed < dwell_s:
                    if goal_handle.is_cancel_requested:
                        abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                        break
                    if not self._safe_to_move():
                        abort_code, abort_detail = ErrorCode.E_SAFETY_BLOCKED, \
                            'safe_to_move=false — 즉시 이탈'
                        break
                    if time.monotonic() > deadline:
                        abort_code, abort_detail = ErrorCode.E_TIMEOUT, \
                            f'max_duration_s({max_duration}) 초과'
                        break
                    time.sleep(tick)
                    dwell_elapsed += tick
                    actual_exposure_s += tick
                    feedback(100.0 * (idx + dwell_elapsed / dwell_s) / n, idx, dwell_elapsed,
                             standoff)
                if abort_code is not None:
                    break
                dwell_completed += 1

        # 3) 대기 위치 이탈 — 성공/실패/취소/타임아웃 관계없이 반드시 시도한다.
        #    permit 이 없으므로 이게 유일한 안전 대응이다 (NIS §6.5 경고).
        parked = False
        try:
            retreat_reason, _ = self._move(park_pose, speed_ratio, goal_handle, 15.0,
                                            ignore_cancel=True)
            parked = (retreat_reason == 'ok')
            if not parked:
                self.get_logger().error(
                    f'[SAFETY] curing_node: 대기 위치 이탈 실패(reason={retreat_reason}) — '
                    'UV 램프가 상시 ON 상태로 알 수 없는 위치를 비추고 있을 수 있음. '
                    '즉시 수동 확인 필요.')
        except Exception as e:
            self.get_logger().error(f'[SAFETY] curing_node: 이탈 시도 중 예외: {e}')

        result.actual_exposure_s = actual_exposure_s
        result.dwell_completed = dwell_completed
        result.mean_standoff_mm = standoff  # 명령값 — 이 노드는 독립 거리 센서가 없어 실측 아님
        result.coverage_ratio = dwell_completed / max(1, len(dwell_xy))
        result.parked = parked

        if abort_code == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, abort_detail,
                                              started_at)
            return result
        if abort_code is not None:
            self._log_abort(abort_code, abort_detail)
            goal_handle.abort()
            result.base = self._result_base(False, abort_code, abort_detail, started_at)
            return result

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        return result

    @staticmethod
    def _reason_to_code(reason, mv_result):
        if reason == 'cancel':
            return 'CANCELLED', '사용자 취소'
        if reason == 'safety':
            return ErrorCode.E_SAFETY_BLOCKED, 'safe_to_move=false'
        if reason == 'timeout':
            return ErrorCode.E_TIMEOUT, 'move_to 타임아웃'
        detail = mv_result.base.error.detail if mv_result is not None else 'move_to 실패'
        code = mv_result.base.error.code if mv_result is not None else ErrorCode.E_MOTION_FAILED
        return code, detail

    # --- MoveTo 클라이언트 헬퍼 (§3.3 취소 전파) -----------------------------------
    def _move(self, pose, speed_ratio, our_goal_handle, timeout_s, ignore_cancel=False):
        goal = MoveTo.Goal()
        goal.target = pose
        goal.frame_id = 'nail_local_frame'
        goal.linear = True
        goal.speed_ratio = speed_ratio
        goal.accel_ratio = speed_ratio
        goal.timeout_s = timeout_s

        if not self._move_client.wait_for_server(timeout_sec=10.0):
            return 'error', None

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        self._move_client.send_goal_async(goal).add_done_callback(on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return 'timeout', None

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return 'error', None
        self._move_goal_handle = gh

        result_done = threading.Event()

        def on_result(fut):
            state['result'] = fut.result()
            result_done.set()

        gh.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        while not result_done.wait(timeout=0.1):
            if not ignore_cancel and our_goal_handle.is_cancel_requested:
                gh.cancel_goal_async()
                self._move_goal_handle = None
                return 'cancel', None
            if not ignore_cancel and not self._safe_to_move():
                gh.cancel_goal_async()
                self._move_goal_handle = None
                return 'safety', None
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._move_goal_handle = None
                return 'timeout', None

        self._move_goal_handle = None
        result = state['result'].result
        if not result.base.success:
            return 'error', result
        return 'ok', result

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
        self.get_logger().error(f'[{code}] curing_node: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = CuringNode()
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
