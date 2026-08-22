"""coating_node — 경계 안쪽으로 오프셋한 영역에만 젤 도포 (NIS §6.4 M03).

**도포 두께는 측정하지 않는다** (SDS §1.3 명시적 비목표). 요구사항은
"빈 영역 없이 덮였는가"로 축소돼 있다. `coverage_ratio` 는 궤적 기반
추정치일 뿐 실제 도포량이 아니며 판정 기준이 아니다 — result 에 두께
필드를 두지 않는다.

boundary_polygon 을 `boundary_offset_mm` 만큼 안쪽으로 축소한 영역에만
도포한다 (큐티클 번짐 방지, FR-16). 실제 이동은 robot_skill_node 의
`/skill/contact_path` 를 호출한다 (SDS §4.1).
"""
import math
import threading
import time

import rclpy
from geometry_msgs.msg import Point, Pose
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import CoatGel, ContactPath
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, ToolState
from nail_msgs.srv import GetStiffnessMap, ValidatePrecondition

from nail_perception.geometry2d import polygon_area, raster_fill

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_OVERFORCE: ErrorCode.SEV_ABORT,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_NO_SCAN: ErrorCode.SEV_ABORT,
    ErrorCode.E_PRECOND_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_INVALID_GOAL: ErrorCode.SEV_ABORT,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


class CoatingNode(Node):

    def __init__(self):
        super().__init__('coating_node')
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

        self._get_map_client = self.create_client(
            GetStiffnessMap, '/scan/get_map', callback_group=self._cb_client)
        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._contact_client = ActionClient(self, ContactPath, '/skill/contact_path',
                                             callback_group=self._cb_client)
        self._contact_goal_handle = None

        self._coat_server = ActionServer(
            self, CoatGel, '/process/coat',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('coating_node ready')

    # --- 파라미터 (NIS §6.4 표) -------------------------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 0.2)
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        d('boundary_offset_mm', 1.0)
        d('target_force_n', 0.8)
        d('max_force_n', 2.0)
        d('path_pitch_mm', 1.5)
        d('feed_speed_mms', 10.0)
        d('passes', 1)
        d('use_compliance', True)
        d('max_duration_s', 45.0)

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
        if self._contact_goal_handle is not None:
            self._contact_goal_handle.cancel_goal_async()
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
        req.stage = ValidatePrecondition.Request.STAGE_COAT
        req.session_id = session_id
        req.required_tool = ToolState.COATER
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

    # --- goal 수락 (§3.1 ②③④, NIS §6.4 동작 1) -----------------------------------
    def _on_goal(self, goal_request):
        if not goal_request.session_id:
            self.get_logger().warn('CoatGel REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('CoatGel REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        found, valid, _map = self._call_get_map(goal_request.session_id)
        if not found or not valid:
            self.get_logger().warn(
                f'CoatGel REJECT: E_NO_SCAN (found={found}, valid={valid})')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'CoatGel REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = CoatGel.Result()

        boundary_offset = self._val(goal.boundary_offset_mm, 'boundary_offset_mm')
        target_force = self._val(goal.target_force_n, 'target_force_n')
        max_force = self._val(goal.max_force_n, 'max_force_n')
        path_pitch = self._val(goal.path_pitch_mm, 'path_pitch_mm')
        feed_speed = self._val(goal.feed_speed_mms, 'feed_speed_mms')
        passes = int(self._val(goal.passes, 'passes'))
        # bool 필드는 "설정 안 함"과 False 를 구분할 수 없어(ROS 메시지 자체의
        # 한계) _val 패턴을 못 쓴다 — goal 값을 그대로 신뢰한다 (ContactPath.
        # use_compliance, PickPlace.verify_grip 과 동일한 처리).
        use_compliance = goal.use_compliance
        max_duration = self._val(goal.max_duration_s, 'max_duration_s')

        # --- 맵 재확인 (goal_callback 과 execute 사이 갱신 가능성 대비) -------------
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

        # boundary_offset_mm 만큼 안쪽으로 침식 (큐티클 번짐 방지, FR-16)
        path_xy = raster_fill(boundary_xy, path_pitch, -boundary_offset)
        if not path_xy:
            detail = (f'boundary_offset_mm({boundary_offset}) 이 너무 커서 '
                      '도포 영역이 남지 않음')
            self._log_abort(ErrorCode.E_INVALID_GOAL, detail)
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_INVALID_GOAL, detail, started_at)
            return result

        def mm_to_pose(xy_mm):
            pose = Pose()
            pose.position.x = xy_mm[0] / 1000.0
            pose.position.y = xy_mm[1] / 1000.0
            pose.position.z = 0.0
            pose.orientation.w = 1.0
            return pose

        cp_goal = ContactPath.Goal()
        cp_goal.waypoints = [mm_to_pose(xy) for xy in path_xy]
        cp_goal.frame_id = 'nail_local_frame'
        cp_goal.session_id = goal.session_id
        cp_goal.target_force_n = target_force
        cp_goal.max_force_n = max_force
        cp_goal.feed_speed_mms = feed_speed
        cp_goal.use_compliance = use_compliance
        cp_goal.abort_on_low_stiffness = False  # 이미 검증된 boundary_polygon 안쪽만 도포
        cp_goal.allowed_polygon = [Point(x=x / 1000.0, y=y / 1000.0, z=0.0)
                                    for x, y in boundary_xy]
        cp_goal.passes = passes
        cp_goal.max_duration_s = max_duration

        def feedback(pct, current_pass):
            fb = CoatGel.Feedback()
            fb.percent = pct
            fb.current_pass = current_pass
            goal_handle.publish_feedback(fb)

        def on_cp_feedback(fb_msg):
            fb = fb_msg.feedback
            feedback(fb.percent, fb.current_pass)

        timeout_s = max_duration + 10.0
        cp_result, err_code, err_detail = self._call_contact_path(
            cp_goal, goal_handle, timeout_s, on_cp_feedback)

        # coverage_ratio — 기록용 추정치. 실제 경로가 덮은 점 수 * pitch^2 를
        # 목표(침식된) 영역 면적으로 나눈다. 판정 기준 아님 (NIS §6.4 명시).
        coverage_ratio = self._estimate_coverage_ratio(path_xy, path_pitch, boundary_xy,
                                                         boundary_offset)

        if err_code == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, err_detail, started_at)
            result.coverage_ratio = coverage_ratio
            return result
        if err_code is not None or not cp_result.base.success:
            code = err_code or cp_result.base.error.code
            detail = err_detail or cp_result.base.error.detail
            self._log_abort(code, detail)
            goal_handle.abort()
            if cp_result is not None:
                result.abort_reason = cp_result.abort_reason
                result.passes_done = cp_result.passes_done
                result.mean_force_n = cp_result.mean_force_n
            result.coverage_ratio = coverage_ratio
            result.base = self._result_base(False, code, detail, started_at)
            return result

        goal_handle.succeed()
        result.mean_force_n = cp_result.mean_force_n
        result.coverage_ratio = coverage_ratio
        result.passes_done = cp_result.passes_done
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        return result

    def _estimate_coverage_ratio(self, path_xy, path_pitch, boundary_xy, boundary_offset):
        target_polygon_area = polygon_area(boundary_xy)
        # 침식된 목표 면적을 다시 다각형으로 재구성하는 대신, 원본 면적에서
        # 테두리 손실을 근사(둘레 x offset)해 뺀다 — 정확한 Minkowski 침식
        # 다각형을 새로 만드는 것보다 훨씬 저렴하고, 기록용 추정치라는 용도에는
        # 충분하다.
        perimeter = sum(
            math.hypot(boundary_xy[(i + 1) % len(boundary_xy)][0] - boundary_xy[i][0],
                       boundary_xy[(i + 1) % len(boundary_xy)][1] - boundary_xy[i][1])
            for i in range(len(boundary_xy)))
        target_area = max(1e-6, target_polygon_area - perimeter * boundary_offset)
        covered_area = len(path_xy) * (path_pitch ** 2)
        return min(1.0, covered_area / target_area)

    # --- ContactPath 클라이언트 헬퍼 (§3.3 취소 전파) ------------------------------
    def _call_contact_path(self, goal, our_goal_handle, timeout_s, feedback_cb=None):
        if not self._contact_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_MOTION_FAILED, 'contact_path 액션 서버 연결 실패'

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        send_future = self._contact_client.send_goal_async(goal, feedback_callback=feedback_cb)
        send_future.add_done_callback(on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return None, ErrorCode.E_TIMEOUT, 'contact_path goal 전송 타임아웃'

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED, 'contact_path goal 거부됨'
        self._contact_goal_handle = gh

        result_done = threading.Event()

        def on_result(fut):
            state['result'] = fut.result()
            result_done.set()

        gh.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        cancelled = False
        while not result_done.wait(timeout=0.1):
            if our_goal_handle.is_cancel_requested and not cancelled:
                gh.cancel_goal_async()
                cancelled = True
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._contact_goal_handle = None
                return None, ErrorCode.E_TIMEOUT, 'contact_path 결과 타임아웃'

        self._contact_goal_handle = None
        result = state['result'].result
        if cancelled:
            return result, 'CANCELLED', '사용자 취소'
        return result, None, None

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
        self.get_logger().error(f'[{code}] coating_node: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = CoatingNode()
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
