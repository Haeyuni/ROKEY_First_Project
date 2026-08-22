"""session_orchestrator — 세션 상태 머신 · 재작업 루프 (NIS §8, C계층).

**직접 로봇을 움직이지 않는다.** 공정 액션 8종(`ChangeTool` ·
`ScanBoundary` · `SandSurface` · `BrushDust` · `CoatGel` · `CureUV` ·
`InspectCure` · `PlaceStone`)을 순서대로 호출하고 결과를 이어 붙이는
것만 한다. 손가락 1개(D-07)이므로 반복은 레이어 하나뿐이다(§8 표기).

여덟 개의 하위 액션이 전부 "goal 보내고 → feedback 받고 → 취소/안전/
타임아웃 감시하며 결과 대기"라는 같은 뼈대를 쓰므로, 스킬 노드들처럼
액션마다 개별 헬퍼를 쓰지 않고 `_call_action()` 하나로 묶었다 — 여기서도
같은 코드를 여덟 번 반복하면 유지보수가 안 된다.

**문서에 없어서 이 구현이 채운 빈틈들**:

1. **HOME 위치**: NIS 어디에도 HOME 의 좌표/TF 프레임 이름이 없다. 툴랙
   슬롯(`slot_*`)과 같은 방식 — `static_transform_publisher` 로 고정된
   TF 프레임을 쓴다고 가정하고, 파라미터(`home_frame_id`, 기본
   `home_frame`)로 이름만 받는다. 실제 launch 파일에 이 프레임을
   발행하는 노드가 있어야 동작한다.
2. **PRECHECK 의 "툴 랙 전수 확인"**: `/tool/get_info` 는 랙 슬롯이
   `rack_config.yaml` 에 *설정*돼 있는지만 답한다 — 슬롯에 물리적으로
   툴이 실제로 꽂혀 있는지 확인할 센서/인터페이스가 없다. 그래서 이
   PRECHECK 단계는 "설정 완결성"만 검증하고, 실제 부재는 이후 첫
   `ChangeTool` 실행 중 `E_GRIP_FAILED` 로 늦게 드러난다 — PickPlace 의
   그리퍼 폭이 명령값이지 실측이 아닌 것과 같은 종류의 하드웨어 한계다.
3. **`PlaceStone.target_position`**: `RunSession.Goal` 에 스톤 부착 좌표를
   실어 보낼 필드가 아예 없다. `enable_stone=true` 인 세션에서는 스캔
   결과 `boundary_polygon` 의 중심점(yaw=0)에 놓는다 — 정확한 부착
   위치 결정 로직은 이 문서 범위 밖이라 자리표시자로만 채운다.
   `enable_stone` 기본값이 false 이고 "축소 대상"으로 명시된 기능이라
   당장 이 한계가 실사용에 걸릴 가능성은 낮다.
4. **진행률**: 각 스테이지에 동일 가중치를 준 "몇 번째 단계/전체 단계"
   비율에, 현재 하위 액션이 보내는 feedback.percent 를 그 단계 내 분수로
   얹는다. NIS 는 가중치 산정 방식을 규정하지 않는다.
"""
import threading
import time

from geometry_msgs.msg import Point, Pose
import rclpy
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup, ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import (
    BrushDust, ChangeTool, CoatGel, CureUV, InspectCure, MoveTo, PlaceStone, RunSession,
    SandSurface, ScanBoundary,
)
from nail_msgs.msg import ErrorCode, ProcessState, SafetyState, ToolState
from nail_msgs.srv import GetToolInfo


class SessionOrchestratorNode(Node):

    def __init__(self):
        super().__init__('session_orchestrator')
        self._declare_parameters()

        self._latest_safety = None
        self._last_safety_rx_monotonic = None
        safety_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                                 durability=DurabilityPolicy.TRANSIENT_LOCAL)
        state_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                                durability=DurabilityPolicy.TRANSIENT_LOCAL)

        self._cb_action = MutuallyExclusiveCallbackGroup()
        self._cb_client = ReentrantCallbackGroup()

        self.create_subscription(SafetyState, '/safety/status', self._on_safety_status,
                                  safety_qos, callback_group=self._cb_client)

        self._status_pub = self.create_publisher(ProcessState, '/process/status', state_qos)
        self._current_state = ProcessState()
        self._current_state.stage = ProcessState.STAGE_IDLE
        self.create_timer(1.0, self._on_publish_timer, callback_group=self._cb_client)

        self._active_goal_handle = None
        self._get_tool_info_client = self.create_client(
            GetToolInfo, '/tool/get_info', callback_group=self._cb_client)
        self._change_tool_client = ActionClient(self, ChangeTool, '/tool/change',
                                                 callback_group=self._cb_client)
        self._scan_client = ActionClient(self, ScanBoundary, '/process/scan',
                                          callback_group=self._cb_client)
        self._sand_client = ActionClient(self, SandSurface, '/process/sand',
                                          callback_group=self._cb_client)
        self._brush_client = ActionClient(self, BrushDust, '/process/brush',
                                           callback_group=self._cb_client)
        self._coat_client = ActionClient(self, CoatGel, '/process/coat',
                                          callback_group=self._cb_client)
        self._cure_client = ActionClient(self, CureUV, '/process/cure',
                                          callback_group=self._cb_client)
        self._inspect_client = ActionClient(self, InspectCure, '/process/inspect',
                                             callback_group=self._cb_client)
        self._stone_client = ActionClient(self, PlaceStone, '/process/place_stone',
                                           callback_group=self._cb_client)
        self._move_client = ActionClient(self, MoveTo, '/skill/move_to',
                                          callback_group=self._cb_client)

        self._run_server = ActionServer(
            self, RunSession, '/session/run',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self.get_logger().info('session_orchestrator ready (NIS §8)')

    # --- 파라미터 (NIS §8 표 + 구현상 필요한 추가값) ------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('node_timeout_s', 0.0)  # 세션 전체는 여러 단계 타임아웃의 합 — 자체 상한 없음
        d('safety_status_timeout_s', 0.2)
        d('log_force_data', False)
        d('layer_total', 2)
        d('max_rework', 2)
        d('max_rework_session', 5)
        d('enable_stone', False)
        d('enable_brush', True)
        d('stage_timeout_scan_s', 300.0)
        d('stage_timeout_sand_s', 120.0)
        d('stage_timeout_brush_s', 60.0)
        d('stage_timeout_coat_s', 90.0)
        d('stage_timeout_cure_s', 150.0)
        d('stage_timeout_inspect_s', 60.0)
        d('stage_timeout_stone_s', 90.0)
        d('tool_change_timeout_s', 60.0)
        d('abort_return_home', True)
        d('precheck_require_all_tools', True)
        # NIS 표에 없는 구현 보조값 — docstring 참고
        d('home_frame_id', 'home_frame')
        d('home_timeout_s', 30.0)
        d('rework_exposure_scale', 1.5)

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
        if self._active_goal_handle is not None:
            self._active_goal_handle.cancel_goal_async()
        return CancelResponse.ACCEPT

    def _on_goal(self, goal_request):
        if not goal_request.session_id:
            self.get_logger().warn('RunSession REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('RunSession REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _val(self, goal_value, param_name):
        return goal_value if goal_value and goal_value > 0.0 else \
            self.get_parameter(param_name).value

    # =========================================================================
    # 모든 하위 액션이 공유하는 send/wait/cancel/timeout 뼈대 (§3.2/§3.3)
    # =========================================================================
    def _call_action(self, client, goal, our_goal_handle, timeout_s, feedback_cb=None,
                      ignore_cancel=False):
        """반환: (result | None, error_code | None | 'CANCELLED').

        ignore_cancel=True 는 §3.2 뒷정리(툴 반납·HOME) 전용이다 — 세션이
        이미 취소 요청을 받은 상태에서 뒷정리 호출까지 같은 goal_handle 의
        is_cancel_requested 를 다시 검사하면, 뒷정리 액션 자체가 시작하자마자
        취소돼 "취소돼도 이탈·HOME 은 한다"(§3.2)를 지킬 수 없다. 안전 검사는
        그대로 둔다 — 안전이 막힌 상태에서 강제로 움직이는 건 애초에 하위
        노드 goal_callback 에서도 거부되므로 의미가 없다.
        """
        if not client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_COMM_LOST

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        client.send_goal_async(goal, feedback_callback=feedback_cb).add_done_callback(
            on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return None, ErrorCode.E_TIMEOUT

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED
        self._active_goal_handle = gh

        result_done = threading.Event()

        def on_result(fut):
            state['result'] = fut.result()
            result_done.set()

        gh.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        while not result_done.wait(timeout=0.1):
            if not ignore_cancel and our_goal_handle.is_cancel_requested:
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, 'CANCELLED'
            if not self._safe_to_move():
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_SAFETY_BLOCKED
            if time.monotonic() > deadline:
                gh.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_TIMEOUT
        self._active_goal_handle = None

        result = state['result'].result
        if not result.base.success:
            return result, result.base.error.code
        return result, None

    def _call_change_tool(self, target_tool, our_goal_handle, timeout_s, ignore_cancel=False):
        goal = ChangeTool.Goal()
        goal.target_tool = target_tool
        goal.verify_after_grip = True
        return self._call_action(self._change_tool_client, goal, our_goal_handle, timeout_s,
                                  ignore_cancel=ignore_cancel)

    # --- PRECHECK (NIS §8: 안착 · 툴 랙 전수 · 통신 · E-Stop 해제) -----------------
    def _run_precheck(self, required_tools):
        reasons = []
        safety = self._latest_safety
        if safety is None:
            return False, ['안전 상태 미수신']
        if not safety.handrest_seated:
            reasons.append('안착 센서 OFF')
        if not safety.comm_ok:
            reasons.append('통신 두절')
        if not safety.estop_released:
            reasons.append('E-Stop 눌림')
        if not safety.safe_to_move:
            reasons.append(f'safe_to_move=false ({list(safety.active_faults)})')

        if self.get_parameter('precheck_require_all_tools').value:
            if not self._get_tool_info_client.wait_for_service(timeout_sec=5.0):
                reasons.append('/tool/get_info 서비스 연결 실패')
            else:
                for tool_id in required_tools:
                    req = GetToolInfo.Request()
                    req.tool_id = tool_id
                    future = self._get_tool_info_client.call_async(req)
                    deadline = time.monotonic() + 5.0
                    while not future.done() and time.monotonic() < deadline:
                        time.sleep(0.02)
                    resp = future.result()
                    if resp is None or not resp.found:
                        reasons.append(f'툴 랙 설정 없음: {tool_id}')
        return len(reasons) == 0, reasons

    # --- HOME 복귀 (docstring #1 — home_frame_id 가정) ----------------------------
    def _return_home(self, our_goal_handle):
        goal = MoveTo.Goal()
        goal.target = Pose()
        goal.target.orientation.w = 1.0
        goal.frame_id = self.get_parameter('home_frame_id').value
        goal.linear = False
        goal.speed_ratio = 0.3
        goal.accel_ratio = 0.3
        goal.timeout_s = self.get_parameter('home_timeout_s').value
        result, err = self._call_action(self._move_client, goal, our_goal_handle,
                                         goal.timeout_s, ignore_cancel=True)
        if err is not None:
            self.get_logger().error(f'[SAFETY] HOME 복귀 실패({err}) — 수동 확인 필요')
            return False
        return True

    # --- ProcessState 발행 (즉시 전이 시 + 1Hz, §8 피드백) -------------------------
    def _publish_state(self, goal_handle, session_id, stage, layer_index, layer_total,
                        rework_count, stage_percent, session_percent, current_tool,
                        last_error_code='', last_error_detail=''):
        st = ProcessState()
        st.header.stamp = self.get_clock().now().to_msg()
        st.session_id = session_id
        st.stage = stage
        st.layer_index = layer_index
        st.layer_total = layer_total
        st.rework_count = rework_count
        st.stage_percent = stage_percent
        st.session_percent = session_percent
        st.current_tool = current_tool
        st.last_error.code = last_error_code
        st.last_error.detail = last_error_detail
        self._current_state = st
        self._status_pub.publish(st)
        if goal_handle is not None:
            fb = RunSession.Feedback()
            fb.state = st
            goal_handle.publish_feedback(fb)

    def _on_publish_timer(self):
        # §8: "상태 전이 시 즉시 + 1Hz" — 마지막으로 만든 상태를 그대로 재발행한다.
        self._current_state.header.stamp = self.get_clock().now().to_msg()
        self._status_pub.publish(self._current_state)

    # --- 진행 단계 시퀀스 (docstring #4) -----------------------------------------
    @staticmethod
    def _build_sequence(enable_brush, enable_stone, layer_total):
        seq = ['PRECHECK', 'SCAN', 'SAND']
        if enable_brush:
            seq.append('BRUSH')
        for i in range(layer_total):
            seq += [f'COAT{i}', f'CURE{i}', f'INSPECT{i}']
        if enable_stone:
            seq.append('STONE')
        seq.append('FINISH')
        return seq

    # =========================================================================
    def _execute(self, goal_handle):
        goal = goal_handle.request
        started_at = self.get_clock().now().to_msg()
        started_mono = time.monotonic()
        result = RunSession.Result()
        session_id = goal.session_id

        layer_total = int(self._val(goal.layer_total, 'layer_total'))
        max_rework = int(self._val(goal.max_rework, 'max_rework'))
        max_rework_session = int(self.get_parameter('max_rework_session').value)
        # bool 필드는 "설정 안 함"과 False 를 구분 못 한다 — coating_node.use_compliance 와
        # 동일하게 goal 값을 그대로 신뢰한다.
        enable_brush = goal.enable_brush
        enable_stone = goal.enable_stone

        p = self.get_parameter
        t_scan = p('stage_timeout_scan_s').value
        t_sand = p('stage_timeout_sand_s').value
        t_brush = p('stage_timeout_brush_s').value
        t_coat = p('stage_timeout_coat_s').value
        t_cure = p('stage_timeout_cure_s').value
        t_inspect = p('stage_timeout_inspect_s').value
        t_stone = p('stage_timeout_stone_s').value
        t_tool = p('tool_change_timeout_s').value

        seq = self._build_sequence(enable_brush, enable_stone, layer_total)
        n_steps = len(seq)

        def progress(step_name, local_pct):
            idx = seq.index(step_name)
            return 100.0 * (idx + max(0.0, min(100.0, local_pct)) / 100.0) / n_steps

        state = {
            'layer_index': 0, 'rework_count': 0, 'current_tool': ToolState.NONE,
            'layer_total': layer_total,
        }

        def emit(stage, local_pct, layer_index=None):
            step_key = f'{stage}{layer_index}' if layer_index is not None and \
                f'{stage}{layer_index}' in seq else stage
            pct = progress(step_key, local_pct) if step_key in seq else 0.0
            self._publish_state(
                goal_handle, session_id, stage, state['layer_index'], layer_total,
                state['rework_count'], local_pct, pct, state['current_tool'])

        all_results = []
        scan_map = None
        total_rework = 0
        abort_code = None
        abort_detail = ''

        # --- PRECHECK ----------------------------------------------------------
        emit(ProcessState.STAGE_PRECHECK, 0.0)
        required_tools = ['probe', 'sander', 'coater', 'uv']
        if enable_brush:
            required_tools.append('brush')
        if enable_stone:
            required_tools.append('tweezers')
        ok, reasons = self._run_precheck(required_tools)
        if not ok:
            abort_code, abort_detail = ErrorCode.E_PRECOND_FAILED, f'PRECHECK 실패: {reasons}'
            return self._finish_abort(goal_handle, result, abort_code, abort_detail,
                                       started_at, started_mono, state, all_results,
                                       scan_map, total_rework)
        emit(ProcessState.STAGE_PRECHECK, 100.0)

        # --- SCAN ----------------------------------------------------------------
        ct_result, err = self._call_change_tool(ToolState.PROBE, goal_handle, t_tool)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'ChangeTool(probe) 실패',
                                        started_at, started_mono, state, all_results,
                                        scan_map, total_rework)
        state['current_tool'] = ToolState.PROBE

        emit(ProcessState.STAGE_SCAN, 0.0)
        scan_goal = ScanBoundary.Goal()
        scan_goal.session_id = session_id

        def on_scan_fb(fb_msg):
            emit(ProcessState.STAGE_SCAN, fb_msg.feedback.overall_percent)

        scan_result, err = self._call_action(self._scan_client, scan_goal, goal_handle, t_scan,
                                              feedback_cb=on_scan_fb)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'ScanBoundary 실패',
                                        started_at, started_mono, state, all_results,
                                        scan_map, total_rework)
        scan_map = scan_result.map
        if not scan_map.valid:
            return self._finish_abort(
                goal_handle, result, ErrorCode.E_NO_SCAN,
                f'스캔 invalid: {scan_map.reject_reason}', started_at, started_mono, state,
                all_results, scan_map, total_rework)
        emit(ProcessState.STAGE_SCAN, 100.0)

        # --- SAND ------------------------------------------------------------------
        ct_result, err = self._call_change_tool(ToolState.SANDER, goal_handle, t_tool)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'ChangeTool(sander) 실패',
                                        started_at, started_mono, state, all_results,
                                        scan_map, total_rework)
        state['current_tool'] = ToolState.SANDER

        emit(ProcessState.STAGE_SAND, 0.0)
        sand_goal = SandSurface.Goal()
        sand_goal.session_id = session_id

        def on_sand_fb(fb_msg):
            emit(ProcessState.STAGE_SAND, fb_msg.feedback.percent)

        sand_result, err = self._call_action(self._sand_client, sand_goal, goal_handle, t_sand,
                                              feedback_cb=on_sand_fb)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'SandSurface 실패',
                                        started_at, started_mono, state, all_results,
                                        scan_map, total_rework)
        emit(ProcessState.STAGE_SAND, 100.0)

        # --- BRUSH (옵션) ------------------------------------------------------------
        if enable_brush:
            ct_result, err = self._call_change_tool(ToolState.BRUSH, goal_handle, t_tool)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'ChangeTool(brush) 실패',
                                            started_at, started_mono, state, all_results,
                                            scan_map, total_rework)
            state['current_tool'] = ToolState.BRUSH

            emit(ProcessState.STAGE_BRUSH, 0.0)
            brush_goal = BrushDust.Goal()
            brush_goal.session_id = session_id

            def on_brush_fb(fb_msg):
                emit(ProcessState.STAGE_BRUSH, fb_msg.feedback.percent)

            brush_result, err = self._call_action(self._brush_client, brush_goal, goal_handle,
                                                   t_brush, feedback_cb=on_brush_fb)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'BrushDust 실패',
                                            started_at, started_mono, state, all_results,
                                            scan_map, total_rework)
            emit(ProcessState.STAGE_BRUSH, 100.0)

        # --- 레이어 루프: COAT → CURE → INSPECT (→ REWORK) ----------------------------
        for layer_index in range(layer_total):
            state['layer_index'] = layer_index

            # COAT
            ct_result, err = self._call_change_tool(ToolState.COATER, goal_handle, t_tool)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'ChangeTool(coater) 실패',
                                            started_at, started_mono, state, all_results,
                                            scan_map, total_rework)
            state['current_tool'] = ToolState.COATER

            emit(ProcessState.STAGE_COAT, 0.0, layer_index)
            coat_goal = CoatGel.Goal()
            coat_goal.session_id = session_id
            coat_goal.layer_index = layer_index
            # bool 필드는 "설정 안 함"과 False 를 구분 못 한다(coating_node 는
            # goal 값을 그대로 신뢰, 자체 폴백 없음) — NIS 기본값(true)을 여기서
            # 명시적으로 채워야 한다.
            coat_goal.use_compliance = True

            def on_coat_fb(fb_msg, _li=layer_index):
                emit(ProcessState.STAGE_COAT, fb_msg.feedback.percent, _li)

            coat_result, err = self._call_action(self._coat_client, coat_goal, goal_handle,
                                                  t_coat, feedback_cb=on_coat_fb)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'CoatGel 실패',
                                            started_at, started_mono, state, all_results,
                                            scan_map, total_rework)
            emit(ProcessState.STAGE_COAT, 100.0, layer_index)

            # CURE (최초 — 전체 영역)
            fail_points = []
            exposure_scale = 0.0
            rework_this_layer = 0
            while True:
                ct_result, err = self._call_change_tool(ToolState.UV, goal_handle, t_tool)
                if err is not None:
                    return self._finish_by_err(goal_handle, result, err,
                                                'ChangeTool(uv) 실패', started_at, started_mono,
                                                state, all_results, scan_map, total_rework)
                state['current_tool'] = ToolState.UV

                stage_label = ProcessState.STAGE_REWORK if rework_this_layer > 0 else \
                    ProcessState.STAGE_CURE
                emit(stage_label, 0.0, layer_index)
                cure_goal = CureUV.Goal()
                cure_goal.session_id = session_id
                cure_goal.layer_index = layer_index
                cure_goal.target_regions = fail_points
                cure_goal.exposure_scale = exposure_scale

                def on_cure_fb(fb_msg, _li=layer_index, _label=stage_label):
                    emit(_label, fb_msg.feedback.percent, _li)

                cure_result, err = self._call_action(self._cure_client, cure_goal, goal_handle,
                                                      t_cure, feedback_cb=on_cure_fb)
                if err is not None:
                    return self._finish_by_err(goal_handle, result, err, 'CureUV 실패',
                                                started_at, started_mono, state, all_results,
                                                scan_map, total_rework)
                emit(stage_label, 100.0, layer_index)

                # INSPECT
                ct_result, err = self._call_change_tool(ToolState.PROBE, goal_handle, t_tool)
                if err is not None:
                    return self._finish_by_err(goal_handle, result, err,
                                                'ChangeTool(probe) 실패', started_at,
                                                started_mono, state, all_results, scan_map,
                                                total_rework)
                state['current_tool'] = ToolState.PROBE

                emit(ProcessState.STAGE_INSPECT, 0.0, layer_index)
                inspect_goal = InspectCure.Goal()
                inspect_goal.session_id = session_id
                inspect_goal.layer_index = layer_index
                # CoatGel.use_compliance 와 같은 이유 — inspection_node 도 bool
                # 필드를 그대로 신뢰한다. NIS 기본값(true)을 명시한다.
                inspect_goal.require_all_pass = True

                def on_inspect_fb(fb_msg, _li=layer_index):
                    emit(ProcessState.STAGE_INSPECT, fb_msg.feedback.percent, _li)

                inspect_result, err = self._call_action(self._inspect_client, inspect_goal,
                                                          goal_handle, t_inspect,
                                                          feedback_cb=on_inspect_fb)
                if err is not None:
                    return self._finish_by_err(goal_handle, result, err, 'InspectCure 실패',
                                                started_at, started_mono, state, all_results,
                                                scan_map, total_rework)
                all_results.extend(inspect_result.results)
                emit(ProcessState.STAGE_INSPECT, 100.0, layer_index)

                if inspect_result.passed:
                    break

                rework_this_layer += 1
                total_rework += 1
                state['rework_count'] = rework_this_layer
                if rework_this_layer > max_rework or total_rework > max_rework_session:
                    detail = (f'레이어 {layer_index} 재작업 {rework_this_layer}회'
                              f'(상한 {max_rework}) / 세션 누적 {total_rework}회'
                              f'(상한 {max_rework_session})')
                    return self._finish_abort(
                        goal_handle, result, ErrorCode.E_REWORK_EXCEEDED, detail, started_at,
                        started_mono, state, all_results, scan_map, total_rework)

                fail_points = list(inspect_result.fail_points)
                exposure_scale = self.get_parameter('rework_exposure_scale').value
                self.get_logger().warn(
                    f'REWORK: layer={layer_index} fail_points={len(fail_points)}개 '
                    f'({rework_this_layer}/{max_rework})')
            state['rework_count'] = 0

        # --- STONE (옵션, docstring #3 — 목표 좌표 가정) ------------------------------
        if enable_stone:
            ct_result, err = self._call_change_tool(ToolState.TWEEZERS, goal_handle, t_tool)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err,
                                            'ChangeTool(tweezers) 실패', started_at,
                                            started_mono, state, all_results, scan_map,
                                            total_rework)
            state['current_tool'] = ToolState.TWEEZERS

            emit(ProcessState.STAGE_STONE, 0.0)
            boundary_xy = [(pt.x, pt.y) for pt in scan_map.region.boundary_polygon] \
                if scan_map is not None else []
            if boundary_xy:
                cx = sum(p[0] for p in boundary_xy) / len(boundary_xy)
                cy = sum(p[1] for p in boundary_xy) / len(boundary_xy)
            else:
                cx = cy = 0.0
            stone_goal = PlaceStone.Goal()
            stone_goal.session_id = session_id
            stone_goal.target_position = Point(x=cx, y=cy, z=0.0)
            stone_goal.target_yaw_deg = 0.0

            def on_stone_fb(fb_msg):
                emit(ProcessState.STAGE_STONE, fb_msg.feedback.percent)

            stone_result, err = self._call_action(self._stone_client, stone_goal, goal_handle,
                                                   t_stone, feedback_cb=on_stone_fb)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'PlaceStone 실패',
                                            started_at, started_mono, state, all_results,
                                            scan_map, total_rework)
            emit(ProcessState.STAGE_STONE, 100.0)

        # --- FINISH ------------------------------------------------------------------
        ct_result, err = self._call_change_tool(ToolState.NONE, goal_handle, t_tool)
        state['current_tool'] = ToolState.NONE
        emit(ProcessState.STAGE_FINISH, 100.0)

        goal_handle.succeed()
        result.success = True
        result.result_code = RunSession.Result.RESULT_COMPLETED
        if scan_map is not None:
            result.scan_result = scan_map
        result.all_results = all_results
        result.final_error.code = ErrorCode.OK
        result.total_rework = total_rework
        result.started_at = started_at
        result.finished_at = self.get_clock().now().to_msg()
        return result

    # --- 실패 분기 도우미 ---------------------------------------------------------
    def _finish_by_err(self, goal_handle, result, err, context, started_at, started_mono,
                        state, all_results, scan_map, total_rework):
        if err == 'CANCELLED':
            return self._finish_cancel(goal_handle, result, context, started_at, state,
                                        all_results, scan_map, total_rework)
        return self._finish_abort(goal_handle, result, err, context, started_at, started_mono,
                                   state, all_results, scan_map, total_rework)

    def _finish_cancel(self, goal_handle, result, detail, started_at, state, all_results,
                        scan_map, total_rework):
        self._cleanup_abort(goal_handle, state)
        goal_handle.canceled()
        result.success = False
        result.result_code = RunSession.Result.RESULT_CANCELLED
        result.final_error.code = ErrorCode.E_CANCELLED
        result.final_error.detail = detail
        if scan_map is not None:
            result.scan_result = scan_map
        result.all_results = all_results
        result.total_rework = total_rework
        result.started_at = started_at
        result.finished_at = self.get_clock().now().to_msg()
        self._publish_state(None, goal_handle.request.session_id, ProcessState.STAGE_ABORTED,
                             state['layer_index'], state['layer_total'],
                             state['rework_count'], 0.0, 0.0, state['current_tool'],
                             ErrorCode.E_CANCELLED, detail)
        return result

    def _finish_abort(self, goal_handle, result, code, detail, started_at, started_mono, state,
                       all_results, scan_map, total_rework):
        self._log_abort(code, detail)
        self._cleanup_abort(goal_handle, state)
        goal_handle.abort()
        result.success = False
        result.result_code = RunSession.Result.RESULT_FAILED if code != ErrorCode.E_SAFETY_BLOCKED \
            else RunSession.Result.RESULT_ABORTED_SAFETY
        result.final_error.code = code
        result.final_error.detail = detail
        result.final_error.severity = ErrorCode.SEV_ABORT
        if scan_map is not None:
            result.scan_result = scan_map
        result.all_results = all_results
        result.total_rework = total_rework
        result.started_at = started_at
        result.finished_at = self.get_clock().now().to_msg()
        self._publish_state(None, goal_handle.request.session_id, ProcessState.STAGE_ABORTED,
                             state['layer_index'], state['layer_total'],
                             state['rework_count'], 0.0, 0.0, state['current_tool'], code,
                             detail)
        return result

    def _cleanup_abort(self, goal_handle, state):
        """§8 ABORT 공통 처리 ①~③.

        ② 반납 실패는 무시하고 ③으로 넘어간다 — 단, **UV 를 들고 있는데
        반납이 실패하면 HOME 복귀 자체를 하지 않는다**(켜진 램프를 들고
        이동 경로 전체를 조사하게 되는 것을 막는다, §8 경고). 이 경우
        `active_faults` 에 남기라고 NIS 는 적었지만, orchestrator 가 그
        목록에 쓸 수 있는 서비스가 없다(safety_monitor 전용) — 대신
        ERROR 로그로 사람 개입을 요청한다.
        """
        held_tool = state['current_tool']
        if held_tool != ToolState.NONE:
            _, err = self._call_change_tool(
                ToolState.NONE, goal_handle, self.get_parameter('tool_change_timeout_s').value,
                ignore_cancel=True)
            tool_returned = err is None
            if tool_returned:
                state['current_tool'] = ToolState.NONE
            elif held_tool == ToolState.UV:
                self.get_logger().error(
                    '[SAFETY] ABORT 중 UV 툴 반납 실패 — HOME 복귀 생략, 현재 위치에서 정지. '
                    '켜진 램프를 든 채 이동하면 경로 전체가 조사된다. 수동 개입 필요.')
                return
            else:
                self.get_logger().error(
                    f'[SAFETY] ABORT 중 {held_tool} 툴 반납 실패 — HOME 은 시도하되 수동 확인 필요')

        if self.get_parameter('abort_return_home').value:
            self._return_home(goal_handle)

    def _log_abort(self, code, detail):
        self.get_logger().error(f'[{code}] session_orchestrator: {detail}')


def main(args=None):
    rclpy.init(args=args)
    node = SessionOrchestratorNode()
    executor = MultiThreadedExecutor(num_threads=6)
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
