"""robot_skill_node — A계층 스킬 프리미티브 (NIS §5.1, SDS §6.1).

두산 컨트롤러를 감싸서 상위(공정) 노드가 어떤 두산 서비스를 어떤 순서로
부르는지 몰라도 되게 만든다. 두산 API 호출은 `dsr_adapter.DsrAdapter`
안에만 있고, 이 파일은 그 래퍼만 호출한다 (SDS §4.1).

제공: /skill/move_to, /skill/pick_place, /skill/contact_path,
      /skill/lateral_contact, /skill/probe_point (Action)
      /force/data(100Hz), /force/data_ui(20Hz), /robot/pose(50Hz) (Topic)

참고로 삼은 문서: docs/노드별_인터페이스명세서_v0.2.md §5.1 (사용자 지정),
docs/개발명세서_SDS.md §3~5, docs/인터페이스정의서_IDS.md (실제 필드명 — nail_msgs).

이 노드는 NIS §2 인터페이스 매트릭스에 `ValidatePrecondition` 이 소비
목록에 없으므로(스킬 계층은 "stage" 개념이 없다) 그 호출은 하지 않는다 —
그건 stage 를 아는 B계층 공정 노드의 몫이다 (§3.1 ④는 공정 노드가 수행).
"""
import math
import threading
import time

import rclpy
import tf2_geometry_msgs  # noqa: F401  (side-effect: PoseStamped 변환자 등록)
import yaml
from geometry_msgs.msg import Point, Pose, PoseStamped, Vector3
from rclpy.action import ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup, ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from tf2_ros import Buffer, TransformListener

from nail_msgs.action import ContactPath, LateralContact, MoveTo, PickPlace, ProbePoint
from nail_msgs.msg import ErrorCode, ForceSample, ResultBase, SafetyState, StiffnessPoint

from .conversions import TaskPose, pose_to_task_pose, task_pose_to_ros_pose
from .dsr_adapter import DsrAdapter, DsrAdapterError
from .regression import compute_stiffness

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_NO_CONTACT: ErrorCode.SEV_RETRY,
    ErrorCode.E_LATERAL_JAM: ErrorCode.SEV_RETRY,
    ErrorCode.E_GRIP_FAILED: ErrorCode.SEV_RETRY,
    ErrorCode.E_OVERFORCE: ErrorCode.SEV_ABORT,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_COMM_LOST: ErrorCode.SEV_ABORT,
    ErrorCode.E_LOW_STIFFNESS: ErrorCode.SEV_SAFETY,
    ErrorCode.E_LATERAL_LIMIT: ErrorCode.SEV_SAFETY,
    ErrorCode.E_TOOL_DROP: ErrorCode.SEV_SAFETY,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


class RobotSkillNode(Node):

    def __init__(self):
        super().__init__('robot_skill_node')
        self._declare_parameters()

        p = self.get_parameter
        self._base_frame_id = p('base_frame_id').value
        self._retreat_mm = p('retreat_mm').value

        self._latest_safety = None
        safety_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                                 durability=DurabilityPolicy.TRANSIENT_LOCAL)
        best_effort_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.BEST_EFFORT,
                                      durability=DurabilityPolicy.VOLATILE)

        self._cb_monitor = ReentrantCallbackGroup()
        self._cb_skill = MutuallyExclusiveCallbackGroup()

        self.create_subscription(SafetyState, p('safety_topic').value,
                                  self._on_safety_status, safety_qos,
                                  callback_group=self._cb_monitor)

        self._tf_buffer = Buffer()
        self._tf_listener = TransformListener(self._tf_buffer, self)

        try:
            self._adapter = DsrAdapter(self, p('dsr_prefix').value, p('robot_model').value)
        except DsrAdapterError as e:
            self.get_logger().error(str(e))
            raise

        self._force_pub = self.create_publisher(ForceSample, '/force/data', best_effort_qos)
        self._force_ui_pub = self.create_publisher(ForceSample, '/force/data_ui', best_effort_qos)
        self._pose_pub = self.create_publisher(PoseStamped, '/robot/pose', best_effort_qos)

        self._wrench_lock = threading.Lock()
        self._filtered_wrench = None
        self.create_timer(1.0 / p('force_pub_rate_hz').value, self._on_force_timer,
                           callback_group=self._cb_monitor)
        self.create_timer(1.0 / p('force_ui_rate_hz').value, self._on_force_ui_timer,
                           callback_group=self._cb_monitor)
        self.create_timer(1.0 / p('pose_pub_rate_hz').value, self._on_pose_timer,
                           callback_group=self._cb_monitor)

        self._targets_cache = None

        self._move_to_server = ActionServer(
            self, MoveTo, '/skill/move_to',
            execute_callback=self._execute_move_to,
            goal_callback=self._on_goal_move_to,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_skill)
        self._pick_place_server = ActionServer(
            self, PickPlace, '/skill/pick_place',
            execute_callback=self._execute_pick_place,
            goal_callback=self._on_goal_pick_place,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_skill)
        self._contact_path_server = ActionServer(
            self, ContactPath, '/skill/contact_path',
            execute_callback=self._execute_contact_path,
            goal_callback=self._on_goal_contact_path,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_skill)
        self._lateral_contact_server = ActionServer(
            self, LateralContact, '/skill/lateral_contact',
            execute_callback=self._execute_lateral_contact,
            goal_callback=self._on_goal_lateral_contact,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_skill)
        self._probe_point_server = ActionServer(
            self, ProbePoint, '/skill/probe_point',
            execute_callback=self._execute_probe_point,
            goal_callback=self._on_goal_probe_point,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_skill)

        self.get_logger().info('robot_skill_node ready')

    def destroy_node(self):
        self._adapter.destroy()
        super().destroy_node()

    # --- 파라미터 (NIS §5.1 표 + SDS §3.6 공통) --------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        # 공통 (SDS §3.6 / NIS §3.6)
        d('dsr_prefix', 'dsr01')
        d('robot_model', 'm0609')
        d('base_frame_id', 'base_0')
        d('node_timeout_s', 120.0)
        d('safety_topic', '/safety/status')
        d('log_force_data', False)
        # use_mock_hardware: NIS §3.6 공통 파라미터로 선언만 한다. 이 노드는
        # mock 분기를 두지 않는다 — 로봇 없이 검증할 때는 두산 공식 가상
        # 모드(SDS §2.3)를 dsr_bringup2 에 `mode:=virtual` 로 띄운다.
        d('use_mock_hardware', False)
        d('retreat_mm', 10.0)
        # 발행 주기
        d('force_pub_rate_hz', 100)
        d('force_ui_rate_hz', 20)
        d('pose_pub_rate_hz', 50)
        d('ft_filter_cutoff_hz', 10.0)
        # 이동 기본값
        d('approach_height_mm', 20.0)
        d('default_max_force_n', 5.0)
        d('motion_timeout_s', 30.0)
        d('move_max_speed_mms', 100.0)
        d('move_max_accel_mms2', 200.0)
        d('move_pose_tolerance_mm', 1.0)
        # ProbePoint
        d('probe_speed_mms', 2.0)
        d('probe_contact_threshold_n', 0.3)
        d('probe_max_depth_mm', 1.0)
        d('probe_max_force_n', 3.0)
        d('probe_regression_min_samples', 10)
        d('release_speed_mms', 1.0)
        d('probe_search_step_mm', 0.1)
        # ContactPath
        d('contact_search_speed_mms', 5.0)
        d('contact_search_max_depth_mm', 15.0)
        d('compliance_stiffness', [500.0, 500.0, 200.0, 50.0, 50.0, 50.0])
        d('contact_force_ramp_mms', 1.0)
        d('contact_stiffness_window', 20)
        # LateralContact
        d('lateral_search_speed_mms', 3.0)
        d('lateral_contact_threshold_n', 0.3)
        d('lateral_max_travel_mm', 8.0)
        d('lateral_jam_force_n', 4.0)
        d('lateral_retreat_mm', 10.0)
        d('lateral_search_step_mm', 0.5)
        # 그리퍼
        d('grip_width_tolerance_mm', 1.0)
        d('tool_drop_width_delta_mm', 2.0)
        d('gripper_settle_s', 1.0)
        d('targets_yaml_path', '')

    # --- 안전 -----------------------------------------------------------------
    def _on_safety_status(self, msg: SafetyState):
        self._latest_safety = msg

    def _safe_to_move(self) -> bool:
        return self._latest_safety is not None and self._latest_safety.safe_to_move

    def _on_cancel(self, goal_handle):
        return CancelResponse.ACCEPT

    # --- 힘/자세 퍼블리시 (SDS §6.1: 별도 타이머 100/20/50 Hz) ------------------
    def _sample_filtered_wrench(self):
        try:
            w = self._adapter.read_wrench()
        except DsrAdapterError:
            return None
        alpha = 1.0  # 컷오프 필터는 아래에서 dt 기반으로 적용
        with self._wrench_lock:
            if self._filtered_wrench is None:
                self._filtered_wrench = w
            else:
                cutoff = self.get_parameter('ft_filter_cutoff_hz').value
                dt = 1.0 / self.get_parameter('force_pub_rate_hz').value
                rc = 1.0 / (2.0 * math.pi * max(cutoff, 0.01))
                alpha = dt / (rc + dt)
                f = self._filtered_wrench
                w = type(w)(
                    f.fx_n + alpha * (w.fx_n - f.fx_n),
                    f.fy_n + alpha * (w.fy_n - f.fy_n),
                    f.fz_n + alpha * (w.fz_n - f.fz_n),
                    f.tx_nm + alpha * (w.tx_nm - f.tx_nm),
                    f.ty_nm + alpha * (w.ty_nm - f.ty_nm),
                    f.tz_nm + alpha * (w.tz_nm - f.tz_nm),
                )
            self._filtered_wrench = w
        return w

    def _wrench_to_msg(self, w):
        fs = ForceSample()
        fs.stamp = self.get_clock().now().to_msg()
        fs.fx_n, fs.fy_n, fs.fz_n = w.fx_n, w.fy_n, w.fz_n
        fs.tx_nm, fs.ty_nm, fs.tz_nm = w.tx_nm, w.ty_nm, w.tz_nm
        return fs

    def _on_force_timer(self):
        w = self._sample_filtered_wrench()
        if w is None:
            return
        self._force_pub.publish(self._wrench_to_msg(w))

    def _on_force_ui_timer(self):
        with self._wrench_lock:
            w = self._filtered_wrench
        if w is None:
            return
        self._force_ui_pub.publish(self._wrench_to_msg(w))

    def _on_pose_timer(self):
        try:
            pose = self._adapter.get_pose()
        except DsrAdapterError:
            return
        msg = PoseStamped()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = self._base_frame_id
        msg.pose = task_pose_to_ros_pose(pose)
        self._pose_pub.publish(msg)

    # --- 공통 헬퍼 --------------------------------------------------------------
    def _transform_pose_to_base(self, pose: Pose, frame_id: str) -> Pose:
        if not frame_id or frame_id == self._base_frame_id:
            return pose
        stamped = PoseStamped()
        stamped.header.frame_id = frame_id
        stamped.header.stamp = rclpy.time.Time().to_msg()
        stamped.pose = pose
        return self._tf_buffer.transform(
            stamped, self._base_frame_id, timeout=rclpy.duration.Duration(seconds=1.0)
        ).pose

    def _transform_point_to_base(self, point: Point, frame_id: str) -> Point:
        pose = Pose()
        pose.position = point
        pose.orientation.w = 1.0
        return self._transform_pose_to_base(pose, frame_id).position

    def _result_base(self, success, code, detail, started_at, final_wrench=None):
        base = ResultBase()
        base.success = success
        base.error.code = code
        base.error.severity = _severity_for(code)
        base.error.detail = detail
        try:
            pose = self._adapter.get_pose()
            base.final_pose = task_pose_to_ros_pose(pose)
        except DsrAdapterError:
            pass
        if final_wrench is not None:
            base.final_fz_n = final_wrench.fz_n
        base.duration_s = max(0.0, time.monotonic() - started_at)
        base.completed_at = self.get_clock().now().to_msg()
        return base

    def _log_abort(self, code, detail):
        self.get_logger().error(f'[{code}] {detail}')

    def _monitor(self, goal_handle, timeout_s, poll_hz, on_tick):
        """이동/접촉 루프 공통 감시자. 취소·안전 확인 후 'ok'/'cancel'/'safety'/'timeout' 반환."""
        reason = {'value': 'ok'}

        def should_abort():
            if goal_handle.is_cancel_requested:
                reason['value'] = 'cancel'
                return True
            if not self._safe_to_move():
                reason['value'] = 'safety'
                return True
            return False

        completed = self._adapter.wait_motion_done(
            timeout_s, poll_hz, on_tick=on_tick, should_abort=should_abort)
        if not completed and reason['value'] == 'ok':
            reason['value'] = 'timeout'
        return reason['value'] if not completed else 'ok'

    def _finish_from_reason(self, reason, goal_handle, started_at, ok_code=ErrorCode.OK,
                             ok_detail='', context=''):
        """reason(_monitor 결과)에 따라 goal_handle 을 종결하고 ResultBase 를 만든다."""
        if reason == 'ok':
            goal_handle.succeed()
            return self._result_base(True, ok_code, ok_detail, started_at)
        if reason == 'cancel':
            goal_handle.canceled()
            return self._result_base(False, ErrorCode.E_CANCELLED, f'{context}: 사용자 취소',
                                      started_at)
        if reason == 'safety':
            self._log_abort(ErrorCode.E_SAFETY_BLOCKED, f'{context}: safe_to_move=false')
            goal_handle.abort()
            return self._result_base(False, ErrorCode.E_SAFETY_BLOCKED,
                                      f'{context}: 안전 상태 위반으로 중단', started_at)
        # timeout
        self._log_abort(ErrorCode.E_TIMEOUT, f'{context}: 타임아웃')
        goal_handle.abort()
        return self._result_base(False, ErrorCode.E_TIMEOUT, f'{context}: 타임아웃', started_at)

    # =========================================================================
    # MoveTo
    # =========================================================================
    def _on_goal_move_to(self, goal_request):
        if not (0.0 < goal_request.speed_ratio <= 1.0):
            self.get_logger().warn('MoveTo REJECT: E_INVALID_GOAL (speed_ratio 범위 밖)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('MoveTo REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _execute_move_to(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()

        try:
            base_pose = self._transform_pose_to_base(goal.target, goal.frame_id)
        except Exception as e:
            goal_handle.abort()
            result = MoveTo.Result()
            result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                             f'TF 변환 실패 ({goal.frame_id}): {e}', started_at)
            return result

        task_pose = pose_to_task_pose(base_pose)
        accel_ratio = goal.accel_ratio if goal.accel_ratio > 0.0 else goal.speed_ratio
        vel = max(1.0, goal.speed_ratio * self.get_parameter('move_max_speed_mms').value)
        acc = max(1.0, accel_ratio * self.get_parameter('move_max_accel_mms2').value)
        timeout_s = goal.timeout_s if goal.timeout_s > 0.0 else \
            self.get_parameter('motion_timeout_s').value

        if goal.linear:
            self._adapter.start_move_line(task_pose, vel, acc)
        else:
            self._adapter.start_move_joint_to_pose(task_pose, vel, acc)

        def on_tick():
            fb = MoveTo.Feedback()
            try:
                pose = self._adapter.get_pose()
                fb.current_pose = task_pose_to_ros_pose(pose)
            except DsrAdapterError:
                pass
            fb.percent = min(99.0, 100.0 * (time.monotonic() - started_at) / max(timeout_s, 0.1))
            goal_handle.publish_feedback(fb)

        reason = self._monitor(goal_handle, timeout_s, 10.0, on_tick)
        result = MoveTo.Result()
        if reason == 'ok':
            try:
                final_pose = self._adapter.get_pose()
                err_mm = math.dist(
                    (final_pose.x_mm, final_pose.y_mm, final_pose.z_mm),
                    (task_pose.x_mm, task_pose.y_mm, task_pose.z_mm))
            except DsrAdapterError:
                err_mm = -1.0
            result.position_error_mm = err_mm
        result.base = self._finish_from_reason(reason, goal_handle, started_at, context='MoveTo')
        return result

    # =========================================================================
    # PickPlace
    # =========================================================================
    def _load_targets(self):
        if self._targets_cache is not None:
            return self._targets_cache
        path = self.get_parameter('targets_yaml_path').value
        targets = {}
        if path:
            try:
                with open(path) as f:
                    targets = yaml.safe_load(f) or {}
            except OSError as e:
                self.get_logger().error(f'targets_yaml_path 로드 실패: {e}')
        self._targets_cache = targets
        return targets

    def _on_goal_pick_place(self, goal_request):
        if goal_request.mode not in (PickPlace.Goal.MODE_PICK, PickPlace.Goal.MODE_PLACE):
            self.get_logger().warn('PickPlace REJECT: E_INVALID_GOAL (mode)')
            return GoalResponse.REJECT
        if not goal_request.target_key:
            self.get_logger().warn('PickPlace REJECT: E_INVALID_GOAL (target_key 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('PickPlace REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _target_task_pose(self, target_key, frame_id):
        entry = self._load_targets().get(target_key)
        if entry is not None:
            pose = Pose()
            pose.position.x = entry.get('x_mm', 0.0) / 1000.0
            pose.position.y = entry.get('y_mm', 0.0) / 1000.0
            pose.position.z = entry.get('z_mm', 0.0) / 1000.0
            pose.orientation.w = 1.0
            base_pose = self._transform_pose_to_base(pose, entry.get('frame_id', frame_id))
            tp = pose_to_task_pose(base_pose)
            tp.rz1_deg = entry.get('rz1_deg', tp.rz1_deg)
            tp.ry_deg = entry.get('ry_deg', tp.ry_deg)
            tp.rz2_deg = entry.get('rz2_deg', tp.rz2_deg)
            return tp
        # targets_yaml_path 에 없으면 target_key 자체를 TF 프레임 이름으로
        # 취급한다 (예: tool_rack 의 slot_* 프레임 — NIS §11.4). 그 프레임의
        # 원점 자세를 그대로 접근 자세로 쓴다 — 랙 슬롯은 CAD 로 고정된 자세를
        # 갖고 있어 방향까지 함께 얻을 수 있다.
        identity = Pose()
        identity.orientation.w = 1.0
        base_pose = self._transform_pose_to_base(identity, target_key)
        return pose_to_task_pose(base_pose)

    def _execute_pick_place(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = PickPlace.Result()
        speed = self.get_parameter('move_max_speed_mms').value * 0.3
        accel = self.get_parameter('move_max_accel_mms2').value * 0.3
        timeout_s = self.get_parameter('motion_timeout_s').value

        def feedback(step, percent):
            fb = PickPlace.Feedback()
            fb.step = step
            fb.percent = percent
            goal_handle.publish_feedback(fb)

        try:
            target = self._target_task_pose(goal.target_key, goal.frame_id)
        except Exception as e:
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_INVALID_GOAL,
                f'target_key "{goal.target_key}" 를 targets_yaml_path 에서도, TF 프레임으로도 '
                f'찾을 수 없음: {e}', started_at)
            return result

        approach = TaskPose(target.x_mm, target.y_mm,
                             target.z_mm + goal.approach_height_mm,
                             target.rz1_deg, target.ry_deg, target.rz2_deg)

        def move_and_wait(pose, step, pct):
            self._adapter.start_move_line(pose, speed, accel)
            return self._monitor(goal_handle, timeout_s, 10.0,
                                  lambda: feedback(step, pct))

        for pose, step, pct in ((approach, 0, 10.0), (target, 1, 30.0)):
            reason = move_and_wait(pose, step, pct)
            if reason != 'ok':
                result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                         context='PickPlace')
                return result

        feedback(2, 60.0)
        if goal.mode == PickPlace.Goal.MODE_PICK:
            width = goal.grip_width_mm if goal.grip_width_mm > 0.0 else goal.expected_width_mm
            grip_ok = self._adapter.gripper_set_width(width)
        else:
            grip_ok = self._adapter.gripper_open()
        time.sleep(self.get_parameter('gripper_settle_s').value)

        if not grip_ok:
            self._adapter.start_move_line(approach, speed, accel)
            self._monitor(goal_handle, timeout_s, 10.0, lambda: None)
            goal_handle.abort()
            self._log_abort(ErrorCode.E_GRIP_FAILED, f'PickPlace {goal.mode}: 그리퍼 명령 실패')
            result.base = self._result_base(False, ErrorCode.E_GRIP_FAILED,
                                              '그리퍼 명령 실패', started_at)
            return result

        measured = goal.expected_width_mm if goal.mode == PickPlace.Goal.MODE_PICK else 0.0
        grip_verified = True
        if goal.mode == PickPlace.Goal.MODE_PICK and goal.verify_grip:
            feedback(3, 80.0)
            # 이 그리퍼 드라이버는 폭 되읽기를 노출하지 않는다 — 명령값을
            # 그대로 measured_width_mm 에 반영한다 (실측 아님. 이전
            # tool_manager 구현과 동일한 하드웨어 제약).
            tol = goal.width_tolerance_mm if goal.width_tolerance_mm > 0.0 else \
                self.get_parameter('grip_width_tolerance_mm').value
            grip_verified = abs(measured - goal.expected_width_mm) <= tol

        feedback(4, 90.0)
        reason = move_and_wait(approach, 4, 100.0)
        result.measured_width_mm = measured
        result.grip_verified = grip_verified
        result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                context='PickPlace')
        return result

    # =========================================================================
    # ContactPath — 법선(+Z) 접근
    # =========================================================================
    def _on_goal_contact_path(self, goal_request):
        if len(goal_request.waypoints) == 0:
            self.get_logger().warn('ContactPath REJECT: E_INVALID_GOAL (waypoints 비어있음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('ContactPath REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _cleanup_normal(self, goal_handle, retreat_mm=None):
        """법선 접근 이탈: +Z 후퇴 후 컴플라이언스/힘 해제 (SDS §3.2)."""
        try:
            self._adapter.start_move_rel_tool_z(
                (retreat_mm or self._retreat_mm), 10.0, 50.0)
            self._monitor(goal_handle, 10.0, 10.0, lambda: None)
        finally:
            self._adapter.compliance_off()
            self._adapter.release_force()

    def _execute_contact_path(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = ContactPath.Result()
        force_log = []
        max_force_measured = 0.0
        max_force_seen = 0.0
        stiffness_samples = []
        min_stiffness_measured = None
        missed = []
        timeout_s = goal.max_duration_s if goal.max_duration_s > 0.0 else \
            self.get_parameter('motion_timeout_s').value
        search_speed = self.get_parameter('contact_search_speed_mms').value
        search_max_depth = self.get_parameter('contact_search_max_depth_mm').value
        contact_threshold = self.get_parameter('probe_contact_threshold_n').value
        max_force_n = goal.max_force_n if goal.max_force_n > 0.0 else \
            self.get_parameter('default_max_force_n').value

        def feedback(pct, current_pass, w=None):
            fb = ContactPath.Feedback()
            fb.percent = pct
            fb.current_pass = current_pass
            if w is not None:
                fb.current_wrench.fx_n, fb.current_wrench.fy_n, fb.current_wrench.fz_n = \
                    w.fx_n, w.fy_n, w.fz_n
                fb.current_wrench.tx_nm, fb.current_wrench.ty_nm, fb.current_wrench.tz_nm = \
                    w.tx_nm, w.ty_nm, w.tz_nm
            goal_handle.publish_feedback(fb)

        try:
            start_base = self._transform_pose_to_base(goal.waypoints[0], goal.frame_id)
        except Exception as e:
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                              f'TF 변환 실패: {e}', started_at)
            return result
        start_pose = pose_to_task_pose(start_base)
        approach = TaskPose(start_pose.x_mm, start_pose.y_mm,
                             start_pose.z_mm + self.get_parameter('approach_height_mm').value,
                             start_pose.rz1_deg, start_pose.ry_deg, start_pose.rz2_deg)

        self._adapter.start_move_line(approach, search_speed * 4, search_speed * 8)
        reason = self._monitor(goal_handle, timeout_s, 20.0, lambda: feedback(5.0, 0))
        if reason != 'ok':
            result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                     context='ContactPath')
            return result

        # 표면 탐색: 접촉 감지까지 조금씩 하강
        step_mm = self.get_parameter('probe_search_step_mm').value
        travelled = 0.0
        contacted = False
        while travelled < search_max_depth:
            if goal_handle.is_cancel_requested or not self._safe_to_move():
                self._cleanup_normal(goal_handle)
                reason = 'cancel' if goal_handle.is_cancel_requested else 'safety'
                result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                         context='ContactPath 탐색')
                return result
            self._adapter.start_move_rel_tool_z(-step_mm, search_speed, search_speed * 2)
            self._monitor(goal_handle, 5.0, 20.0, lambda: None)
            travelled += step_mm
            w = self._adapter.read_wrench()
            force_log.append(self._wrench_to_msg(w))
            if abs(w.fz_n) >= contact_threshold:
                contacted = True
                break

        if not contacted:
            self._cleanup_normal(goal_handle)
            self._log_abort(ErrorCode.E_NO_CONTACT,
                             f'ContactPath: {search_max_depth}mm 탐색 후 접촉 미검출')
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_NO_CONTACT,
                                              '탐색 깊이 내 표면 미검출', started_at)
            result.force_log = force_log
            return result

        if goal.use_compliance:
            self._adapter.compliance_on(self.get_parameter('compliance_stiffness').value)
        self._adapter.set_desired_force([0.0, 0.0, -abs(goal.target_force_n), 0.0, 0.0, 0.0],
                                         [0, 0, 1, 0, 0, 0])

        passes = max(1, goal.passes)
        n_wp = len(goal.waypoints)
        aborted = None
        for pass_idx in range(passes):
            for wp_idx, wp in enumerate(goal.waypoints):
                if aborted:
                    missed.append(pass_idx * n_wp + wp_idx)
                    continue
                try:
                    base_wp = self._transform_pose_to_base(wp, goal.frame_id)
                except Exception:
                    missed.append(pass_idx * n_wp + wp_idx)
                    continue
                target = pose_to_task_pose(base_wp)
                self._adapter.start_move_line(target, goal.feed_speed_mms,
                                               goal.feed_speed_mms * 2)

                def on_tick():
                    nonlocal max_force_measured, max_force_seen
                    w = self._adapter.read_wrench()
                    force_log.append(self._wrench_to_msg(w))
                    mag = math.sqrt(w.fx_n ** 2 + w.fy_n ** 2 + w.fz_n ** 2)
                    max_force_seen = max(max_force_seen, mag)
                    max_force_measured = max(max_force_measured, abs(w.fz_n))
                    stiffness_samples.append((travelled, w.fz_n))
                    pct = 100.0 * (pass_idx * n_wp + wp_idx + 1) / (passes * n_wp)
                    feedback(min(99.0, pct), pass_idx, w)

                reason = self._monitor(goal_handle, timeout_s, 20.0, on_tick)
                w = self._adapter.read_wrench()
                if reason != 'ok':
                    aborted = reason
                    missed.append(pass_idx * n_wp + wp_idx)
                    continue
                if max_force_seen > max_force_n:
                    aborted = 'overforce'
                    missed.append(pass_idx * n_wp + wp_idx)
                    continue
                if goal.abort_on_low_stiffness and len(stiffness_samples) >= \
                        self.get_parameter('contact_stiffness_window').value:
                    window = stiffness_samples[-self.get_parameter('contact_stiffness_window').value:]
                    k, r2, n = compute_stiffness(window, contact_threshold, 3)
                    if k is not None and r2 >= 0.5:
                        min_stiffness_measured = k if min_stiffness_measured is None \
                            else min(min_stiffness_measured, k)
                        if k < goal.min_stiffness_n_per_mm:
                            aborted = 'low_stiffness'
                            missed.append(pass_idx * n_wp + wp_idx)
            result.passes_done = pass_idx + 1
            if aborted:
                break

        self._cleanup_normal(goal_handle)

        mean_force = (sum(math.sqrt(f.fx_n ** 2 + f.fy_n ** 2 + f.fz_n ** 2) for f in force_log)
                      / len(force_log)) if force_log else 0.0
        result.mean_force_n = mean_force
        result.max_force_measured_n = max_force_seen
        result.min_stiffness_measured_n_per_mm = min_stiffness_measured or 0.0
        result.missed_segment_indices = missed
        result.force_log = force_log

        if aborted == 'overforce':
            self._log_abort(ErrorCode.E_OVERFORCE, f'ContactPath: max_force_n({max_force_n}) 초과')
            goal_handle.abort()
            result.abort_reason = 'ABORT_OVERFORCE'
            result.base = self._result_base(False, ErrorCode.E_OVERFORCE, '힘 상한 초과',
                                              started_at)
            return result
        if aborted == 'low_stiffness':
            self._log_abort(ErrorCode.E_LOW_STIFFNESS,
                             f'ContactPath: min_stiffness({goal.min_stiffness_n_per_mm}) 미만')
            goal_handle.abort()
            result.abort_reason = 'ABORT_LOW_STIFFNESS'
            result.base = self._result_base(False, ErrorCode.E_LOW_STIFFNESS,
                                              '강성 하한 미만 — 피부 접촉 의심', started_at)
            return result
        if aborted in ('cancel', 'safety', 'timeout'):
            result.base = self._finish_from_reason(aborted, goal_handle, started_at,
                                                     context='ContactPath')
            return result

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        return result

    # =========================================================================
    # LateralContact — 수평 접근 (v0.2 신설, ★ 연마 전용)
    # =========================================================================
    def _on_goal_lateral_contact(self, goal_request):
        if goal_request.travel_limit_mm <= 0.0:
            self.get_logger().warn(
                'LateralContact REJECT: E_INVALID_GOAL (travel_limit_mm <= 0 — '
                '접근 방향 확인 필요, SDS §5.3)')
            return GoalResponse.REJECT
        v = goal_request.approach_vector
        if math.sqrt(v.x ** 2 + v.y ** 2 + v.z ** 2) < 1e-6:
            self.get_logger().warn('LateralContact REJECT: E_INVALID_GOAL (approach_vector 0)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('LateralContact REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    @staticmethod
    def _approach_axis_index(v: Vector3):
        """SDS §5.3: 접근 벡터의 지배적 성분을 축(0=X 1=Y 2=Z)으로 근사한다.

        LateralContact 는 수평 접근 전용이라 실사용에서는 X/Y 다.
        """
        comps = [abs(v.x), abs(v.y), abs(v.z)]
        return comps.index(max(comps))

    def _cleanup_lateral(self, goal_handle, approach_vec_base, retreat_mm):
        try:
            dx = -approach_vec_base.x * retreat_mm
            dy = -approach_vec_base.y * retreat_mm
            dz = -approach_vec_base.z * retreat_mm
            self._adapter.start_move_rel_tool_xyz(dx, dy, dz, 10.0, 50.0)
            self._monitor(goal_handle, 10.0, 10.0, lambda: None)
        finally:
            self._adapter.compliance_off()
            self._adapter.release_force()

    def _execute_lateral_contact(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = LateralContact.Result()
        force_log = []
        max_force_measured = 0.0
        max_jam_force = 0.0
        max_travel = 0.0

        node_limit = self.get_parameter('lateral_max_travel_mm').value
        applied_limit = min(goal.travel_limit_mm, node_limit) if node_limit > 0.0 \
            else goal.travel_limit_mm
        result.applied_travel_limit_mm = applied_limit

        # approach_vector 는 goal.frame_id 기준 — base 로 회전만 변환 (병진 없음)
        try:
            origin = self._transform_point_to_base(Point(x=0.0, y=0.0, z=0.0), goal.frame_id)
            tip = self._transform_point_to_base(
                Point(x=goal.approach_vector.x, y=goal.approach_vector.y,
                      z=goal.approach_vector.z), goal.frame_id)
        except Exception as e:
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                              f'TF 변환 실패: {e}', started_at)
            return result
        approach_vec = Vector3(x=tip.x - origin.x, y=tip.y - origin.y, z=tip.z - origin.z)
        n = math.sqrt(approach_vec.x ** 2 + approach_vec.y ** 2 + approach_vec.z ** 2) or 1.0
        approach_vec = Vector3(x=approach_vec.x / n, y=approach_vec.y / n, z=approach_vec.z / n)
        axis = self._approach_axis_index(approach_vec)
        axis_sign = [approach_vec.x, approach_vec.y, approach_vec.z][axis]
        axis_sign = 1.0 if axis_sign >= 0.0 else -1.0

        timeout_s = goal.max_duration_s if goal.max_duration_s > 0.0 else \
            self.get_parameter('motion_timeout_s').value
        search_speed = self.get_parameter('lateral_search_speed_mms').value
        step_mm = self.get_parameter('lateral_search_step_mm').value
        contact_threshold = self.get_parameter('lateral_contact_threshold_n').value
        retreat_mm = goal.retreat_mm if goal.retreat_mm > 0.0 else \
            self.get_parameter('lateral_retreat_mm').value

        def feedback(pct, current_pass, travel_mm, w=None):
            fb = LateralContact.Feedback()
            fb.percent = pct
            fb.current_pass = current_pass
            fb.travel_mm = travel_mm
            if w is not None:
                fb.current_wrench.fx_n, fb.current_wrench.fy_n, fb.current_wrench.fz_n = \
                    w.fx_n, w.fy_n, w.fz_n
            goal_handle.publish_feedback(fb)

        def axis_component(w):
            return [w.fx_n, w.fy_n, w.fz_n][axis] * axis_sign

        # 1) 작업 평면 높이로 첫 waypoint 이동 (waypoints[0] 에 이미 work_plane
        #    높이가 반영되어 있다고 가정 — nail_local_frame 은 scan_node 가
        #    소유하며 이 노드는 그 좌표계 규약을 강제하지 않는다)
        if len(goal.waypoints) > 0:
            try:
                start_base = self._transform_pose_to_base(goal.waypoints[0], goal.frame_id)
            except Exception as e:
                goal_handle.abort()
                result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                                  f'TF 변환 실패: {e}', started_at)
                return result
            start_pose = pose_to_task_pose(start_base)
            self._adapter.start_move_line(start_pose, search_speed * 4, search_speed * 8)
            reason = self._monitor(goal_handle, timeout_s, 20.0, lambda: feedback(5.0, 0, 0.0))
            if reason != 'ok':
                result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                         context='LateralContact')
                return result

        # 2) 수평 탐색: 접촉 감지 or travel_limit_mm 초과
        travelled = 0.0
        contacted = False
        while travelled < applied_limit:
            if goal_handle.is_cancel_requested or not self._safe_to_move():
                self._cleanup_lateral(goal_handle, approach_vec, retreat_mm)
                reason = 'cancel' if goal_handle.is_cancel_requested else 'safety'
                result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                         context='LateralContact 탐색')
                return result
            dx, dy, dz = approach_vec.x * step_mm, approach_vec.y * step_mm, approach_vec.z * step_mm
            self._adapter.start_move_rel_tool_xyz(dx, dy, dz, search_speed, search_speed * 2)
            self._monitor(goal_handle, 5.0, 20.0, lambda: None)
            travelled += step_mm
            max_travel = max(max_travel, travelled)
            w = self._adapter.read_wrench()
            force_log.append(self._wrench_to_msg(w))
            feedback(10.0, 0, travelled, w)
            if axis_component(w) >= contact_threshold:
                contacted = True
                break

        if not contacted:
            self._cleanup_lateral(goal_handle, approach_vec, retreat_mm)
            self._log_abort(ErrorCode.E_LATERAL_LIMIT,
                             f'LateralContact: travel={travelled:.2f}mm '
                             f'limit={applied_limit:.2f}mm 까지 접촉 미검출')
            goal_handle.abort()
            result.abort_reason = 'ABORT_LATERAL_LIMIT'
            result.max_travel_mm = max_travel
            result.base = self._result_base(False, ErrorCode.E_LATERAL_LIMIT,
                                              '접근축 진행 한계 초과 — 접촉 미검출', started_at)
            result.force_log = force_log
            return result

        # 3~4) target_force 까지 접근 + 컴플라이언스 (접근축만 힘 제어)
        stiffness = [3000.0] * 6
        stiffness[axis] = 200.0
        self._adapter.compliance_on(stiffness)
        force_6d = [0.0] * 6
        force_6d[axis] = -abs(goal.target_force_n) * axis_sign
        axis_mask = [0] * 6
        axis_mask[axis] = 1
        self._adapter.set_desired_force(force_6d, axis_mask)

        # 5) waypoints 순회
        passes = max(1, goal.passes)
        n_wp = len(goal.waypoints)
        aborted = None
        for pass_idx in range(passes):
            for wp_idx, wp in enumerate(goal.waypoints):
                if aborted:
                    continue
                try:
                    base_wp = self._transform_pose_to_base(wp, goal.frame_id)
                except Exception:
                    continue
                target = pose_to_task_pose(base_wp)
                self._adapter.start_move_line(target, goal.feed_speed_mms,
                                               goal.feed_speed_mms * 2)

                def on_tick():
                    nonlocal max_force_measured, max_jam_force, travelled, max_travel
                    w = self._adapter.read_wrench()
                    force_log.append(self._wrench_to_msg(w))
                    a_force = abs(axis_component(w))
                    max_force_measured = max(max_force_measured, a_force)
                    other = [abs(w.fx_n), abs(w.fy_n), abs(w.fz_n)]
                    other.pop(axis)
                    jam = max(other)
                    max_jam_force = max(max_jam_force, jam)
                    pct = 100.0 * (pass_idx * n_wp + wp_idx + 1) / (passes * n_wp)
                    feedback(min(99.0, pct), pass_idx, travelled, w)

                reason = self._monitor(goal_handle, timeout_s, 20.0, on_tick)
                if reason != 'ok':
                    aborted = reason
                    continue
                if max_force_measured > goal.max_force_n:
                    aborted = 'overforce'
                    continue
                if travelled > applied_limit:
                    aborted = 'lateral_limit'
                    continue
                if max_jam_force > goal.jam_force_n:
                    aborted = 'jam'
                    continue
            result.passes_done = pass_idx + 1
            if aborted:
                break

        self._cleanup_lateral(goal_handle, approach_vec, retreat_mm)

        mean_force = (sum(abs(axis_component(self._msg_to_wrench(f))) for f in force_log)
                      / len(force_log)) if force_log else 0.0
        result.mean_force_n = mean_force
        result.max_force_measured_n = max_force_measured
        result.max_travel_mm = max_travel
        result.max_jam_force_n = max_jam_force
        result.force_log = force_log

        if aborted == 'overforce':
            self._log_abort(ErrorCode.E_OVERFORCE,
                             f'LateralContact: max_force_n({goal.max_force_n}) 초과')
            goal_handle.abort()
            result.abort_reason = 'ABORT_OVERFORCE'
            result.base = self._result_base(False, ErrorCode.E_OVERFORCE, '접근축 힘 상한 초과',
                                              started_at)
            return result
        if aborted == 'lateral_limit':
            self._log_abort(ErrorCode.E_LATERAL_LIMIT,
                             f'LateralContact: travel={travelled:.2f}mm '
                             f'limit={applied_limit:.2f}mm 초과 진행')
            goal_handle.abort()
            result.abort_reason = 'ABORT_LATERAL_LIMIT'
            result.base = self._result_base(False, ErrorCode.E_LATERAL_LIMIT,
                                              '접근축 진행 한계 초과 ★', started_at)
            return result
        if aborted == 'jam':
            self._log_abort(ErrorCode.E_LATERAL_JAM,
                             f'LateralContact: jam_force_n({goal.jam_force_n}) 초과')
            goal_handle.abort()
            result.abort_reason = 'ABORT_LATERAL_JAM'
            result.base = self._result_base(False, ErrorCode.E_LATERAL_JAM,
                                              '진행축 저항 급증 — 모서리 걸림 의심', started_at)
            return result
        if aborted in ('cancel', 'safety', 'timeout'):
            result.base = self._finish_from_reason(aborted, goal_handle, started_at,
                                                     context='LateralContact')
            return result

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        return result

    @staticmethod
    def _msg_to_wrench(fs: ForceSample):
        from .dsr_adapter import Wrench
        return Wrench(fs.fx_n, fs.fy_n, fs.fz_n, fs.tx_nm, fs.ty_nm, fs.tz_nm)

    # =========================================================================
    # ProbePoint — 스캔·택프리 검사 공유 최소 단위 (SDS §5.1)
    # =========================================================================
    def _on_goal_probe_point(self, goal_request):
        if goal_request.max_depth_mm <= 0.0:
            self.get_logger().warn('ProbePoint REJECT: E_INVALID_GOAL (max_depth_mm <= 0)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('ProbePoint REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _execute_probe_point(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = ProbePoint.Result()
        waveform = []

        approach_speed = goal.probe_speed_mms if goal.probe_speed_mms > 0.0 else \
            self.get_parameter('probe_speed_mms').value
        contact_threshold = goal.contact_threshold_n if goal.contact_threshold_n > 0.0 else \
            self.get_parameter('probe_contact_threshold_n').value
        max_depth = goal.max_depth_mm if goal.max_depth_mm > 0.0 else \
            self.get_parameter('probe_max_depth_mm').value
        max_force = goal.max_force_n if goal.max_force_n > 0.0 else \
            self.get_parameter('probe_max_force_n').value
        lateral_limit = goal.lateral_force_limit_n if goal.lateral_force_limit_n > 0.0 else 2.0
        min_samples = self.get_parameter('probe_regression_min_samples').value
        step_mm = self.get_parameter('probe_search_step_mm').value
        approach_height = goal.approach_height_mm if goal.approach_height_mm > 0.0 else \
            self.get_parameter('approach_height_mm').value

        try:
            target_base = self._transform_point_to_base(goal.target, goal.frame_id)
        except Exception as e:
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                              f'TF 변환 실패: {e}', started_at)
            return result

        try:
            current = self._adapter.get_pose()
        except DsrAdapterError as e:
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                              f'현재 자세 조회 실패: {e}', started_at)
            return result

        approach_pose = TaskPose(target_base.x * 1000.0, target_base.y * 1000.0,
                                  target_base.z * 1000.0 + approach_height,
                                  current.rz1_deg, current.ry_deg, current.rz2_deg)

        self._adapter.start_move_line(approach_pose, approach_speed * 4, approach_speed * 8)
        reason = self._monitor(goal_handle, self.get_parameter('motion_timeout_s').value, 10.0,
                                lambda: None)
        if reason != 'ok':
            result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                     context='ProbePoint 접근')
            return result

        try:
            tare = self._adapter.read_wrench()
        except DsrAdapterError as e:
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                              f'힘 센서 조회 실패: {e}', started_at)
            return result

        def relative(w):
            from .dsr_adapter import Wrench
            return Wrench(w.fx_n - tare.fx_n, w.fy_n - tare.fy_n, w.fz_n - tare.fz_n,
                          w.tx_nm - tare.tx_nm, w.ty_nm - tare.ty_nm, w.tz_nm - tare.tz_nm)

        def feedback(depth_mm, force_n):
            fb = ProbePoint.Feedback()
            fb.current_depth_mm = depth_mm
            fb.current_force_n = force_n
            goal_handle.publish_feedback(fb)

        samples = []
        travelled = 0.0
        contacted = False
        n_steps = max(1, int(math.ceil(max_depth / step_mm)))
        for _ in range(n_steps):
            if goal_handle.is_cancel_requested or not self._safe_to_move():
                self._probe_retreat(approach_pose)
                reason = 'cancel' if goal_handle.is_cancel_requested else 'safety'
                result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                         context='ProbePoint 하강')
                return result
            self._adapter.start_move_rel_tool_z(-step_mm, approach_speed, approach_speed * 2)
            self._monitor(goal_handle, 5.0, 20.0, lambda: None)
            travelled += step_mm
            w = self._adapter.read_wrench()
            waveform.append(self._wrench_to_msg(w))
            r = relative(w)
            feedback(travelled, r.fz_n)

            if math.hypot(r.fx_n, r.fy_n) > lateral_limit:
                self._probe_retreat(approach_pose)
                goal_handle.abort()
                result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                                  '탐색 중 측면 힘 초과', started_at)
                result.waveform = waveform
                return result

            samples.append((travelled, r.fz_n))
            if abs(r.fz_n) >= contact_threshold:
                contacted = True
                break
            if abs(r.fz_n) >= max_force:
                break

        point = StiffnessPoint()
        point.source = goal.source_tag or StiffnessPoint.SRC_FINE
        point.valid = False

        if not contacted:
            self._probe_retreat(approach_pose)
            self._log_abort(ErrorCode.E_NO_CONTACT,
                             f'ProbePoint: {max_depth}mm 하강 후 접촉 미검출 '
                             f'(target base=({target_base.x*1000:.1f},{target_base.y*1000:.1f}))')
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_NO_CONTACT,
                                              '최대 깊이 내 접촉 미검출', started_at)
            result.point = point
            result.waveform = waveform
            return result

        stiffness, r2, n_used = compute_stiffness(samples, contact_threshold, min_samples)
        result.regression_samples = n_used

        release_force_n = 0.0
        if goal.measure_release:
            release_speed = goal.release_speed_mms if goal.release_speed_mms > 0.0 else \
                self.get_parameter('release_speed_mms').value
            min_fz = 0.0
            retract_total = travelled + self.get_parameter('retreat_mm').value
            retracted = 0.0
            while retracted < retract_total:
                if goal_handle.is_cancel_requested or not self._safe_to_move():
                    break
                step = min(step_mm, retract_total - retracted)
                self._adapter.start_move_rel_tool_z(step, release_speed, release_speed * 2)
                self._monitor(goal_handle, 5.0, 20.0, lambda: None)
                retracted += step
                w = self._adapter.read_wrench()
                waveform.append(self._wrench_to_msg(w))
                r = relative(w)
                min_fz = min(min_fz, r.fz_n)
                feedback(max(travelled - retracted, 0.0), r.fz_n)
            release_force_n = min_fz
        else:
            self._probe_retreat(approach_pose)

        # IDS StiffnessPoint.position 은 "nail_frame 기준, mm" 로 명시돼 있다
        # (ProbePoint.target 의 geometry_msgs/Point 는 TF 표준대로 m 다) —
        # 여기서만 m -> mm 로 바꾼다.
        point.position = Point(x=target_base.x * 1000.0, y=target_base.y * 1000.0,
                                z=target_base.z * 1000.0)
        point.stiffness_n_per_mm = stiffness or 0.0
        point.release_force_n = release_force_n
        point.contact_depth_mm = travelled
        point.lateral_force_n = math.hypot(samples[-1][1], 0.0) if samples else 0.0
        point.valid = stiffness is not None and r2 >= 0.5

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        result.point = point
        result.waveform = waveform
        return result

    def _probe_retreat(self, approach_pose):
        self._adapter.start_move_line(approach_pose, 10.0, 30.0)
        # 후퇴는 베스트-에포트 — 취소/안전 사유로 여기 온 경우에도 로봇을
        # 표면에서 띄우는 것이 우선이므로 완료를 기다리되 실패해도 무시한다.
        try:
            deadline = time.monotonic() + 10.0
            while self._adapter.is_moving() and time.monotonic() < deadline:
                time.sleep(0.05)
        except DsrAdapterError:
            pass


def main(args=None):
    rclpy.init(args=args)
    node = RobotSkillNode()
    executor = MultiThreadedExecutor(num_threads=8)
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
