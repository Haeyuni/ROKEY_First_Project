"""공중 보정 ProbePoint를 3mm/1mm 격자로 실행해 손톱 경계를 측정한다."""
import copy
import math
import statistics
import threading
import time

import rclpy
from geometry_msgs.msg import Point
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node

from nail_msgs.action import ProbePoint, ScanBoundary
from nail_msgs.msg import BoundaryMap, ErrorCode, ResultBase
from nail_msgs.srv import ValidatePrecondition

from .geometry2d import (
    central_contact_component, grid_contour_polygon, grid_transition_midpoints, make_grid)


class ScanBoundaryNode(Node):

    def __init__(self):
        super().__init__('scan_boundary_node')
        self.declare_parameter('base_frame_id', 'base_0')

        self._probe_goal_handle = None
        self._running = False
        self._late_probe_pending = False
        self._running_lock = threading.Lock()
        self._cb_action = MutuallyExclusiveCallbackGroup()
        self._cb_client = MutuallyExclusiveCallbackGroup()
        self._validate_client = self.create_client(
            ValidatePrecondition, '/safety/validate', callback_group=self._cb_client)
        self._probe_client = ActionClient(
            self, ProbePoint, '/skill/probe_point', callback_group=self._cb_client)
        self._server = ActionServer(
            self, ScanBoundary, '/process/scan_boundary',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)
        self.get_logger().info('scan_boundary_node ready (독립 Probe 검증용)')

    def _on_cancel(self, _goal_handle):
        if self._probe_goal_handle is not None:
            self._probe_goal_handle.cancel_goal_async()
        return CancelResponse.ACCEPT

    @staticmethod
    def _unit(vector):
        length = math.sqrt(vector.x ** 2 + vector.y ** 2 + vector.z ** 2)
        if length < 1e-9:
            return None
        return vector.x / length, vector.y / length, vector.z / length

    @staticmethod
    def _dot(a, b):
        return sum(x * y for x, y in zip(a, b))

    @staticmethod
    def _distance(a, b):
        return math.sqrt(
            (a.x - b.x) ** 2 + (a.y - b.y) ** 2 + (a.z - b.z) ** 2)

    @classmethod
    def _corner_dimensions(cls, corners):
        width = (cls._distance(corners[0].position, corners[1].position)
                 + cls._distance(corners[3].position, corners[2].position)) / 2.0
        height = (cls._distance(corners[0].position, corners[3].position)
                  + cls._distance(corners[1].position, corners[2].position)) / 2.0
        return width * 1000.0, height * 1000.0

    @classmethod
    def _corners_form_rectangle(cls, corners):
        def vector(a, b):
            return b.x - a.x, b.y - a.y, b.z - a.z

        top = vector(corners[0].position, corners[1].position)
        left = vector(corners[0].position, corners[3].position)
        top_length = math.sqrt(sum(value * value for value in top))
        left_length = math.sqrt(sum(value * value for value in left))
        if top_length < 1e-6 or left_length < 1e-6:
            return False
        return abs(sum(a * b for a, b in zip(top, left)) / (top_length * left_length)) <= 0.2

    def _call_validate(self, session_id, timeout_s=5.0):
        if not self._validate_client.wait_for_service(timeout_sec=timeout_s):
            return False, ['ValidatePrecondition 서비스 연결 실패']
        request = ValidatePrecondition.Request()
        request.stage = ValidatePrecondition.Request.STAGE_PROBE
        request.session_id = session_id
        # Probe는 tool_manager 등록 전이므로 운영자의 장착 확인을 goal에서 별도로 받는다.
        request.required_tool = ''
        future = self._validate_client.call_async(request)
        deadline = time.monotonic() + timeout_s
        while not future.done():
            if time.monotonic() > deadline:
                return False, ['ValidatePrecondition 응답 타임아웃']
            time.sleep(0.02)
        response = future.result()
        if response is None:
            return False, ['ValidatePrecondition 응답 없음']
        return response.ok, list(response.blocking_reasons)

    def _on_goal(self, goal):
        with self._running_lock:
            if self._running or self._late_probe_pending:
                self.get_logger().warn('ScanBoundary REJECT: 이전 스캔/Probe가 종료되지 않음')
                return GoalResponse.REJECT
        base_frame = self.get_parameter('base_frame_id').value
        corner_count = len(goal.scan_corners)
        width_mm, height_mm = self._corner_dimensions(goal.scan_corners) \
            if corner_count == 4 else (0.0, 0.0)
        valid = (
            bool(goal.session_id)
            and goal.manual_probe_tool_confirmed
            and (not goal.frame_id or goal.frame_id == base_frame)
            and corner_count == 4
            and self._corners_form_rectangle(goal.scan_corners)
            and len(goal.dummy_references) >= 1
            and 3.0 <= width_mm <= 40.0
            and 3.0 <= height_mm <= 40.0
            and 1.0 <= goal.coarse_pitch_mm <= 5.0
            and 0.5 <= goal.fine_pitch_mm < goal.coarse_pitch_mm
            and max(goal.fine_pitch_mm, goal.coarse_pitch_mm / 2.0)
            <= goal.boundary_band_mm <= 5.0
            and 1 <= goal.fine_max_points <= 1000
            and 10.0 <= goal.air_offset_z_mm <= 150.0
            and 0.5 <= goal.max_depth_mm <= 20.0
            and 0.1 <= goal.probe_speed_mms <= 2.0
            and 0.05 <= goal.comparison_margin_n <= 2.0
            and 0.5 <= goal.max_force_n <= 5.0
            and 0.1 <= goal.lateral_force_limit_n <= goal.max_force_n
            and 1 <= goal.confirm_samples <= 10
            and 0.1 <= goal.stiffness_depth_mm <= 2.0
            and 0.0 < goal.material_min_separation_n_per_mm <= 20.0
            and 1 <= goal.reference_repeats <= 5
            and 5.0 <= goal.point_timeout_s <= 120.0)
        if valid:
            coarse = make_grid(width_mm, height_mm, goal.coarse_pitch_mm)
            valid = len(coarse) <= 400
        if not valid:
            self.get_logger().warn(
                'ScanBoundary REJECT: E_INVALID_GOAL (사각형, 기준점 또는 격자 범위 오류)')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate(goal.session_id)
        if not ok:
            self.get_logger().warn(f'ScanBoundary REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        with self._running_lock:
            if self._running:
                return GoalResponse.REJECT
            self._running = True
        return GoalResponse.ACCEPT

    @staticmethod
    def _result_base(success, code, detail, started_at, clock):
        base = ResultBase()
        base.success = success
        base.error.code = code
        base.error.severity = ErrorCode.SEV_NONE if success else ErrorCode.SEV_ABORT
        if code in (ErrorCode.E_SAFETY_BLOCKED, ErrorCode.E_OVERFORCE):
            base.error.severity = ErrorCode.SEV_SAFETY
        base.error.detail = detail
        base.duration_s = max(0.0, time.monotonic() - started_at)
        base.completed_at = clock.now().to_msg()
        return base

    @staticmethod
    def _pose_at(corners, orientation_source, x_mm, y_mm, width_mm, height_mm):
        """네 공중 모서리 안의 격자점을 bilinear 보간한다."""
        u = (x_mm + width_mm / 2.0) / width_mm
        v = (y_mm + height_mm / 2.0) / height_mm
        weights = ((1.0 - u) * (1.0 - v), u * (1.0 - v), u * v, (1.0 - u) * v)
        pose = copy.deepcopy(orientation_source)
        pose.position.x = sum(weight * corner.position.x
                              for weight, corner in zip(weights, corners))
        pose.position.y = sum(weight * corner.position.y
                              for weight, corner in zip(weights, corners))
        pose.position.z = sum(weight * corner.position.z
                              for weight, corner in zip(weights, corners))
        return pose

    def _call_probe(self, probe_goal, parent_goal, timeout_s):
        if not self._probe_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_MOTION_FAILED, 'ProbePoint 서버 연결 실패'
        sent = threading.Event()
        state = {'expired': False, 'pending_gate': False}
        state_lock = threading.Lock()

        def clear_late_probe(_future=None):
            with self._running_lock:
                self._late_probe_pending = False

        def on_goal_response(future):
            child = future.result()
            with state_lock:
                state['handle'] = child
                expired = state['expired']
            if expired and child is not None and child.accepted:
                child.cancel_goal_async()
                child.get_result_async().add_done_callback(clear_late_probe)
            elif expired:
                clear_late_probe()
            sent.set()

        self._probe_client.send_goal_async(probe_goal).add_done_callback(on_goal_response)
        send_deadline = time.monotonic() + timeout_s
        while not sent.wait(timeout=0.1):
            code = None
            detail = ''
            if parent_goal.is_cancel_requested:
                code, detail = ErrorCode.E_CANCELLED, 'ProbePoint 전송 중 사용자 취소'
            elif time.monotonic() > send_deadline:
                code, detail = ErrorCode.E_TIMEOUT, 'ProbePoint goal 전송 타임아웃'
            if code is not None:
                with state_lock:
                    state['expired'] = True
                    with self._running_lock:
                        self._late_probe_pending = True
                return None, code, detail
        child = state.get('handle')
        if child is None or not child.accepted:
            return None, ErrorCode.E_MOTION_FAILED, 'ProbePoint goal 거부됨'
        self._probe_goal_handle = child

        done = threading.Event()

        def on_result(future):
            with state_lock:
                state['result'] = future.result().result
                pending_gate = state['pending_gate']
                done.set()
            if pending_gate:
                clear_late_probe()

        child.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        cancel_reason = None
        cancel_detail = ''
        cancel_deadline = None
        while not done.wait(timeout=0.1):
            if parent_goal.is_cancel_requested and cancel_reason is None:
                child.cancel_goal_async()
                cancel_reason = ErrorCode.E_CANCELLED
                cancel_detail = '사용자 취소를 ProbePoint에 전파함'
                cancel_deadline = time.monotonic() + 15.0
            elif time.monotonic() > deadline and cancel_reason is None:
                child.cancel_goal_async()
                cancel_reason = ErrorCode.E_TIMEOUT
                cancel_detail = 'ProbePoint 결과 타임아웃 뒤 취소함'
                cancel_deadline = time.monotonic() + 15.0
            if cancel_deadline is not None and time.monotonic() > cancel_deadline:
                self._probe_goal_handle = None
                with state_lock:
                    if done.is_set():
                        continue
                    state['pending_gate'] = True
                    with self._running_lock:
                        self._late_probe_pending = True
                return None, ErrorCode.E_COMM_LOST, 'ProbePoint 취소 완료를 확인하지 못함'
        self._probe_goal_handle = None
        if cancel_reason is not None:
            return state.get('result'), cancel_reason, cancel_detail
        child_result = state['result']
        if not child_result.base.success:
            return child_result, child_result.base.error.code, child_result.base.error.detail
        return child_result, None, ''

    def _probe_goal(self, goal, pose, source):
        probe = ProbePoint.Goal()
        probe.search_start = pose
        # 경계 스캔은 PickPlace 뒤에도 항상 base 좌표계 수직 아래로 누른다.
        probe.press_direction.x = 0.0
        probe.press_direction.y = 0.0
        probe.press_direction.z = -1.0
        probe.frame_id = goal.frame_id
        probe.source = source
        probe.air_offset_z_mm = goal.air_offset_z_mm
        probe.max_depth_mm = goal.max_depth_mm
        probe.probe_speed_mms = goal.probe_speed_mms
        probe.comparison_margin_n = goal.comparison_margin_n
        probe.max_force_n = goal.max_force_n
        probe.lateral_force_limit_n = goal.lateral_force_limit_n
        probe.confirm_samples = goal.confirm_samples
        probe.stiffness_depth_mm = goal.stiffness_depth_mm
        probe.timeout_s = goal.point_timeout_s
        probe.manual_probe_tool_confirmed = goal.manual_probe_tool_confirmed
        return probe

    @staticmethod
    def _probe_wait_timeout(point_timeout_s):
        # ProbePoint 한 점은 공중/실제 각각 접근, 탐색, 복귀를 수행한다.
        return point_timeout_s * 6.0 + 5.0

    def _measure_references(self, goal, parent_goal, poses, source):
        measurements = []
        for pose in poses:
            for _ in range(goal.reference_repeats):
                probe_result, code, detail = self._call_probe(
                    self._probe_goal(goal, pose, source), parent_goal,
                    self._probe_wait_timeout(goal.point_timeout_s))
                if code is not None:
                    return None, code, detail
                measurement = probe_result.measurement
                if (not measurement.valid or not measurement.contact_detected
                        or measurement.stiffness_n_per_mm <= 0.0):
                    return None, ErrorCode.E_NO_BOUNDARY, \
                        '기준점에서 유효한 강성 측정을 얻지 못함'
                measurements.append(measurement)
        return measurements, None, ''

    @staticmethod
    def _is_nail_material(measurement, nail_stiffness, dummy_stiffness):
        if (not measurement.valid or not measurement.contact_detected
                or measurement.stiffness_n_per_mm <= 0.0):
            return False
        stiffness = measurement.stiffness_n_per_mm
        return abs(stiffness - nail_stiffness) < abs(stiffness - dummy_stiffness)

    def _seed_index(self, grid, classifications, corners, nail_reference,
                    width_mm, height_mm):
        candidates = [entry for entry in grid if classifications[(entry[0], entry[1])]]
        if not candidates:
            return None
        return min(
            candidates,
            key=lambda entry: self._distance(
                self._pose_at(corners, nail_reference, entry[2], entry[3],
                              width_mm, height_mm).position,
                nail_reference.position))[:2]

    def _publish_feedback(self, goal_handle, stage, measurement, done, total, candidates):
        feedback = ScanBoundary.Feedback()
        feedback.stage = stage
        if measurement is not None:
            feedback.last_measurement = measurement
        feedback.points_done = done
        feedback.points_total = total
        feedback.boundary_candidate_count = candidates
        feedback.percent = 100.0 * done / max(1, total)
        goal_handle.publish_feedback(feedback)

    def _finish_error(self, goal_handle, result, code, detail, started_at):
        if code == ErrorCode.E_CANCELLED:
            goal_handle.canceled()
        else:
            goal_handle.abort()
        result.base = self._result_base(False, code, detail, started_at, self.get_clock())
        return result

    def _execute(self, goal_handle):
        started_at = time.monotonic()
        try:
            return self._execute_scan(goal_handle, started_at)
        except Exception as exc:
            self.get_logger().error(f'ScanBoundary 예외 중단: {exc}')
            result = ScanBoundary.Result()
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_MOTION_FAILED, str(exc),
                started_at, self.get_clock())
            return result
        finally:
            with self._running_lock:
                self._running = False

    def _execute_scan(self, goal_handle, started_at):
        goal = goal_handle.request
        result = ScanBoundary.Result()
        corners = goal.scan_corners
        width_mm, height_mm = self._corner_dimensions(corners)
        nail_references, code, detail = self._measure_references(
            goal, goal_handle, [goal.nail_reference],
            ProbePoint.Goal.SOURCE_NAIL_REFERENCE)
        if code is not None:
            return self._finish_error(goal_handle, result, code, detail, started_at)
        dummy_references, code, detail = self._measure_references(
            goal, goal_handle, goal.dummy_references,
            ProbePoint.Goal.SOURCE_DUMMY_REFERENCE)
        if code is not None:
            return self._finish_error(goal_handle, result, code, detail, started_at)

        nail_stiffness = statistics.median(
            item.stiffness_n_per_mm for item in nail_references)
        dummy_stiffness = statistics.median(
            item.stiffness_n_per_mm for item in dummy_references)
        if abs(nail_stiffness - dummy_stiffness) < goal.material_min_separation_n_per_mm:
            detail = (
                f'손톱/더미 강성 차이={abs(nail_stiffness - dummy_stiffness):.3f}N/mm가 '
                f'최소값={goal.material_min_separation_n_per_mm:.3f}N/mm보다 작음')
            return self._finish_error(
                goal_handle, result, ErrorCode.E_NO_BOUNDARY, detail, started_at)

        coarse_grid = make_grid(width_mm, height_mm, goal.coarse_pitch_mm)
        measurements = []
        coarse_classes = {}
        reference_count = len(nail_references) + len(dummy_references)
        total = reference_count + len(coarse_grid)

        for ix, iy, x_mm, y_mm in coarse_grid:
            pose = self._pose_at(
                corners, goal.nail_reference, x_mm, y_mm, width_mm, height_mm)
            probe_result, code, detail = self._call_probe(
                self._probe_goal(goal, pose, ProbePoint.Goal.SOURCE_COARSE),
                goal_handle, self._probe_wait_timeout(goal.point_timeout_s))
            if code is not None:
                return self._finish_error(goal_handle, result, code, detail, started_at)
            measurement = probe_result.measurement
            measurements.append(measurement)
            coarse_classes[(ix, iy)] = self._is_nail_material(
                measurement, nail_stiffness, dummy_stiffness)
            self._publish_feedback(
                goal_handle, ScanBoundary.Feedback.STAGE_COARSE,
                measurement, reference_count + len(measurements), total, 0)

        seed_index = self._seed_index(
            coarse_grid, coarse_classes, corners, goal.nail_reference, width_mm, height_mm)
        component = central_contact_component(coarse_grid, coarse_classes, seed_index)
        coarse_classes = {
            index: index in component for index in coarse_classes
        }
        coarse_transitions = grid_transition_midpoints(coarse_grid, coarse_classes)
        if len(coarse_transitions) < 3:
            detail = ('거친 격자에서 손톱 안/밖 전환을 3개 이상 찾지 못함; '
                      '탐색 영역, 시작 높이 또는 힘 임계값 확인 필요')
            return self._finish_error(
                goal_handle, result, ErrorCode.E_NO_BOUNDARY, detail, started_at)

        fine_grid_all = make_grid(width_mm, height_mm, goal.fine_pitch_mm)
        fine_grid = [entry for entry in fine_grid_all
                     if min(math.hypot(entry[2] - x, entry[3] - y)
                            for x, y in coarse_transitions) <= goal.boundary_band_mm]
        if len(fine_grid) > goal.fine_max_points:
            detail = (f'정밀 후보 {len(fine_grid)}개가 fine_max_points='
                      f'{goal.fine_max_points}를 초과함')
            return self._finish_error(
                goal_handle, result, ErrorCode.E_INVALID_GOAL, detail, started_at)

        fine_classes = {}
        total += len(fine_grid)
        for ix, iy, x_mm, y_mm in fine_grid:
            pose = self._pose_at(
                corners, goal.nail_reference, x_mm, y_mm, width_mm, height_mm)
            probe_result, code, detail = self._call_probe(
                self._probe_goal(goal, pose, ProbePoint.Goal.SOURCE_FINE),
                goal_handle, self._probe_wait_timeout(goal.point_timeout_s))
            if code is not None:
                return self._finish_error(goal_handle, result, code, detail, started_at)
            measurement = probe_result.measurement
            measurements.append(measurement)
            fine_classes[(ix, iy)] = self._is_nail_material(
                measurement, nail_stiffness, dummy_stiffness)
            self._publish_feedback(
                goal_handle, ScanBoundary.Feedback.STAGE_FINE,
                measurement, reference_count + len(measurements), total,
                len(coarse_transitions))

        fine_seed = self._seed_index(
            fine_grid, fine_classes, corners, goal.nail_reference, width_mm, height_mm)
        fine_component = central_contact_component(fine_grid, fine_classes, fine_seed)
        fine_classes = {index: index in fine_component for index in fine_classes}
        fine_transitions = grid_transition_midpoints(fine_grid, fine_classes)
        if len(fine_transitions) < 3:
            return self._finish_error(
                goal_handle, result, ErrorCode.E_NO_BOUNDARY,
                '정밀 격자에서 경계를 다시 검출하지 못함', started_at)
        boundary_offsets = grid_contour_polygon(fine_grid, fine_classes)
        if len(boundary_offsets) < 3:
            return self._finish_error(
                goal_handle, result, ErrorCode.E_NO_BOUNDARY,
                '유효한 경계 다각형을 만들 수 없음', started_at)

        boundary_map = BoundaryMap()
        boundary_map.header.stamp = self.get_clock().now().to_msg()
        boundary_map.header.frame_id = goal.frame_id or self.get_parameter('base_frame_id').value
        boundary_map.session_id = goal.session_id
        boundary_map.measurements = measurements
        for x_mm, y_mm in boundary_offsets:
            pose = self._pose_at(
                corners, goal.nail_reference, x_mm, y_mm, width_mm, height_mm)
            boundary_map.boundary_polygon.append(Point(
                x=pose.position.x, y=pose.position.y, z=pose.position.z))
        boundary_map.coarse_pitch_mm = goal.coarse_pitch_mm
        boundary_map.fine_pitch_mm = goal.fine_pitch_mm
        boundary_map.coarse_point_count = len(coarse_grid)
        boundary_map.fine_point_count = len(fine_grid)
        boundary_map.boundary_candidate_count = len(boundary_offsets)
        boundary_map.contact_ratio = (
            sum(item.contact_detected for item in measurements) / len(measurements))
        boundary_map.nail_reference_stiffness_n_per_mm = nail_stiffness
        boundary_map.dummy_reference_stiffness_n_per_mm = dummy_stiffness
        boundary_map.material_separation_n_per_mm = abs(nail_stiffness - dummy_stiffness)
        boundary_map.valid = True
        result.map = boundary_map
        goal_handle.succeed()
        result.base = self._result_base(
            True, ErrorCode.OK, '', started_at, self.get_clock())
        self.get_logger().info(
            f'ScanBoundary 완료: coarse={len(coarse_grid)}, fine={len(fine_grid)}, '
            f'boundary={len(boundary_offsets)}')
        return result


def main(args=None):
    rclpy.init(args=args)
    node = ScanBoundaryNode()
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
