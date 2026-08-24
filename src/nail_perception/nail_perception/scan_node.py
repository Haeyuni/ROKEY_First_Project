"""scan_node — 접촉식 높이·재질 스캔 및 경계 인식 (NIS §6.1 ★★, SDS §5.2).

v0.3 에서 판별 근거를 바꿨다.

원래는 거친 격자로 **강성**을 떠서 손톱(고강성)/피부(저강성) 두 군집으로
가르는 구조였다. 실기에서 이게 안 갈리는데, 임계값 알고리즘 탓이 아니라
축 하나가 부족해서다.

  * 위치제어 로봇으로 단단한 면을 누르면 측정되는 기울기는 재질 강성이
    아니라 로봇·툴 컴플라이언스라 상한에서 포화한다.
  * **작업 대상이 손 모형이면 손톱과 손의 재질이 사실상 같다.** 강성 축은
    이 경우 원리적으로 죽어 있다 — 더 잘 재봐야 같은 값이 나온다.

그래서 이 노드는 이제 프로브를 **강성계가 아니라 접촉식 높이 측정기**로
먼저 쓴다. 재질이 같아도 기하는 남는다: 손톱판은 주변보다 솟아 있고
가장자리에 단차가 있다. 접촉 높이는 꺾임점 탐색으로 재므로 표본 간격보다
정밀하고, 힘 크기나 센서 바이어스와 무관하다.

  1단계 사전 측정  네 귀퉁이를 찍어 작업면 기울기를 잡는다. 이후 모든 점의
                   접근 높이를 그 면에 맞춰, 어디서든 같은 거리만 내려가
                   접촉하게 만든다 (기울어진 면에서 압입 예산이 편차에
                   통째로 먹히던 문제 해결, 미끄러질 기회도 줄어든다).
  2단계 거친 스캔  전 특징(높이·강성·완화·이력)을 뜬다.
  3단계 판별       축마다 혼자서 얼마나 갈리는지 재고 그 값으로 가중해
                   한 축으로 융합한다. 오늘 실제로 작동하는 축이 알아서
                   발언권을 갖는다 — 강성이 죽어 있으면 가중치가 0 이 되고
                   높이 축이 판을 이끈다.
  4단계 정밀 스캔  경계 후보(라벨이 갈리는 이웃 + 높이 기울기가 큰 곳)만
                   촘촘히 다시 뜬다. 이때 거친 단계에서 가중치를 못 받은
                   축은 아예 측정하지 않는다(HEIGHT 프로파일) — 점당 시간이
                   1/3 로 준다.

실제 이동/압입은 이 노드가 하지 않는다 — robot_skill_node 의
`/skill/probe_point` 를 매 점마다 호출한다 (이 노드는 dsr_msgs2 를 import
하지 않는다, SDS §4.1).
"""
import csv
import math
import os
import threading
import time

import numpy as np
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
from .nail_classifier import FEATURE_SPECS, classify, describe, height_driven
from .surface3d import fit_surface, local_slope


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
        # 접근 높이를 작업면에 맞춰 보정하므로(사전 측정 참고) 하강 예산을
        # 바짝 조일 수 있다. 예전에는 기울기 편차가 예산을 통째로 먹었다.
        d('probe_approach_height_mm', 3.0)
        # 하강 예산. 0 이면 approach_height + allowance 로 스스로 정한다.
        #
        # 예산은 사실상 공짜다 — 하강은 힘이 잡히는 순간 멈추므로, 넉넉히
        # 잡아도 실제로 표면이 없을 때만 시간을 쓴다. 반대로 모자라면 그
        # 점은 통째로 유실되고, 유실된 점 때문에 표면 모델이 편향되어 곡률
        # 제거가 실패하고, 그러면 높이 축까지 같이 죽는다 (시뮬레이션에서
        # 130점 중 52점이 이 경로로 사라졌다).
        d('probe_depth_mm', 0.0)
        d('probe_depth_allowance_mm', 6.0)
        # E_NO_CONTACT 재시도 시 예산을 얼마나 더 줄지. 같은 조건으로 다시
        # 내려보내는 재시도는 정의상 같은 결과를 낸다 — 반드시 깊여야 한다.
        d('probe_no_contact_depth_step_mm', 3.0)
        d('probe_target_travel_mm', 3.0)  # 접근 높이 보정의 목표 접촉 거리
        d('probe_max_force_n', 2.0)
        d('probe_timeout_s', 20.0)
        d('probe_no_contact_retry', 2)  # SDS §7.3 retry.probe_no_contact
        # 미끄러진 점은 힘을 낮춰 한 번 더 찍는다. 마찰원뿔 안으로 들어가면
        # 같은 경사에서도 붙어 있는다.
        d('probe_slip_retry', 1)
        d('probe_slip_retry_force_ratio', 0.5)
        d('probe_slip_ratio_limit', 0.45)
        # 측정 프로파일. 'auto' 면 거친 스캔에서 가중치를 받은 축을 보고
        # 정밀 스캔의 프로파일을 정한다 — 힘 축이 전부 죽어 있으면 정밀
        # 단계는 높이만 뜨면 되므로 점당 시간이 1/3 로 준다.
        d('probe_profile_coarse', 'full')
        d('probe_profile_fine', 'auto')

        # --- 작업면 사전 측정 -------------------------------------------------
        # 기울어진 면에서 접근 높이를 일정하게 만드는 것이 목적이다. 켜두면
        # 점당 하강 시간이 줄고 미끄러질 기회도 준다.
        d('survey_enabled', True)
        d('survey_approach_height_mm', 12.0)
        d('survey_depth_mm', 18.0)
        d('survey_inset_ratio', 0.85)   # 영역 대비 사전 측정점 위치
        # 3x3. 네 귀퉁이(4점)로는 평면밖에 못 맞추는데, 손가락은 원통이라
        # 곡률이 몇 mm 나 된다 — 그만큼이 하강 예산에서 그대로 새고, 심하면
        # 예산을 넘겨 미접촉으로 죽는다(시뮬레이션에서 130점 중 56점 유실).
        d('survey_grid', 3)
        d('survey_min_points', 3)
        d('survey_max_tilt_deg', 30.0)  # 넘으면 안착 이상으로 보고 중단

        # --- 판별 ---------------------------------------------------------------
        # 'fused' 가 기본. 'stiffness' 는 예전 동작(강성 단독 Otsu)으로 되돌린다.
        d('classifier_method', 'fused')
        d('min_feature_margin', 1.0)
        # 축이 만든 라벨이 공간적으로 뭉쳐 있어야 발언권을 준다. Otsu 는
        # 순수 노이즈도 잘라내고 그 분리도가 10 을 넘기도 하므로, 분리도
        # 하한만으로는 허위 축을 못 거른다.
        d('min_feature_coherence', 0.25)
        # 표면 모델: 손가락은 평면이 아니라 원통에 가깝고 손톱판도 볼록하다.
        # 재질이 같은 모형에서는 높이가 유일한 판별 축이 되므로, 그 축을
        # 오염시키는 곡률을 먼저 빼야 한다. 'auto' 면 잔차가 작은 쪽을 쓴다.
        d('surface_model', 'auto')      # 'plane' | 'quadric' | 'auto'
        # 배경 표면을 손톱 **바깥에서만** 다시 맞추는 반복 횟수. 0 이면 한 번만
        # 맞춘다(= 손톱이 배경을 들어올린 채로 판별).
        d('surface_refit_passes', 2)
        # 배경 씨앗: 공칭 손톱 영역의 이 배수 밖에 있는 점들로 배경을 먼저
        # 맞춘다. scan_margin_mm 이 존재하는 이유가 "경계가 공칭 영역 밖에
        # 있을 수 있다"이므로, 그 바깥 테두리는 손톱이 아니라고 봐도 된다.
        # 씨앗을 안 주면 첫 분류가 뒤집힐 때 반복 재적합이 그 위에서 발산한다.
        d('background_seed_ratio', 1.0)
        d('background_min_ratio', 0.25)  # 배경 표본이 전체의 이 비율 밑으로 내려가면 중단
        d('coherence_radius_ratio', 1.6)  # coarse_pitch 배수
        # 고강성(손톱) 군집이 스캔 영역의 이 비율을 넘으면 판별을 믿지 않는다.
        # 분리도만 보면 "104점 대 6점"처럼 한쪽이 거의 다인 엉터리 분할도
        # 높은 점수를 받는다 — 손톱이 마진 포함 영역을 다 채울 수는 없다.
        d('hard_cluster_max_ratio', 0.75)
        # 높이 축이 판별을 이끌 때 쓰는 대체 합격 기준: 단차 / 배경 잔차.
        #
        # separation_margin_min(2.0) 은 강성 군집화용으로 정해진 값이라
        # 높이 축에는 잘못된 자다. 군집 분리도는 손톱 **자신의** 높이 편차
        # (손톱판은 볼록해서 0.2~0.5mm 에 걸쳐 있다)를 군집 내 분산으로 세어
        # 벌점을 매기는데, 그건 측정 문제가 아니라 손톱의 모양이다. 기하
        # 축에서 물리적으로 옳은 척도는 "단차가 배경 잡음보다 얼마나 큰가"다.
        # 4.0 = 단차가 배경 잡음의 네 배. 3.0 으로 두면 "단차는 있으나 점별
        # 분류는 절반만 맞는" 상태가 통과한다(시뮬레이션에서 0.15mm 단차가
        # snr 3.2 로 통과하면서 정확도는 67% 였다). 실물 CSV 로그를 보고
        # 조정할 첫 번째 값이다.
        d('height_step_snr_min', 4.0)
        d('background_bandwidth_ratio', 0.34)
        d('flip_band_sigma', 0.75)
        # 정밀 스캔 대상 선정에 높이 기울기를 함께 쓴다. 재질이 같으면 경계는
        # 높이가 급히 꺾이는 능선으로만 나타나므로 이 축이 가장 날카롭다.
        d('gradient_candidate_ratio', 0.25)  # 기울기 상위 몇 %를 후보로

        # --- 튜닝용 기록 ----------------------------------------------------------
        # 비워두면 기록하지 않는다. 경로를 주면 점별 특징을 CSV 로 남긴다 —
        # 어느 축이 실제로 갈리는지는 실물 데이터를 봐야만 정해진다.
        d('probe_log_dir', '')

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
    def _call_probe_point(self, x_mm, y_mm, frame_id, source_tag, timeout_s,
                           our_goal_handle, *, profile, approach_height_mm,
                           max_depth_mm, max_force_n):
        """반환: (StiffnessPoint | None, error_code | None).

        error_code 가 'CANCELLED' 면 our_goal_handle 취소로 중단된 것이다.
        """
        if not self._probe_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_COMM_LOST

        p = self.get_parameter
        goal = ProbePointAction.Goal()
        goal.target = Point(x=x_mm / 1000.0, y=y_mm / 1000.0, z=0.0)
        goal.frame_id = frame_id
        goal.profile = profile
        goal.approach_height_mm = approach_height_mm
        goal.max_depth_mm = max_depth_mm
        goal.max_force_n = max_force_n
        goal.slip_ratio_limit = p('probe_slip_ratio_limit').value
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

    def _probe_with_retry(self, x_mm, y_mm, frame_id, source_tag, timeout_s,
                           our_goal_handle, max_retry, *, profile,
                           approach_height_mm, max_depth_mm, max_force_n):
        """재시도 두 종류를 함께 다룬다.

        * E_NO_CONTACT — SDS §7.3 retry.probe_no_contact. 그대로 다시.
        * 미끄러짐 — 힘을 낮춰 다시 찍는다. 접선/법선 비가 마찰계수를 넘어서
          미끄러지는 것이므로, 누르는 힘을 줄여 마찰원뿔 안으로 들어가면 같은
          경사에서도 붙어 있는다. 첫 시도의 기하 특징은 살아 있으니, 재시도가
          또 미끄러져도 그 점을 버리지는 않는다.
        """
        p = self.get_parameter
        slip_retry_left = int(p('probe_slip_retry').value)
        slip_ratio = p('probe_slip_retry_force_ratio').value
        force = max_force_n
        attempt = 0
        best = None
        while True:
            point, err = self._call_probe_point(
                x_mm, y_mm, frame_id, source_tag, timeout_s, our_goal_handle,
                profile=profile, approach_height_mm=approach_height_mm,
                max_depth_mm=max_depth_mm, max_force_n=force)
            if err == 'CANCELLED':
                return None, 'CANCELLED'
            if point is not None and point.valid:
                if point.reject_reason != 'SLIPPED' or slip_retry_left <= 0:
                    return point, None
                # 미끄러졌다 — 기하 특징은 살려두고 힘을 낮춰 한 번 더.
                best = point
                slip_retry_left -= 1
                force = max(force * slip_ratio, 0.3)
                self.get_logger().warn(
                    f'scan: ({x_mm:.1f},{y_mm:.1f}) 미끄러짐(비 {point.slip_ratio:.2f}, '
                    f'기울기 {point.surface_tilt_deg:.1f}°) — 힘 {force:.2f}N 으로 재시도')
                continue
            if point is None:
                # 통신/타임아웃 등. 앞선 시도에서 건진 점이 있으면 그걸 쓰고,
                # 없으면 오류를 그대로 올린다 — 여기서 None,None 을 돌려주면
                # 실패가 성공으로 둔갑한다.
                return (best, None) if best is not None else (None, err)
            if err is None:
                return point, None
            if err != ErrorCode.E_NO_CONTACT or attempt >= max_retry:
                return (best, None) if best is not None else (point, err)
            # 같은 예산으로 다시 내려보내면 정의상 같은 결과가 나온다. 예산을
            # 늘려야 한다 — 하강은 힘이 잡히는 순간 멈추므로 깊게 잡아도 위험이
            # 늘지 않는다.
            attempt += 1
            max_depth_mm += p('probe_no_contact_depth_step_mm').value
            self.get_logger().warn(
                f'[{err}] scan: ({x_mm:.1f},{y_mm:.1f}) 접촉 실패 — '
                f'하강 예산 {max_depth_mm:.1f}mm 로 재시도 {attempt}/{max_retry}')

    def _aborted(self, goal_handle):
        if goal_handle.is_cancel_requested:
            return 'CANCELLED'
        if not self._safe_to_move():
            return ErrorCode.E_SAFETY_BLOCKED
        return None

    # --- 작업면 사전 측정 ------------------------------------------------------------
    def _survey_work_plane(self, frame_id, area_x, area_y, margin, goal_handle):
        """네 귀퉁이 + 중앙을 찍어 "접근 자세에서 표면까지의 거리" 평면을 구한다.

        접촉 높이(base z)가 아니라 **하강 거리**에 평면을 맞추는 게 핵심이다.
        하강 거리는 "이 점에서 얼마나 내려가야 닿는가"를 직접 말해주므로,
        nail_frame 이 base 에 대해 기울어 있든 말든 그대로 접근 높이 보정에
        쓸 수 있다. 접촉 높이 쪽은 프레임 기울기가 섞여 들어가 보정의 입력으로
        쓰면 이중 보정이 된다 (재질 단차를 볼 때는 반대로 contact_z 를 쓴다 —
        거기서는 평면 적합이 프레임 기울기를 선형항으로 흡수해 준다).

        반환: (travel_plane | None, survey_points, halt_or_None)
        """
        p = self.get_parameter
        inset = p('survey_inset_ratio').value
        hx = (area_x / 2.0 + margin) * inset
        hy = (area_y / 2.0 + margin) * inset
        grid_n = max(2, int(p('survey_grid').value))
        spots = [(hx * (2.0 * i / (grid_n - 1) - 1.0), hy * (2.0 * j / (grid_n - 1) - 1.0))
                 for i in range(grid_n) for j in range(grid_n)]

        approach = p('survey_approach_height_mm').value
        depth = p('survey_depth_mm').value or \
            (approach + p('probe_depth_allowance_mm').value)
        timeout_s = p('probe_timeout_s').value
        rows, points = [], []
        for x, y in spots:
            halt = self._aborted(goal_handle)
            if halt:
                return None, points, halt
            point, err = self._probe_with_retry(
                x, y, frame_id, StiffnessPoint.SRC_SURVEY, timeout_s, goal_handle,
                int(p('probe_no_contact_retry').value),
                profile=ProbePointAction.Goal.PROFILE_HEIGHT,
                approach_height_mm=approach, max_depth_mm=depth,
                max_force_n=p('probe_max_force_n').value)
            if err == 'CANCELLED':
                return None, points, 'CANCELLED'
            if point is None or not point.valid:
                continue
            points.append(point)
            rows.append((x, y, point.contact_travel_mm))

        if len(rows) < int(p('survey_min_points').value):
            self.get_logger().warn(
                f'작업면 사전 측정 유효점 {len(rows)}개 — 보정 없이 진행한다')
            return None, points, None

        model = fit_surface(rows, mode=p('surface_model').value)
        self.get_logger().info(
            f'작업면 사전 측정: {len(rows)}점, 모델 {model.kind}, '
            f'기울기 {model.tilt_deg:.2f}°, 잔차 rms {model.rms:.3f}mm')
        if model.tilt_deg > p('survey_max_tilt_deg').value:
            self.get_logger().warn(
                f'작업면 기울기 {model.tilt_deg:.1f}° 가 상한 '
                f'{p("survey_max_tilt_deg").value}° 초과 — 안착 위치를 의심할 것. '
                '접근 높이 보정은 계속 적용한다.')
        return model, points, None

    def _approach_height_for(self, travel_model, x_mm, y_mm, base_height, target_travel):
        """그 점에서 쓸 접근 높이 [mm].

        접근 높이를 δ 만큼 올리면 접촉까지의 하강 거리도 δ 만큼 늘어난다.
        예측 하강 거리가 target_travel 이 되도록 그 차이만큼 접근 높이를
        깎는다. 기울어진(그리고 휜) 면에서도 모든 점이 같은 거리만 내려가
        접촉하므로, 하강 예산을 편차가 아니라 압입에 쓸 수 있다.
        """
        if travel_model is None:
            return base_height
        corrected = base_height - (float(travel_model.z(x_mm, y_mm)) - target_travel)
        return float(min(max(corrected, 0.5), base_height * 3.0))

    def _refine_travel_model(self, travel_model, xy, points):
        """거친 스캔 결과로 하강 거리 모델을 다시 맞춘다.

        사전 측정은 아홉 점이지만 거친 스캔은 수십 점이다. 정밀 스캔에
        들어가기 전에 실측으로 모델을 갈아끼우면 접근 높이가 훨씬 정확해지고,
        그만큼 압입 예산이 남는다. 실패하면 기존 모델을 그대로 쓴다.
        """
        rows = [(xy[i][0], xy[i][1], pt.contact_travel_mm)
                for i, pt in enumerate(points) if pt.contact_travel_mm > 0.0]
        if len(rows) < 8:
            return travel_model
        model = fit_surface(rows, mode=self.get_parameter('surface_model').value)
        self.get_logger().info(
            f'하강 거리 모델 갱신: {len(rows)}점, {model.kind}, rms {model.rms:.3f}mm')
        return model

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

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # --- 표면 모델 및 판별 ---------------------------------------------------------
    def _surface_residuals(self, xy, contact_z):
        """접촉 높이에서 작업면 성분을 빼고 **재질 단차만** 남긴다.

        손가락은 평면이 아니라 원통에 가깝고 손톱판도 볼록하다. 평면만 빼면
        그 곡률이 잔차에 남아 "가운데가 솟았다"가 되는데, 그건 재질이 아니라
        모양이다. 재질이 같은 모형에서는 높이가 사실상 유일한 판별 축이므로,
        그 축을 오염시키는 성분은 반드시 먼저 뺀다.

        로버스트 적합(잔차 상위 절단)을 쓴다 — 그냥 맞추면 솟아 있는 손톱
        쪽으로 면이 들려 단차가 그만큼 깎인다.

        반환: (residuals, model_name, rms_mm, tilt_deg)
        """
        pts = np.column_stack([np.asarray(xy, dtype=float),
                               np.asarray(contact_z, dtype=float)])
        model = fit_surface(pts, mode=self.get_parameter('surface_model').value)
        return model.residuals(pts), model.kind, model.rms, model.tilt_deg

    def _classify_points(self, points, xy, coarse_pitch, area_x, area_y):
        """다특징 융합 판별. 반환: (Classification, residuals, model_name, rms, tilt).

        배경 표면을 **반복 재적합**한다. 한 번만 맞추면 솟아 있는 손톱이
        배경을 자기 쪽으로 들어올려, 정작 그 손톱의 단차가 깎인다. 첫 판별로
        손톱을 대충 떼어낸 뒤 나머지(피부·작업대)에만 다시 맞추면 배경이
        편향되지 않고, 단차가 온전히 잔차로 남는다.

        손톱을 뺀 나머지는 손톱을 둘러싼 고리 모양이므로, 그 위에서 맞춘
        곡면을 손톱 자리에서 읽는 것은 외삽이 아니라 내삽이다.
        """
        p = self.get_parameter
        pts3 = np.column_stack([np.asarray(xy, dtype=float),
                                np.asarray([pt.contact_z_mm for pt in points], dtype=float)])
        mode = p('surface_model').value

        # 배경 씨앗 — 공칭 손톱 영역 밖의 테두리. 여기는 손톱일 수 없다.
        seed = self._background_seed(xy, area_x, area_y)
        bandwidth = max(area_x, area_y) * p('background_bandwidth_ratio').value
        model = fit_surface(pts3[seed] if seed.sum() >= 8 else pts3, mode=mode,
                            bandwidth_mm=bandwidth)
        resid = model.residuals(pts3)

        if p('classifier_method').value == 'stiffness':
            # 예전 동작으로 되돌리는 탈출구. 강성 축이 살아 있는 실물에서
            # 비교 기준이 필요할 때 쓴다.
            from .nail_classifier import Classification
            vals = np.array([pt.stiffness_n_per_mm for pt in points], dtype=float)
            thr = compute_threshold(vals)
            labels = vals >= thr
            res = Classification(threshold=float(thr), scores=vals, labels=labels,
                                 method='stiffness')
            hi, lo = vals[labels], vals[~labels]
            res.margin = separation_margin(hi, lo) if hi.size and lo.size else 0.0
            res.stiffness_threshold = float(thr)
            return res, resid, model.kind, model.rms, model.tilt_deg

        def run(residuals):
            return classify(
                points, extra={'height_residual_mm': residuals}, xy=xy,
                min_feature_margin=p('min_feature_margin').value,
                min_feature_coherence=p('min_feature_coherence').value,
                threshold_method=p('cluster_method').value,
                coherence_radius_mm=coarse_pitch * p('coherence_radius_ratio').value,
                flip_band_sigma=p('flip_band_sigma').value)

        min_background = max(8, int(len(points) * p('background_min_ratio').value))
        for _ in range(max(0, int(p('surface_refit_passes').value))):
            res = run(resid)
            background = ~res.labels
            if background.sum() < min_background:
                # 배경 표본이 이만큼도 안 남았다는 건 분류가 무너졌다는 뜻이다.
                # 그 위에서 다시 맞추면 배경이 손톱을 따라가 버린다 — 멈춘다.
                break
            refit = fit_surface(pts3[background], mode=mode, bandwidth_mm=bandwidth)
            candidate = refit.residuals(pts3)
            if not np.all(np.isfinite(candidate)):
                break
            model, resid = refit, candidate

        final = run(resid)
        if model.rms > 1e-6:
            final.height_snr = abs(final.height_step_mm) / model.rms
        return final, resid, model.kind, model.rms, model.tilt_deg

    def _background_seed(self, xy, area_x, area_y):
        """손톱일 수 없는 테두리 점 마스크.

        스캔 영역은 "공칭 손톱 영역 + scan_margin_mm" 로 잡혀 있고, 마진이
        존재하는 이유가 경계가 공칭 영역 밖에 있을 수 있다는 것이다. 뒤집어
        말하면 공칭 영역 **밖**은 손톱이 아니라고 봐도 된다.
        """
        pts = np.asarray(xy, dtype=float)
        if pts.size == 0:
            return np.zeros(0, dtype=bool)
        hx = max(area_x / 2.0, 1e-6)
        hy = max(area_y / 2.0, 1e-6)
        ratio = self.get_parameter('background_seed_ratio').value
        r2 = (pts[:, 0] / hx) ** 2 + (pts[:, 1] / hy) ** 2
        seed = r2 >= ratio ** 2
        if seed.sum() >= 8:
            return seed
        # 마진이 좁아 테두리 점이 부족하면 반경 상위 40% 로 대체한다.
        k = max(8, int(pts.shape[0] * 0.4))
        fallback = np.zeros(pts.shape[0], dtype=bool)
        fallback[np.argsort(r2)[-k:]] = True
        return fallback

    def _accept(self, cls, margin_min, hard_max_ratio, snr_min):
        """판별을 신뢰할지 결정한다. 반환: (ok, detail).

        축에 맞는 자를 댄다. 힘 축이 이끄는 판별은 예전대로 군집 분리도로
        보고, 높이 축이 이끄는 판별은 "단차 / 배경 잔차" 로 본다. 어느 쪽이든
        한쪽 군집이 영역을 거의 다 차지하면 그건 분할이 아니라 붕괴다.
        """
        total = cls.hard_count + cls.soft_count
        ratio = cls.hard_count / max(1, total)
        if ratio > hard_max_ratio:
            return False, (f'고강성 군집이 영역의 {ratio:.0%} — 상한 {hard_max_ratio:.0%} '
                           f'초과. 분할이 아니라 붕괴로 본다')
        if cls.margin >= margin_min:
            return True, f'분리도 {cls.margin:.2f} ≥ {margin_min}'
        if height_driven(cls):
            if cls.height_snr >= snr_min:
                return True, (f'분리도는 {cls.margin:.2f} 로 낮지만 높이 축이 판별을 '
                              f'이끌고 단차/배경잡음 = {cls.height_snr:.1f} ≥ {snr_min} — '
                              f'기하 기준으로 통과')
            return False, (f'높이 축 기준 미달: 단차 {cls.height_step_mm:+.3f}mm, '
                           f'단차/배경잡음 {cls.height_snr:.1f} < {snr_min}')
        return False, f'분리도 {cls.margin:.2f} < {margin_min}'

    def _fine_profile(self, result):
        """정밀 스캔에서 어떤 프로파일을 쓸지 거친 스캔 결과로 정한다.

        거친 단계에서 힘 축이 하나도 가중치를 못 받았다면(재질이 같은 모형이
        정확히 그렇다) 정밀 단계에서 유지·계단·제하를 재는 건 순수한 낭비다.
        높이만 뜨면 점당 시간이 1/3 로 줄고, 얕게 눌러도 되니 미끄러질 기회도
        함께 준다.
        """
        setting = self.get_parameter('probe_profile_fine').value
        if setting != 'auto':
            return setting
        force_axes = {name for name in result.weights if name != 'height_residual_mm'}
        if force_axes:
            return ProbePointAction.Goal.PROFILE_FULL
        self.get_logger().info(
            '거친 스캔에서 힘 특징이 전부 무의미 — 정밀 스캔은 높이만 측정한다 '
            '(재질이 같은 모형에서 정상)')
        return ProbePointAction.Goal.PROFILE_HEIGHT

    # --- 튜닝용 기록 ------------------------------------------------------------------
    def _log_points(self, session_id, points, xy, resid, result):
        """점별 특징을 CSV 로 남긴다.

        어느 축이 실제로 갈리는지는 실물 데이터를 봐야만 정해진다. 파형은
        너무 크고 맵 메시지는 사람이 읽기 나쁘므로, 특징만 따로 뽑아 둔다.
        """
        directory = self.get_parameter('probe_log_dir').value
        if not directory:
            return
        try:
            os.makedirs(directory, exist_ok=True)
            path = os.path.join(
                directory, f'scan_{session_id}_{time.strftime("%Y%m%d_%H%M%S")}.csv')
            names = [name for name, _d, _doc in FEATURE_SPECS]
            with open(path, 'w', newline='') as fh:
                w = csv.writer(fh)
                w.writerow(['x_mm', 'y_mm', 'source', 'label_nail', 'score',
                             'contact_z_mm', 'contact_travel_mm', 'height_residual_mm',
                             'surface_tilt_deg', 'slip_ratio', 'slip_events',
                             'stiffness_valid', 'curve_valid', 'hold_valid',
                             'reject_reason'] + names)
                for i, pt in enumerate(points):
                    w.writerow([
                        f'{xy[i][0]:.3f}', f'{xy[i][1]:.3f}', pt.source,
                        int(result.labels[i]), f'{result.scores[i]:.4f}',
                        f'{pt.contact_z_mm:.4f}', f'{pt.contact_travel_mm:.4f}',
                        f'{resid[i]:.4f}', f'{pt.surface_tilt_deg:.2f}',
                        f'{pt.slip_ratio:.3f}', pt.slip_events,
                        int(pt.stiffness_valid), int(pt.curve_valid), int(pt.hold_valid),
                        pt.reject_reason,
                    ] + [f'{(resid[i] if n == "height_residual_mm" else getattr(pt, n, 0.0)):.4f}'
                         for n in names])
            self.get_logger().info(f'스캔 특징 기록: {path}')
        except OSError as e:
            self.get_logger().warn(f'스캔 특징 기록 실패 ({e}) — 스캔은 계속한다')

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
        max_force_n = p('probe_max_force_n').value
        probe_timeout_s = p('probe_timeout_s').value
        no_contact_retry = int(p('probe_no_contact_retry').value)
        base_height = p('probe_approach_height_mm').value
        max_depth = p('probe_depth_mm').value or \
            (base_height + p('probe_depth_allowance_mm').value)
        target_travel = p('probe_target_travel_mm').value
        coarse_profile = p('probe_profile_coarse').value or ProbePointAction.Goal.PROFILE_FULL

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

        # --- 0단계: 작업면 사전 측정 ------------------------------------------------
        travel_plane = None
        if p('survey_enabled').value:
            feedback('COARSE', None, 0, 0, 0, 0.0, 0.0)
            travel_plane, _survey_points, halt = self._survey_work_plane(
                frame_id, area_x, area_y, margin, goal_handle)
            if halt:
                return self._abort_result(goal_handle, halt, '작업면 사전 측정 중 취소/안전 위반',
                                           started_at)

        def probe_at(x, y, source_tag, profile):
            return self._probe_with_retry(
                x, y, frame_id, source_tag, probe_timeout_s, goal_handle, no_contact_retry,
                profile=profile,
                approach_height_mm=self._approach_height_for(
                    travel_plane, x, y, base_height, target_travel),
                max_depth_mm=max_depth, max_force_n=max_force_n)

        def run_coarse(pitch):
            grid = make_grid(area_x, area_y, margin, pitch)
            points = {}
            n = len(grid)
            for idx, (key, (x, y)) in enumerate(grid.items()):
                halt = self._aborted(goal_handle)
                if halt:
                    return grid, points, halt
                point, err = probe_at(x, y, StiffnessPoint.SRC_COARSE, coarse_profile)
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
            return [(key, pt) for key, pt in points_dict.items()
                    if pt is not None and pt.valid]

        valid_coarse = valid_list(coarse_points)
        insufficiency = self._check_coarse_sufficiency(
            [pt for _k, pt in valid_coarse], len(coarse_grid), coarse_min_valid,
            p('invalid_point_max_ratio').value)
        if insufficiency:
            code, detail = insufficiency
            self._log_abort(code, detail)
            goal_handle.abort()
            result = ScanBoundary.Result()
            result.base = self._result_base(False, code, detail, started_at)
            return result

        coarse_xy = [coarse_grid[k] for k, _pt in valid_coarse]
        coarse_pts = [pt for _k, pt in valid_coarse]
        cls, resid, model, rms, tilt = self._classify_points(
            coarse_pts, coarse_xy, coarse_pitch, area_x, area_y)
        self.get_logger().info(f'거친 스캔 판별 [{model} rms={rms:.3f}mm '
                                f'tilt={tilt:.1f}°] {describe(cls)}')
        travel_plane = self._refine_travel_model(travel_plane, coarse_xy, coarse_pts)

        # --- 완화책: 군집 점수가 아슬아슬하면 더 촘촘한 피치로 한 번 더 (SDS §5.2 ⚠️) ---
        marginal = (cls.hard_count < coarse_min_per_cluster * 1.5 or
                    cls.soft_count < coarse_min_per_cluster * 1.5)
        if marginal:
            retry_pitch = p('coarse_retry_pitch_mm').value
            self.get_logger().warn(
                f'거친 스캔 군집 점수 아슬아슬(hard={cls.hard_count} soft={cls.soft_count}, '
                f'기준 {coarse_min_per_cluster}) — {retry_pitch}mm 로 재스캔')
            coarse_grid, coarse_points, halt = run_coarse(retry_pitch)
            if halt:
                return self._abort_result(goal_handle, halt, '거친 재스캔 중 취소/안전 위반',
                                           started_at)
            valid_coarse = valid_list(coarse_points)
            insufficiency = self._check_coarse_sufficiency(
                [pt for _k, pt in valid_coarse], len(coarse_grid), coarse_min_valid,
                p('invalid_point_max_ratio').value)
            if insufficiency:
                code, detail = insufficiency
                detail = f'재스캔 후에도: {detail}'
                self._log_abort(code, detail)
                goal_handle.abort()
                result = ScanBoundary.Result()
                result.base = self._result_base(False, code, detail, started_at)
                return result
            coarse_pitch = retry_pitch
            coarse_xy = [coarse_grid[k] for k, _pt in valid_coarse]
            coarse_pts = [pt for _k, pt in valid_coarse]
            cls, resid, model, rms, tilt = self._classify_points(
                coarse_pts, coarse_xy, coarse_pitch, area_x, area_y)
            self.get_logger().info(f'거친 재스캔 판별 [{model}] {describe(cls)}')

        ok, why = self._accept(cls, separation_margin_min,
                               p('hard_cluster_max_ratio').value,
                               p('height_step_snr_min').value)
        if cls.hard_count < coarse_min_per_cluster or \
                cls.soft_count < coarse_min_per_cluster or not ok:
            detail = (f'경계 판별 불가: {why} (hard={cls.hard_count} '
                      f'soft={cls.soft_count}, 군집 최소 {coarse_min_per_cluster}) | '
                      f'{describe(cls)}')
            self._log_points(goal.session_id, coarse_pts, coarse_xy, resid, cls)
            return self._publish_invalid_and_abort(
                goal_handle, ErrorCode.E_SEPARATION_LOW, detail, started_at, goal.session_id,
                frame_id, coarse_pts, [], cls, resid, coarse_pitch, fine_pitch,
                model, rms, tilt)

        # --- 경계 후보 선정 ------------------------------------------------------
        # (1) 라벨이 갈리는 4-이웃 쌍의 중점
        label_of = {}
        for i, (key, _pt) in enumerate(valid_coarse):
            label_of[key] = bool(cls.labels[i])
        candidates = []
        for a_key, b_key in adjacent_pairs_4(coarse_grid):
            if a_key not in label_of or b_key not in label_of:
                continue
            if label_of[a_key] != label_of[b_key]:
                candidates.append(_midpoint(coarse_grid[a_key], coarse_grid[b_key]))

        # (2) 높이 기울기가 큰 곳. 재질이 같으면 경계는 높이가 급히 꺾이는
        #     능선으로만 나타나므로, 라벨이 아직 흔들리는 구간도 여기서 잡힌다.
        slopes = local_slope(coarse_xy, resid, coarse_pitch * 1.6)
        if slopes.size:
            ratio = p('gradient_candidate_ratio').value
            k = max(1, int(round(slopes.size * ratio)))
            for idx in np.argsort(slopes)[-k:]:
                if slopes[idx] > 0.0:
                    candidates.append(coarse_xy[int(idx)])
        feedback('CANDIDATE', None, 0, 0, len(candidates), 100.0, 30.0)

        # --- 2단계: 정밀 스캔 -----------------------------------------------------
        fine_profile = self._fine_profile(cls)
        measured_coarse_xy = set(coarse_grid.values())
        tol = coarse_pitch * 0.1
        band_r = boundary_band / 2.0
        fine_full = make_grid(area_x, area_y, margin, fine_pitch)

        def already_measured(xy_):
            return any(_dist(xy_, m) <= tol for m in measured_coarse_xy)

        fine_targets = []
        if candidates:
            for xy_ in fine_full.values():
                if already_measured(xy_):
                    continue
                if any(_dist(xy_, c) <= band_r for c in candidates):
                    fine_targets.append(xy_)

        if len(fine_targets) > fine_max_points:
            fine_targets.sort(key=lambda v: min(_dist(v, c) for c in candidates))
            self.get_logger().warn(
                f'정밀 격자 {len(fine_targets)}점 > fine_max_points({fine_max_points}) — '
                '후보점 근접 우선으로 절단')
            fine_targets = fine_targets[:fine_max_points]

        fine_points, fine_xy = [], []
        n_fine = len(fine_targets)
        for idx, (x, y) in enumerate(fine_targets):
            halt = self._aborted(goal_handle)
            if halt:
                return self._abort_result(goal_handle, halt, '정밀 스캔 중 취소/안전 위반',
                                           started_at)
            point, err = probe_at(x, y, StiffnessPoint.SRC_FINE, fine_profile)
            if err == 'CANCELLED':
                return self._abort_result(goal_handle, 'CANCELLED', '정밀 스캔 중 취소',
                                           started_at)
            if point is not None and point.valid:
                fine_points.append(point)
                fine_xy.append((x, y))
            feedback('FINE', point, idx + 1, n_fine, len(candidates),
                     100.0 * (idx + 1) / max(1, n_fine),
                     30.0 + 70.0 * (idx + 1) / max(1, n_fine))

        # --- 판정 및 산출 ---------------------------------------------------------
        all_points = coarse_pts + fine_points
        all_xy = list(coarse_xy) + fine_xy
        cls2, resid2, model2, rms2, tilt2 = self._classify_points(
            all_points, all_xy, min(coarse_pitch, fine_pitch), area_x, area_y)
        self.get_logger().info(f'최종 판별 [{model2} rms={rms2:.3f}mm] {describe(cls2)}')
        self._log_points(goal.session_id, all_points, all_xy, resid2, cls2)

        ok2, why2 = self._accept(cls2, separation_margin_min,
                                 p('hard_cluster_max_ratio').value,
                                 p('height_step_snr_min').value)
        if not ok2:
            detail = f'정밀 데이터 반영 후에도 판별 불가: {why2} | {describe(cls2)}'
            return self._publish_invalid_and_abort(
                goal_handle, ErrorCode.E_SEPARATION_LOW, detail, started_at, goal.session_id,
                frame_id, coarse_pts, fine_points, cls2, resid2, coarse_pitch, fine_pitch,
                model2, rms2, tilt2)

        self.get_logger().info(f'경계 확정: {why2}')
        self._annotate(all_points, cls2, resid2)
        hard_xy = [all_xy[i] for i in range(len(all_points)) if cls2.labels[i]]
        soft_xy = [all_xy[i] for i in range(len(all_points)) if not cls2.labels[i]]
        boundary_xy = convex_hull(hard_xy)
        forbidden_xy = convex_hull(soft_xy)

        stiffness_map = self._build_map(
            goal.session_id, frame_id, all_points, coarse_pitch, fine_pitch,
            len(coarse_pts), len(fine_points), len(candidates), True, cls2,
            coarse_pitch, boundary_xy, forbidden_xy, '', model2, rms2, tilt2)

        self._broadcast_nail_local_frame(frame_id, boundary_xy)
        self._store_and_publish_map(goal.session_id, stiffness_map)

        goal_handle.succeed()
        result = ScanBoundary.Result()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        result.map = stiffness_map
        return result

    @staticmethod
    def _annotate(points, cls, resid):
        """판별 결과를 점 메시지에 되돌려 심는다 (웹·리포트가 그대로 읽는다)."""
        for i, pt in enumerate(points):
            pt.height_residual_mm = float(resid[i])
            pt.nail_score = float(cls.scores[i])

    # --- 실패 시 맵(valid=false) 발행 후 ABORT (NIS §6.1 에러표) -----------------
    def _publish_invalid_and_abort(self, goal_handle, code, detail, started_at, session_id,
                                    frame_id, coarse_points, fine_points, cls, resid,
                                    coarse_pitch, fine_pitch, model, rms, tilt):
        self._log_abort(code, detail)
        all_points = list(coarse_points) + [pt for pt in fine_points if pt is not None]
        self._annotate(all_points, cls, resid)
        stiffness_map = self._build_map(
            session_id, frame_id, all_points, coarse_pitch, fine_pitch, len(coarse_points),
            len(fine_points), 0, False, cls, coarse_pitch, [], [], detail, model, rms, tilt)
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
                    coarse_count, fine_count, candidate_count, valid, cls, pitch_used,
                    boundary_xy, forbidden_xy, reject_reason, model, rms, tilt):
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
        msg.threshold_k_n_per_mm = cls.stiffness_threshold
        msg.separation_margin = cls.margin
        msg.cluster_hard_count = cls.hard_count
        msg.cluster_soft_count = cls.soft_count
        msg.reject_reason = reject_reason

        msg.classifier_method = cls.method
        msg.discriminant_threshold = cls.threshold
        names = sorted(cls.feature_margins, key=lambda k: -cls.feature_margins[k])
        msg.feature_names = names
        msg.feature_margins = [cls.feature_margins[n] for n in names]
        msg.feature_weights = [cls.weights.get(n, 0.0) for n in names]
        msg.height_step_mm = cls.height_step_mm
        msg.height_step_snr = cls.height_snr
        msg.spatial_flips = cls.spatial_flips
        msg.work_plane_tilt_deg = tilt
        msg.work_plane_rms_mm = rms

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
