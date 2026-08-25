"""brushing_node — 연마 후 케라틴 분진 제거 (NIS §6.3 M02).

**검증 신호가 없다.** "분진이 제거됐는가"는 힘으로 측정할 수 없으므로, 성공
기준은 "지정 영역을 빠짐없이 통과했는가"(경로 커버리지) 뿐이다. 이 노드의
결과에는 그 이상을 주장하는 필드를 두지 않는다.

실제 이동은 robot_skill_node의 `/skill/contact_path`가 담당한다. 이 노드는
브러시 전용 여섯 티칭 Pose로 곡면 왕복 경로를 만들고 goal을 조립한다.
"""
import threading
import time

import rclpy
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import BrushDust, ContactPath
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, ToolState
from nail_msgs.srv import ValidatePrecondition

from .taught_surface import (SurfaceConfigError, build_surface_path,
                             surface_is_configured)

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_PRECOND_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_INVALID_GOAL: ErrorCode.SEV_ABORT,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


class BrushingNode(Node):

    def __init__(self):
        super().__init__('brushing_node')
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

        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._contact_client = ActionClient(self, ContactPath, '/skill/contact_path',
                                             callback_group=self._cb_client)
        self._contact_goal_handle = None

        self._brush_server = ActionServer(
            self, BrushDust, '/process/brush',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('brushing_node ready')

    # --- 파라미터 (NIS §6.3 표) -------------------------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 1.0)
        d('node_timeout_s', 120.0)
        d('passes', 2)
        d('path_pitch_mm', 2.0)
        d('feed_speed_mms', 20.0)
        d('coverage_margin_mm', 0.0)
        d('max_duration_s', 30.0)
        d('contact_offset_mm', 0.0)
        d('surface_config_path', '')
        d('surface_name', 'brush')
        d('arc_segment_length_mm', 4.0)
        d('arc_min_sagitta_mm', 0.05)
        d('arc_min_radius_mm', 1.0)
        d('arc_max_radius_mm', 100.0)
        d('arc_max_z_change_mm', 2.0)
        d('arc_max_orientation_change_deg', 10.0)

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
    def _call_validate_precondition(self, session_id, timeout_s=5.0):
        if not self._validate_client.wait_for_service(timeout_sec=timeout_s):
            return False, ['ValidatePrecondition 서비스 연결 실패']
        req = ValidatePrecondition.Request()
        req.stage = ValidatePrecondition.Request.STAGE_BRUSH
        req.session_id = session_id
        req.required_tool = ToolState.BRUSH
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
            self.get_logger().warn('BrushDust REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('BrushDust REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        if not surface_is_configured(
                self.get_parameter('surface_config_path').value,
                self.get_parameter('surface_name').value):
            self.get_logger().warn(
                'BrushDust REJECT: E_INVALID_GOAL (브러시 6-Pose 티칭 미설정)')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'BrushDust REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = BrushDust.Result()

        passes = int(self._val(goal.passes, 'passes'))
        path_pitch = self._val(goal.path_pitch_mm, 'path_pitch_mm')
        feed_speed = self._val(goal.feed_speed_mms, 'feed_speed_mms')
        coverage_margin = self._val(goal.coverage_margin_mm, 'coverage_margin_mm')
        max_duration = self._val(goal.max_duration_s, 'max_duration_s')

        p = self.get_parameter
        try:
            path = build_surface_path(
                p('surface_config_path').value, p('surface_name').value,
                path_pitch, coverage_margin,
                p('arc_segment_length_mm').value,
                p('arc_min_sagitta_mm').value,
                p('arc_min_radius_mm').value,
                p('arc_max_radius_mm').value,
                p('arc_max_z_change_mm').value,
                p('arc_max_orientation_change_deg').value)
        except SurfaceConfigError as exc:
            detail = f'브러시 티칭 경로 생성 실패: {exc}'
            self._log_abort(ErrorCode.E_INVALID_GOAL, detail)
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_INVALID_GOAL, detail, started_at)
            return result

        cp_goal = ContactPath.Goal()
        cp_goal.waypoints = path.waypoints
        cp_goal.circular_via_indices = path.circular_via_indices
        cp_goal.frame_id = path.frame_id
        cp_goal.reference_key = ''
        cp_goal.session_id = goal.session_id
        cp_goal.feed_speed_mms = feed_speed
        cp_goal.contact_offset_mm = self.get_parameter('contact_offset_mm').value
        cp_goal.allowed_polygon = path.allowed_polygon
        cp_goal.passes = passes
        cp_goal.max_duration_s = max_duration

        def feedback(pct, current_pass):
            fb = BrushDust.Feedback()
            fb.percent = pct
            fb.current_pass = current_pass
            goal_handle.publish_feedback(fb)

        def on_cp_feedback(fb_msg):
            fb = fb_msg.feedback
            feedback(fb.percent, fb.current_pass)

        timeout_s = max_duration + 10.0
        cp_result, err_code, err_detail = self._call_contact_path(
            cp_goal, goal_handle, timeout_s, on_cp_feedback)

        if err_code == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, err_detail, started_at)
            return result
        if err_code is not None or not cp_result.base.success:
            code = err_code or cp_result.base.error.code
            detail = err_detail or cp_result.base.error.detail
            self._log_abort(code, detail)
            goal_handle.abort()
            if cp_result is not None:
                result.abort_reason = cp_result.abort_reason
                result.passes_done = cp_result.passes_done
            result.base = self._result_base(False, code, detail, started_at)
            return result

        goal_handle.succeed()
        result.passes_done = cp_result.passes_done
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        return result

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
        self.get_logger().error(f'[{code}] brushing_node: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = BrushingNode()
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
