"""inspection_node — 택프리(tack-free) 3점 검사 (NIS §6.6 M06 ★, v0.2 개정).

**D-04 확정으로 이 노드가 품질 판정의 유일한 근거다.** 프로브로 경화면을
눌렀다 떼면서(`ProbePoint(measure_release=true)`) 이탈 시 딸려오는 인장력을
잰다 — 그 값이 `tack_threshold_n`을 넘으면 아직 끈적하다(미경화)는 뜻이다.

검사점은 중앙 · 좌 · 우 3점으로 고정한다(v0.1의 격자 방식 폐기). 좌우 두
점이 이 프로젝트의 핵심 주장(사이드월 음영 사각지대를 국소 조사로
해결한다)을 검증하는 지점이다 — 중앙만 봐서는 일괄 조사 방식과 결과가
같아 국소 조사의 우위를 보일 수 없다. **3점은 표본일 뿐 전수 검사가
아니다**: 검사한 3곳이 우연히 다 경화됐다면 국소 미경화 반점은 못
잡는다. 결과는 "손톱 전체가 경화됨"이 아니라 "검사한 3개 지점이 경화
기준을 만족함"으로만 해석해야 한다(BRD R7, 과대 주장 리스크).

`ProbePoint`가 `probe_max_force_n`(젤을 뚫지 않을 상한)에 닿았다는 것은
robot_skill_node 입장에선 정상 종료 조건 중 하나지만(SDS §5.1), 검사에서
그 얕은 깊이(`probe_depth_mm`, 기본 0.3mm) 안에서 힘 상한에 먼저 닿았다면
"젤을 뚫었을 가능성"을 뜻한다(NIS §6.6 에러표 E_OVERFORCE). ProbePoint
액션 자체는 이 구분을 반환값으로 알려주지 않으므로, 여기서는 파형의 최대
|Fz|가 요청한 `probe_max_force_n`에 닿았는지로 자체 판정한다.
"""
import threading
import time

from geometry_msgs.msg import Point
import rclpy
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import InspectCure
from nail_msgs.action import ProbePoint as ProbePointAction
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, StiffnessPoint, ToolState, \
    ValidationResult
from nail_msgs.srv import GetStiffnessMap, ValidatePrecondition

from nail_perception.geometry2d import centroid, ray_polygon_distance

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_NO_SCAN: ErrorCode.SEV_ABORT,
    ErrorCode.E_PRECOND_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_OVERFORCE: ErrorCode.SEV_ABORT,
    ErrorCode.E_NO_CONTACT: ErrorCode.SEV_ABORT,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


def _abort_reason(code):
    """robot_skill_node 의 ABORT_LOW_STIFFNESS / ABORT_OVERFORCE 표기와 맞춘다
    ("E_" 접두어를 뗀 "ABORT_" + 코드)."""
    return 'ABORT_' + (code[2:] if code.startswith('E_') else code)


class InspectionNode(Node):

    def __init__(self):
        super().__init__('inspection_node')
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

        # IDS §3.11 부록: /validation/result 는 RELIABLE, depth 20
        result_qos = QoSProfile(depth=20, reliability=ReliabilityPolicy.RELIABLE,
                                 durability=DurabilityPolicy.VOLATILE)
        self._result_pub = self.create_publisher(ValidationResult, '/validation/result',
                                                   result_qos)

        self._get_map_client = self.create_client(
            GetStiffnessMap, '/scan/get_map', callback_group=self._cb_client)
        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._probe_client = ActionClient(self, ProbePointAction, '/skill/probe_point',
                                           callback_group=self._cb_client)
        self._probe_goal_handle = None

        self._inspect_server = ActionServer(
            self, InspectCure, '/process/inspect',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('inspection_node ready — 중앙·좌·우 3점 고정 (NIS §6.6)')

    # --- 파라미터 (NIS §6.6 표) -------------------------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 0.2)
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        d('sample_point_count', 3)  # v0.2 고정. 문서화 목적 — 코드는 항상 3점을 시도한다
        d('center_offset_x_ratio', 0.0)
        d('side_offset_y_ratio', 0.6)
        d('min_edge_clearance_mm', 1.5)
        d('tack_threshold_n', 0.15)
        d('require_all_pass', True)
        d('probe_depth_mm', 0.3)
        d('probe_max_force_n', 1.5)
        d('release_speed_mms', 1.0)
        d('point_timeout_s', 10.0)

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
        if self._probe_goal_handle is not None:
            self._probe_goal_handle.cancel_goal_async()
        return CancelResponse.ACCEPT

    # --- 서비스 폴링 헬퍼 (curing_node/scan_node 와 동일 패턴) ---------------------
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
        req.stage = ValidatePrecondition.Request.STAGE_INSPECT
        req.session_id = session_id
        req.required_tool = ToolState.PROBE
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
            self.get_logger().warn('InspectCure REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('InspectCure REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        found, valid, _map = self._call_get_map(goal_request.session_id)
        if not found or not valid:
            self.get_logger().warn(f'InspectCure REJECT: E_NO_SCAN (found={found}, valid={valid})')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'InspectCure REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # --- 3점 좌표 산출 (NIS §6.6 동작 2) -----------------------------------------
    def _sample_points(self, boundary_xy, center_ratio, side_ratio, min_clearance):
        """중앙·좌·우 좌표를 산출한다. 반환: [(point_label, (x,y)|None), ...] (3개 고정).

        좌우점은 중앙에서 ±Y 로 쏜 반직선이 boundary_xy 에 닿는 거리(d) 의
        side_ratio 배만큼 떨어뜨린다. 남는 여유(d*(1-ratio))가
        min_clearance 를 못 채우면 ratio 를 깎아서라도 맞추고, d 자체가
        min_clearance 이하라 깎을 여지도 없으면 그 점은 건너뛴다(WARN).
        """
        cx, cy = centroid(boundary_xy)
        xs = [p[0] for p in boundary_xy]
        half_x = (max(xs) - min(xs)) / 2.0
        center_xy = (cx + center_ratio * half_x, cy)

        points = [(ValidationResult.POINT_CENTER, center_xy)]
        for label, direction in ((ValidationResult.POINT_LEFT, (0.0, 1.0)),
                                  (ValidationResult.POINT_RIGHT, (0.0, -1.0))):
            d = ray_polygon_distance(center_xy, direction, boundary_xy, 'nearest')
            if d is None or d <= min_clearance:
                self.get_logger().warn(
                    f'inspection: {label} 점 — 경계까지 거리({d})가 '
                    f'min_edge_clearance_mm({min_clearance}) 이하라 건너뜀 (2점 검사로 진행)')
                points.append((label, None))
                continue
            max_ratio = 1.0 - min_clearance / d
            used_ratio = min(max(side_ratio, 0.0), max_ratio)
            if used_ratio < side_ratio:
                self.get_logger().warn(
                    f'inspection: {label} 점 — side_offset_y_ratio {side_ratio}→{used_ratio:.3f} '
                    f'축소 (min_edge_clearance_mm({min_clearance}) 확보)')
            offset = d * used_ratio
            xy = (center_xy[0] + direction[0] * offset, center_xy[1] + direction[1] * offset)
            points.append((label, xy))
        return points

    # --- ProbePoint 클라이언트 헬퍼 (scan_node 와 동일 패턴 + measure_release) -----
    def _call_probe_point(self, x_mm, y_mm, frame_id, depth_mm, max_force_n, release_speed_mms,
                           timeout_s, our_goal_handle):
        """반환: (StiffnessPoint|None, ForceSample[]|list, error_code|None|'CANCELLED')."""
        if not self._probe_client.wait_for_server(timeout_sec=10.0):
            return None, [], ErrorCode.E_COMM_LOST

        goal = ProbePointAction.Goal()
        goal.target = Point(x=x_mm / 1000.0, y=y_mm / 1000.0, z=0.0)
        goal.frame_id = frame_id
        goal.max_depth_mm = depth_mm
        goal.max_force_n = max_force_n
        goal.measure_release = True
        goal.release_speed_mms = release_speed_mms
        goal.source_tag = StiffnessPoint.SRC_VERIFY

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        self._probe_client.send_goal_async(goal).add_done_callback(on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return None, [], ErrorCode.E_TIMEOUT

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return None, [], ErrorCode.E_SAFETY_BLOCKED
        self._probe_goal_handle = gh

        result_done = threading.Event()

        def on_result(fut):
            state['result'] = fut.result()
            result_done.set()

        gh.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        while not result_done.wait(timeout=0.1):
            if our_goal_handle.is_cancel_requested:
                gh.cancel_goal_async()
                self._probe_goal_handle = None
                return None, [], 'CANCELLED'
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._probe_goal_handle = None
                return None, [], ErrorCode.E_TIMEOUT
        self._probe_goal_handle = None

        result = state['result'].result
        if not result.base.success:
            return result.point, list(result.waveform), result.base.error.code
        return result.point, list(result.waveform), None

    @staticmethod
    def _hit_force_cap(waveform, probe_max_force_n, eps=1e-3):
        """probe_max_force_n 에 닿았는지 — ProbePoint 자체는 이걸 별도 신호로
        안 주므로(SDS §5.1: 정상 종료 조건), 파형의 최대 |Fz|로 자체 판정한다."""
        if not waveform or probe_max_force_n <= 0.0:
            return False
        return max(abs(w.fz_n) for w in waveform) >= probe_max_force_n - eps

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = InspectCure.Result()

        center_ratio = self._val(goal.center_offset_x_ratio, 'center_offset_x_ratio')
        side_ratio = self._val(goal.side_offset_y_ratio, 'side_offset_y_ratio')
        min_clearance = self._val(goal.min_edge_clearance_mm, 'min_edge_clearance_mm')
        tack_threshold = self._val(goal.tack_threshold_n, 'tack_threshold_n')
        # bool 필드는 "설정 안 함"과 False 를 구분 못 한다(ROS 메시지 자체의 한계) —
        # coating_node.use_compliance 와 동일하게 goal 값을 그대로 신뢰한다.
        require_all_pass = goal.require_all_pass
        depth_mm = self._val(goal.probe_depth_mm, 'probe_depth_mm')
        max_force_n = self._val(goal.probe_max_force_n, 'probe_max_force_n')
        release_speed = self._val(goal.release_speed_mms, 'release_speed_mms')
        point_timeout = self._val(goal.point_timeout_s, 'point_timeout_s')

        found, valid, stiffness_map = self._call_get_map(goal.session_id)
        if not found or not valid:
            detail = f'GetStiffnessMap: found={found} valid={valid}'
            self._log_abort(ErrorCode.E_NO_SCAN, detail)
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_NO_SCAN, detail, started_at)
            result.abort_reason = _abort_reason(ErrorCode.E_NO_SCAN)
            return result

        boundary_xy = [(pt.x, pt.y) for pt in stiffness_map.region.boundary_polygon]
        if len(boundary_xy) < 3:
            detail = 'boundary_polygon 점 3개 미만 — 검사점 좌표 산출 불가'
            self._log_abort(ErrorCode.E_NO_SCAN, detail)
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_NO_SCAN, detail, started_at)
            result.abort_reason = _abort_reason(ErrorCode.E_NO_SCAN)
            return result

        frame_id = stiffness_map.frame_id
        sample_points = self._sample_points(boundary_xy, center_ratio, side_ratio, min_clearance)
        n = len(sample_points)

        def feedback(pct, last_result):
            fb = InspectCure.Feedback()
            fb.percent = pct
            if last_result is not None:
                fb.last_result = last_result
            goal_handle.publish_feedback(fb)

        results = []
        fail_points = []
        points_measured = 0
        abort_code = None
        abort_detail = ''

        for idx, (label, xy) in enumerate(sample_points):
            if goal_handle.is_cancel_requested:
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if not self._safe_to_move():
                abort_code, abort_detail = ErrorCode.E_SAFETY_BLOCKED, 'safe_to_move=false'
                break

            if xy is None:
                vr = self._make_result(goal.session_id, goal.layer_index, label,
                                        Point(x=0.0, y=0.0, z=0.0), 0.0, 0.0, tack_threshold,
                                        ValidationResult.RESULT_SKIP, [])
                results.append(vr)
                self._result_pub.publish(vr)
                feedback(100.0 * (idx + 1) / n, vr)
                continue

            point, waveform, err = self._call_probe_point(
                xy[0], xy[1], frame_id, depth_mm, max_force_n, release_speed, point_timeout,
                goal_handle)

            if err == 'CANCELLED':
                abort_code, abort_detail = 'CANCELLED', '사용자 취소'
                break
            if err == ErrorCode.E_NO_CONTACT:
                self.get_logger().warn(
                    f'[{err}] inspection: {label} 점 표면 미검출 — 판정 불가로 표시, 계속')
                vr = self._make_result(goal.session_id, goal.layer_index, label,
                                        Point(x=xy[0], y=xy[1], z=0.0), 0.0, 0.0, tack_threshold,
                                        ValidationResult.RESULT_SKIP, waveform)
                results.append(vr)
                self._result_pub.publish(vr)
                feedback(100.0 * (idx + 1) / n, vr)
                continue
            if err is not None:
                abort_code, abort_detail = err, f'{label} 점 ProbePoint 실패'
                break

            if self._hit_force_cap(waveform, max_force_n):
                abort_code, abort_detail = ErrorCode.E_OVERFORCE, (
                    f'{label} 점: probe_max_force_n({max_force_n}) 도달 — 젤 관통 가능성, '
                    f'contact_depth={point.contact_depth_mm:.3f}mm < probe_depth_mm({depth_mm})')
                break

            release_force = point.release_force_n
            grading = ValidationResult.RESULT_FAIL if abs(release_force) > tack_threshold \
                else ValidationResult.RESULT_PASS
            vr = self._make_result(
                goal.session_id, goal.layer_index, label, point.position, release_force,
                point.stiffness_n_per_mm, tack_threshold, grading,
                waveform if grading == ValidationResult.RESULT_FAIL else [])
            results.append(vr)
            self._result_pub.publish(vr)
            points_measured += 1
            if grading == ValidationResult.RESULT_FAIL:
                fail_points.append(Point(x=point.position.x, y=point.position.y,
                                          z=point.position.z))
            feedback(100.0 * (idx + 1) / n, vr)

        if abort_code == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, abort_detail,
                                              started_at)
            result.results = results
            result.points_measured = points_measured
            return result
        if abort_code is not None:
            self._log_abort(abort_code, abort_detail)
            goal_handle.abort()
            result.base = self._result_base(False, abort_code, abort_detail, started_at)
            result.abort_reason = _abort_reason(abort_code)
            result.results = results
            result.points_measured = points_measured
            return result

        measured_results = [r for r in results if r.result != ValidationResult.RESULT_SKIP]
        if require_all_pass:
            passed = len(measured_results) > 0 and \
                all(r.result == ValidationResult.RESULT_PASS for r in measured_results)
        else:
            # require_all_pass=false — NIS 는 이 경우의 판정 기준을 명시하지
            # 않는다. "하나라도 FAIL 이면 전체 FAIL"이 아니라는 뜻으로 보고,
            # 측정된 점 중 하나라도 PASS 면 통과로 둔다.
            passed = any(r.result == ValidationResult.RESULT_PASS for r in measured_results)

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        result.passed = passed
        result.results = results
        result.fail_points = fail_points
        result.points_measured = points_measured
        return result

    def _make_result(self, session_id, layer_index, label, position, release_force_n,
                      stiffness_n_per_mm, threshold_n, grading, waveform):
        vr = ValidationResult()
        vr.session_id = session_id
        vr.layer_index = layer_index
        vr.point_label = label
        vr.position = position
        vr.release_force_n = release_force_n
        vr.stiffness_n_per_mm = stiffness_n_per_mm
        vr.threshold_n = threshold_n
        vr.result = grading
        vr.waveform = waveform
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
        self.get_logger().error(f'[{code}] inspection_node: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = InspectionNode()
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
