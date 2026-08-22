"""safety_monitor_node — 안전 상태 감시 · 전제조건 검증 (NIS §7 ★★).

**이 노드가 살아있다는 것 자체가 이 프로젝트의 안전 계층 대부분이다.**
v0.2에서 UV 상시 ON으로 바뀌면서(§7.0) 소프트웨어가 램프를 끌 방법이
없어졌다 — 이 노드가 하는 일은 "램프를 끄는 것"이 아니라 "로봇 움직임을
멈추는 것"으로 좁혀졌다. `/safety/status.safe_to_move`, 그 하나가 이
프로젝트 전체가 참조하는 유일한 안전 신호다(§3.4, 예외 없음).

**하드웨어 접점 매핑에 대한 솔직한 고백**: NIS §7 소비 인터페이스는
"dsr 상태 토픽(하트비트) · dsr Digital Input"이라고만 적혀 있고, 정확한
토픽/서비스 이름은 이 저장소 어디에도 없다(실제 두산 컨트롤러 드라이버
설정에서만 확정된다). 이 구현은:

  - 안착·더스트 Digital Input 은 `DSR_ROBOT2.get_digital_input(channel)`
    (공식 파이썬 API의 GPIO 분류에 실제로 존재)을 그대로 쓴다.
  - E-Stop 은 애초에 Control Box DI 채널로 추측 매핑했었으나(2026-08-22
    실기 커미셔닝) 해당 채널이 물리 버튼과 무관하게 항상 고정값을 반환하는
    것으로 확인되어 **폐기**했다. 대신 `get_robot_state()`(원래 하트비트
    용도로만 쓰던 동기 폴링 함수)가 그대로 노출하는 `robot_state` enum을
    쓴다 — 실측: 평상시 3(STATE_SAFE_OFF), E-Stop 누르면 6(STATE_EMERGENCY_
    STOP), 풀면 3으로 복귀. `_ROBOT_STATE_EMERGENCY_STOP = {6, 7}`(RobotState.msg
    주석상 6/7 둘 다 EMERGENCY_STOP) 에 포함되면 눌림으로 판정한다. 이 호출이
    실패/타임아웃이면 여전히 `FAULT_COMM_LOST`.
  - 안착·더스트 DI 극성(눌림=HIGH 인지 LOW 인지)은 배선마다 다르다.
    "센서 감지 = HIGH"로 가정했다. `*_active_high` 파라미터로 배선이
    반대면 뒤집는다 — **커미셔닝 때 실제로 눌러/막아 보고 반드시 확인할 것.**

**래치 vs 실시간 결함**: `FAULT_ESTOP`/`FAULT_COMM_LOST`/`FAULT_TOOL_DROP`
은 물리 원인이 사라져도 `ResetSafety` 를 불러야 풀린다(§7 결함 코드 표의
"해제 조건"에 전부 "+ ResetSafety" 가 붙어 있다). `FAULT_NO_HANDREST` 는
"재안착"만 조건이라 안착되는 즉시 자동으로 사라진다. 코드에서
`_latched_faults`(래치, ResetSafety 로만 해제)와 `_live_faults`(DI 를
그대로 반영, 매 주기 재계산)를 분리한 이유다.

**FAULT_NO_DUST 를 상시 결함에 안 넣은 이유**: 파라미터 이름 자체가
`require_dust_for_sanding` — 연마 중에만 요구한다는 뜻이다. 이 노드는
지금 어떤 공정이 실행 중인지 모른다(세션 상태를 추적하지 않는다,
그건 orchestrator 의 일이다). 그래서 더스트 OFF 를 전역 `active_faults`
에 넣어 항상 로봇을 못 움직이게 만드는 대신, `ValidatePrecondition`
에서 `stage==SAND` 일 때만 `blocking_reasons` 로 낸다.
"""
import threading

from nail_msgs.msg import ErrorCode, SafetyState, StiffnessMap, ToolState
from nail_msgs.srv import ResetSafety, ValidatePrecondition
import rclpy
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup, ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_skill.dsr_adapter import DsrAdapter, DsrAdapterError


class SafetyMonitorNode(Node):

    # dsr_msgs2/RobotState.msg: robot_state 6/7 = STATE_EMERGENCY_STOP (실측 확인,
    # 2026-08-22 실기 테스트 — DI 채널 기반 판정은 배선 오류로 폐기).
    _ROBOT_STATE_EMERGENCY_STOP = frozenset({6, 7})

    def __init__(self):
        super().__init__('safety_monitor')
        self._declare_parameters()
        p = self.get_parameter

        try:
            self._adapter = DsrAdapter(self, p('dsr_prefix').value, p('robot_model').value,
                                        client_node_name='safety_monitor_dsr_client')
        except DsrAdapterError as e:
            self.get_logger().error(str(e))
            raise

        self._state_lock = threading.Lock()
        self._prev_estop_pressed = False
        self._estop_pressed = False
        self._handrest_seated = False
        self._dust_on = False
        self._comm_ok = False
        self._latched_faults = set()   # ResetSafety 로만 해제
        self._live_faults = set()      # DI 를 그대로 반영, 매 주기 재계산
        self._current_tool = ToolState.NONE
        self._scan_valid = False
        self._latest_safe_to_move = False
        self._poll_guard = threading.Lock()
        self._poll_thread = None

        transient_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                                    durability=DurabilityPolicy.TRANSIENT_LOCAL)

        self._cb_sub = ReentrantCallbackGroup()
        self._cb_srv = MutuallyExclusiveCallbackGroup()
        # DSR 폴링은 동기 호출이며 한 번에 하나만 실행해야 한다. 구독과 같은
        # Reentrant 그룹에 두면 느린 응답 중 다음 timer callback이 겹쳐
        # 정상 통신까지 FAULT_COMM_LOST로 오판할 수 있다.
        self._cb_poll = MutuallyExclusiveCallbackGroup()

        self.create_subscription(ToolState, '/tool/status', self._on_tool_status,
                                  transient_qos, callback_group=self._cb_sub)
        self.create_subscription(StiffnessMap, '/stiffness/map', self._on_stiffness_map,
                                  transient_qos, callback_group=self._cb_sub)

        self._status_pub = self.create_publisher(SafetyState, '/safety/status', transient_qos)

        self.create_service(ValidatePrecondition, '/safety/validate',
                             self._on_validate_precondition, callback_group=self._cb_srv)
        self.create_service(ResetSafety, '/safety/reset',
                             self._on_reset_safety, callback_group=self._cb_srv)

        self.create_timer(1.0 / p('publish_rate_hz').value, self._on_publish_timer,
                           callback_group=self._cb_poll)

        if p('uv_software_control').value:
            self.get_logger().warn(
                'uv_software_control=true — v0.1 permit 구조는 이 구현에 없다. '
                'v0.2 는 UV 상시 ON 이 전제이므로(§7.0) 이 값은 무시된다.')
        if p('auto_reset').value:
            self.get_logger().warn(
                '[SAFETY] auto_reset=true — 래치 결함이 물리 조건 해소만으로 자동 '
                '해제된다. NIS §7 권고 위반. 실습/디버깅 외에는 쓰지 말 것.')

        self.get_logger().info('safety_monitor ready — /safety/status 가 유일한 안전 신호다')

    def destroy_node(self):
        self._adapter.destroy()
        super().destroy_node()

    # --- 파라미터 (NIS §7 표 + dsr 접속/극성 보조값) -----------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        d('use_mock_hardware', False)  # robot_skill_node 와 동일 — dsr_bringup2 virtual 모드로 대체
        d('dsr_prefix', 'dsr01')
        d('robot_model', 'm0609')

        d('publish_rate_hz', 20)
        d('heartbeat_timeout_ms', 200)
        d('di_handrest_channel', 2)
        d('di_dust_channel', 3)
        d('require_handrest', True)
        d('require_dust_for_sanding', True)
        d('require_scan_valid', True)
        d('uv_software_control', False)
        d('auto_reset', False)

        # DI 극성 — docstring 참고, 커미셔닝 때 검증 필요
        d('handrest_active_high', True)
        d('dust_active_high', True)

    # --- /tool/status, /stiffness/map 구독 --------------------------------------
    def _on_tool_status(self, msg):
        with self._state_lock:
            self._current_tool = msg.current_tool
            # 툴을 물고 있는데 파지 검증이 깨졌다 — 낙하로 간주(래치).
            # ToolState 에 별도의 "방금 떨어짐" 플래그가 없어 grip_verified
            # 를 그대로 신호로 쓴다 — robot_skill_node/tool_manager 가
            # 이동 중 폭 이상을 감지하면 이 필드를 false 로 채워 발행한다는
            # 전제다(§5.1 tool_drop_width_delta_mm).
            if msg.current_tool != ToolState.NONE and not msg.grip_verified:
                if SafetyState.FAULT_TOOL_DROP not in self._latched_faults:
                    self.get_logger().error(
                        '[FAULT_TOOL_DROP] /tool/status: grip_verified=false — '
                        '자동 복구 금지, 사람이 확인 후 ResetSafety 필요')
                self._latched_faults.add(SafetyState.FAULT_TOOL_DROP)

    def _on_stiffness_map(self, msg):
        with self._state_lock:
            self._scan_valid = bool(msg.valid)

    # --- ① 상태 수집 루프 (NIS §7 동작 ①②) ---------------------------------------
    def _on_publish_timer(self):
        p = self.get_parameter
        handrest_ch = p('di_handrest_channel').value
        dust_ch = p('di_dust_channel').value
        handrest_hi = p('handrest_active_high').value
        dust_hi = p('dust_active_high').value
        timeout_s = p('heartbeat_timeout_ms').value / 1000.0

        comm_ok = True
        estop_pressed = self._estop_pressed
        handrest_seated = self._handrest_seated
        dust_on = self._dust_on

        try:
            handrest_raw, dust_raw, robot_state_raw = self._poll_hardware(
                handrest_ch, dust_ch, timeout_s)
            estop_pressed = robot_state_raw in self._ROBOT_STATE_EMERGENCY_STOP
            handrest_seated = handrest_raw if handrest_hi else (not handrest_raw)
            dust_on = dust_raw if dust_hi else (not dust_raw)
        except Exception as e:  # noqa: BLE001 - 모든 드라이버 오류는 통신 결함으로 fail-safe
            comm_ok = False
            self.get_logger().error(
                f'[FAULT_COMM_LOST] DI/컨트롤러 응답 없음: {e}',
                throttle_duration_sec=2.0)

        with self._state_lock:
            # --- ② E-Stop: robot_state가 EMERGENCY_STOP으로 전이하는 에지에서만 새로 래치한다.
            #     현재 눌려있는 동안은 에지와 무관하게 아래 safe_to_move
            #     자체가 이미 False 다 — 래치는 "떼도 계속 막는" 역할이다.
            if estop_pressed and not self._prev_estop_pressed:
                self._latched_faults.add(SafetyState.FAULT_ESTOP)
                self.get_logger().error(
                    '[FAULT_ESTOP] E-Stop 감지 — ResetSafety 전까지 안전 결함 유지')
            self._prev_estop_pressed = estop_pressed
            self._estop_pressed = estop_pressed
            self._handrest_seated = handrest_seated
            self._dust_on = dust_on

            if not comm_ok:
                self._latched_faults.add(SafetyState.FAULT_COMM_LOST)
            self._comm_ok = comm_ok

            # --- 실시간 결함: 안착 (require_handrest 일 때만 감시) ---------------
            self._live_faults.discard(SafetyState.FAULT_NO_HANDREST)
            if p('require_handrest').value and not handrest_seated:
                self._live_faults.add(SafetyState.FAULT_NO_HANDREST)

            active_faults = sorted(self._latched_faults | self._live_faults)
            safe_to_move = (not estop_pressed) and comm_ok and len(active_faults) == 0
            self._latest_safe_to_move = safe_to_move

            msg = SafetyState()
            msg.header.stamp = self.get_clock().now().to_msg()
            msg.safe_to_move = safe_to_move
            msg.estop_released = not estop_pressed
            msg.comm_ok = comm_ok
            msg.handrest_seated = handrest_seated
            msg.dust_extraction_on = dust_on
            msg.tool_grip_ok = SafetyState.FAULT_TOOL_DROP not in active_faults
            msg.scan_valid = self._scan_valid
            msg.active_faults = active_faults
            msg.reason = active_faults[0] if active_faults else ''

        if p('auto_reset').value:
            self._attempt_reset(confirm=True, operator_note='auto_reset=true')

        self._status_pub.publish(msg)

    def _poll_hardware(self, handrest_ch, dust_ch, timeout_s):
        """DI 2개와 robot_state(하트비트 겸 E-Stop 판정)를 한 작업으로 읽고
        전체 timeout을 적용한다.

        DSR 동기 호출이 내부에서 영구 대기할 수 있으므로 worker를 사용하되,
        이전 worker가 아직 살아 있으면 새 worker를 만들지 않는다. 통신 장애
        동안 매 timer tick마다 daemon thread가 누적되는 것을 막기 위함이다.
        """
        result = {}

        def run():
            try:
                handrest = self._adapter.read_digital_input(handrest_ch)
                dust = self._adapter.read_digital_input(dust_ch)
                robot_state = self._adapter.read_robot_state()
                if robot_state is None or \
                        (isinstance(robot_state, (int, float)) and robot_state < 0):
                    raise DsrAdapterError(
                        f'get_robot_state 응답 오류 (state={robot_state})')
                result['value'] = (handrest, dust, robot_state)
            except Exception as e:  # noqa: BLE001 - 스레드 경계 너머로 그대로 전달
                result['error'] = e

        with self._poll_guard:
            if self._poll_thread is not None and self._poll_thread.is_alive():
                raise TimeoutError('이전 DI/하트비트 조회가 아직 종료되지 않음')
            t = threading.Thread(target=run, daemon=True, name='safety_dsr_poll')
            self._poll_thread = t
        t.start()
        t.join(timeout_s)
        if t.is_alive():
            raise TimeoutError(f'DI/하트비트가 {timeout_s * 1000:.0f}ms 내 응답 없음')
        with self._poll_guard:
            if self._poll_thread is t:
                self._poll_thread = None
        if 'error' in result:
            raise result['error']
        return result.get('value')

    # --- ③ 전제조건 검사 (NIS §7 동작 ③ 표) ---------------------------------------
    def _on_validate_precondition(self, request, response):
        p = self.get_parameter
        reasons = []

        with self._state_lock:
            handrest_seated = self._handrest_seated
            current_tool = self._current_tool
            scan_valid = self._scan_valid
            dust_on = self._dust_on
            safe_to_move = self._latest_safe_to_move
            active_faults = sorted(self._latched_faults | self._live_faults)

        if not safe_to_move:
            reasons.append(f'safe_to_move=false (active_faults={active_faults})')

        if p('require_handrest').value and not handrest_seated:
            reasons.append('안착 센서 OFF')

        if request.required_tool and current_tool != request.required_tool:
            reasons.append(
                f'툴 불일치: 요구={request.required_tool}, 현재={current_tool or "(없음)"}')

        Stage = ValidatePrecondition.Request
        scan_required_stages = (Stage.STAGE_SAND, Stage.STAGE_COAT, Stage.STAGE_CURE,
                                 Stage.STAGE_INSPECT)
        if request.stage in scan_required_stages and p('require_scan_valid').value \
                and not scan_valid:
            reasons.append(f'{ErrorCode.E_NO_SCAN}: 스캔 유효하지 않음')

        if request.stage == Stage.STAGE_SAND and p('require_dust_for_sanding').value \
                and not dust_on:
            reasons.append('더스트 컬렉터 OFF (연마 중 배기 인터록)')

        response.ok = len(reasons) == 0
        response.blocking_reasons = reasons
        if not response.ok:
            self.get_logger().warn(
                f'ValidatePrecondition REJECT: stage={request.stage} session={request.session_id} '
                f'reasons={reasons}')
        return response

    # --- ResetSafety (NIS §7 반환값 / 결함 코드 표) -------------------------------
    def _on_reset_safety(self, request, response):
        result = self._attempt_reset(request.confirm, request.operator_note)
        response.ok = result[0]
        response.remaining_faults = result[1]
        return response

    def _attempt_reset(self, confirm, operator_note):
        """반환: (ok, remaining_faults). confirm=false 면 아무것도 바꾸지 않고
        현재 잔여 결함만 보고한다(§7: "무조건 성공시키지 마세요")."""
        with self._state_lock:
            if not confirm:
                return False, sorted(self._latched_faults | self._live_faults)

            # ESTOP/COMM_LOST 는 물리 조건이 실제로 풀렸을 때만 래치를 뗀다.
            if not self._estop_pressed:
                self._latched_faults.discard(SafetyState.FAULT_ESTOP)
            if self._comm_ok:
                self._latched_faults.discard(SafetyState.FAULT_COMM_LOST)
            # TOOL_DROP 은 재확인할 센서가 없다 — "사람이 치운 뒤" 라는 조건을
            # 검증할 방법이 confirm=true(+operator_note) 자체뿐이라, 여기서는
            # 운영자 확인을 그대로 신뢰한다(§7 결함 코드 표).
            self._latched_faults.discard(SafetyState.FAULT_TOOL_DROP)

            remaining = sorted(self._latched_faults | self._live_faults)
            ok = len(remaining) == 0
        if ok:
            self.get_logger().info(f'ResetSafety: 결함 해제됨 (note="{operator_note}")')
        else:
            self.get_logger().warn(f'ResetSafety: 잔여 결함 {remaining} — 리셋 거부 (note="{operator_note}")')
        return ok, remaining


def main(args=None):
    rclpy.init(args=args)
    node = SafetyMonitorNode()
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
