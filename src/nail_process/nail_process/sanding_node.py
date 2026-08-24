"""sanding_node — 수평 접근 연마 (NIS §6.2 M01, SDS §5.3 ★★).

`LateralContact` 를 쓰는 유일한 공정 노드다. 압입을 하지 않는 수평 접근에서는
강성 감시(`E_LOW_STIFFNESS`)가 작동하지 않으므로, 피부 접촉을 막는 방어선은
`travel_limit_mm` 하나뿐이다 — 이 값을 손톱 경계에서 기하학적으로 계산하는
`_compute_travel_limit` 이 이 노드에서 가장 중요한 함수다 (상수로 하드코딩
금지, SDS §12 체크리스트).

★ v0.3 변경: 경계의 출처가 바뀌었다. 예전에는 scan_node 의 강성 맵
(`GetStiffnessMap`)에서 `boundary_polygon` 을 받아왔지만, 스캔이 폐지되면서
이제는 **티칭된 손톱 크기 파라미터**(`nail_size_x_mm` / `nail_size_y_mm`)로
타원 경계를 직접 만든다. 즉 `travel_limit_mm` 의 신뢰도가 곧 그 티칭값의
신뢰도다 — 측정으로 검증되지 않으니 `nail_bringup/config/static_frames.yaml`
의 `nail_region` 을 실제 손톱보다 작게 잡아야 안전하다. 강성이 낮은 영역을
피하는 `forbidden_polygon` 은 측정할 방법이 없어져 함께 사라졌고,
`forbidden_margin_mm` 도 그래서 제거됐다.

이 노드도 dsr_msgs2 를 import 하지 않는다 — 실제 이동은 robot_skill_node 의
`/skill/lateral_contact` 를 호출한다 (SDS §4.1).
"""
import math
import threading
import time

import rclpy
from geometry_msgs.msg import Pose, Vector3
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import LateralContact, SandSurface
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, TaskPose, ToolState
from nail_msgs.srv import ValidatePrecondition

from nail_perception.geometry2d import (
    centroid, nail_boundary_polygon, oscillating_sweep, ray_polygon_distance,
)
from nail_skill.conversions import task_pose_to_ros_pose

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_LATERAL_JAM: ErrorCode.SEV_RETRY,
    ErrorCode.E_OVERFORCE: ErrorCode.SEV_ABORT,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_INVALID_GOAL: ErrorCode.SEV_ABORT,
    ErrorCode.E_LATERAL_LIMIT: ErrorCode.SEV_SAFETY,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


# 툴이 nail_local_frame 의 -Z(표면) 를 향하도록 고정하는 자세 — 로컬 X축
# 기준 180도 회전 (curing_node._FACE_DOWN_QUAT 와 동일). nail_local_frame
# 은 roll/pitch/yaw 가 전부 0(수평, identity)이라 orientation.w=1.0 을
# 그대로 쓰면 base_link 기준 A=0,B=0,C=0 이 되어 이 워크스페이스의 모든
# 실측 좌표(B≈±180°)와 반대 방향이 된다 — 실기에서 큰 B축 재정렬 도중
# 특이점/도달불가로 ABORT 되는 것으로 확인됨.
_FACE_DOWN_QUAT = (1.0, 0.0, 0.0, 0.0)  # (x, y, z, w)


def _sub(a, b):
    return (a[0] - b[0], a[1] - b[1])


def _add_scaled(origin, vec, t):
    return (origin[0] + vec[0] * t, origin[1] + vec[1] * t)


def _dot(a, b):
    return a[0] * b[0] + a[1] * b[1]


def _rotate90(v):
    """반시계 90도 회전 — 접근축에 수직인 이송축(feed_axis)을 얻는다."""
    return (-v[1], v[0])


class SandingNode(Node):

    def __init__(self):
        super().__init__('sanding_node')
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
        self._lateral_client = ActionClient(self, LateralContact, '/skill/lateral_contact',
                                             callback_group=self._cb_client)
        self._lateral_goal_handle = None

        self._sand_server = ActionServer(
            self, SandSurface, '/process/sand',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('sanding_node ready')

    # --- 파라미터 (NIS §6.2 표) -------------------------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 1.0)
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        # 접근
        d('approach_side', SandSurface.Goal.SIDE_FREE_EDGE)
        d('work_plane_offset_mm', 0.0)
        d('approach_pitch_deg', 0.0)
        # 힘
        d('target_force_n', 2.0)
        d('max_force_n', 4.0)
        d('jam_force_n', 4.0)
        # 경로
        d('passes', 3)
        d('step_over_mm', 1.5)
        d('feed_speed_mms', 8.0)
        d('max_duration_s', 60.0)
        # 한 pass(z 높이 고정) 안에서 진입측 호를 앞뒤로 왕복하는 횟수 —
        # 실제 손질처럼 한 번 왕복으로 안 끝내고 N번 문질러야 한다는 요청으로
        # 추가(2026-08-24). 1이면 기존과 동일(진입→반대편→원위치 1회).
        d('oscillations', 3)
        # goal.waypoints 가 비어 있을 때(=session_orchestrator 등 대부분의
        # 호출) 자동으로 쓸 기본 수동 왕복 경로 — sander_work/sand_work_r/
        # sand_work_l(targets.yaml)의 base_link 절대좌표(mm)를 변환 없이
        # 그대로 쓴 값, 거기서 Y만 전체 -1mm 한 값이다(2026-08-24 실측 조정).
        # custom waypoints 모드는 base_link 로 해석되므로(코드 참고)
        # targets.yaml 값을 그대로 복사해 넣으면 된다 — nail_local_frame
        # 상대좌표로 옮길 필요 없음. TaskPose와 동일한 x_mm,y_mm,z_mm,
        # rz1_deg,ry_deg,rz2_deg 6개씩 묶어 점 개수만큼 이어붙인 float64[] —
        # 길이가 6의 배수이고 점이 2개 이상이어야 적용된다. 빈 배열이면(기본)
        # goal.waypoints 가 비었을 때 기존처럼 경계 계산으로 대체한다.
        d('default_waypoints', [
            370.07, -17.42, 396.82, 6.78, 179.93, 6.89,
            380.04, -13.55, 393.82, 91.89, -178.54, 119.15,
            370.12, -3.14, 391.11, 76.93, -176.01, 34.45,
        ])
        # 손톱 경계 ★ — launch 가 static_frames.yaml 의 nail_region 에서 주입한다.
        # 여기 기본값은 launch 없이 `ros2 run` 으로 띄웠을 때만 쓰인다.
        d('nail_size_x_mm', 16.0)
        d('nail_size_y_mm', 13.0)
        d('nail_boundary_points', 24)
        # 안전 ★
        d('travel_limit_margin_mm', 2.0)
        # 힘 센서 미작동 대응 — compliance 로 곡률을 못 따라가므로 경계
        # 곡선 자체를 waypoint 로 삼아 이만큼(mm) 안쪽으로 눌러 넣는다.
        # travel_limit_mm(경계까지 거리 - margin) 을 넘으면 REJECT.
        d('engagement_depth_mm', 2.0)

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
        if self._lateral_goal_handle is not None:
            self._lateral_goal_handle.cancel_goal_async()
        return CancelResponse.ACCEPT

    # --- 기본 수동 왕복 경로 (goal.waypoints 비었을 때 대체) -----------------------
    def _default_waypoints_taskposes(self):
        """default_waypoints 파라미터(x_mm,y_mm,z_mm,rz1_deg,ry_deg,rz2_deg 를
        점 개수만큼 이어붙인 float64[])를 TaskPose 리스트로 변환. 길이가
        6의 배수가 아니거나 점이 2개 미만이면 빈 리스트(=경계 계산으로 대체)."""
        flat = list(self.get_parameter('default_waypoints').value)
        if len(flat) < 12 or len(flat) % 6 != 0:
            return []
        poses = []
        for i in range(0, len(flat), 6):
            tp = TaskPose()
            (tp.x_mm, tp.y_mm, tp.z_mm,
             tp.rz1_deg, tp.ry_deg, tp.rz2_deg) = flat[i:i + 6]
            poses.append(tp)
        return poses

    # --- 손톱 경계 (v0.3: 스캔 대신 티칭값) ----------------------------------------
    def _nail_boundary(self):
        """nail_local_frame 기준 손톱 경계 다각형. 빈 리스트면 파라미터가 잘못된 것."""
        return nail_boundary_polygon(
            self.get_parameter('nail_size_x_mm').value,
            self.get_parameter('nail_size_y_mm').value,
            int(self.get_parameter('nail_boundary_points').value))

    # --- 서비스 폴링 헬퍼 --------------------------------------------------------
    def _call_validate_precondition(self, session_id, timeout_s=5.0):
        if not self._validate_client.wait_for_service(timeout_sec=timeout_s):
            return False, ['ValidatePrecondition 서비스 연결 실패']
        req = ValidatePrecondition.Request()
        req.stage = ValidatePrecondition.Request.STAGE_SAND
        req.session_id = session_id
        req.required_tool = ToolState.SANDER
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

    # --- goal 수락 (§3.1 ②③④, NIS §6.2 동작 1~2) --------------------------------
    def _on_goal(self, goal_request):
        if not goal_request.session_id:
            self.get_logger().warn('SandSurface REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('SandSurface REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        if len(self._nail_boundary()) < 3:
            self.get_logger().warn(
                'SandSurface REJECT: E_INVALID_GOAL — nail_size_x_mm/nail_size_y_mm/'
                'nail_boundary_points 파라미터가 유효하지 않아 손톱 경계를 만들 수 없음')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'SandSurface REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # --- 접근 벡터 (NIS §6.2 동작 3) ---------------------------------------------
    def _approach_vector(self, approach_side, pitch_deg):
        """nail_local_frame 기준 수평 단위벡터. free_edge=-X, left=+Y, right=-Y."""
        base = {
            SandSurface.Goal.SIDE_FREE_EDGE: (-1.0, 0.0),
            SandSurface.Goal.SIDE_LEFT: (0.0, 1.0),
            SandSurface.Goal.SIDE_RIGHT: (0.0, -1.0),
        }.get(approach_side, (-1.0, 0.0))
        pitch = math.radians(pitch_deg)
        vx = base[0] * math.cos(pitch)
        vy = base[1] * math.cos(pitch)
        vz = math.sin(pitch)
        return base, (vx, vy, vz)

    # --- travel_limit_mm ★ (SDS §5.3 compute_travel_limit) -----------------------
    def _compute_travel_limit(self, start_xy, base_xy, boundary_xy, margin_mm):
        """진입점에서 접근 방향으로 얼마나 더 들어가도 되는가 (mm).

        경계 안에서 접근 방향으로 반직선을 쏴 반대편 경계까지의 거리를 재고,
        거기서 `travel_limit_margin_mm` 을 뺀다. 상수 하드코딩 금지 —
        이 값이 손이 아니라 손톱만 갈리게 하는 유일한 방어선이다.
        """
        eps = 0.05  # start_xy 는 경계 위 — t=0 자기교차 회피용 미세 오프셋
        ray_origin = _add_scaled(start_xy, base_xy, eps)
        d_boundary = ray_polygon_distance(ray_origin, base_xy, boundary_xy, 'nearest')
        if d_boundary is None:
            return None
        return d_boundary + eps - margin_mm

    def _entry_point(self, base_xy, boundary_xy):
        """손톱 경계 무게중심에서 -approach_vector 방향으로 쏴 만나는
        경계 위 점 = 접근이 시작되는 손톱 가장자리 (free_edge 쪽 등)."""
        c = centroid(boundary_xy)
        neg = (-base_xy[0], -base_xy[1])
        d = ray_polygon_distance(c, neg, boundary_xy, 'nearest')
        if d is None:
            return None
        return _add_scaled(c, neg, d)

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        result = SandSurface.Result()

        approach_side = goal.approach_side or self.get_parameter('approach_side').value
        pitch_deg = self._val(goal.approach_pitch_deg, 'approach_pitch_deg')
        work_plane = self._val(goal.work_plane_offset_mm, 'work_plane_offset_mm')
        target_force = self._val(goal.target_force_n, 'target_force_n')
        max_force = self._val(goal.max_force_n, 'max_force_n')
        jam_force = self._val(goal.jam_force_n, 'jam_force_n')
        passes = int(self._val(goal.passes, 'passes'))
        step_over = self._val(goal.step_over_mm, 'step_over_mm')
        feed_speed = self._val(goal.feed_speed_mms, 'feed_speed_mms')
        max_duration = self._val(goal.max_duration_s, 'max_duration_s')
        margin_mm = self._val(goal.travel_limit_margin_mm, 'travel_limit_margin_mm')
        oscillations = max(1, int(self.get_parameter('oscillations').value))

        base_xy, approach_vec_3d = self._approach_vector(approach_side, pitch_deg)

        # --- 수동 지정 waypoints ★ — 있으면(goal 이 직접 주거나, goal 이
        #     비어서 default_waypoints 파라미터로 대체됐으면) 경계 계산을
        #     전부 건너뛰고 그 Pose들(위치+자세, nail_local_frame, m)을
        #     오실레이션 왕복 경로로 쓴다 — passes 의 z 스텝은 이 모드에서
        #     무시된다. 경계 모드와 달리 face-down 을 강제하지 않고 각 Pose 의
        #     orientation 을 그대로 쓴다.
        custom_poses = list(goal.waypoints)
        source = 'goal.waypoints'
        if len(custom_poses) < 2:
            custom_poses = self._default_waypoints_taskposes()
            source = 'default_waypoints 파라미터'
        use_custom_waypoints = len(custom_poses) >= 2
        travel_limit_mm = None

        if use_custom_waypoints:
            self.get_logger().warn(
                f'SandSurface: waypoints {len(custom_poses)}개 수동 지정({source}, TaskPose, '
                '자세 포함) — 경계/진입점/travel_limit_mm/engagement_depth_mm 검증을 전부 '
                '건너뛴다. 좌표·자세가 안전한지는 호출자 책임(NFR-09 방어선 없음).')
            sweep_poses = [task_pose_to_ros_pose(tp)
                           for tp in oscillating_sweep(custom_poses, oscillations)]
        else:
            # --- 손톱 경계 (파라미터 재확인 — 실기에서 param set 으로 바뀔 수 있다) ---
            boundary_xy = self._nail_boundary()
            if len(boundary_xy) < 3:
                detail = ('손톱 경계 생성 실패 — nail_size_x_mm='
                          f"{self.get_parameter('nail_size_x_mm').value}, nail_size_y_mm="
                          f"{self.get_parameter('nail_size_y_mm').value}, nail_boundary_points="
                          f"{self.get_parameter('nail_boundary_points').value}")
                self._log_abort(ErrorCode.E_INVALID_GOAL, detail)
                goal_handle.abort()
                result.base = self._result_base(False, ErrorCode.E_INVALID_GOAL, detail, started_at)
                return result

            start_xy = self._entry_point(base_xy, boundary_xy)
            if start_xy is None:
                detail = f'approach_side="{approach_side}" 방향이 boundary_polygon 과 교차하지 않음'
                self._log_abort(ErrorCode.E_INVALID_GOAL, detail)
                goal_handle.abort()
                result.base = self._result_base(False, ErrorCode.E_INVALID_GOAL, detail, started_at)
                return result

            # --- travel_limit_mm ★ ------------------------------------------------
            travel_limit_mm = self._compute_travel_limit(
                start_xy, base_xy, boundary_xy, margin_mm)
            if travel_limit_mm is None or travel_limit_mm <= 0.0:
                detail = (f'travel_limit_mm={travel_limit_mm} <= 0 — 접근 방향("{approach_side}") '
                          f'또는 travel_limit_margin_mm({margin_mm}) 재검토 필요. '
                          'NFR-09: 이 값이 유일한 피부 접촉 방어선.')
                self._log_abort(ErrorCode.E_INVALID_GOAL, detail)
                goal_handle.abort()
                result.base = self._result_base(False, ErrorCode.E_INVALID_GOAL, detail, started_at)
                return result
            result.computed_travel_limit_mm = travel_limit_mm

            engagement_mm = self.get_parameter('engagement_depth_mm').value
            if engagement_mm > travel_limit_mm:
                detail = (f'engagement_depth_mm({engagement_mm}) > travel_limit_mm'
                          f'({travel_limit_mm:.2f}) — 피부 접촉 방어선(NFR-09) 초과 위험')
                self._log_abort(ErrorCode.E_INVALID_GOAL, detail)
                goal_handle.abort()
                result.base = self._result_base(False, ErrorCode.E_INVALID_GOAL, detail, started_at)
                return result

            # --- 이송 경로: 진입측 호(arc)를 압입 깊이만큼 눌러 넣은 곡선 -------------
            # 힘 센서가 없어 compliance 로 곡률을 못 따라가므로(§ 논의 참고),
            # boundary_xy(타원 근사 다각형) 중 진입점 쪽 절반을 feed_axis 순으로
            # 뽑아 그 곡선 자체를 waypoint 로 쓴다. base_xy(approach_vec) 방향으로
            # engagement_depth_mm 만큼 offset 해서 실제 연마 깊이를 만든다.
            feed_axis = _rotate90(base_xy)
            centroid_xy = centroid(boundary_xy)
            arc_points = sorted(
                (p for p in boundary_xy if _dot(_sub(p, centroid_xy), base_xy) <= 0.0),
                key=lambda p: _dot(_sub(p, start_xy), feed_axis))
            if len(arc_points) < 2:
                arc_points = [start_xy, start_xy]
            engaged_arc = [_add_scaled(p, base_xy, engagement_mm) for p in arc_points]

            # 왕복(오실레이션) 스트로크 N회
            sweep_xy_mm = oscillating_sweep(engaged_arc, oscillations)

        def mm_to_pose(xy_mm, z_mm):
            """경계 모드 전용 — face-down 고정 자세로 Pose 를 만든다. 수동
            waypoints 모드는 goal.waypoints 의 Pose(자세 포함)를 그대로 쓴다."""
            pose = Pose()
            pose.position.x = xy_mm[0] / 1000.0
            pose.position.y = xy_mm[1] / 1000.0
            pose.position.z = z_mm / 1000.0
            pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w = \
                _FACE_DOWN_QUAT
            return pose

        def feedback(pct, current_pass, travel_mm, wrench=None):
            fb = SandSurface.Feedback()
            fb.percent = pct
            fb.current_pass = current_pass
            fb.travel_mm = travel_mm
            if wrench is not None:
                fb.current_wrench = wrench
            goal_handle.publish_feedback(fb)

        # --- passes 만큼 반복. step_over_mm 는 패스마다 작업 높이(Z)를 옮겨
        #     손톱 가장자리의 다른 높이대를 훑는다 (NIS §6.2 파라미터 설명:
        #     "step_over_mm: 패스 간 이송 간격") -------------------------------------
        mean_forces, max_forces, max_travels, max_jams = [], [], [], []
        abort_code, abort_reason, abort_detail = None, '', ''
        passes_done = 0
        deadline = time.monotonic() + max_duration

        for pass_idx in range(passes):
            if goal_handle.is_cancel_requested:
                abort_code = 'CANCELLED'
                break
            if not self._safe_to_move():
                abort_code = ErrorCode.E_SAFETY_BLOCKED
                abort_detail = 'safe_to_move=false'
                break
            if time.monotonic() > deadline:
                self.get_logger().warn(
                    f'sanding: max_duration_s({max_duration}) 초과 — {pass_idx}/{passes} 패스만 수행')
                break

            z_mm = work_plane + pass_idx * step_over
            lc_goal = LateralContact.Goal()
            lc_goal.approach_vector = Vector3(x=approach_vec_3d[0], y=approach_vec_3d[1],
                                               z=approach_vec_3d[2])
            if use_custom_waypoints:
                # targets.yaml 값을 변환 없이 그대로 붙여넣을 수 있도록
                # base_link 절대좌표로 해석한다(nail_local_frame 상대좌표
                # 변환은 실수하기 쉬워 반복적으로 문제가 됐음).
                lc_goal.waypoints = list(sweep_poses)
                lc_goal.work_plane_offset_mm = sweep_poses[0].position.z * 1000.0
                lc_goal.frame_id = 'base_link'
            else:
                lc_goal.waypoints = [mm_to_pose(p, z_mm) for p in sweep_xy_mm]
                lc_goal.work_plane_offset_mm = z_mm
                lc_goal.frame_id = 'nail_local_frame'
            lc_goal.session_id = goal.session_id
            lc_goal.target_force_n = target_force
            lc_goal.max_force_n = max_force
            lc_goal.jam_force_n = jam_force
            lc_goal.feed_speed_mms = feed_speed
            # waypoints 자체가 이미 engagement_depth_mm 만큼 눌러 넣은 좌표라,
            # LateralContact 진입 시 waypoints[0]로 이동하면 목표 깊이에
            # 도달한다 — 여기 travel_limit_mm 은 그 이후 추가 탐색/전진 폭이라
            # 0에 가까운 값(goal 검증상 0 초과만 요구)만 준다. 큰 값을 그대로
            # 넘기면 waypoints[0] 도달 후 추가로 그만큼 더 파고들어 이중으로
            # 눌러 넣는다.
            lc_goal.travel_limit_mm = 0.1
            lc_goal.retreat_mm = margin_mm
            lc_goal.passes = 1
            lc_goal.max_duration_s = max(1.0, deadline - time.monotonic())

            def on_lc_feedback(fb_msg):
                fb = fb_msg.feedback
                feedback(100.0 * (pass_idx + fb.percent / 100.0) / passes, pass_idx,
                          fb.travel_mm, fb.current_wrench)

            lc_result, err_code, err_detail = self._call_lateral_contact(
                lc_goal, goal_handle, lc_goal.max_duration_s + 5.0, on_lc_feedback)

            if err_code == 'CANCELLED':
                abort_code = 'CANCELLED'
                break
            if err_code is not None or not lc_result.base.success:
                abort_code = err_code or lc_result.base.error.code
                abort_detail = err_detail or lc_result.base.error.detail
                abort_reason = f'ABORT_{abort_code}' if not lc_result.abort_reason \
                    else lc_result.abort_reason
                if lc_result is not None:
                    mean_forces.append(lc_result.mean_force_n)
                    max_forces.append(lc_result.max_force_measured_n)
                    max_travels.append(lc_result.max_travel_mm)
                    max_jams.append(lc_result.max_jam_force_n)
                break

            passes_done += 1
            mean_forces.append(lc_result.mean_force_n)
            max_forces.append(lc_result.max_force_measured_n)
            max_travels.append(lc_result.max_travel_mm)
            max_jams.append(lc_result.max_jam_force_n)

        result.mean_force_n = sum(mean_forces) / len(mean_forces) if mean_forces else 0.0
        result.max_force_measured_n = max(max_forces) if max_forces else 0.0
        result.max_travel_mm = max(max_travels) if max_travels else 0.0
        result.max_jam_force_n = max(max_jams) if max_jams else 0.0
        result.passes_done = passes_done
        result.abort_reason = abort_reason

        if abort_code == 'CANCELLED':
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, '사용자 취소',
                                              started_at)
            return result
        if abort_code is not None:
            self._log_abort(abort_code, f'sanding pass {passes_done}: {abort_detail}')
            goal_handle.abort()
            result.base = self._result_base(False, abort_code, abort_detail, started_at)
            return result

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        return result

    # --- LateralContact 클라이언트 헬퍼 (§3.3 취소 전파) ---------------------------
    def _call_lateral_contact(self, goal, our_goal_handle, timeout_s, feedback_cb=None):
        if not self._lateral_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_MOTION_FAILED, 'lateral_contact 액션 서버 연결 실패'

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        send_future = self._lateral_client.send_goal_async(goal, feedback_callback=feedback_cb)
        send_future.add_done_callback(on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return None, ErrorCode.E_TIMEOUT, 'lateral_contact goal 전송 타임아웃'

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED, 'lateral_contact goal 거부됨'
        self._lateral_goal_handle = gh

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
                self._lateral_goal_handle = None
                return None, ErrorCode.E_TIMEOUT, 'lateral_contact 결과 타임아웃'

        self._lateral_goal_handle = None
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
        self.get_logger().error(f'[{code}] sanding_node: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = SandingNode()
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
