"""robot_skill_node — A계층 스킬 프리미티브 (NIS §5.1, SDS §6.1).

두산 컨트롤러를 감싸서 상위(공정) 노드가 어떤 두산 서비스를 어떤 순서로
부르는지 몰라도 되게 만든다. 두산 API 호출은 `dsr_adapter.DsrAdapter`
안에만 있고, 이 파일은 그 래퍼만 호출한다 (SDS §4.1).

제공: /skill/move_to, /skill/pick_place, /skill/contact_path,
      /skill/lateral_contact, /skill/probe_point (Action)
      /robot/pose(50Hz) (Topic)

`/skill/probe_point`는 별도 TCP가 없는 새 Probe의 검증용 스킬이다. 각 점에서
공중 경로와 실제 경로를 같은 자세·속도로 비교하며, 전체 공정에는 자동 연결하지
않는다.

참고로 삼은 문서: docs/노드별_인터페이스명세서_v0.2.md §5.1 (사용자 지정),
docs/개발명세서_SDS.md §3~5, docs/인터페이스정의서_IDS.md (실제 필드명 — nail_msgs).

이 노드는 NIS §2 인터페이스 매트릭스에 `ValidatePrecondition` 이 소비
목록에 없으므로(스킬 계층은 "stage" 개념이 없다) 그 호출은 하지 않는다 —
그건 stage 를 아는 B계층 공정 노드의 몫이다 (§3.1 ④는 공정 노드가 수행).
"""
import math
import statistics
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
from nail_msgs.msg import ErrorCode, ProbeMeasurement, ResultBase, SafetyState
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
    ErrorCode.E_OVERFORCE: ErrorCode.SEV_SAFETY,
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
        self._unscrew_j6_warned = False

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
        # ProbePoint: 실기 비교 시험에서 사용한 저속·힘 상한을 기본 안전 한계로 둔다.
        d('probe_approach_speed_mms', 5.0)
        d('probe_accel_mms2', 1.0)
        d('probe_sample_hz', 10.0)
        d('probe_baseline_samples', 30)
        d('probe_min_detect_force_n', 0.15)
        d('probe_hard_force_limit_n', 5.0)
        d('probe_preapproach_mm', 10.0)
        d('probe_retreat_speed_mms', 2.0)
        # 그리퍼
        d('gripper_settle_s', 1.0)
        # PICK 하강 전 / PLACE 놓기 개방폭(mm). 그리퍼 완전개방('o')은 랙 슬롯
        # 간격보다 넓게 벌어져 옆 슬롯/구조물과 부딪힌다 — 실측으로 조정.
        d('gripper_open_width_mm', 60.0)
        d('targets_yaml_path', '')
        # --- 나사 뚜껑 풀기 (PickPlace goal.unscrew) ---------------------------
        # 한 번에 다 돌리지 않고 unscrew_segment_deg 씩 끊어 도는 이유는
        # _turn_tool_z() 주석 참고 (매 구간 J6 잔여 가동범위 재계산 + 취소 확인).
        #
        # ⚠️ unscrew_total_deg 의 부호가 곧 회전 방향이다. tool +Z 는 그리퍼가
        #    뚜껑을 내려다보는 방향(아래)이므로, 오른나사를 푸는 방향(위에서
        #    봤을 때 반시계)은 tool +Z 기준으로는 **음수** 회전이다. 실기에서
        #    처음 돌릴 때는 unscrew_total_deg 를 -90 정도로 줄여 방향부터
        #    눈으로 확인하고, 반대로 돌면 부호를 뒤집을 것.
        # 1.5 바퀴. 손목을 반대쪽 끝까지 미리 감아도(_prewind_wrist) 한 번에
        # 돌 수 있는 이론 최대는 (J6 한계 360 - 여유 15) × 2 = 690° 다 —
        # 그 이상을 넣으면 항상 690° 에서 잘리고 "가동범위 소진" 경고가 뜬다.
        # 뚜껑이 1.5 바퀴로 안 풀리면 690 미만에서 올려 가며 맞출 것.
        d('unscrew_total_deg', -540.0)
        # 닫을 때(PLACE) 돌릴 각도. 0 이면 -unscrew_total_deg 를 그대로 쓴다 —
        # 푼 만큼만 되감으면 원래 상태로 돌아가고 과조임이 안 생긴다.
        d('unscrew_close_deg', 0.0)
        d('unscrew_segment_deg', 180.0)
        # 첫 구간만 짧게 돌려 "명령 1도당 J6 가 어느 쪽으로 몇 도 움직이는가"를
        # 실측한다 — 그리퍼 장착 방향에 따라 부호가 달라서 미리 가정할 수 없다.
        d('unscrew_probe_deg', 5.0)
        # 나사 피치. 한 바퀴 돌 때 뚜껑이 떠오르는 높이(mm). 이만큼 tool -Z 로
        # 같이 이동해 주지 않으면 뚜껑을 축방향으로 잡아당기거나 눌러버린다.
        d('unscrew_lift_per_turn_mm', 1.5)
        d('unscrew_speed_degs', 45.0)
        d('unscrew_accel_degs2', 90.0)
        # J6 가동범위(±deg)와 그 앞에서 멈출 여유. "방해되지 않는 선에서 최대한"
        # 돌린다는 건 결국 손목이 한계에 닿기 직전까지만 돈다는 뜻이다.
        d('unscrew_j6_limit_deg', 360.0)
        d('unscrew_j6_margin_deg', 15.0)
        # 손목을 감고/푸는 회전은 전부 슬롯 이 높이 위에서만 한다(mm, 슬롯 z
        # 기준). 뚜껑을 잡기 전에 그리퍼를 벌린 채 최대 두 바퀴를 휘두르므로
        # 랙 슬롯 바로 위(approach_height_mm, 20mm)에서 하면 옆 슬롯을 친다.
        # pick_place_transit_clearance_mm 와 같은 높이로 두는 게 기본이다 —
        # 이미 그 높이로 수평 이동해 오는 길이라 새로 검증할 게 없다.
        d('unscrew_prewind_height_mm', 100.0)
        # 손목을 미리 감는 방향(J6 부호). 이 장비는 + 쪽으로 감는다(실기 확인).
        # 즉 뚜껑을 푸는 회전은 J6 를 - 쪽으로 민다. 실측이 이 값과 어긋나면
        # unscrew_total_deg 의 부호가 반대일 가능성이 높아 경고를 남긴다.
        d('unscrew_prewind_j6_sign', 1.0)
        # --- 뚜껑을 푼 뒤 붓에 묻은 젤 훑어내기 -------------------------------
        # 뚜껑(=붓)을 뽑으면 젤이 잔뜩 묻어 나온다. 그대로 손톱으로 가면
        # 흘러넘치므로, 병 입구 높이에서 X 로 몇 번 왕복시켜 여분을 병 턱에
        # 훑어 떨어뜨린다. unscrew_wipe_lift_mm 는 슬롯 파지 z 기준 높이다 —
        # 붓이 병 입구에 살짝 닿는 높이로 실측해서 맞출 것.
        # 0 을 주면 이 단계를 통째로 건너뛴다.
        d('unscrew_wipe_lift_mm', 30.0)
        d('unscrew_wipe_x_mm', 5.0)
        d('unscrew_wipe_cycles', 3)
        d('unscrew_wipe_speed_mms', 20.0)

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

    # --- 나사 뚜껑 풀기 ----------------------------------------------------------
    def _joint6_deg(self):
        """손목(J6) 현재 각도. 드라이버가 관절값을 안 주면 None (경고 1회)."""
        try:
            joints = self._adapter.get_joints()
        except DsrAdapterError:
            joints = None
        if joints is None:
            if not self._unscrew_j6_warned:
                self._unscrew_j6_warned = True
                self.get_logger().warn(
                    '관절 각도를 읽을 수 없어(get_current_posj 없음) 뚜껑 풀기 중 '
                    'J6 잔여 가동범위를 확인하지 못합니다 — unscrew_total_deg 를 '
                    '그대로 돌립니다. 손목 한계에 걸리면 그 구간이 '
                    'E_MOTION_FAILED 로 끝납니다.')
            return None
        return joints[5]

    def _turn_tool_z(self, goal_handle, timeout_s, on_tick, direction, budget_deg,
                      lift_per_turn_mm, j6_rate=None):
        """tool Z 축 둘레로 direction(+1/-1) 쪽으로 최대 budget_deg 만큼 돌린다.

        한 번의 이동 명령으로 다 돌리지 않고 unscrew_segment_deg 씩 쪼개는 이유:
          · 매 구간 J6 를 실측해 남은 가동범위를 다시 계산한다. 예산을 다 못
            쓰더라도 손목 한계 직전까지는 돌린다.
          · 구간 사이에 취소/안전 확인이 들어간다. 720° 를 한 명령으로 보내면
            도는 동안 아무것도 끼어들 수 없다.

        j6_rate(명령 1도당 J6 변화량, 부호 포함)를 모르면 첫 구간을
        unscrew_probe_deg 만큼만 짧게 돌려 실측한다 — 그리퍼 장착 방향에 따라
        부호가 달라서 미리 가정할 수 없고, 이 값이 있어야 잔여 가동범위를
        계산할 수 있다.

        lift_per_turn_mm 는 한 바퀴당 tool Z 로 함께 이동할 거리(부호 포함).
        음수 = tool -Z = 뚜껑이 떠오르는 쪽(푸는 방향), 양수 = 잠기며 내려가는 쪽.

        반환: (reason, 실제 회전량[deg, 부호 포함], j6_rate)
        """
        p = self.get_parameter
        segment_deg = abs(p('unscrew_segment_deg').value) or 90.0
        probe_deg = abs(p('unscrew_probe_deg').value) or 5.0
        vel_degs = p('unscrew_speed_degs').value
        acc_degs2 = p('unscrew_accel_degs2').value
        j6_limit = abs(p('unscrew_j6_limit_deg').value)
        j6_margin = abs(p('unscrew_j6_margin_deg').value)

        remaining = abs(budget_deg)
        turned = 0.0
        seg_cap = segment_deg if j6_rate is not None else probe_deg

        while remaining > 1.0:
            seg = min(seg_cap, remaining)
            j6_before = self._joint6_deg()

            if j6_rate is not None and j6_before is not None and abs(j6_rate) > 1e-6:
                # 이번 회전으로 J6 가 향하는 쪽(+/-)의 남은 가동범위
                side = 1.0 if j6_rate * direction > 0.0 else -1.0
                headroom = j6_limit - j6_margin - side * j6_before
                seg = min(seg, max(0.0, headroom / abs(j6_rate)))
            if seg < 1.0:
                self.get_logger().warn(
                    f'뚜껑 회전: J6 가동범위 소진(J6={j6_before:.1f}°, '
                    f'한계 ±{j6_limit:.0f}°) — 예산 {abs(budget_deg):.0f}° 중 '
                    f'{turned:.0f}° 만 돌리고 중단합니다.')
                break

            lift_mm = lift_per_turn_mm * seg / 360.0
            try:
                self._adapter.start_rotate_tool_z(
                    direction * seg, lift_mm,
                    vel_mms=max(1.0, abs(lift_mm) * 4.0), acc_mms2=20.0,
                    vel_degs=vel_degs, acc_degs2=acc_degs2)
            except DsrAdapterError as exc:
                self.get_logger().error(f'뚜껑 회전: 명령 거부 — {exc}')
                return 'unreachable', turned * direction, j6_rate

            reason = self._monitor(goal_handle, timeout_s, 10.0, on_tick)
            if reason != 'ok':
                return reason, turned * direction, j6_rate

            turned += seg
            remaining -= seg
            seg_cap = segment_deg   # 탐색 구간은 첫 회뿐

            j6_after = self._joint6_deg()
            if j6_before is not None and j6_after is not None:
                delta = j6_after - j6_before
                if abs(delta) < 0.5 * seg:
                    # 명령은 받아들여졌는데 손목이 안 움직였다 = 한계에 붙었거나
                    # 회전이 tool Z 가 아닌 다른 축으로 흡수됐다. 더 밀어붙이면
                    # 뚜껑을 비틀게 되므로 여기서 멈춘다.
                    self.get_logger().warn(
                        f'뚜껑 회전: {seg:.0f}° 명령에 J6 가 {delta:.1f}° 만 '
                        f'움직였습니다 — 중단({turned:.0f}° 완료).')
                    break
                j6_rate = delta / (direction * seg)

        return 'ok', turned * direction, j6_rate

    def _prewind_wrist(self, goal_handle, timeout_s, on_tick, direction, need_deg):
        """돌리기 전에 손목을 반대쪽 끝으로 미리 감아 need_deg 만큼 여유를 만든다.

        이게 없으면 실제로 돌릴 수 있는 각도가 슬롯 자세를 티칭할 때 우연히
        정해진 J6 시작각에 좌우된다 — 같은 코드가 어떤 날은 두 바퀴를 돌고
        어떤 날은 몇 도만 돌고 만다. 여기서 먼저 반대쪽으로 감아 두면 항상
        need_deg 만큼은 확보된 상태로 시작한다.

        ⚠️ 그리퍼를 벌린 채 최대 두 바퀴를 휘두르는 동작이다. 반드시 슬롯에서
        unscrew_prewind_height_mm 만큼 떨어진 높이에서만 호출할 것.

        반환: (reason, j6_rate, 감은 각도[deg, 부호 포함])
              감은 각도는 나중에 그대로 되감기 위해 호출부가 누적해 둔다.
        """
        probe_deg = abs(self.get_parameter('unscrew_probe_deg').value) or 5.0
        j6_limit = abs(self.get_parameter('unscrew_j6_limit_deg').value)
        j6_margin = abs(self.get_parameter('unscrew_j6_margin_deg').value)

        # 어느 쪽으로 감아야 하는지 알려면 먼저 j6_rate 를 실측해야 한다.
        reason, wound, j6_rate = self._turn_tool_z(
            goal_handle, timeout_s, on_tick, direction, probe_deg, 0.0)
        if reason != 'ok':
            return reason, j6_rate, wound

        j6 = self._joint6_deg()
        if j6 is None or j6_rate is None or abs(j6_rate) < 1e-6:
            # J6 를 못 읽으면 어디까지 감겨 있는지 알 수 없어 감을 근거가 없다.
            # 그대로 진행하고, 부족하면 _turn_tool_z 가 도중에 멈춘다.
            return 'ok', j6_rate, wound

        # 감는 쪽(-direction)이 실제로 J6 를 설정한 부호 쪽으로 미는지 확인한다.
        # 어긋나면 뚜껑 회전이 J6 를 감는 방향과 같은 쪽으로 밀고 있다는 뜻이라
        # unscrew_total_deg 의 부호가 반대일 가능성이 높다.
        prewind_sign = self.get_parameter('unscrew_prewind_j6_sign').value
        if j6_rate * -direction * prewind_sign < 0.0:
            self.get_logger().warn(
                f'손목 감기 방향이 설정(unscrew_prewind_j6_sign={prewind_sign:+.0f})과 '
                f'반대입니다 — 실측 j6_rate={j6_rate:+.2f}, 회전 방향={direction:+.0f}. '
                'unscrew_total_deg 의 부호가 반대(= 뚜껑을 조이는 방향)일 수 '
                '있으니 저속으로 방향부터 확인하세요.')

        side = 1.0 if j6_rate * direction > 0.0 else -1.0
        headroom = (j6_limit - j6_margin - side * j6) / abs(j6_rate)
        shortfall = need_deg - headroom
        if shortfall <= 1.0:
            self.get_logger().info(
                f'손목 감기 불필요 — J6={j6:.1f}° 에서 이미 {headroom:.0f}° 여유 '
                f'(필요 {need_deg:.0f}°)')
            return 'ok', j6_rate, wound

        # 필요한 만큼만 감는다 — 끝까지 감으면 쓸데없이 크게 휘두른다.
        reason, extra, j6_rate = self._turn_tool_z(
            goal_handle, timeout_s, on_tick, -direction, shortfall, 0.0, j6_rate)
        self.get_logger().info(
            f'손목 감기 {extra:.0f}° — J6 {j6:.1f}° → {self._joint6_deg():.1f}° '
            f'(여유 {headroom:.0f}° → {need_deg:.0f}° 목표)')
        return reason, j6_rate, wound + extra

    @staticmethod
    def _with_current_orientation(pose, current):
        """pose 의 위치는 그대로 두고 자세만 현재 자세로 바꾼다.

        손목을 감아 둔 상태에서 티칭 자세로 절대 이동하면 그 각도가 통째로
        되감긴다 — 뚜껑을 쥔 채라면 도로 조이는 회전이 된다. 감아 둔 구간
        안에서는 위치만 명령하고 자세는 건드리지 않기 위한 것.
        """
        return TaskPose(pose.x_mm, pose.y_mm, pose.z_mm,
                         current.rz1_deg, current.ry_deg, current.rz2_deg)

    def _wipe_on_bottle(self, goal_handle, timeout_s, on_tick, slot_target):
        """붓에 묻은 젤 여분을 병 입구에 훑어 떨어뜨린다 (도포량 조절).

        뚜껑(=붓)을 뽑으면 젤이 잔뜩 묻어 나온다. 그대로 손톱으로 가면
        흘러넘치므로 병 입구 높이에서 X 로 왕복시켜 여분을 병 턱에 닦는다.
        슬롯 XY 를 기준으로 앞(+X) → 뒤(-X) 를 unscrew_wipe_cycles 번 반복하고
        가운데로 돌아온다.

        손목은 아직 감긴 채다 — 여기서는 위치만 명령하고 자세는 건드리지
        않는다. 티칭 자세로 절대 이동하면 감아 둔 각도가 되감기면서 아직
        병 안에 있는 붓을 비튼다.

        회전이 끝난 직후 이 높이에 있어야 의미가 있으므로 반드시 뚜껑 풀기와
        손목 되감기(_unwind_at_clearance) **사이**에서 부른다.
        """
        p = self.get_parameter
        lift_mm = p('unscrew_wipe_lift_mm').value
        x_mm = abs(p('unscrew_wipe_x_mm').value)
        cycles = int(p('unscrew_wipe_cycles').value)
        speed = max(1.0, p('unscrew_wipe_speed_mms').value)
        if lift_mm <= 0.0 or x_mm <= 0.0 or cycles <= 0:
            return 'ok'

        try:
            wound = self._adapter.get_pose()
        except DsrAdapterError as exc:
            self.get_logger().warn(f'젤 훑기 생략 — 현재 자세 조회 실패: {exc}')
            return 'ok'

        def go(dx_mm):
            pose = TaskPose(slot_target.x_mm + dx_mm, slot_target.y_mm,
                             slot_target.z_mm + lift_mm,
                             wound.rz1_deg, wound.ry_deg, wound.rz2_deg)
            self._adapter.start_move_line(pose, speed, speed * 2)
            reason = self._monitor(goal_handle, timeout_s, 10.0, on_tick)
            reason, _ = self._verify_position_reached(reason, pose)
            return reason

        self.get_logger().info(
            f'젤 훑기: 슬롯 z+{lift_mm:.0f}mm 에서 X ±{x_mm:.1f}mm 를 {cycles}회 왕복')
        reason = go(0.0)
        if reason != 'ok':
            return reason
        for _ in range(cycles):
            for dx in (x_mm, -x_mm):
                reason = go(dx)
                if reason != 'ok':
                    return reason
        return go(0.0)

    def _unwind_at_clearance(self, goal_handle, timeout_s, on_tick, slot_target,
                              lift_mm, move_and_wait, step, net_wound_deg, j6_rate):
        """감긴 손목으로 슬롯을 빠져나온 뒤, 안전 높이에서 손목을 되감는다.

        두 단계로 나누는 이유: 뚜껑을 뽑는(또는 놓고 빠지는) 동안에는 자세를
        건드리면 안 되고, 되감기는 뚜껑에서 자유로워진 뒤 슬롯에서 충분히
        떨어진 높이에서만 해야 한다.

        되감기를 "티칭 자세로 절대 이동"이 아니라 상대 회전으로 하는 이유:
        movel 의 목표 자세는 회전행렬이라 +700° 와 -20° 를 구분하지 못한다.
        절대 이동을 시키면 컨트롤러가 최단 경로(≤180°)만 돌아서 J6 는 감긴
        채로 남는다. 그 상태를 방치하면 다음 movejx(sol 고정)에서 손목이
        갑자기 한 바퀴 풀린다 — 여기서 상대 회전으로 정확히 되돌려 놓는다.
        """
        clearance_mm = max(lift_mm,
                            self.get_parameter('unscrew_prewind_height_mm').value)
        above = TaskPose(slot_target.x_mm, slot_target.y_mm,
                          slot_target.z_mm + clearance_mm,
                          slot_target.rz1_deg, slot_target.ry_deg, slot_target.rz2_deg)
        try:
            cur = self._adapter.get_pose()
        except DsrAdapterError as exc:
            self.get_logger().warn(
                f'현재 자세를 못 읽음({exc}) — 슬롯 자세 그대로 상승합니다.')
            return move_and_wait(above, step, 95.0)

        # 1) 자세는 그대로 둔 채 수직으로만 빠져나온다
        reason = move_and_wait(self._with_current_orientation(above, cur), step, 93.0)
        if reason != 'ok':
            return reason

        # 2) 감은 만큼 정확히 되감는다 (순수 회전, 축방향 이동 없음)
        if abs(net_wound_deg) > 1.0:
            reason, undone, _ = self._turn_tool_z(
                goal_handle, timeout_s, on_tick,
                -1.0 if net_wound_deg > 0.0 else 1.0,
                abs(net_wound_deg), 0.0, j6_rate)
            self.get_logger().info(
                f'손목 되감기 {undone:.0f}° (감은 총량 {net_wound_deg:.0f}°) '
                f'— J6={self._joint6_deg()}')
            if reason != 'ok':
                return reason

        # 3) 남은 잔여 각도(회전 오차)를 티칭 자세로 맞춰 마무리한다
        return move_and_wait(above, step, 97.0)

    def _execute_pick_place(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = PickPlace.Result()
        speed_ratio = goal.move_speed_ratio if goal.move_speed_ratio > 0.0 else 0.3
        speed = self.get_parameter('move_max_speed_mms').value * speed_ratio
        accel = self.get_parameter('move_max_accel_mms2').value * speed_ratio
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

        # 나사 뚜껑(코터 젤 병)은 슬롯에서 잡거나 놓는 그 자리에서 돌린다.
        #   PICK  : 잡고 → 풀고 → 들어 올린다
        #   PLACE : 내려놓으면서 → 닫고 → 놓는다
        # 슬롯 좌표(티칭값)는 "뚜껑이 잠긴 상태"의 자세다. 그래서 닫을 때는
        # 나사가 물리기 시작하는 높이(= 잠기며 내려올 거리만큼 위)로 내려간
        # 다음, 회전으로 슬롯 z 까지 잠가 내려간다.
        slot_target = target
        do_unscrew = goal.mode == PickPlace.Goal.MODE_PICK and goal.unscrew
        do_close = goal.mode == PickPlace.Goal.MODE_PLACE and goal.unscrew
        winding = do_unscrew or do_close

        open_deg = self.get_parameter('unscrew_total_deg').value
        close_deg = self.get_parameter('unscrew_close_deg').value or -open_deg
        turn_deg = open_deg if do_unscrew else close_deg
        pitch_mm = abs(self.get_parameter('unscrew_lift_per_turn_mm').value)
        # 회전 중 tool Z 로 함께 갈 거리. 푸는 쪽은 떠오르므로 tool -Z(음수),
        # 잠그는 쪽은 내려가므로 tool +Z(양수)다.
        lift_per_turn = -pitch_mm if do_unscrew else pitch_mm
        close_rise_mm = pitch_mm * abs(close_deg) / 360.0

        if goal.mode == PickPlace.Goal.MODE_PLACE:
            clearance = self.get_parameter('place_clearance_mm').value
            if do_close:
                clearance += close_rise_mm
            if clearance > 0.0:
                target = TaskPose(target.x_mm, target.y_mm, target.z_mm + clearance,
                                   target.rz1_deg, target.ry_deg, target.rz2_deg)
                self.get_logger().info(
                    f'PickPlace place: place_clearance_mm+나사 여유={clearance:.2f} 적용 '
                    f'→ z {target.z_mm - clearance:.2f} → {target.z_mm:.2f}mm')

        if goal.approach_key:
            try:
                approach = self._target_task_pose(goal.approach_key, goal.frame_id)
            except Exception as e:
                goal_handle.abort()
                result.base = self._result_base(
                    False, ErrorCode.E_INVALID_GOAL,
                    f'approach_key "{goal.approach_key}" 조회 실패: {e}', started_at)
                return result
        else:
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

        # 뚜껑을 돌리기 전에 손목을 반대쪽 끝으로 미리 감아 둔다. 이걸 안 하면
        # 실제로 돌 수 있는 각도가 슬롯 자세를 티칭할 때 우연히 정해진 J6
        # 시작각에 좌우된다(같은 코드가 두 바퀴를 돌기도, 몇 도만 돌기도 한다).
        # 뚜껑에서 충분히 떨어진 높이에서만 한다 — 여기서 최대 두 바퀴를 휘두른다.
        j6_rate = None
        net_wound_deg = 0.0   # 감은 총량 — 뚜껑에서 자유로워진 뒤 그대로 되감는다
        if winding:
            wind_h = self.get_parameter('unscrew_prewind_height_mm').value
            wind_pose = TaskPose(slot_target.x_mm, slot_target.y_mm,
                                  slot_target.z_mm + wind_h, slot_target.rz1_deg,
                                  slot_target.ry_deg, slot_target.rz2_deg)
            reason = move_and_wait(wind_pose, 3, 8.0)
            if reason != 'ok':
                result.base = self._finish_from_reason(
                    reason, goal_handle, started_at, context='PickPlace(손목 감기 위치)')
                return result

            reason, j6_rate, net_wound_deg = self._prewind_wrist(
                goal_handle, timeout_s, lambda: feedback(3, 9.0),
                1.0 if turn_deg >= 0.0 else -1.0, abs(turn_deg))
            if reason != 'ok':
                result.base = self._finish_from_reason(
                    reason, goal_handle, started_at, context='PickPlace(손목 감기)')
                return result

            # 여기서부터 뚜껑을 놓을 때까지는 자세를 명령하지 않는다 — 티칭
            # 자세로 절대 이동하면 방금 감아 둔 각도가 통째로 되감긴다.
            try:
                wound = self._adapter.get_pose()
            except DsrAdapterError as exc:
                goal_handle.abort()
                self._log_abort(ErrorCode.E_MOTION_FAILED, f'손목 감기 후 자세 조회 실패: {exc}')
                result.base = self._result_base(
                    False, ErrorCode.E_MOTION_FAILED,
                    f'손목 감기 후 자세를 못 읽어 중단: {exc}', started_at)
                return result
            approach = self._with_current_orientation(approach, wound)
            target = self._with_current_orientation(target, wound)

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

        # PLACE 는 놓기 **전에** 쥔 채로 돌려서 뚜껑을 잠근다. 여기서 슬롯
        # 티칭 z 까지 잠기며 내려간다(위에서 close_rise_mm 만큼 띄워 뒀다).
        if do_close:
            reason, result.unscrew_done_deg, j6_rate = self._turn_tool_z(
                goal_handle, timeout_s, lambda: feedback(3, 55.0),
                1.0 if turn_deg >= 0.0 else -1.0, abs(turn_deg),
                lift_per_turn, j6_rate)
            net_wound_deg += result.unscrew_done_deg
            if reason != 'ok':
                result.base = self._finish_from_reason(
                    reason, goal_handle, started_at, context='PickPlace(뚜껑 닫기)')
                return result
            self.get_logger().info(
                f'PickPlace place: 뚜껑 {result.unscrew_done_deg:.0f}° 잠금 '
                f'(목표 {turn_deg:.0f}°)')

        feedback(2, 60.0)
        if goal.mode == PickPlace.Goal.MODE_PICK:
            # 돌릴 때 미끄러지면 안 되므로 뚜껑을 풀 때는 지정된 최소 폭까지
            # 끝까지 오므린다. RG2 는 뚜껑에 막혀 그 폭에 못 미친 채 멈추고,
            # 남은 힘을 전부 파지력으로 쓴다 — 0 을 주면 최대 파지력이 된다.
            grip_ok = self._adapter.gripper_set_width(
                max(0.0, goal.unscrew_grip_width_mm) if do_unscrew else width)
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

        # PICK 은 잡은 자리에서 먼저 풀고 나서 들어 올린다 — 풀지 않은 채
        # 상승하면 병째로 딸려 올라오거나 뚜껑이 뜯긴다.
        if do_unscrew:
            reason, result.unscrew_done_deg, j6_rate = self._turn_tool_z(
                goal_handle, timeout_s, lambda: feedback(3, 75.0),
                1.0 if turn_deg >= 0.0 else -1.0, abs(turn_deg),
                lift_per_turn, j6_rate)
            net_wound_deg += result.unscrew_done_deg
            if reason != 'ok':
                result.base = self._finish_from_reason(
                    reason, goal_handle, started_at, context='PickPlace(뚜껑 풀기)')
                return result
            self.get_logger().info(
                f'PickPlace pick: 뚜껑 {result.unscrew_done_deg:.0f}° 풀림 '
                f'(목표 {turn_deg:.0f}°)')

            # 병에서 완전히 빠져나가기 전에 붓의 젤 여분을 병 입구에 훑는다.
            reason = self._wipe_on_bottle(
                goal_handle, timeout_s, lambda: feedback(3, 85.0), slot_target)
            if reason != 'ok':
                result.base = self._finish_from_reason(
                    reason, goal_handle, started_at, context='PickPlace(젤 훑기)')
                return result

        feedback(4, 90.0)
        # 감긴 손목으로 슬롯을 빠져나온 뒤 안전 높이에서 되감는다. 되감아
        # 두지 않으면 이후의 모든 절대 이동이 700° 짜리 손목 회전을 덤으로
        # 끌고 다닌다.
        if winding:
            reason = self._unwind_at_clearance(
                goal_handle, timeout_s, lambda: feedback(4, 95.0), slot_target,
                max(self.get_parameter('pick_lift_mm').value, goal.approach_height_mm)
                if goal.mode == PickPlace.Goal.MODE_PICK else goal.approach_height_mm,
                move_and_wait, 4, net_wound_deg, j6_rate)
            result.base = self._finish_from_reason(reason, goal_handle, started_at,
                                                    context='PickPlace(손목 되감기)')
            return result

        if goal.retreat_key:
            try:
                retreat = self._target_task_pose(goal.retreat_key, goal.frame_id)
            except Exception as e:
                goal_handle.abort()
                result.base = self._result_base(
                    False, ErrorCode.E_INVALID_GOAL,
                    f'retreat_key "{goal.retreat_key}" 조회 실패: {e}', started_at)
                return result
        # PICK 은 툴을 슬롯에서 완전히 빼내야 하므로 approach 보다 높이 든다.
        # PLACE 는 툴을 놓고 빠지는 것뿐이라 approach 로 그대로 복귀한다.
        elif goal.mode == PickPlace.Goal.MODE_PICK:
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
    # ProbePoint — 공중 힘 프로파일과 실제 탐색 프로파일 비교
    # =========================================================================
    @staticmethod
    def _probe_vector_tuple(vector):
        length = math.sqrt(vector.x ** 2 + vector.y ** 2 + vector.z ** 2)
        if length < 1e-9:
            return None
        return vector.x / length, vector.y / length, vector.z / length

    @staticmethod
    def _probe_force_metrics(force, baseline, axis):
        delta = [value - zero for value, zero in zip(force, baseline)]
        normal = sum(delta[index] * axis[index] for index in range(3))
        compression = max(0.0, -normal)
        lateral = math.sqrt(sum(
            (delta[index] - normal * axis[index]) ** 2 for index in range(3)))
        total = math.sqrt(sum(value * value for value in delta[:3]))
        return compression, lateral, total

    def _on_goal_probe_point(self, goal):
        axis = self._probe_vector_tuple(goal.press_direction)
        valid = (
            goal.manual_probe_tool_confirmed
            and (not goal.frame_id or goal.frame_id == self._base_frame_id)
            and axis is not None
            and 10.0 <= goal.air_offset_z_mm <= 150.0
            and 0.5 <= goal.max_depth_mm <= 20.0
            and 0.1 <= goal.probe_speed_mms <= 2.0
            and 0.05 <= goal.comparison_margin_n <= 2.0
            and 0.5 <= goal.max_force_n
            <= min(5.0, self.get_parameter('probe_hard_force_limit_n').value)
            and 0.1 <= goal.lateral_force_limit_n <= goal.max_force_n
            and 1 <= goal.confirm_samples <= 10
            and 0.0 <= goal.stiffness_depth_mm <= 2.0
            and 5.0 <= goal.timeout_s <= 120.0)
        if not valid:
            self.get_logger().warn(
                'ProbePoint REJECT: E_INVALID_GOAL (도구 확인, base frame, 축 또는 안전 범위 오류)')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _probe_baseline(self, goal_handle):
        samples = []
        count = max(2, self.get_parameter('probe_baseline_samples').value)
        period = 1.0 / max(1.0, self.get_parameter('probe_sample_hz').value)
        for _ in range(count):
            if goal_handle.is_cancel_requested:
                return None, 'cancel'
            samples.append(self._adapter.get_tool_force())
            time.sleep(period)
        return [statistics.fmean(values) for values in zip(*samples)], 'ok'

    def _probe_motion_reason(self, completed, goal_handle, target):
        """이동 완료 여부와 실제 목표 Pose 도달 여부를 함께 판정한다."""
        if not completed:
            return 'cancel' if goal_handle.is_cancel_requested else 'timeout'
        return self._verify_position_reached('ok', target)[0]

    def _probe_profile(self, goal_handle, search_start, axis, threshold, phase, timeout_s):
        goal = goal_handle.request
        speed = self.get_parameter('probe_approach_speed_mms').value

        self._adapter.start_move_joint_to_pose(search_start, speed, speed * 2.0)
        completed = self._adapter.wait_motion_done(
            timeout_s, 10.0, should_abort=lambda: goal_handle.is_cancel_requested)
        reason = self._probe_motion_reason(completed, goal_handle, search_start)
        if reason != 'ok':
            return None, reason

        baseline, reason = self._probe_baseline(goal_handle)
        if reason != 'ok':
            return None, reason

        target = TaskPose(
            search_start.x_mm + axis[0] * goal.max_depth_mm,
            search_start.y_mm + axis[1] * goal.max_depth_mm,
            search_start.z_mm + axis[2] * goal.max_depth_mm,
            search_start.rz1_deg, search_start.ry_deg, search_start.rz2_deg)
        peak = {'compression': 0.0, 'lateral': 0.0, 'total': 0.0, 'torque': 0.0}
        state = {
            'reason': None, 'confirmed': 0,
            'last': (0.0, 0.0, 0.0), 'last_force': baseline,
            'contact_traveled': None, 'contact_pose': None,
            'contact_compression': 0.0, 'stiffness_compression': 0.0,
            'stiffness_n_per_mm': 0.0,
        }

        def should_abort():
            if goal_handle.is_cancel_requested:
                state['reason'] = 'cancel'
                return True
            force = self._adapter.get_tool_force()
            compression, lateral, total = self._probe_force_metrics(force, baseline, axis)
            raw_total = math.sqrt(sum(value * value for value in force[:3]))
            torque = math.sqrt(sum((force[i] - baseline[i]) ** 2 for i in range(3, 6)))
            state['last'] = (compression, lateral, total)
            state['last_force'] = force
            peak['compression'] = max(peak['compression'], compression)
            peak['lateral'] = max(peak['lateral'], lateral)
            peak['total'] = max(peak['total'], raw_total)
            peak['torque'] = max(peak['torque'], torque)

            actual = self._adapter.get_pose()
            traveled = sum((value - start) * direction for value, start, direction in zip(
                (actual.x_mm, actual.y_mm, actual.z_mm),
                (search_start.x_mm, search_start.y_mm, search_start.z_mm), axis))
            feedback = ProbePoint.Feedback()
            feedback.phase = phase
            feedback.traveled_mm = max(0.0, traveled)
            feedback.compression_force_n = compression
            feedback.lateral_force_n = lateral
            feedback.total_force_n = raw_total
            feedback.confirmed_samples = state['confirmed']
            goal_handle.publish_feedback(feedback)

            if raw_total >= goal.max_force_n or lateral >= goal.lateral_force_limit_n:
                state['reason'] = 'force'
                return True
            if threshold is not None and compression >= threshold:
                state['confirmed'] += 1
                if state['confirmed'] >= goal.confirm_samples:
                    if state['contact_traveled'] is None:
                        state['reason'] = 'contact'
                        state['contact_traveled'] = max(0.0, traveled)
                        state['contact_pose'] = actual
                        state['contact_compression'] = compression
                    if goal.stiffness_depth_mm <= 0.0:
                        return True
            else:
                state['confirmed'] = 0
            if state['contact_traveled'] is not None:
                post_contact_mm = max(0.0, traveled - state['contact_traveled'])
                if post_contact_mm >= goal.stiffness_depth_mm:
                    state['stiffness_compression'] = compression
                    state['stiffness_n_per_mm'] = max(
                        0.0, (compression - state['contact_compression']) / post_contact_mm)
                    return True
            return False

        self._adapter.start_move_line(
            target, goal.probe_speed_mms,
            self.get_parameter('probe_accel_mms2').value)
        completed = self._adapter.wait_motion_done(
            timeout_s, self.get_parameter('probe_sample_hz').value,
            should_abort=should_abort)
        reason = state['reason'] if state['reason'] == 'contact' \
            else ('ok' if completed else (state['reason'] or 'timeout'))
        if reason == 'ok':
            reason = self._probe_motion_reason(completed, goal_handle, target)

        stopped = self._adapter.get_pose()
        traveled = sum((value - start) * direction for value, start, direction in zip(
            (stopped.x_mm, stopped.y_mm, stopped.z_mm),
            (search_start.x_mm, search_start.y_mm, search_start.z_mm), axis))

        # 탐색 완료 후 시작점으로 복귀
        if reason != 'cancel':
            self._adapter.start_move_line(
                search_start, goal.probe_speed_mms,
                self.get_parameter('probe_accel_mms2').value)
            completed = self._adapter.wait_motion_done(
                timeout_s, 10.0, should_abort=lambda: goal_handle.is_cancel_requested)
            retreat_reason = self._probe_motion_reason(
                completed, goal_handle, search_start)
            if retreat_reason != 'ok':
                reason = retreat_reason

        return {
            'reason': reason,
            'contact_detected': state['reason'] == 'contact',
            'stopped': stopped,
            'contact_pose': state['contact_pose'] or stopped,
            'traveled': max(0.0, traveled),
            'peak_compression': peak['compression'],
            'peak_lateral': peak['lateral'],
            'peak_total': peak['total'],
            'peak_torque': peak['torque'],
            'last_force': state['last_force'],
            'confirmed': state['confirmed'],
            'contact_compression': state['contact_compression'],
            'stiffness_compression': state['stiffness_compression'],
            'stiffness_n_per_mm': state['stiffness_n_per_mm'],
        }, reason

    def _probe_measurement(self, goal, profile, air=None, force_limit=False):
        measurement = ProbeMeasurement()
        measurement.header.stamp = self.get_clock().now().to_msg()
        measurement.header.frame_id = self._base_frame_id
        measurement.source = goal.source or ProbePoint.Goal.SOURCE_MANUAL
        measurement.requested_point = goal.search_start.position
        measurement.valid = (
            profile is not None
            and profile['reason'] in ('ok', 'contact')
            and not force_limit)
        measurement.reached_max_force = force_limit
        if profile is None:
            return measurement
        measurement.contact_point = task_pose_to_ros_pose(profile['contact_pose']).position
        measurement.contact_detected = profile.get(
            'contact_detected', profile['reason'] == 'contact')
        measurement.traveled_mm = profile['traveled']
        measurement.air_peak_compression_n = air['peak_compression'] if air else 0.0
        measurement.contact_peak_compression_n = profile['peak_compression']
        measurement.separation_n = measurement.contact_peak_compression_n \
            - measurement.air_peak_compression_n
        measurement.peak_lateral_force_n = profile['peak_lateral']
        measurement.peak_total_force_n = profile['peak_total']
        measurement.peak_torque_nm = profile['peak_torque']
        measurement.contact_compression_n = profile['contact_compression']
        measurement.stiffness_compression_n = profile['stiffness_compression']
        measurement.stiffness_n_per_mm = profile['stiffness_n_per_mm']
        force = profile['last_force']
        measurement.stopped_wrench.force.x = force[0]
        measurement.stopped_wrench.force.y = force[1]
        measurement.stopped_wrench.force.z = force[2]
        measurement.stopped_wrench.torque.x = force[3]
        measurement.stopped_wrench.torque.y = force[4]
        measurement.stopped_wrench.torque.z = force[5]
        measurement.confirmed_samples = profile['confirmed']
        return measurement

    def _finish_probe_failure(self, reason, goal_handle, result, started_at, detail,
                              profile=None, air=None):
        if profile is not None:
            result.measurement = self._probe_measurement(
                goal_handle.request, profile, air, force_limit=reason == 'force')
        if reason == 'force':
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_OVERFORCE, detail, started_at)
        else:
            result.base = self._finish_from_reason(
                reason, goal_handle, started_at, context='ProbePoint')
        return result

    def _execute_probe_point(self, goal_handle):
        started_at = time.monotonic()
        goal = goal_handle.request
        result = ProbePoint.Result()
        axis = self._probe_vector_tuple(goal.press_direction)
        start = pose_to_task_pose(goal.search_start)
        air_start = TaskPose(
            start.x_mm, start.y_mm, start.z_mm + goal.air_offset_z_mm,
            start.rz1_deg, start.ry_deg, start.rz2_deg)

        try:
            air, reason = self._probe_profile(
                goal_handle, air_start, axis, None,
                ProbePoint.Feedback.PHASE_AIR, goal.timeout_s)
            if reason != 'ok':
                detail = '공중 힘 프로파일에서 힘 상한 또는 이동 오류 발생'
                return self._finish_probe_failure(
                    reason, goal_handle, result, started_at, detail, air)

            threshold = max(
                self.get_parameter('probe_min_detect_force_n').value,
                air['peak_compression'] + goal.comparison_margin_n)
            contact, reason = self._probe_profile(
                goal_handle, start, axis, threshold,
                ProbePoint.Feedback.PHASE_CONTACT, goal.timeout_s)
        except Exception as exc:
            self._adapter.stop()
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_MOTION_FAILED,
                f'ProbePoint 실행 실패: {exc}', started_at)
            return result

        if reason not in ('ok', 'contact'):
            detail = '실제 탐색에서 힘 상한 또는 이동 오류 발생'
            return self._finish_probe_failure(
                reason, goal_handle, result, started_at, detail, contact, air)

        contact['reason'] = reason
        measurement = self._probe_measurement(goal, contact, air)
        result.measurement = measurement

        goal_handle.succeed()
        detail = '접촉 감지' if measurement.contact_detected else '최대 깊이까지 접촉 없음'
        result.base = self._result_base(True, ErrorCode.OK, detail, started_at)
        self.get_logger().info(
            f'ProbePoint {detail}: air={measurement.air_peak_compression_n:.3f}N '
            f'contact={measurement.contact_peak_compression_n:.3f}N '
            f'separation={measurement.separation_n:.3f}N '
            f'traveled={measurement.traveled_mm:.2f}mm')
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
