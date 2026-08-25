"""robot_skill_node — A계층 스킬 프리미티브 (NIS §5.1, SDS §6.1).

두산 컨트롤러를 감싸서 상위(공정) 노드가 어떤 두산 서비스를 어떤 순서로
부르는지 몰라도 되게 만든다. 두산 API 호출은 `dsr_adapter.DsrAdapter`
안에만 있고, 이 파일은 그 래퍼만 호출한다 (SDS §4.1).

제공: /skill/move_to, /skill/pick_place, /skill/contact_path,
      /skill/lateral_contact (Action)
      /robot/pose(50Hz) (Topic)

`/skill/probe_point`(ProbePoint) 는 폐지됐다 — 이 스킬을 쓰던 scan_node /
inspection_node 가 함께 제거됐고, 남은 stone_node 는 티칭된
`nail_local_frame` 높이를 그대로 써서 압입 탐색 없이 동작한다. 법선 접촉이
필요하면 `ContactPath` 를 쓴다.

참고로 삼은 문서: docs/노드별_인터페이스명세서_v0.2.md §5.1 (사용자 지정),
docs/개발명세서_SDS.md §3~5, docs/인터페이스정의서_IDS.md (실제 필드명 — nail_msgs).

이 노드는 NIS §2 인터페이스 매트릭스에 `ValidatePrecondition` 이 소비
목록에 없으므로(스킬 계층은 "stage" 개념이 없다) 그 호출은 하지 않는다 —
그건 stage 를 아는 B계층 공정 노드의 몫이다 (§3.1 ④는 공정 노드가 수행).
"""
import math
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

from nail_msgs.action import ContactPath, LateralContact, MoveTo, PickPlace
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState
from nail_perception.geometry2d import point_in_polygon, point_to_polygon_distance

from .conversions import TaskPose, pose_to_task_pose, task_pose_to_ros_pose
from .dsr_adapter import DsrAdapter, DsrAdapterError

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_GRIP_FAILED: ErrorCode.SEV_RETRY,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_COMM_LOST: ErrorCode.SEV_ABORT,
    ErrorCode.E_LATERAL_LIMIT: ErrorCode.SEV_SAFETY,
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
        self._last_safety_rx_monotonic = None
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

        self._pose_pub = self.create_publisher(PoseStamped, '/robot/pose', best_effort_qos)

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
        d('safety_status_timeout_s', 1.0)
        # use_mock_hardware: NIS §3.6 공통 파라미터로 선언만 한다. 이 노드는
        # mock 분기를 두지 않는다 — 로봇 없이 검증할 때는 두산 공식 가상
        # 모드(SDS §2.3)를 dsr_bringup2 에 `mode:=virtual` 로 띄운다.
        d('use_mock_hardware', False)
        d('retreat_mm', 10.0)
        # 발행 주기
        d('pose_pub_rate_hz', 50)
        # 이동 기본값 — PickPlace 가 goal.approach_height_mm 을 안 채웠을 때
        # 쓰는 fallback. 0 이면 접근 지점이 목표 z와 같아져 위로 뜨지 않고
        # 바로 그 자리에서 시작한다.
        d('approach_height_mm', 20.0)
        # PLACE 시 목표 z 를 이만큼 올려서 놓는다(mm). 0 이면 PICK 과 정확히
        # 같은 높이. 이동 중 툴이 그리퍼 안에서 미끄러지면 그만큼 슬롯 바닥을
        # 눌러버리므로, 실기에서는 1~3mm 를 줘서 살짝 떨어뜨리는 편이 안전하다.
        d('place_clearance_mm', 0.0)
        # PICK 파지 후 상승 높이(mm, 슬롯 목표 z 기준). approach_height_mm 은
        # 하강 직전 접근용이라 20mm 정도로 낮은데, 툴을 뽑아낼 때는 툴 길이보다
        # 충분히 높이 들어올려야 슬롯을 완전히 벗어난다. goal 의
        # approach_height_mm 보다 작으면 그 값으로 올린다(하강 방지).
        d('pick_lift_mm', 100.0)
        # via_key 경유 시 "목표 XY 위 수평 이동"에 쓸 높이(mm, 목표 z 기준
        # 여유분). via_key 자체의 높이를 그대로 쓰면 그 XY 에서는 너무 높아
        # NOT REACHABLE 이 나는 경우가 실기에서 확인됨(툴마다 도달 가능한
        # 높이 범위가 다름) — 대신 목표 z 로부터의 여유분으로 조절한다.
        # via_key 높이보다 낮은 쪽을 쓴다(그보다 더 올라갈 필요는 없음).
        d('pick_place_transit_clearance_mm', 100.0)
        d('motion_timeout_s', 30.0)
        d('move_max_speed_mms', 300.0)
        d('move_max_accel_mms2', 600.0)
        d('move_pose_tolerance_mm', 1.0)
        # 좌표 전용 ContactPath/LateralContact
        d('lateral_search_speed_mms', 3.0)
        d('lateral_retreat_mm', 10.0)
        # 그리퍼
        d('gripper_settle_s', 1.0)
        # PICK 하강 전 / PLACE 놓기 개방폭(mm). 그리퍼 완전개방('o')은 랙 슬롯
        # 간격보다 넓게 벌어져 옆 슬롯/구조물과 부딪힌다 — 실측으로 조정.
        d('gripper_open_width_mm', 60.0)
        d('targets_yaml_path', '')

    # --- 안전 -----------------------------------------------------------------
    def _on_safety_status(self, msg: SafetyState):
        self._latest_safety = msg
        self._last_safety_rx_monotonic = time.monotonic()

    def _safe_to_move(self) -> bool:
        timeout_s = self.get_parameter('safety_status_timeout_s').value
        return (self._latest_safety is not None
                and self._latest_safety.safe_to_move
                and self._last_safety_rx_monotonic is not None
                and time.monotonic() - self._last_safety_rx_monotonic <= timeout_s)

    def _on_cancel(self, goal_handle):
        return CancelResponse.ACCEPT

    # --- 자세 퍼블리시 ---------------------------------------------------------
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

    def _result_base(self, success, code, detail, started_at):
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
        if reason == 'unreachable':
            self._log_abort(ErrorCode.E_MOTION_FAILED,
                             f'{context}: 목표 위치 도달 실패 (NOT REACHABLE 등으로 컨트롤러가 '
                             '이동 명령을 거부했거나 오차가 허용치를 초과)')
            goal_handle.abort()
            return self._result_base(False, ErrorCode.E_MOTION_FAILED,
                                      f'{context}: 목표 위치 도달 실패', started_at)
        # timeout
        self._log_abort(ErrorCode.E_TIMEOUT, f'{context}: 타임아웃')
        goal_handle.abort()
        return self._result_base(False, ErrorCode.E_TIMEOUT, f'{context}: 타임아웃', started_at)

    def _verify_position_reached(self, reason, target_pose):
        """_monitor() 가 'ok' 를 반환해도 실제로 목표에 도달했는지 확인한다.

        두산 컨트롤러가 NOT REACHABLE 등으로 이동 명령 자체를 거부하면 로봇이
        BUSY 상태로 전이되지 않는다 — wait_motion_done() 은 '움직일 필요 없이
        이미 도착'과 '애초에 이동을 시작도 못함'을 구분 못 해 둘 다 즉시
        완료로 본다. 여기서 실제 위치와 목표 위치를 비교해 move_pose_tolerance_mm
        를 넘으면 'unreachable' 로 바꿔 실패 처리한다.

        반환: (reason, err_mm). reason=='ok' 가 아니었거나 위치 조회에
        실패하면 err_mm 은 -1.0 (측정 안 됨).
        """
        if reason != 'ok':
            return reason, -1.0
        try:
            final_pose = self._adapter.get_pose()
        except DsrAdapterError:
            return reason, -1.0
        err_mm = math.dist(
            (final_pose.x_mm, final_pose.y_mm, final_pose.z_mm),
            (target_pose.x_mm, target_pose.y_mm, target_pose.z_mm))
        tol_mm = self.get_parameter('move_pose_tolerance_mm').value
        if err_mm > tol_mm:
            return 'unreachable', err_mm
        return reason, err_mm

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
        result = MoveTo.Result()

        if goal.target_key:
            # PickPlace 와 동일한 조회 경로 — targets.yaml 의 rz1/ry/rz2 를
            # quaternion 변환 없이 그대로 쓴다. target/frame_id(Pose) 로 같은
            # 자세를 보내려면 quaternion 을 직접 계산해야 하는데, ry가 ±180°
            # 근처(짐벌락)일 때 atan2 왕복 변환이 다른 자세로 튀는 문제가
            # 실기에서 확인됨 — target_key 는 그 변환을 안 거쳐 안전하다.
            try:
                task_pose = self._target_task_pose(goal.target_key, goal.frame_id)
            except Exception as e:
                goal_handle.abort()
                result.base = self._result_base(
                    False, ErrorCode.E_INVALID_GOAL,
                    f'target_key "{goal.target_key}" 를 targets_yaml_path 에서도, TF 프레임으로도 '
                    f'찾을 수 없음: {e}', started_at)
                return result
        else:
            try:
                base_pose = self._transform_pose_to_base(goal.target, goal.frame_id)
            except Exception as e:
                goal_handle.abort()
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
        reason, err_mm = self._verify_position_reached(reason, task_pose)
        if err_mm >= 0.0:
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
        if goal_request.mode == PickPlace.Goal.MODE_PICK and goal_request.grip_width_mm <= 0.0:
            self.get_logger().warn('PickPlace REJECT: E_INVALID_GOAL (grip_width_mm <= 0)')
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

        if goal.mode == PickPlace.Goal.MODE_PLACE:
            clearance = self.get_parameter('place_clearance_mm').value
            if clearance > 0.0:
                target = TaskPose(target.x_mm, target.y_mm, target.z_mm + clearance,
                                   target.rz1_deg, target.ry_deg, target.rz2_deg)
                self.get_logger().info(
                    f'PickPlace place: place_clearance_mm={clearance} 적용 '
                    f'→ z {target.z_mm - clearance:.2f} → {target.z_mm:.2f}mm')

        approach = TaskPose(target.x_mm, target.y_mm,
                             target.z_mm + goal.approach_height_mm,
                             target.rz1_deg, target.ry_deg, target.rz2_deg)

        def move_and_wait(pose, step, pct):
            self._adapter.start_move_line(pose, speed, accel)
            reason = self._monitor(goal_handle, timeout_s, 10.0,
                                    lambda: feedback(step, pct))
            reason, _ = self._verify_position_reached(reason, pose)
            return reason

        settle_s = self.get_parameter('gripper_settle_s').value

        # target_key 에 via_key 가 지정돼 있으면 approach 전에 그 경유점부터
        # 들른다. 실기에서 대각선 이동이 랙/구조물에 부딪히는 문제가 있었는데,
        # 계산으로 경유점을 만들면 특이점/IK 문제로 오히려 더 위험했다
        # (실측 확인됨) — 대신 펜던트로 직접 티칭해 검증된 좌표를 쓴다.
        via_key = (self._load_targets().get(goal.target_key) or {}).get('via_key')
        if via_key:
            try:
                via_pose = self._target_task_pose(via_key, goal.frame_id)
            except Exception as e:
                goal_handle.abort()
                result.base = self._result_base(
                    False, ErrorCode.E_INVALID_GOAL,
                    f'via_key "{via_key}" 를 targets_yaml_path 에서도, TF 프레임으로도 '
                    f'찾을 수 없음: {e}', started_at)
                return result
            reason = move_and_wait(via_pose, 0, 3.0)
            if reason != 'ok':
                result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                         context='PickPlace(경유)')
                return result

            # 경유점에서 approach 로 바로 가면 XY 와 Z 가 동시에 바뀌어 여전히
            # 대각선이 된다 — 이미 쥔 툴의 돌출부가 부딪히는 문제가 실기에서
            # 재현됨. 목표 XY 위로 수평 이동부터 하고(아래), 그 다음
            # approach→target 은 같은 XY 라서 자동으로 수직 하강이 된다.
            # 이 수평 이동의 높이는 via_key 의 높이를 그대로 쓰지 않는다 —
            # 그 XY 에서는 너무 높아 NOT REACHABLE 이 나는 경우가 실기에서
            # 확인됨(툴마다 실제 도달 가능한 높이 범위가 다름). 대신 목표 z
            # 로부터의 여유분(pick_place_transit_clearance_mm)과 via 높이 중
            # 더 낮은 쪽을 쓴다.
            transit_clearance = self.get_parameter('pick_place_transit_clearance_mm').value
            transit_z_mm = min(via_pose.z_mm, target.z_mm + transit_clearance)
            above_target = TaskPose(target.x_mm, target.y_mm, transit_z_mm,
                                     target.rz1_deg, target.ry_deg, target.rz2_deg)
            reason = move_and_wait(above_target, 0, 7.0)
            if reason != 'ok':
                result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                         context='PickPlace(경유-상공)')
                return result

        reason = move_and_wait(approach, 0, 10.0)
        if reason != 'ok':
            result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                     context='PickPlace')
            return result

        # PICK 은 접근 높이에서 그리퍼를 **먼저 연 뒤** 하강한다. 닫힌 채로
        # 내려가면 툴/슬롯에 부딪힌다. PLACE 는 반대로 툴을 쥔 채 내려가야
        # 하므로 여기서 열지 않는다 (도착 후 아래에서 연다).
        # already_holding 이면(예: 핀셋을 쥔 채 스톤만 집기) 이 개방 자체를
        #건너뛴다 — 완전개방하면 쥐고 있던 툴을 놓쳐버린다.
        open_width = self.get_parameter('gripper_open_width_mm').value
        width = goal.grip_width_mm

        if goal.mode == PickPlace.Goal.MODE_PICK and not goal.already_holding:
            if not self._adapter.gripper_set_width(open_width):
                goal_handle.abort()
                self._log_abort(ErrorCode.E_GRIP_FAILED,
                                 'PickPlace pick: 하강 전 그리퍼 개방 명령 실패')
                result.base = self._result_base(False, ErrorCode.E_GRIP_FAILED,
                                                  '하강 전 그리퍼 개방 명령 실패', started_at)
                return result
            time.sleep(settle_s)

        reason = move_and_wait(target, 1, 30.0)
        if reason != 'ok':
            result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                     context='PickPlace')
            return result

        feedback(2, 60.0)
        if goal.mode == PickPlace.Goal.MODE_PICK:
            grip_ok = self._adapter.gripper_set_width(width)
        elif goal.already_holding:
            # 완전개방(open_width) 대신 지정된 폭까지만 벌린다 — 예: 스톤만
            # 놓고 핀셋 손잡이는 계속 쥔 채 유지.
            grip_ok = self._adapter.gripper_set_width(width)
        else:
            grip_ok = self._adapter.gripper_set_width(open_width)
        time.sleep(settle_s)

        if not grip_ok:
            self._adapter.start_move_line(approach, speed, accel)
            self._monitor(goal_handle, timeout_s, 10.0, lambda: None)
            goal_handle.abort()
            self._log_abort(ErrorCode.E_GRIP_FAILED, f'PickPlace {goal.mode}: 그리퍼 명령 실패')
            result.base = self._result_base(False, ErrorCode.E_GRIP_FAILED,
                                              '그리퍼 명령 실패', started_at)
            return result

        feedback(4, 90.0)
        # PICK 은 툴을 슬롯에서 완전히 빼내야 하므로 approach 보다 높이 든다.
        # PLACE 는 툴을 놓고 빠지는 것뿐이라 approach 로 그대로 복귀한다.
        if goal.mode == PickPlace.Goal.MODE_PICK:
            lift_mm = max(self.get_parameter('pick_lift_mm').value,
                          goal.approach_height_mm)
            retreat = TaskPose(target.x_mm, target.y_mm, target.z_mm + lift_mm,
                                target.rz1_deg, target.ry_deg, target.rz2_deg)
            self.get_logger().info(
                f'PickPlace pick: 파지 후 상승 z {target.z_mm:.1f} → {retreat.z_mm:.1f}mm '
                f'(pick_lift_mm={lift_mm})')
        else:
            retreat = approach
        reason = move_and_wait(retreat, 4, 100.0)
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
        if goal_request.feed_speed_mms <= 0.0:
            self.get_logger().warn('ContactPath REJECT: E_INVALID_GOAL (feed_speed_mms <= 0)')
            return GoalResponse.REJECT
        arc_indices = set(goal_request.circular_via_indices)
        if (len(arc_indices) != len(goal_request.circular_via_indices)
                or any(index <= 0 or index + 1 >= len(goal_request.waypoints)
                       for index in arc_indices)
                or any(index + 1 in arc_indices for index in arc_indices)):
            self.get_logger().warn(
                'ContactPath REJECT: E_INVALID_GOAL (circular_via_indices 형식 오류)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('ContactPath REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _cleanup_normal(self, goal_handle, retreat_mm=None):
        """법선 경로 종료 후 검증된 툴 -Z 방향으로 이탈한다."""
        self._adapter.start_move_rel_tool_z(
            -(retreat_mm or self._retreat_mm), 10.0, 50.0)
        self._adapter.wait_motion_done(
            10.0, 10.0, should_abort=lambda: not self._safe_to_move())

    @staticmethod
    def _tool_z_axis(task_pose):
        """두산 ZYZ 자세에서 tool +Z 단위벡터를 base 좌표로 계산한다."""
        rz1 = math.radians(task_pose.rz1_deg)
        ry = math.radians(task_pose.ry_deg)
        return (
            math.cos(rz1) * math.sin(ry),
            math.sin(rz1) * math.sin(ry),
            math.cos(ry),
        )

    def _execute_contact_path(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = ContactPath.Result()
        missed = []
        timeout_s = goal.max_duration_s if goal.max_duration_s > 0.0 else \
            self.get_parameter('motion_timeout_s').value

        def feedback(pct, current_pass):
            fb = ContactPath.Feedback()
            fb.percent = pct
            fb.current_pass = current_pass
            goal_handle.publish_feedback(fb)

        contact_offset = goal.contact_offset_mm

        try:
            if goal.reference_key:
                reference = self._target_task_pose(goal.reference_key, self._base_frame_id)
                reference_tool_z = self._tool_z_axis(reference)
                offset_origin = self._transform_point_to_base(
                    Point(x=0.0, y=0.0, z=0.0), goal.frame_id)

                def target_from_waypoint(wp):
                    point_base = self._transform_point_to_base(wp.position, goal.frame_id)
                    dx = (point_base.x - offset_origin.x) * 1000.0
                    dy = (point_base.y - offset_origin.y) * 1000.0
                    dz = (point_base.z - offset_origin.z) * 1000.0
                    return TaskPose(
                        reference.x_mm + dx + reference_tool_z[0] * contact_offset,
                        reference.y_mm + dy + reference_tool_z[1] * contact_offset,
                        reference.z_mm + dz + reference_tool_z[2] * contact_offset,
                        reference.rz1_deg, reference.ry_deg, reference.rz2_deg)
            else:
                def target_from_waypoint(wp):
                    task_pose = pose_to_task_pose(
                        self._transform_pose_to_base(wp, goal.frame_id))
                    axis = self._tool_z_axis(task_pose)
                    return TaskPose(
                        task_pose.x_mm + axis[0] * contact_offset,
                        task_pose.y_mm + axis[1] * contact_offset,
                        task_pose.z_mm + axis[2] * contact_offset,
                        task_pose.rz1_deg, task_pose.ry_deg, task_pose.rz2_deg)
        except Exception as e:
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_INVALID_GOAL,
                f'접촉 기준점 또는 경로 좌표계 조회 실패: {e}', started_at)
            return result

        try:
            start_pose = target_from_waypoint(goal.waypoints[0])
        except Exception as e:
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_MOTION_FAILED,
                f'첫 경로점 TF 변환 실패: {e}', started_at)
            return result

        tool_z = self._tool_z_axis(start_pose)
        approach_height = self.get_parameter('approach_height_mm').value
        approach = TaskPose(
            start_pose.x_mm - tool_z[0] * approach_height,
            start_pose.y_mm - tool_z[1] * approach_height,
            start_pose.z_mm - tool_z[2] * approach_height,
            start_pose.rz1_deg, start_pose.ry_deg, start_pose.rz2_deg)

        approach_speed = max(1.0, goal.feed_speed_mms)
        try:
            self._adapter.start_move_joint_to_pose(
                approach, approach_speed, approach_speed * 2)
        except Exception as exc:
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_MOTION_FAILED,
                f'ContactPath 접근 MoveJX 명령 실패: {exc}', started_at)
            return result
        reason = self._monitor(goal_handle, timeout_s, 20.0, lambda: feedback(5.0, 0))
        reason, _ = self._verify_position_reached(reason, approach)
        if reason != 'ok':
            result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                     context='ContactPath')
            return result

        passes = max(1, goal.passes)
        n_wp = len(goal.waypoints)
        allowed_xy = [(pt.x, pt.y) for pt in goal.allowed_polygon]
        arc_indices = set(goal.circular_via_indices)
        aborted = None
        max_path_error_mm = 0.0
        for pass_idx in range(passes):
            wp_idx = 0
            while wp_idx < n_wp:
                is_circle = wp_idx in arc_indices
                command_indices = (wp_idx, wp_idx + 1) if is_circle else (wp_idx,)
                outside = next((index for index in command_indices
                                if len(allowed_xy) >= 3
                                and not point_in_polygon(
                                    goal.waypoints[index].position.x,
                                    goal.waypoints[index].position.y, allowed_xy)
                                and point_to_polygon_distance(
                                    (goal.waypoints[index].position.x,
                                     goal.waypoints[index].position.y), allowed_xy) > 1e-6), None)
                if outside is not None:
                    self.get_logger().error(
                        f'ContactPath: waypoint {outside}가 allowed_polygon 밖 — 경로 중단')
                    missed.append(pass_idx * n_wp + outside)
                    aborted = 'unreachable'
                    break
                try:
                    targets = [target_from_waypoint(goal.waypoints[index])
                               for index in command_indices]
                except Exception as exc:
                    self.get_logger().error(
                        f'ContactPath: waypoint {wp_idx} 변환 실패: {exc}')
                    missed.append(pass_idx * n_wp + wp_idx)
                    aborted = 'unreachable'
                    break

                try:
                    if is_circle:
                        self._adapter.start_move_circle(
                            targets[0], targets[1], goal.feed_speed_mms,
                            goal.feed_speed_mms * 2)
                        target = targets[1]
                        final_index = wp_idx + 1
                    else:
                        target = targets[0]
                        final_index = wp_idx
                        self._adapter.start_move_line(
                            target, goal.feed_speed_mms, goal.feed_speed_mms * 2)
                except DsrAdapterError as exc:
                    self.get_logger().error(f'ContactPath 이동 명령 거부: {exc}')
                    missed.extend(pass_idx * n_wp + index for index in command_indices)
                    aborted = 'unreachable'
                    break

                def on_tick():
                    pct = 100.0 * (pass_idx * n_wp + final_index + 1) / (passes * n_wp)
                    feedback(min(99.0, pct), pass_idx)

                reason = self._monitor(goal_handle, timeout_s, 20.0, on_tick)
                reason, err_mm = self._verify_position_reached(reason, target)
                if err_mm >= 0.0:
                    max_path_error_mm = max(max_path_error_mm, err_mm)
                if reason != 'ok':
                    aborted = reason
                    missed.append(pass_idx * n_wp + final_index)
                    break
                wp_idx = final_index + 1
            if not aborted:
                result.passes_done = pass_idx + 1
            if aborted:
                break

        self._cleanup_normal(goal_handle)
        result.path_error_mm = max_path_error_mm
        result.missed_segment_indices = missed
        if aborted is not None:
            result.abort_reason = f'ABORT_{aborted.upper()}'
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
        if len(goal_request.waypoints) == 0:
            self.get_logger().warn('LateralContact REJECT: E_INVALID_GOAL (waypoints 비어있음)')
            return GoalResponse.REJECT
        if goal_request.feed_speed_mms <= 0.0:
            self.get_logger().warn('LateralContact REJECT: E_INVALID_GOAL (feed_speed_mms <= 0)')
            return GoalResponse.REJECT
        v = goal_request.approach_vector
        if math.sqrt(v.x ** 2 + v.y ** 2 + v.z ** 2) < 1e-6:
            self.get_logger().warn('LateralContact REJECT: E_INVALID_GOAL (approach_vector 0)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('LateralContact REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _cleanup_lateral(self, goal_handle, approach_vec_base, retreat_mm):
        dx = -approach_vec_base.x * retreat_mm
        dy = -approach_vec_base.y * retreat_mm
        dz = -approach_vec_base.z * retreat_mm
        self._adapter.start_move_rel_base_xyz(dx, dy, dz, 10.0, 50.0)
        self._adapter.wait_motion_done(
            10.0, 10.0, should_abort=lambda: not self._safe_to_move())

    def _execute_lateral_contact(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = LateralContact.Result()

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

        timeout_s = goal.max_duration_s if goal.max_duration_s > 0.0 else \
            self.get_parameter('motion_timeout_s').value
        approach_speed = self.get_parameter('lateral_search_speed_mms').value
        retreat_mm = goal.retreat_mm if goal.retreat_mm > 0.0 else \
            self.get_parameter('lateral_retreat_mm').value

        def feedback(pct, current_pass):
            fb = LateralContact.Feedback()
            fb.percent = pct
            fb.current_pass = current_pass
            goal_handle.publish_feedback(fb)

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
                    aborted = 'unreachable'
                    break
                target = pose_to_task_pose(base_wp)
                speed = approach_speed if wp_idx == 0 else goal.feed_speed_mms
                self._adapter.start_move_line(target, speed, speed * 2)

                def on_tick():
                    pct = 100.0 * (pass_idx * n_wp + wp_idx + 1) / (passes * n_wp)
                    feedback(min(99.0, pct), pass_idx)

                reason = self._monitor(goal_handle, timeout_s, 20.0, on_tick)
                reason, _ = self._verify_position_reached(reason, target)
                if reason != 'ok':
                    aborted = reason
                    break
            if not aborted:
                result.passes_done = pass_idx + 1
            if aborted:
                break

        self._cleanup_lateral(goal_handle, approach_vec, retreat_mm)
        if aborted is not None:
            result.abort_reason = f'ABORT_{aborted.upper()}'
            result.base = self._finish_from_reason(aborted, goal_handle, started_at,
                                                     context='LateralContact')
            return result

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        return result


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
