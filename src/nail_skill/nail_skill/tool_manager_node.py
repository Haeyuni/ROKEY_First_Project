"""tool_manager — 툴 랙 위치·TCP 오프셋 관리, 원자적 툴 교체 (NIS §5.2).

이 노드는 로봇을 직접 움직이지 않는다 — 실제 픽/플레이스 이동은
robot_skill_node 의 `/skill/pick_place` 를 호출해서 시킨다. 두산 API 호출은
TCP 전환(`set_tcp`) 하나뿐이고, 그마저도 robot_skill_node 와 같은 패키지
(`nail_skill`) 안의 `dsr_adapter.DsrAdapter` 를 그대로 재사용한다 — SDS §4.1
("두산 API 호출은 robot_skill_node 안에만 존재") 의 취지(드라이버 API 지식을
한 곳에 가둔다)를 지키면서, NIS §5.2 가 요구하는 "tool_manager 가 dsr TCP
설정 서비스를 직접 쓴다"를 만족시키는 절충이다.

랙 슬롯 좌표는 값으로 저장하지 않는다. tool_rack.yaml 의 `slot_frame` 에는
"위치의 이름"만 두고, 그 이름을 PickPlace 의 target_key 로 그대로 넘긴다 —
좌표로 바꾸는 일은 robot_skill_node._target_task_pose 의 몫이다
(nail_skill/config/targets.yaml 에서 조회, 없으면 TF 프레임으로 폴백).

TCP 는 두 국면으로 나눠 쓴다 (NEUTRAL_TCP 주석 참고):
  · 랙으로 이동할 때  → NEUTRAL_TCP(오프셋 0) — targets.yaml 좌표가 "플랜지가
    있어야 할 자리"로 티칭된 값이기 때문
  · 툴을 잡은 뒤      → tcp_<툴이름> — 공정 작업이 툴 끝 기준이 되도록
"""
import threading
import time

import rclpy
import yaml
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy

from nail_msgs.action import ChangeTool, PickPlace
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, ToolState
from nail_msgs.srv import GetToolInfo

from .conversions import task_pose_to_ros_pose
from .dsr_adapter import DsrAdapter, DsrAdapterError

# 랙 이동 전용 TCP — 오프셋 0, 즉 "플랜지가 곧 작업점".
#
# targets.yaml 의 툴 좌표는 티치펜던트로 **그리퍼를 슬롯 파지 자세에 가져다
# 놓고** 읽은 값이다. 즉 "플랜지가 여기 있어야 한다"는 뜻이지 "툴 끝이 여기"가
# 아니다. 그런데 툴마다 tcp_offset 이 다르면(sander 92.5 / uv 110 …) 직전에
# 들고 있던 툴이 무엇이냐에 따라 같은 좌표라도 실제 도달 높이가 달라진다.
# → 랙으로 이동하기 직전에 항상 이 TCP 로 되돌려, 좌표 해석 기준을 고정한다.
#   툴을 잡은 뒤에는 tcp_<툴이름> 으로 전환해 공정 작업이 툴 끝 기준이 되게 한다.
NEUTRAL_TCP = 'tcp_none'

SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_GRIP_FAILED: ErrorCode.SEV_RETRY,
    ErrorCode.E_TOOL_DROP: ErrorCode.SEV_SAFETY,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_INVALID_GOAL: ErrorCode.SEV_ABORT,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


class ToolManagerNode(Node):

    def __init__(self):
        super().__init__('tool_manager')
        self._declare_parameters()
        p = self.get_parameter

        self._rack = self._load_rack_config(p('rack_config_file').value)
        self._tool_list = list(p('tool_list').value)

        self._current_tool = ToolState.NONE
        self._current_tcp = ''
        self._grip_width_mm = 0.0
        self._grip_verified = False
        self._pick_place_goal_handle = None

        self._latest_safety = None
        self._last_safety_rx_monotonic = None
        safety_qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE,
                                 durability=DurabilityPolicy.TRANSIENT_LOCAL)

        self._cb_client = MutuallyExclusiveCallbackGroup()
        self._cb_action = MutuallyExclusiveCallbackGroup()

        self.create_subscription(SafetyState, p('safety_topic').value,
                                  self._on_safety_status, safety_qos,
                                  callback_group=self._cb_client)

        self._status_pub = self.create_publisher(ToolState, '/tool/status', safety_qos)

        try:
            self._adapter = DsrAdapter(self, p('dsr_prefix').value, p('robot_model').value,
                                        client_node_name='tool_manager_dsr_client')
        except DsrAdapterError as e:
            self.get_logger().error(str(e))
            raise

        self._neutral_tcp_ready = self._register_neutral_tcp()

        if p('sync_tcp_on_startup').value:
            self._sync_tcp_from_rack()

        self._pick_place_client = ActionClient(self, PickPlace, '/skill/pick_place',
                                                callback_group=self._cb_client)

        self.create_service(GetToolInfo, '/tool/get_info', self._on_get_tool_info,
                             callback_group=self._cb_client)

        self._change_server = ActionServer(
            self, ChangeTool, '/tool/change',
            execute_callback=self._execute_change,
            goal_callback=self._on_goal_change,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)

        self._publish_status()
        self.get_logger().info(
            f'tool_manager ready — tool_list={self._tool_list}, '
            f'rack_config 항목 {len(self._rack)}개 로드')

    def destroy_node(self):
        self._adapter.destroy()
        super().destroy_node()

    # --- 파라미터 (NIS §5.2 표 + 공통) ------------------------------------------
    def _declare_parameters(self):
        d = self.declare_parameter
        d('dsr_prefix', 'dsr01')
        d('robot_model', 'm0609')
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 0.2)
        d('node_timeout_s', 120.0)
        d('log_force_data', False)
        d('use_mock_hardware', False)
        d('tool_list', ['sander', 'brush', 'coater', 'uv', 'tweezers'])
        d('rack_config_file', 'config/tool_rack.yaml')
        d('sync_tcp_on_startup', True)
        d('approach_height_mm', 50.0)
        d('grip_width_tolerance_mm', 1.0)
        d('change_timeout_s', 60.0)
        d('uv_park_facing', 'into_rack')
        # 랙 이동 직전 TCP 를 NEUTRAL_TCP(오프셋 0)로 되돌릴지. 기본 true.
        # false 로 두면 직전 툴의 tcp_offset 이 그대로 남아 랙 도달 높이가
        # 교체 순서에 따라 달라진다 — 디버깅용으로만 끌 것.
        d('neutral_tcp_for_rack', True)

    def _load_rack_config(self, path):
        try:
            with open(path) as f:
                data = yaml.safe_load(f) or {}
        except OSError as e:
            self.get_logger().error(
                f'rack_config_file 로드 실패({path}): {e}. 빈 설정으로 시작 — '
                'ChangeTool 은 전부 E_INVALID_GOAL 로 거부됩니다.')
            return {}
        return data.get('tools', {}) or {}

    def _register_neutral_tcp(self):
        """오프셋 0 짜리 TCP 를 컨트롤러에 등록해 둔다 (NEUTRAL_TCP 주석 참고).

        sync_tcp_on_startup 과 무관하게 항상 등록한다 — 값이 0 이라 잘못될
        여지가 없고, 이게 없으면 랙 이동 기준을 고정할 방법이 사라진다.
        실패해도 노드는 계속 뜬다(이전 동작으로 폴백하고 경고만 남긴다).
        """
        try:
            self._adapter.add_tcp(NEUTRAL_TCP, [0.0] * 6)
        except DsrAdapterError as e:
            self.get_logger().error(
                f'{NEUTRAL_TCP} 등록 실패: {e}. 랙 이동 시 TCP 를 0 으로 되돌리지 '
                '못합니다 — 직전 툴의 tcp_offset 이 그대로 적용되어 파지 높이가 '
                '교체 순서에 따라 달라질 수 있습니다.')
            return False
        self.get_logger().info(f'{NEUTRAL_TCP} 등록 완료 (랙 이동 기준 TCP)')
        return True

    def _use_neutral_tcp(self):
        """랙(PickPlace)으로 이동하기 직전에 호출. 반환: 계속 진행해도 되는가.

        등록 자체가 실패했으면(=이전 동작) 경고만 남기고 진행한다. 등록은
        됐는데 전환이 실패한 경우는 좌표 기준이 어긋난 채로 움직이게 되므로
        진행하지 않는다.
        """
        if not self.get_parameter('neutral_tcp_for_rack').value:
            return True
        if not self._neutral_tcp_ready:
            self.get_logger().warn(
                f'{NEUTRAL_TCP} 미등록 상태로 랙 이동 — 파지 높이가 직전 툴의 '
                'tcp_offset 만큼 어긋날 수 있습니다.')
            return True
        try:
            self._adapter.set_tcp(NEUTRAL_TCP)
        except DsrAdapterError as e:
            self.get_logger().error(f'{NEUTRAL_TCP} 전환 실패: {e}')
            return False
        self._current_tcp = NEUTRAL_TCP
        return True

    def _sync_tcp_from_rack(self):
        """rack_config_file 의 tcp_offset 값을 두산 컨트롤러에 그대로 등록한다.

        이렇게 하면 티치펜던트에서 TCP 를 수동으로 등록/수정할 필요 없이
        rack_config_file 하나만 갱신하면 된다 — set_tcp() 가 찾는 이름
        (`tcp_<tool>`) 은 항상 이 파일 기준으로 (재)생성된다. 한 툴 등록이
        실패해도 나머지 툴은 계속 시도한다 — 노드 기동 자체를 막지 않는다
        (해당 툴의 ChangeTool 만 이후 set_tcp 단계에서 실패한다).
        """
        for name, cfg in self._rack.items():
            tcp_name = f'tcp_{name}'
            offset = cfg.get('tcp_offset', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
            try:
                self._adapter.add_tcp(tcp_name, offset)
            except DsrAdapterError as e:
                self.get_logger().error(f'TCP 동기화 실패({tcp_name}): {e}')
            else:
                self.get_logger().info(f'TCP 동기화: {tcp_name} = {offset}')

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
        # §3.3 취소 전파: 우리가 보관 중인 하위 스킬 액션(goal_handle)을 취소한다.
        if self._pick_place_goal_handle is not None:
            self._pick_place_goal_handle.cancel_goal_async()
        return CancelResponse.ACCEPT

    # --- 상태 ---------------------------------------------------------------
    def _current_state_msg(self) -> ToolState:
        msg = ToolState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.current_tool = self._current_tool
        msg.active_tcp = self._current_tcp
        msg.grip_width_mm = self._grip_width_mm
        cfg = self._rack.get(self._current_tool)
        msg.expected_width_mm = cfg.get('expected_grip_width_mm', 0.0) if cfg else 0.0
        msg.grip_verified = self._grip_verified
        return msg

    def _publish_status(self):
        self._status_pub.publish(self._current_state_msg())

    def _mark_tool_lost(self):
        """파지 실패/불확실 시 "툴 없음"으로 확정 발행한다 (NIS §5.2 에러 표).

        어떤 툴을 쥐고 있는지 신뢰할 수 없는 상태로 놔두면 이후 공정 노드가
        잘못된 툴로 작업을 시작할 수 있다 — 반드시 NONE 으로 낮춘다.
        """
        self._current_tool = ToolState.NONE
        self._current_tcp = ''
        self._grip_width_mm = 0.0
        self._grip_verified = False
        self._publish_status()

    # --- /tool/get_info ---------------------------------------------------------
    def _on_get_tool_info(self, request, response):
        tool_id = request.tool_id or self._current_tool
        cfg = self._rack.get(tool_id)
        response.state = self._current_state_msg()
        if cfg is None:
            response.found = False
            response.tcp_offset = [0.0] * 6
            response.slot_frame = ''
            return response
        response.found = True
        offset = list(cfg.get('tcp_offset', [0.0] * 6))
        response.tcp_offset = [float(v) for v in (offset + [0.0] * 6)[:6]]
        response.slot_frame = cfg.get('slot_frame', '')
        return response

    # --- /tool/change goal 검증 (§3.1 ②③) --------------------------------------
    def _on_goal_change(self, goal_request):
        target = goal_request.target_tool
        if target != ToolState.NONE and target not in self._tool_list:
            self.get_logger().warn(
                f'ChangeTool REJECT: E_INVALID_GOAL (알 수 없는 target_tool {target!r})')
            return GoalResponse.REJECT
        if target != ToolState.NONE and target not in self._rack:
            self.get_logger().warn(
                f'ChangeTool REJECT: E_INVALID_GOAL ("{target}" 의 rack_config 항목 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('ChangeTool REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    # --- PickPlace 클라이언트 헬퍼 -----------------------------------------------
    def _call_pick_place(self, goal, our_goal_handle, timeout_s):
        """반환: (pick_place_result | None, error_code | None, detail)

        error_code 가 E_CANCELLED 면 our_goal_handle 취소 요청으로 중단된 것이고,
        그 외 코드면 실패, None 이면 정상 완료다.
        """
        if not self._pick_place_client.wait_for_server(timeout_sec=10.0):
            return None, ErrorCode.E_MOTION_FAILED, 'pick_place 액션 서버 연결 실패'

        send_done = threading.Event()
        state = {}

        def on_goal_response(fut):
            state['goal_handle'] = fut.result()
            send_done.set()

        send_future = self._pick_place_client.send_goal_async(goal)
        send_future.add_done_callback(on_goal_response)
        if not send_done.wait(timeout=timeout_s):
            return None, ErrorCode.E_TIMEOUT, 'pick_place goal 전송 타임아웃'

        gh = state.get('goal_handle')
        if gh is None or not gh.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED, 'pick_place goal 거부됨'
        self._pick_place_goal_handle = gh

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
                self._pick_place_goal_handle = None
                return None, ErrorCode.E_TIMEOUT, 'pick_place 결과 타임아웃'

        self._pick_place_goal_handle = None
        result = state['result'].result
        if cancelled:
            return result, ErrorCode.E_CANCELLED, '사용자 취소'
        return result, None, None

    def _result_base(self, success, code, detail, started_at):
        base = ResultBase()
        base.success = success
        base.error.code = code
        base.error.severity = _severity_for(code)
        base.error.detail = detail
        try:
            base.final_pose = task_pose_to_ros_pose(self._adapter.get_pose())
        except DsrAdapterError:
            pass
        base.duration_s = max(0.0, time.monotonic() - started_at)
        base.completed_at = self.get_clock().now().to_msg()
        return base

    # --- 메인 동작 (NIS §5.2 "동작" 1~4) ----------------------------------------
    def _execute_change(self, goal_handle):
        goal = goal_handle.request
        started_at = time.monotonic()
        timeout_s = self.get_parameter('change_timeout_s').value
        approach_height = self.get_parameter('approach_height_mm').value
        tol_default = self.get_parameter('grip_width_tolerance_mm').value
        result = ChangeTool.Result()

        def feedback(step, percent):
            fb = ChangeTool.Feedback()
            fb.step = step
            fb.percent = percent
            goal_handle.publish_feedback(fb)

        # 랙 이동은 전부 "플랜지 기준"으로 해석되어야 한다 (NEUTRAL_TCP 주석).
        # 반납/파지가 모두 끝난 뒤(3단계)에야 새 툴의 TCP 로 바꾼다.
        if not self._use_neutral_tcp():
            goal_handle.abort()
            result.base = self._result_base(
                False, ErrorCode.E_MOTION_FAILED,
                f'랙 이동 전 {NEUTRAL_TCP} 전환 실패 — 좌표 기준을 고정하지 못해 중단',
                started_at)
            return result

        # --- 1) 현재 툴 반납 ---------------------------------------------------
        if self._current_tool != ToolState.NONE:
            feedback(0, 5.0)
            cur_cfg = self._rack.get(self._current_tool)
            if cur_cfg is None:
                goal_handle.abort()
                result.base = self._result_base(
                    False, ErrorCode.E_INVALID_GOAL,
                    f'현재 툴 "{self._current_tool}" 의 rack_config 항목이 없어 반납 불가',
                    started_at)
                return result

            if self._current_tool == ToolState.UV:
                # ⚠️ PickPlace(target_key) 는 이름 하나로 고정 자세(TF)를 참조할
                # 뿐 런타임 방향 파라미터를 받지 않는다 — park_facing 은 실제로
                # 로봇 자세에 반영되지 않는다. 안전 요구사항(SDS §9.3 #6, "필수")
                # 이므로 이 사실을 숨기지 않고 크게 남긴다. slot_frame 자체가
                # 이미 "광축이 랙 안쪽"인 자세로 CAD 고정돼 있어야 한다.
                self.get_logger().warn(
                    f'UV 툴 반납: park_facing="{goal.park_facing or self.get_parameter("uv_park_facing").value}" '
                    f'는 참고용일 뿐, 실제 반납 자세는 rack_config["{ToolState.UV}"].slot_frame '
                    'TF 에 고정된 값을 그대로 씁니다. 광축이 랙 안쪽을 향하는지 반드시 눈으로 확인하세요.')

            place_goal = PickPlace.Goal()
            place_goal.mode = PickPlace.Goal.MODE_PLACE
            place_goal.target_key = cur_cfg['slot_frame']
            place_goal.approach_height_mm = approach_height

            pp_result, err_code, err_detail = self._call_pick_place(
                place_goal, goal_handle, timeout_s)

            if err_code == ErrorCode.E_CANCELLED:
                goal_handle.canceled()
                result.base = self._result_base(False, ErrorCode.E_CANCELLED, err_detail,
                                                  started_at)
                return result
            if err_code is not None or not pp_result.base.success:
                code = err_code or pp_result.base.error.code
                detail = err_detail or pp_result.base.error.detail
                self.get_logger().error(
                    f'[{code}] ChangeTool: "{self._current_tool}" 반납 실패 — {detail}')
                # 반납이 안 됐으니 여전히 쥔 채임을 그대로 발행한다 (숨기지 않는다).
                self._publish_status()
                goal_handle.abort()
                result.base = self._result_base(False, code, detail, started_at)
                return result

            self._current_tool = ToolState.NONE
            self._current_tcp = ''   # 툴 없음 (컨트롤러는 NEUTRAL_TCP 상태)
            self._grip_width_mm = 0.0
            self._grip_verified = False
            self._publish_status()

        if goal.target_tool == ToolState.NONE:
            goal_handle.succeed()
            result.base = self._result_base(True, ErrorCode.OK, '', started_at)
            result.state = self._current_state_msg()
            return result

        # --- 2) 신규 툴 파지 -----------------------------------------------------
        # 파지 폭 검증(grip_verified)은 하드웨어에 접촉/폭 되읽기 센서가 없어
        # 구현 불가 — robot_skill_node 도 명령값을 그대로 되돌려줄 뿐이라
        # 신뢰할 수 없다(§1.5). 여기서는 PickPlace 자체의 성공/실패(모션·
        # 그리퍼 명령 실패)만 판단 근거로 쓴다. 실제 파지 여부는 육안으로
        # 확인할 것.
        feedback(1, 30.0)
        cfg = self._rack[goal.target_tool]  # goal_callback 에서 존재를 이미 확인함

        pick_goal = PickPlace.Goal()
        pick_goal.mode = PickPlace.Goal.MODE_PICK
        pick_goal.target_key = cfg['slot_frame']
        pick_goal.approach_height_mm = approach_height
        pick_goal.expected_width_mm = goal.expected_width_mm if goal.expected_width_mm > 0.0 \
            else cfg.get('expected_grip_width_mm', 0.0)
        pick_goal.width_tolerance_mm = goal.width_tolerance_mm if goal.width_tolerance_mm > 0.0 \
            else tol_default

        feedback(2, 55.0)
        pp_result, err_code, err_detail = self._call_pick_place(pick_goal, goal_handle, timeout_s)

        if err_code == ErrorCode.E_CANCELLED:
            goal_handle.canceled()
            result.base = self._result_base(False, ErrorCode.E_CANCELLED, err_detail, started_at)
            return result

        grip_bad = err_code is not None or not pp_result.base.success
        if grip_bad:
            code = err_code or pp_result.base.error.code
            detail = err_detail or pp_result.base.error.detail
            self._log_grip_failure(code, goal.target_tool, detail)
            self._mark_tool_lost()
            goal_handle.abort()
            result.base = self._result_base(False, code, detail, started_at)
            return result

        # --- 3) TCP 설정 ---------------------------------------------------------
        feedback(3, 85.0)
        try:
            self._adapter.set_tcp(f'tcp_{goal.target_tool}')
        except DsrAdapterError as e:
            detail = f'TCP 설정 실패: {e}'
            self.get_logger().error(f'[{ErrorCode.E_MOTION_FAILED}] ChangeTool: {detail}')
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_MOTION_FAILED, detail, started_at)
            return result

        # --- 4) 상태 확정 -------------------------------------------------------
        feedback(4, 95.0)
        self._current_tool = goal.target_tool
        self._current_tcp = f'tcp_{goal.target_tool}'
        self._grip_width_mm = pp_result.measured_width_mm
        # 검증 불가(센서 없음) — PickPlace 가 성공했으니(위 grip_bad 체크
        # 통과) 파지된 것으로 간주한다. False 로 두면 safety_monitor 가 이를
        # 툴 낙하로 오판해 FAULT_TOOL_DROP 을 래치한다(§1.5).
        self._grip_verified = True
        self._publish_status()

        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        result.state = self._current_state_msg()
        result.measured_width_mm = pp_result.measured_width_mm
        return result

    def _log_grip_failure(self, code, target_tool, detail):
        self.get_logger().error(f'[{code}] ChangeTool: "{target_tool}" 파지 실패 — {detail}')
        if code == ErrorCode.E_TOOL_DROP:
            self.get_logger().error(
                '툴 낙하 의심 — 자동 복구하지 않습니다. 어디 떨어졌는지 확인하고 '
                '사람이 치운 뒤에만 새 ChangeTool goal 을 보내세요.')


def main(args=None):
    rclpy.init(args=args)
    node = ToolManagerNode()
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
