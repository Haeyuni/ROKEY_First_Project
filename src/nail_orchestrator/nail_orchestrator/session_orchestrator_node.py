"""session_orchestrator — 세션 상태 머신 (NIS §8, C계층).

**직접 로봇을 움직이지 않는다.** 공정 액션 6종(`ChangeTool` ·
`SandSurface` · `BrushDust` · `CoatGel` · `CureUV` · `PlaceStone`)을
순서대로 호출하고 결과를 이어 붙이는 것만 한다. 손가락 1개(D-07)이므로
반복은 레이어 하나뿐이다(§8 표기).

여섯 개의 하위 액션이 전부 "goal 보내고 → feedback 받고 → 취소/안전/
타임아웃 감시하며 결과 대기"라는 같은 뼈대를 쓰므로, 스킬 노드들처럼
액션마다 개별 헬퍼를 쓰지 않고 `_call_action()` 하나로 묶었다.

현재 공정 순서는 `PRECHECK → SAND → BRUSH → COAT → CURE → STONE → FINISH`다.
  - 경화 상태를 자동 판정하거나 다시 굽는 루프는 없다. 경화 부족은 사람이
    확인하고 `curing_node`의 `dwell_s_per_point`를 조정한다.
   - 스톤 부착 위치와 기울어진 자세는 stone_node의 4-Pose 티칭 설정으로 정한다.

**문서에 없어서 이 구현이 채운 빈틈들**:

1. **HOME 위치**: `targets.yaml`의 티칭된 `rack_transit`을 기본 HOME으로
   쓴다. 이는 랙과 작업대 사이 안전 경유점으로 실측된 target_key이므로,
   임의 TF 좌표를 만들지 않는다.
2. **PRECHECK 의 "툴 랙 전수 확인"**: `/tool/get_info` 는 랙 슬롯이
   `rack_config.yaml` 에 *설정*돼 있는지만 답한다 — 슬롯에 물리적으로
   툴이 실제로 꽂혀 있는지 확인할 센서/인터페이스가 없다. 그래서 이
   PRECHECK 단계는 "설정 완결성"만 검증하고, 실제 부재는 이후 첫
   `ChangeTool` 실행 중 `E_GRIP_FAILED` 로 늦게 드러난다 — PickPlace 의
   그리퍼 폭이 명령값이지 실측이 아닌 것과 같은 종류의 하드웨어 한계다.
3. **스톤 부착 위치**: `RunSession.Goal`에는 스톤 부착 좌표 필드가 없다.
   `enable_stone=true`인 세션에서는 stone_node의 고정 4-Pose 티칭 설정을 쓴다.
4. **진행률**: 각 스테이지에 동일 가중치를 준 "몇 번째 단계/전체 단계"
   비율에, 현재 하위 액션이 보내는 feedback.percent 를 그 단계 내 분수로
   얹는다. NIS 는 가중치 산정 방식을 규정하지 않는다.
"""
import threading
import time

import rclpy
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup, ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import (
    BrushDust, ChangeTool, CoatGel, CureUV, MoveTo, PlaceStone, RunSession, SandSurface,
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
        self._session_running = False
        self._latest_tool_state = None
        self._last_tool_state_rx_monotonic = None
        self._get_tool_info_client = self.create_client(
            GetToolInfo, '/tool/get_info', callback_group=self._cb_client)
        self.create_subscription(ToolState, '/tool/status', self._on_tool_status,
                                 state_qos, callback_group=self._cb_client)
        self._change_tool_client = ActionClient(self, ChangeTool, '/tool/change',
                                                 callback_group=self._cb_client)
        self._sand_client = ActionClient(self, SandSurface, '/process/sand',
                                          callback_group=self._cb_client)
        self._brush_client = ActionClient(self, BrushDust, '/process/brush',
                                           callback_group=self._cb_client)
        self._coat_client = ActionClient(self, CoatGel, '/process/coat',
                                          callback_group=self._cb_client)
        self._cure_client = ActionClient(self, CureUV, '/process/cure',
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
        d('safety_status_timeout_s', 1.0)
        d('layer_total', 2)
        d('enable_stone', False)
        d('stage_timeout_sand_s', 120.0)
        d('stage_timeout_brush_s', 60.0)
        d('stage_timeout_coat_s', 90.0)
        d('stage_timeout_cure_s', 150.0)
        d('stage_timeout_stone_s', 90.0)
        d('tool_change_timeout_s', 60.0)
        d('tool_status_timeout_s', 5.0)
        d('abort_return_home', True)
        d('precheck_require_all_tools', True)
        d('allowed_target_materials', ['silicone_model', 'artificial_tip'])
        # NIS 표에 없는 구현 보조값 — docstring 참고
        d('home_target_key', 'rack_transit')
        d('home_timeout_s', 30.0)
        # ChangeTool 직후 공통 이동: 경유점(랙 쪽 안전 지점) → <툴>_work
        # (targets.yaml). 대각선 이동으로 방금 집은 툴이 다른 것과 부딪히는
        # 문제 대응 — robot_skill_node 의 PickPlace via_key 라우팅과 같은
        # 이유. tool_transit_key 는 모든 툴이 공유하는 단일 경유점 이름이다
        # (지금 targets.yaml 에서 6개 툴 전부 via_key: rack_transit 로
        # 맞춰져 있음 — 바뀌면 이 파라미터도 같이 바꿀 것).
        d('tool_transit_key', 'rack_transit')
        d('tool_transit_speed_ratio', 0.3)
        d('tool_transit_timeout_s', 30.0)
        d('child_cancel_wait_s', 10.0)

    # --- 안전 -----------------------------------------------------------------
    def _on_safety_status(self, msg):
        self._latest_safety = msg
        self._last_safety_rx_monotonic = time.monotonic()

    def _on_tool_status(self, msg):
        self._latest_tool_state = msg
        self._last_tool_state_rx_monotonic = time.monotonic()

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
        if self._session_running:
            self.get_logger().warn('RunSession REJECT: 이미 세션 실행 중')
            return GoalResponse.REJECT
        if not goal_request.session_id:
            self.get_logger().warn('RunSession REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        allowed_materials = self.get_parameter('allowed_target_materials').value
        if goal_request.target_material not in allowed_materials:
            self.get_logger().warn(
                f'RunSession REJECT: E_INVALID_GOAL (허용되지 않은 target_material: '
                f'{goal_request.target_material!r})')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('RunSession REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        self._session_running = True
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
        deadline = time.monotonic() + timeout_s
        if not client.wait_for_server(timeout_sec=min(10.0, timeout_s)):
            return None, ErrorCode.E_COMM_LOST

        send_done = threading.Event()
        state = {}
        state_lock = threading.Lock()

        def on_goal_response(fut):
            try:
                goal_handle = fut.result()
            except Exception as exc:
                with state_lock:
                    state['send_error'] = exc
                send_done.set()
                return
            with state_lock:
                state['goal_handle'] = goal_handle
                expired = state.get('expired', False)
            if expired and goal_handle is not None and goal_handle.accepted:
                goal_handle.cancel_goal_async()
            send_done.set()

        try:
            client.send_goal_async(
                goal, feedback_callback=feedback_cb).add_done_callback(on_goal_response)
        except Exception as exc:
            self.get_logger().error(f'하위 goal 전송 실패: {exc}')
            return None, ErrorCode.E_COMM_LOST

        while not send_done.wait(timeout=0.1):
            error = None
            if not ignore_cancel and our_goal_handle.is_cancel_requested:
                error = 'CANCELLED'
            elif not self._safe_to_move():
                error = ErrorCode.E_SAFETY_BLOCKED
            elif time.monotonic() > deadline:
                error = ErrorCode.E_TIMEOUT
            if error is not None:
                with state_lock:
                    state['expired'] = True
                    late_goal = state.get('goal_handle')
                if late_goal is not None and late_goal.accepted:
                    late_goal.cancel_goal_async()
                return None, error

        with state_lock:
            gh = state.get('goal_handle')
            send_error = state.get('send_error')
        if send_error is not None:
            self.get_logger().error(f'하위 goal 응답 실패: {send_error}')
            return None, ErrorCode.E_COMM_LOST
        if gh is None or not gh.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED
        self._active_goal_handle = gh

        result_done = threading.Event()

        def on_result(fut):
            try:
                state['result'] = fut.result()
            except Exception as exc:
                state['result_error'] = exc
            result_done.set()

        gh.get_result_async().add_done_callback(on_result)

        def cancel_and_wait(error):
            try:
                gh.cancel_goal_async()
                wait_s = self.get_parameter('child_cancel_wait_s').value
                if not result_done.wait(timeout=wait_s):
                    self.get_logger().error('하위 액션 취소 후 결과 수신 실패')
                    error = ErrorCode.E_COMM_LOST
            except Exception as exc:
                self.get_logger().error(f'하위 액션 취소 요청 실패: {exc}')
                error = ErrorCode.E_COMM_LOST
            self._active_goal_handle = None
            return None, error

        while not result_done.wait(timeout=0.1):
            if not ignore_cancel and our_goal_handle.is_cancel_requested:
                return cancel_and_wait('CANCELLED')
            if not self._safe_to_move():
                return cancel_and_wait(ErrorCode.E_SAFETY_BLOCKED)
            if time.monotonic() > deadline:
                return cancel_and_wait(ErrorCode.E_TIMEOUT)
        self._active_goal_handle = None

        if state.get('result_error') is not None:
            self.get_logger().error(f'하위 액션 결과 수신 실패: {state["result_error"]}')
            return None, ErrorCode.E_COMM_LOST
        wrapped_result = state.get('result')
        result = wrapped_result.result if wrapped_result is not None else None
        if result is None or not hasattr(result, 'base'):
            return None, ErrorCode.E_COMM_LOST
        if not result.base.success:
            return result, result.base.error.code
        return result, None

    def _call_change_tool(self, target_tool, our_goal_handle, timeout_s, ignore_cancel=False):
        goal = ChangeTool.Goal()
        goal.target_tool = target_tool
        result, err = self._call_action(self._change_tool_client, goal, our_goal_handle, timeout_s,
                                        ignore_cancel=ignore_cancel)
        if err is not None:
            return result, err
        return result, self._wait_for_tool_status(
            target_tool, our_goal_handle, ignore_cancel=ignore_cancel)

    def _wait_for_tool_status(self, expected_tool, our_goal_handle, ignore_cancel=False):
        """ChangeTool 완료 뒤 상태 토픽도 목표 툴을 가리킬 때까지 기다린다.

        공정 노드는 goal 수락 전에 /safety/validate가 구독한 /tool/status를 본다.
        액션 결과만 받은 직후 다음 공정 goal을 보내면 그 구독자가 이전 툴을
        보고 거부할 수 있어, 상태 확인을 오케스트레이터의 교체 단계에 포함한다.
        """
        deadline = time.monotonic() + self.get_parameter('tool_status_timeout_s').value
        while time.monotonic() <= deadline:
            if (self._latest_tool_state is not None
                    and self._latest_tool_state.current_tool == expected_tool):
                return None
            if not ignore_cancel and our_goal_handle.is_cancel_requested:
                return 'CANCELLED'
            if not self._safe_to_move():
                return ErrorCode.E_SAFETY_BLOCKED
            time.sleep(0.02)
        self.get_logger().error(
            f'/tool/status가 ChangeTool 결과({expected_tool})로 갱신되지 않았다')
        return ErrorCode.E_TIMEOUT

    def _call_move_to_key(self, target_key, our_goal_handle, timeout_s, linear=True,
                          ignore_cancel=False):
        goal = MoveTo.Goal()
        goal.target_key = target_key
        goal.frame_id = 'base_link'
        goal.linear = linear
        ratio = self.get_parameter('tool_transit_speed_ratio').value
        goal.speed_ratio = ratio
        goal.accel_ratio = ratio
        goal.timeout_s = timeout_s
        return self._call_action(self._move_client, goal, our_goal_handle, timeout_s,
                                 ignore_cancel=ignore_cancel)

    def _go_to_work(self, tool_key, our_goal_handle, move_to_work=True):
        """ChangeTool 직후 공통 이동: 경유점(tool_transit_key) → <tool_key>_work.

        반환: (work_result, None) 성공 / (None, err) 실패 — err 은
        _finish_by_err 에 그대로 넘기면 된다. 대각선 이동으로 방금 집은 툴이
        랙/구조물과 부딪히는 문제 대응(PickPlace via_key 라우팅과 동일한 이유).
        """
        transit_key = self.get_parameter('tool_transit_key').value
        timeout_s = self.get_parameter('tool_transit_timeout_s').value
        _, err = self._call_move_to_key(transit_key, our_goal_handle, timeout_s)
        if err is not None:
            return None, err
        # 브러시/코터의 6-Pose 경로는 ContactPath가 접근까지 담당하므로
        # orchestrator가 별도 작업점으로 먼저 직접 이동하지 않는다.
        if not move_to_work:
            return None, None
        work_result, err = self._call_move_to_key(f'{tool_key}_work', our_goal_handle, timeout_s)
        if err is not None:
            return None, err
        return work_result, None

    # --- PRECHECK (NIS §8: 툴 랙 전수 · 통신 · E-Stop 해제) -----------------------
    # 안착 센서는 현재 하드웨어에 미장착이라 판정 자체가 불가능해 이 precheck
    # 에서 뺐다(safety_monitor_node.py 도 동일) — 센서가 설치되면 되살릴 것.
    def _run_precheck(self, required_tools):
        reasons = []
        safety = self._latest_safety
        if safety is None:
            return False, ['안전 상태 미수신']
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

    # --- HOME 복귀 (docstring #1 — 티칭된 target_key 사용) --------------------------
    def _return_home(self, our_goal_handle):
        timeout_s = self.get_parameter('home_timeout_s').value
        _, err = self._call_move_to_key(
            self.get_parameter('home_target_key').value, our_goal_handle, timeout_s,
            linear=False, ignore_cancel=True)
        if err is not None:
            self.get_logger().error(f'[SAFETY] HOME 복귀 실패({err}) — 수동 확인 필요')
            return False
        return True

    # --- ProcessState 발행 (즉시 전이 시 + 1Hz, §8 피드백) -------------------------
    def _publish_state(self, goal_handle, session_id, stage, layer_index, layer_total,
                        stage_percent, session_percent, current_tool,
                        last_error_code='', last_error_detail=''):
        st = ProcessState()
        st.header.stamp = self.get_clock().now().to_msg()
        st.session_id = session_id
        st.stage = stage
        st.layer_index = layer_index
        st.layer_total = layer_total
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
    def _build_sequence(enable_stone, layer_total):
        seq = ['PRECHECK', 'SAND', 'BRUSH']
        for i in range(layer_total):
            seq += [f'COAT{i}', f'CURE{i}']
        if enable_stone:
            seq.append('STONE')
        seq.append('FINISH')
        return seq

    # =========================================================================
    def _execute(self, goal_handle):
        try:
            return self._run_session(goal_handle)
        finally:
            self._session_running = False

    def _run_session(self, goal_handle):
        goal = goal_handle.request
        started_at = self.get_clock().now().to_msg()
        started_mono = time.monotonic()
        result = RunSession.Result()
        session_id = goal.session_id

        layer_total = int(self._val(goal.layer_total, 'layer_total'))
        enable_stone = goal.enable_stone

        p = self.get_parameter
        t_sand = p('stage_timeout_sand_s').value
        t_brush = p('stage_timeout_brush_s').value
        t_coat = p('stage_timeout_coat_s').value
        t_cure = p('stage_timeout_cure_s').value
        t_stone = p('stage_timeout_stone_s').value
        t_tool = p('tool_change_timeout_s').value

        seq = self._build_sequence(enable_stone, layer_total)
        n_steps = len(seq)

        def progress(step_name, local_pct):
            idx = seq.index(step_name)
            return 100.0 * (idx + max(0.0, min(100.0, local_pct)) / 100.0) / n_steps

        state = {
            'layer_index': 0, 'current_tool': ToolState.NONE, 'layer_total': layer_total,
        }

        def emit(stage, local_pct, layer_index=None):
            step_key = f'{stage}{layer_index}' if layer_index is not None and \
                f'{stage}{layer_index}' in seq else stage
            pct = progress(step_key, local_pct) if step_key in seq else 0.0
            self._publish_state(
                goal_handle, session_id, stage, state['layer_index'], layer_total,
                local_pct, pct, state['current_tool'])

        abort_code = None
        abort_detail = ''

        # --- PRECHECK ----------------------------------------------------------
        emit(ProcessState.STAGE_PRECHECK, 0.0)
        required_tools = ['sander', 'coater', 'uv']
        required_tools.append('brush')
        if enable_stone:
            required_tools.append('tweezers')
        ok, reasons = self._run_precheck(required_tools)
        if not ok:
            abort_code, abort_detail = ErrorCode.E_PRECOND_FAILED, f'PRECHECK 실패: {reasons}'
            return self._finish_abort(goal_handle, result, abort_code, abort_detail,
                                       started_at, started_mono, state)
        emit(ProcessState.STAGE_PRECHECK, 100.0)

        # --- SAND ------------------------------------------------------------------
        _, err = self._call_change_tool(ToolState.SANDER, goal_handle, t_tool)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'ChangeTool(sander) 실패',
                                        started_at, started_mono, state)
        state['current_tool'] = ToolState.SANDER

        _, err = self._go_to_work('sander', goal_handle)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, '경유/작업위치 이동 실패(sander)',
                                        started_at, started_mono, state)

        emit(ProcessState.STAGE_SAND, 0.0)
        sand_goal = SandSurface.Goal()
        sand_goal.session_id = session_id

        def on_sand_fb(fb_msg):
            emit(ProcessState.STAGE_SAND, fb_msg.feedback.percent)

        _, err = self._call_action(self._sand_client, sand_goal, goal_handle, t_sand,
                                    feedback_cb=on_sand_fb)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'SandSurface 실패',
                                        started_at, started_mono, state)
        emit(ProcessState.STAGE_SAND, 100.0)

        # --- BRUSH (고정 공정) ------------------------------------------------------
        _, err = self._call_change_tool(ToolState.BRUSH, goal_handle, t_tool)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'ChangeTool(brush) 실패',
                                        started_at, started_mono, state)
        state['current_tool'] = ToolState.BRUSH

        _, err = self._go_to_work('brush', goal_handle, move_to_work=False)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err,
                                        '경유/작업위치 이동 실패(brush)', started_at,
                                        started_mono, state)

        emit(ProcessState.STAGE_BRUSH, 0.0)
        brush_goal = BrushDust.Goal()
        brush_goal.session_id = session_id

        def on_brush_fb(fb_msg):
            emit(ProcessState.STAGE_BRUSH, fb_msg.feedback.percent)

        _, err = self._call_action(self._brush_client, brush_goal, goal_handle,
                                    t_brush, feedback_cb=on_brush_fb)
        if err is not None:
            return self._finish_by_err(goal_handle, result, err, 'BrushDust 실패',
                                        started_at, started_mono, state)
        emit(ProcessState.STAGE_BRUSH, 100.0)

        # --- 레이어 루프: COAT → CURE ---------------------------------------------
        # v0.3: INSPECT 와 REWORK 가 빠져 루프가 아니라 그냥 순차 진행이다.
        # 경화가 충분한지는 아무도 검증하지 않는다 (docstring ★ 참고).
        for layer_index in range(layer_total):
            state['layer_index'] = layer_index

            # COAT
            _, err = self._call_change_tool(ToolState.COATER, goal_handle, t_tool)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'ChangeTool(coater) 실패',
                                            started_at, started_mono, state)
            state['current_tool'] = ToolState.COATER

            _, err = self._go_to_work('coater', goal_handle, move_to_work=False)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err,
                                            '경유/작업위치 이동 실패(coater)', started_at,
                                            started_mono, state)

            emit(ProcessState.STAGE_COAT, 0.0, layer_index)
            coat_goal = CoatGel.Goal()
            coat_goal.session_id = session_id
            coat_goal.layer_index = layer_index

            def on_coat_fb(fb_msg, _li=layer_index):
                emit(ProcessState.STAGE_COAT, fb_msg.feedback.percent, _li)

            _, err = self._call_action(self._coat_client, coat_goal, goal_handle,
                                        t_coat, feedback_cb=on_coat_fb)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'CoatGel 실패',
                                            started_at, started_mono, state)
            emit(ProcessState.STAGE_COAT, 100.0, layer_index)

            # CURE — 전체 영역 1회
            _, err = self._call_change_tool(ToolState.UV, goal_handle, t_tool)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'ChangeTool(uv) 실패',
                                            started_at, started_mono, state)
            state['current_tool'] = ToolState.UV

            _, err = self._go_to_work('uv', goal_handle)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err,
                                            '경유/작업위치 이동 실패(uv)', started_at,
                                            started_mono, state)

            emit(ProcessState.STAGE_CURE, 0.0, layer_index)
            cure_goal = CureUV.Goal()
            cure_goal.session_id = session_id
            cure_goal.layer_index = layer_index

            def on_cure_fb(fb_msg, _li=layer_index):
                emit(ProcessState.STAGE_CURE, fb_msg.feedback.percent, _li)

            _, err = self._call_action(self._cure_client, cure_goal, goal_handle,
                                        t_cure, feedback_cb=on_cure_fb)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'CureUV 실패',
                                            started_at, started_mono, state)
            emit(ProcessState.STAGE_CURE, 100.0, layer_index)

        # --- STONE (옵션, docstring #3 — 목표 좌표 가정) ------------------------------
        if enable_stone:
            _, err = self._call_change_tool(ToolState.TWEEZERS, goal_handle, t_tool)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err,
                                            'ChangeTool(tweezers) 실패', started_at,
                                            started_mono, state)
            state['current_tool'] = ToolState.TWEEZERS
            # 다른 툴과 달리 <tool>_work 로 먼저 가지 않는다 — targets.yaml 에
            # tweezers_work 가 없고, PlaceStone 이 곧바로 stone_tray 로 PICK 을
            # 나가면서 via_key(rack_transit) 라우팅을 타기 때문이다.

            emit(ProcessState.STAGE_STONE, 0.0)
            stone_goal = PlaceStone.Goal()
            stone_goal.session_id = session_id
            # 부착 위치와 기울어진 자세는 stone_node의 4-Pose 티칭 설정이 정한다.

            def on_stone_fb(fb_msg):
                emit(ProcessState.STAGE_STONE, fb_msg.feedback.percent)

            _, err = self._call_action(self._stone_client, stone_goal, goal_handle,
                                        t_stone, feedback_cb=on_stone_fb)
            if err is not None:
                return self._finish_by_err(goal_handle, result, err, 'PlaceStone 실패',
                                            started_at, started_mono, state)
            emit(ProcessState.STAGE_STONE, 100.0)

        # --- FINISH ------------------------------------------------------------------
        _, err = self._call_change_tool(ToolState.NONE, goal_handle, t_tool)
        if err is not None:
            return self._finish_by_err(
                goal_handle, result, err, 'FINISH 툴 반납 실패',
                started_at, started_mono, state)
        state['current_tool'] = ToolState.NONE
        emit(ProcessState.STAGE_FINISH, 100.0)

        goal_handle.succeed()
        result.success = True
        result.result_code = RunSession.Result.RESULT_COMPLETED
        result.final_error.code = ErrorCode.OK
        result.started_at = started_at
        result.finished_at = self.get_clock().now().to_msg()
        return result


    # --- 실패 분기 도우미 ---------------------------------------------------------
    def _finish_by_err(self, goal_handle, result, err, context, started_at, started_mono,
                        state):
        if err == 'CANCELLED':
            return self._finish_cancel(goal_handle, result, context, started_at, state)
        return self._finish_abort(goal_handle, result, err, context, started_at, started_mono,
                                   state)

    def _finish_cancel(self, goal_handle, result, detail, started_at, state):
        self._cleanup_abort(goal_handle, state)
        goal_handle.canceled()
        result.success = False
        result.result_code = RunSession.Result.RESULT_CANCELLED
        result.final_error.code = ErrorCode.E_CANCELLED
        result.final_error.detail = detail
        result.started_at = started_at
        result.finished_at = self.get_clock().now().to_msg()
        self._publish_state(None, goal_handle.request.session_id, ProcessState.STAGE_ABORTED,
                             state['layer_index'], state['layer_total'],
                             0.0, 0.0, state['current_tool'],
                             ErrorCode.E_CANCELLED, detail)
        return result

    def _finish_abort(self, goal_handle, result, code, detail, started_at, started_mono, state):
        self._log_abort(code, detail)
        self._cleanup_abort(goal_handle, state)
        goal_handle.abort()
        result.success = False
        result.result_code = RunSession.Result.RESULT_FAILED if code != ErrorCode.E_SAFETY_BLOCKED \
            else RunSession.Result.RESULT_ABORTED_SAFETY
        result.final_error.code = code
        result.final_error.detail = detail
        result.final_error.severity = ErrorCode.SEV_ABORT
        result.started_at = started_at
        result.finished_at = self.get_clock().now().to_msg()
        self._publish_state(None, goal_handle.request.session_id, ProcessState.STAGE_ABORTED,
                             state['layer_index'], state['layer_total'],
                             0.0, 0.0, state['current_tool'], code, detail)
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
