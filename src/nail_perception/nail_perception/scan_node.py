"""scan_node — 2단계 접촉 강성 스캔 및 경계 인식 (NIS §6.1 ★★, SDS §5.2).

거친 격자(3mm)로 손톱/피부 강성 군집을 먼저 찾고, 두 군집이 갈리는 경계
부근만 정밀 격자(1mm)로 다시 떠서 경계 다각형을 확정한다. 손톱 위치를 놓치면
(고강성 군집 점수 부족) 정밀 단계로 넘어가지 않고 즉시 실패시킨다 — 잘못된
후보 위에서 정밀 스캔을 도는 건 시간 낭비이자 위험 신호를 놓치는 것이다.

실제 이동/압입은 이 노드가 하지 않는다 — robot_skill_node 의
`/skill/probe_point` 를 매 점마다 호출한다 (이 노드는 dsr_msgs2 를 import
하지 않는다, SDS §4.1).
"""
import math
import threading
import time

from nail_msgs.action import ScanBoundary
from nail_msgs.msg import (
    BoundaryRegion, ErrorCode, StiffnessMap, StiffnessPoint, ToolState,
)
from nail_msgs.action import ProbePoint as ProbePointAction
from nail_msgs.srv import GetStiffnessMap, ValidatePrecondition
from nail_msgs.msg import SafetyState
import rclpy
from geometry_msgs.msg import Point
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from tf2_ros import TransformBroadcaster

from .clustering import compute_threshold, separation_margin
from .geometry2d import adjacent_pairs_4, centroid, convex_hull, make_grid, pca_major_axis_deg


def _dist(a, b):
    return math.hypot(a[0] - b[0], a[1] - b[1])


def _midpoint(a, b):
    return ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0)


class ScanNode(Node):

    def __init__(self):
        super().__init__('scan_node')
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

        self._map_pub = self.create_publisher(StiffnessMap, '/stiffness/map', safety_qos)
        self._tf_broadcaster = TransformBroadcaster(self)

        self._probe_client = ActionClient(self, ProbePointAction, '/skill/probe_point',
                                           callback_group=self._cb_client)
        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._probe_goal_handle = None

        self._maps = {}  # session_id -> StiffnessMap (최근 것 우선, 크기 제한)
        self._maps_lock = threading.Lock()

        self.create_service(GetStiffnessMap, '/scan/get_map', self._on_get_map,
                             callback_group=self._cb_client)

        self._scan_server = ActionServer(
            self, ScanBoundary, '/process/scan',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('scan_node ready')

    # --- 파라미터 (NIS §6.1 표) ------------------------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 0.2)
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        # 스캔 영역
        d('scan_area_x_mm', 16.0)
        d('scan_area_y_mm', 13.0)
        d('scan_margin_mm', 2.0)
        d('frame_id', 'nail_frame')
        # 1단계
        d('coarse_pitch_mm', 3.0)
        d('coarse_retry_pitch_mm', 2.0)
        d('coarse_min_valid_points', 20)
        d('coarse_min_per_cluster', 5)
        d('cluster_method', 'otsu')
        # 2단계
        d('fine_pitch_mm', 1.0)
        d('boundary_band_mm', 3.0)
        d('fine_max_points', 120)
        # 판정
        d('separation_margin_min', 2.0)
        d('invalid_point_max_ratio', 0.2)
        # 프로브
        d('probe_depth_mm', 0.5)
        d('probe_max_force_n', 2.0)
        d('probe_timeout_s', 6.0)
        d('probe_no_contact_retry', 2)  # SDS §7.3 retry.probe_no_contact

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

    def _call_validate_precondition(self, session_id, timeout_s=5.0):
        if not self._validate_client.wait_for_service(timeout_sec=timeout_s):
            return False, ['ValidatePrecondition 서비스 연결 실패']
        req = ValidatePrecondition.Request()
        req.stage = ValidatePrecondition.Request.STAGE_SCAN
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
            self.get_logger().warn('ScanBoundary REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('ScanBoundary REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'ScanBoundary REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    # --- ProbePoint 클라이언트 헬퍼 ----------------------------------------------
    def _call_probe_point(self, x_mm, y_mm, frame_id, depth_mm, max_force_n, source_tag,
                           timeout_s, our_goal_handle):
        """반환: (StiffnessPoint | None, error_code | None).

        error_code 가 'CANCELLED' 면 our_goal_handle 취소로 중단된 것이다.
        """
        if not self._probe_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_COMM_LOST

        goal = ProbePointAction.Goal()
        goal.target = Point(x=x_mm / 1000.0, y=y_mm / 1000.0, z=0.0)
        goal.frame_id = frame_id
        goal.max_depth_mm = depth_mm
        goal.max_force_n = max_force_n
        goal.measure_release = False
        goal.source_tag = source_tag

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
                return None, 'CANCELLED'
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._probe_goal_handle = None
                return None, ErrorCode.E_TIMEOUT
        self._probe_goal_handle = None

        result = state['result'].result
        if not result.base.success:
            return result.point, result.base.error.code
        return result.point, None

    def _probe_with_retry(self, x_mm, y_mm, frame_id, depth_mm, max_force_n, source_tag,
                           timeout_s, our_goal_handle, max_retry):
        """SDS §7.3 retry.probe_no_contact — E_NO_CONTACT 만 재시도한다."""
        attempt = 0
        while True:
            point, err = self._call_probe_point(x_mm, y_mm, frame_id, depth_mm, max_force_n,
                                                 source_tag, timeout_s, our_goal_handle)
            if err == 'CANCELLED':
                return None, 'CANCELLED'
            if err is None or point is None or point.valid:
                return point, None
            if err != ErrorCode.E_NO_CONTACT or attempt >= max_retry:
                return point, err
            attempt += 1
            self.get_logger().warn(
                f'[{err}] scan: ({x_mm:.1f},{y_mm:.1f}) 접촉 실패 — 재시도 {attempt}/{max_retry}')

    def _check_coarse_sufficiency(self, valid_points, grid_size, min_valid, max_invalid_ratio):
        """coarse_min_valid_points / invalid_point_max_ratio 두 기준을 함께 본다.

        전자는 "군집화에 쓸 표본이 절대적으로 부족한가", 후자는 "접촉 자체가
        전반적으로 안 되고 있는가(안착 위치 의심)" — 서로 다른 실패 코드를 남겨
        원인 분석이 갈리게 한다.
        """
        if len(valid_points) < min_valid:
            return ErrorCode.E_COARSE_INSUFFICIENT, (
                f'유효점 {len(valid_points)}/{grid_size} < coarse_min_valid_points({min_valid})')
        invalid_ratio = 1.0 - (len(valid_points) / grid_size if grid_size else 0.0)
        if invalid_ratio > max_invalid_ratio:
            return ErrorCode.E_NO_CONTACT, (
                f'미접촉 비율 {invalid_ratio:.0%} > invalid_point_max_ratio'
                f'({max_invalid_ratio:.0%}) — 안착 위치 의심')
        return None

    # --- 군집 평가 헬퍼 ----------------------------------------------------------
    def _cluster_eval(self, points, method):
        vals = [p.stiffness_n_per_mm for p in points]
        threshold = compute_threshold(vals, method)
        hard = [p for p in points if p.stiffness_n_per_mm >= threshold]
        soft = [p for p in points if p.stiffness_n_per_mm < threshold]
        margin = separation_margin([p.stiffness_n_per_mm for p in hard],
                                    [p.stiffness_n_per_mm for p in soft])
        return threshold, hard, soft, margin

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        p = self.get_parameter

        frame_id = goal.frame_id or p('frame_id').value
        area_x = self._val(goal.area_x_mm, 'scan_area_x_mm')
        area_y = self._val(goal.area_y_mm, 'scan_area_y_mm')
        margin = self._val(goal.margin_mm, 'scan_margin_mm')
        coarse_pitch = self._val(goal.coarse_pitch_mm, 'coarse_pitch_mm')
        coarse_min_valid = int(self._val(goal.coarse_min_valid_points, 'coarse_min_valid_points'))
        coarse_min_per_cluster = int(self._val(goal.coarse_min_per_cluster,
                                                'coarse_min_per_cluster'))
        fine_pitch = self._val(goal.fine_pitch_mm, 'fine_pitch_mm')
        boundary_band = self._val(goal.boundary_band_mm, 'boundary_band_mm')
        fine_max_points = int(self._val(goal.fine_max_points, 'fine_max_points'))
        separation_margin_min = self._val(goal.separation_margin_min, 'separation_margin_min')
        depth_mm = p('probe_depth_mm').value
        max_force_n = p('probe_max_force_n').value
        probe_timeout_s = p('probe_timeout_s').value
        cluster_method = p('cluster_method').value
        no_contact_retry = int(p('probe_no_contact_retry').value)

        def feedback(stage, last_point, points_done, points_total, candidate_count,
                     stage_pct, overall_pct):
            fb = ScanBoundary.Feedback()
            fb.stage = stage
            if last_point is not None:
                fb.last_point = last_point
            fb.points_done = points_done
            fb.points_total = points_total
            fb.candidate_count = candidate_count
            fb.stage_percent = stage_pct
            fb.overall_percent = overall_pct
            goal_handle.publish_feedback(fb)

        def aborted_by_safety_or_cancel():
            if goal_handle.is_cancel_requested:
                return 'CANCELLED'
            if not self._safe_to_move():
                return ErrorCode.E_SAFETY_BLOCKED
            return None

        def run_coarse(pitch):
            grid = make_grid(area_x, area_y, margin, pitch)
            points = {}
            n = len(grid)
            for idx, (key, (x, y)) in enumerate(grid.items()):
                halt = aborted_by_safety_or_cancel()
                if halt:
                    return grid, points, halt
                point, err = self._probe_with_retry(
                    x, y, frame_id, depth_mm, max_force_n, StiffnessPoint.SRC_COARSE,
                    probe_timeout_s, goal_handle, no_contact_retry)
                if err == 'CANCELLED':
                    return grid, points, 'CANCELLED'
                points[key] = point
                feedback('COARSE', point, idx + 1, n, 0,
                         100.0 * (idx + 1) / n, 30.0 * (idx + 1) / n)
            return grid, points, None

        # --- 1단계: 거친 스캔 ---------------------------------------------------
        coarse_grid, coarse_points, halt = run_coarse(coarse_pitch)
        if halt:
            return self._abort_result(goal_handle, halt, '거친 스캔 중 취소/안전 위반', started_at)

        def valid_list(points_dict):
            return [pt for pt in points_dict.values() if pt is not None and pt.valid]

        valid_coarse = valid_list(coarse_points)
        insufficiency = self._check_coarse_sufficiency(
            valid_coarse, len(coarse_grid), coarse_min_valid,
            p('invalid_point_max_ratio').value)
        if insufficiency:
            code, detail = insufficiency
            self._log_abort(code, detail)
            goal_handle.abort()
            result = ScanBoundary.Result()
            result.base = self._result_base(False, code, detail, started_at)
            return result

        threshold, hard, soft, margin_val = self._cluster_eval(valid_coarse, cluster_method)

        # --- 완화책: 군집 점수가 아슬아슬하면 더 촘촘한 피치로 한 번 더 (SDS §5.2 ⚠️) ---
        marginal = (len(hard) < coarse_min_per_cluster * 1.5 or
                    len(soft) < coarse_min_per_cluster * 1.5)
        if marginal:
            retry_pitch = p('coarse_retry_pitch_mm').value
            self.get_logger().warn(
                f'거친 스캔 군집 점수 아슬아슬(hard={len(hard)} soft={len(soft)}, '
                f'기준 {coarse_min_per_cluster}) — {retry_pitch}mm 로 재스캔')
            coarse_grid, coarse_points, halt = run_coarse(retry_pitch)
            if halt:
                return self._abort_result(goal_handle, halt, '거친 재스캔 중 취소/안전 위반',
                                           started_at)
            valid_coarse = valid_list(coarse_points)
            insufficiency = self._check_coarse_sufficiency(
                valid_coarse, len(coarse_grid), coarse_min_valid,
                p('invalid_point_max_ratio').value)
            if insufficiency:
                code, detail = insufficiency
                detail = f'재스캔 후에도: {detail}'
                self._log_abort(code, detail)
                goal_handle.abort()
                result = ScanBoundary.Result()
                result.base = self._result_base(False, code, detail, started_at)
                return result
            threshold, hard, soft, margin_val = self._cluster_eval(valid_coarse, cluster_method)

        if len(hard) < coarse_min_per_cluster or len(soft) < coarse_min_per_cluster or \
                margin_val < separation_margin_min:
            detail = (f'군집 부족/분리도 낮음: hard={len(hard)} soft={len(soft)} '
                      f'margin={margin_val:.2f} (기준 {separation_margin_min})')
            return self._publish_invalid_and_abort(
                goal_handle, ErrorCode.E_SEPARATION_LOW, detail, started_at, goal.session_id,
                frame_id, valid_coarse, [], threshold, margin_val, coarse_pitch, fine_pitch)

        # --- 경계 후보 선정 ------------------------------------------------------
        candidates = []
        for a_key, b_key in adjacent_pairs_4(coarse_grid):
            pa, pb = coarse_points.get(a_key), coarse_points.get(b_key)
            if pa is None or pb is None or not pa.valid or not pb.valid:
                continue
            ka, kb = pa.stiffness_n_per_mm, pb.stiffness_n_per_mm
            if (ka >= threshold) != (kb >= threshold):
                candidates.append(_midpoint(coarse_grid[a_key], coarse_grid[b_key]))
        feedback('CANDIDATE', None, 0, 0, len(candidates), 100.0, 30.0)

        # --- 2단계: 정밀 스캔 -----------------------------------------------------
        measured_coarse_xy = set(coarse_grid.values())
        tol = coarse_pitch * 0.1
        band_r = boundary_band / 2.0
        fine_full = make_grid(area_x, area_y, margin, fine_pitch)

        def already_measured(xy):
            return any(_dist(xy, m) <= tol for m in measured_coarse_xy)

        fine_targets = []
        if candidates:
            for xy in fine_full.values():
                if already_measured(xy):
                    continue
                if any(_dist(xy, c) <= band_r for c in candidates):
                    fine_targets.append(xy)

        if len(fine_targets) > fine_max_points:
            fine_targets.sort(key=lambda xy: min(_dist(xy, c) for c in candidates))
            self.get_logger().warn(
                f'정밀 격자 {len(fine_targets)}점 > fine_max_points({fine_max_points}) — '
                '후보점 근접 우선으로 절단')
            fine_targets = fine_targets[:fine_max_points]

        fine_points = []
        n_fine = len(fine_targets)
        for idx, (x, y) in enumerate(fine_targets):
            halt = aborted_by_safety_or_cancel()
            if halt:
                return self._abort_result(goal_handle, halt, '정밀 스캔 중 취소/안전 위반',
                                           started_at)
            point, err = self._probe_with_retry(
                x, y, frame_id, depth_mm, max_force_n, StiffnessPoint.SRC_FINE,
                probe_timeout_s, goal_handle, no_contact_retry)
            if err == 'CANCELLED':
                return self._abort_result(goal_handle, 'CANCELLED', '정밀 스캔 중 취소',
                                           started_at)
            if point is not None:
                fine_points.append(point)
            feedback('FINE', point, idx + 1, n_fine, len(candidates),
                     100.0 * (idx + 1) / max(1, n_fine),
                     30.0 + 70.0 * (idx + 1) / max(1, n_fine))

        # --- 판정 및 산출 ---------------------------------------------------------
        all_points = valid_coarse + [pt for pt in fine_points if pt.valid]
        threshold2, hard2, soft2, margin2 = self._cluster_eval(all_points, cluster_method)

        if margin2 < separation_margin_min:
            detail = (f'정밀 데이터 반영 후에도 분리도 부족: margin={margin2:.2f} '
                      f'(기준 {separation_margin_min})')
            return self._publish_invalid_and_abort(
                goal_handle, ErrorCode.E_SEPARATION_LOW, detail, started_at, goal.session_id,
                frame_id, valid_coarse, fine_points, threshold2, margin2, coarse_pitch, fine_pitch)

        boundary_xy = convex_hull([(pt.position.x, pt.position.y) for pt in hard2])
        forbidden_xy = convex_hull([(pt.position.x, pt.position.y) for pt in soft2])

        stiffness_map = self._build_map(
            goal.session_id, frame_id, all_points, coarse_pitch, fine_pitch,
            len(valid_coarse), len(fine_points), len(candidates), True, threshold2, margin2,
            len(hard2), len(soft2), '', boundary_xy, forbidden_xy)

        self._broadcast_nail_local_frame(frame_id, boundary_xy)
        self._store_and_publish_map(goal.session_id, stiffness_map)

        goal_handle.succeed()
        result = ScanBoundary.Result()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        result.map = stiffness_map
        return result

    # --- 실패 시 맵(valid=false) 발행 후 ABORT (NIS §6.1 에러표) -----------------
    def _publish_invalid_and_abort(self, goal_handle, code, detail, started_at, session_id,
                                    frame_id, coarse_points, fine_points, threshold, margin_val,
                                    coarse_pitch, fine_pitch):
        self._log_abort(code, detail)
        all_points = coarse_points + [pt for pt in fine_points if pt is not None]
        hard = [pt for pt in all_points if pt.valid and pt.stiffness_n_per_mm >= threshold]
        soft = [pt for pt in all_points if pt.valid and pt.stiffness_n_per_mm < threshold]
        stiffness_map = self._build_map(
            session_id, frame_id, all_points, coarse_pitch, fine_pitch, len(coarse_points),
            len(fine_points), 0, False, threshold, margin_val, len(hard), len(soft), detail,
            [], [])
        self._store_and_publish_map(session_id, stiffness_map)
        goal_handle.abort()
        result = ScanBoundary.Result()
        result.base = self._result_base(False, code, detail, started_at)
        result.map = stiffness_map
        return result

    def _abort_result(self, goal_handle, halt, context, started_at):
        result = ScanBoundary.Result()
        if halt == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, context, started_at)
            return result
        self._log_abort(ErrorCode.E_SAFETY_BLOCKED, context)
        goal_handle.abort()
        result.base = self._result_base(False, ErrorCode.E_SAFETY_BLOCKED, context, started_at)
        return result

    # --- StiffnessMap 조립 -------------------------------------------------------
    def _build_map(self, session_id, frame_id, all_points, coarse_pitch, fine_pitch,
                    coarse_count, fine_count, candidate_count, valid, threshold_k, margin_val,
                    cluster_hard, cluster_soft, reject_reason, boundary_xy, forbidden_xy):
        msg = StiffnessMap()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = frame_id
        msg.session_id = session_id
        msg.frame_id = frame_id
        msg.points = [pt for pt in all_points if pt is not None]
        msg.coarse_pitch_mm = coarse_pitch
        msg.fine_pitch_mm = fine_pitch
        msg.coarse_point_count = coarse_count
        msg.fine_point_count = fine_count
        msg.candidate_count = candidate_count
        msg.valid = valid
        msg.threshold_k_n_per_mm = threshold_k
        msg.separation_margin = margin_val
        msg.cluster_hard_count = cluster_hard
        msg.cluster_soft_count = cluster_soft
        msg.reject_reason = reject_reason

        region = BoundaryRegion()
        region.boundary_polygon = [Point(x=x, y=y, z=0.0) for x, y in boundary_xy]
        region.forbidden_polygon = [Point(x=x, y=y, z=0.0) for x, y in forbidden_xy]
        region.coat_polygon = []  # coating_node 가 boundary_offset_mm 로 자체 계산 (스캔 소관 아님)
        region.boundary_offset_mm = 0.0
        region.repeat_deviation_mm = 0.0  # 반복측정 미수행
        region.reliable = valid
        msg.region = region

        msg.created_at = self.get_clock().now().to_msg()
        return msg

    def _broadcast_nail_local_frame(self, parent_frame, boundary_xy):
        if len(boundary_xy) < 3:
            return
        cx, cy = centroid(boundary_xy)
        yaw_deg = pca_major_axis_deg(boundary_xy)
        yaw = math.radians(yaw_deg)

        from geometry_msgs.msg import TransformStamped
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = parent_frame
        t.child_frame_id = 'nail_local_frame'
        t.transform.translation.x = cx / 1000.0
        t.transform.translation.y = cy / 1000.0
        t.transform.translation.z = 0.0
        t.transform.rotation.z = math.sin(yaw / 2.0)
        t.transform.rotation.w = math.cos(yaw / 2.0)
        self._tf_broadcaster.sendTransform(t)

    def _store_and_publish_map(self, session_id, stiffness_map):
        with self._maps_lock:
            self._maps[session_id] = stiffness_map
            if len(self._maps) > 8:
                oldest = next(iter(self._maps))
                del self._maps[oldest]
        self._map_pub.publish(stiffness_map)

    # --- /scan/get_map -----------------------------------------------------------
    def _on_get_map(self, request, response):
        with self._maps_lock:
            m = self._maps.get(request.session_id)
        if m is None:
            response.found = False
            response.error.code = ErrorCode.E_INVALID_GOAL
            response.error.severity = ErrorCode.SEV_ABORT
            response.error.detail = f'session_id "{request.session_id}" 에 대한 맵 없음'
            return response
        response.found = True
        response.map = m
        return response

    # --- 공통 ---------------------------------------------------------------------
    def _result_base(self, success, code, detail, started_at):
        from nail_msgs.msg import ResultBase
        base = ResultBase()
        base.success = success
        base.error.code = code
        base.error.severity = ErrorCode.SEV_NONE if code in (ErrorCode.OK, ErrorCode.E_CANCELLED) \
            else ErrorCode.SEV_ABORT
        base.error.detail = detail
        base.duration_s = max(0.0, time.monotonic() - started_at)
        base.completed_at = self.get_clock().now().to_msg()
        return base

    def _log_abort(self, code, detail):
        self.get_logger().error(f'[{code}] scan_node: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = ScanNode()
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
