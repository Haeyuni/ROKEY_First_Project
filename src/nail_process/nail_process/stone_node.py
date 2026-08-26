"""stone_node — 핀셋으로 리본 파츠 중앙을 집어 티칭 경로로 부착한다.

F/T 센서는 필요한 정밀도를 신뢰할 수 없어 사용하지 않는다. 따라서 핀셋과
파츠의 기울어진 결은 `targets.yaml`에 티칭한 전체 TASK-BASE Pose를 그대로
사용하며, 파지/압착/이탈 구간은 모두 저속 MoveL이다.
"""
import threading
import time

from geometry_msgs.msg import Point
import rclpy
from rclpy.action import ActionClient, ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
import yaml

from nail_msgs.action import MoveTo, PickPlace, PlaceStone
from nail_msgs.msg import ErrorCode, ResultBase, SafetyState, ToolState
from nail_msgs.srv import ValidatePrecondition


SEVERITY_BY_CODE = {
    ErrorCode.OK: ErrorCode.SEV_NONE,
    ErrorCode.E_CANCELLED: ErrorCode.SEV_NONE,
    ErrorCode.E_INVALID_GOAL: ErrorCode.SEV_ABORT,
    ErrorCode.E_PRECOND_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_GRIP_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_MOTION_FAILED: ErrorCode.SEV_ABORT,
    ErrorCode.E_SAFETY_BLOCKED: ErrorCode.SEV_SAFETY,
    ErrorCode.E_TIMEOUT: ErrorCode.SEV_ABORT,
    ErrorCode.E_COMM_LOST: ErrorCode.SEV_ABORT,
}


def _severity_for(code):
    return SEVERITY_BY_CODE.get(code, ErrorCode.SEV_ABORT)


def _abort_reason(code):
    return 'ABORT_' + (code[2:] if code.startswith('E_') else code)


class StoneConfigError(ValueError):
    pass


class StoneNode(Node):

    def __init__(self):
        super().__init__('stone_node')
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
        self._pick_place_client = ActionClient(self, PickPlace, '/skill/pick_place',
                                                callback_group=self._cb_client)
        self._move_client = ActionClient(self, MoveTo, '/skill/move_to',
                                         callback_group=self._cb_client)
        self._active_goal_handle = None
        self._stone_server = ActionServer(
            self, PlaceStone, '/process/place_stone',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=self._cb_action)
        self.get_logger().info('stone_node ready (taught ribbon-part path)')

    def _declare_parameters(self):
        d = self.declare_parameter
        d('safety_topic', '/safety/status')
        d('safety_status_timeout_s', 1.0)
        d('node_timeout_s', 120.0)
        d('press_duration_s', 3.0)
        d('stone_config_path', '')

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

    def _load_config(self):
        path = self.get_parameter('stone_config_path').value
        if not path:
            raise StoneConfigError('stone_config_path가 설정되지 않음')
        try:
            with open(path) as stream:
                config = yaml.safe_load(stream) or {}
        except OSError as exc:
            raise StoneConfigError(f'스톤 티칭 설정 파일을 읽을 수 없음: {exc}') from exc
        except yaml.YAMLError as exc:
            raise StoneConfigError(f'스톤 티칭 YAML 형식 오류: {exc}') from exc
        if config.get('configured') is not True:
            raise StoneConfigError('configured=true가 아님 (실기 티칭/공중 검증 전 이동 금지)')
        keys = ('pick_approach_key', 'pick_key', 'place_approach_key', 'place_key')
        if any(not isinstance(config.get(key), str) or not config[key] for key in keys):
            raise StoneConfigError('네 개의 티칭 Pose 키가 모두 필요함')
        for key in ('pinch_width_mm', 'release_width_mm'):
            if float(config.get(key, 0.0)) <= 0.0:
                raise StoneConfigError(f'{key}는 양수여야 함')
        for key in ('pick_speed_ratio', 'place_speed_ratio'):
            if not 0.0 < float(config.get(key, 0.0)) <= 1.0:
                raise StoneConfigError(f'{key}는 0 초과 1 이하이어야 함')
        # 압착 이동(place_approach->place)만 더 저속으로 누르고 싶을 때 쓴다.
        # 없으면 place_speed_ratio 를 그대로 쓴다(하위호환).
        press_ratio = config.get('place_press_speed_ratio', config.get('place_speed_ratio'))
        if not 0.0 < float(press_ratio) <= 1.0:
            raise StoneConfigError('place_press_speed_ratio는 0 초과 1 이하이어야 함')
        return config

    def _call_validate_precondition(self, session_id, timeout_s=5.0):
        if not self._validate_client.wait_for_service(timeout_sec=timeout_s):
            return False, ['ValidatePrecondition 서비스 연결 실패']
        req = ValidatePrecondition.Request()
        req.stage = ValidatePrecondition.Request.STAGE_STONE
        req.session_id = session_id
        req.required_tool = ToolState.TWEEZERS
        future = self._validate_client.call_async(req)
        deadline = time.monotonic() + timeout_s
        while not future.done():
            if time.monotonic() > deadline:
                return False, ['ValidatePrecondition 응답 타임아웃']
            time.sleep(0.02)
        response = future.result()
        if response is None:
            return False, ['ValidatePrecondition 응답 없음']
        return response.ok, list(response.blocking_reasons)

    def _on_goal(self, goal_request):
        if not goal_request.session_id:
            self.get_logger().warn('PlaceStone REJECT: E_INVALID_GOAL (session_id 없음)')
            return GoalResponse.REJECT
        if not self._safe_to_move():
            self.get_logger().warn('PlaceStone REJECT: E_SAFETY_BLOCKED')
            return GoalResponse.REJECT
        try:
            self._load_config()
        except StoneConfigError as exc:
            self.get_logger().warn(f'PlaceStone REJECT: E_INVALID_GOAL ({exc})')
            return GoalResponse.REJECT
        ok, reasons = self._call_validate_precondition(goal_request.session_id)
        if not ok:
            self.get_logger().warn(f'PlaceStone REJECT: E_PRECOND_FAILED {reasons}')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def _wait_result(self, client, goal, timeout_s, our_goal_handle):
        if not client.wait_for_server(timeout_sec=min(10.0, timeout_s)):
            return None, ErrorCode.E_COMM_LOST
        sent = threading.Event()
        state = {}

        def on_goal_response(future):
            try:
                state['goal_handle'] = future.result()
            except Exception as exc:
                state['send_error'] = exc
            sent.set()

        try:
            client.send_goal_async(goal).add_done_callback(on_goal_response)
        except Exception:
            return None, ErrorCode.E_COMM_LOST
        if not sent.wait(timeout=timeout_s):
            return None, ErrorCode.E_TIMEOUT
        if state.get('send_error') is not None:
            return None, ErrorCode.E_COMM_LOST
        handle = state.get('goal_handle')
        if handle is None or not handle.accepted:
            return None, ErrorCode.E_SAFETY_BLOCKED
        self._active_goal_handle = handle

        completed = threading.Event()

        def on_result(future):
            try:
                state['result'] = future.result()
            except Exception as exc:
                state['result_error'] = exc
            completed.set()

        handle.get_result_async().add_done_callback(on_result)
        deadline = time.monotonic() + timeout_s
        while not completed.wait(timeout=0.1):
            if our_goal_handle.is_cancel_requested:
                handle.cancel_goal_async()
                self._active_goal_handle = None
                return None, 'CANCELLED'
            if not self._safe_to_move():
                handle.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_SAFETY_BLOCKED
            if time.monotonic() > deadline:
                handle.cancel_goal_async()
                self._active_goal_handle = None
                return None, ErrorCode.E_TIMEOUT
        self._active_goal_handle = None
        wrapped = state.get('result')
        result = wrapped.result if wrapped is not None else None
        if state.get('result_error') is not None or result is None or not result.base.success:
            return result, result.base.error.code if result is not None else ErrorCode.E_COMM_LOST
        return result, None

    def _call_pick_place(self, mode, target_key, approach_key, retreat_key, grip_width_mm,
                         speed_ratio, timeout_s, our_goal_handle):
        goal = PickPlace.Goal()
        goal.mode = mode
        goal.target_key = target_key
        goal.frame_id = 'base_link'
        goal.approach_key = approach_key
        goal.retreat_key = retreat_key
        goal.grip_width_mm = grip_width_mm
        goal.move_speed_ratio = speed_ratio
        goal.already_holding = True
        return self._wait_result(self._pick_place_client, goal, timeout_s, our_goal_handle)

    def _call_move_to_key(self, target_key, speed_ratio, timeout_s, our_goal_handle):
        goal = MoveTo.Goal()
        goal.target_key = target_key
        goal.frame_id = 'base_link'
        goal.linear = True
        goal.speed_ratio = speed_ratio
        goal.accel_ratio = speed_ratio
        goal.timeout_s = timeout_s
        return self._wait_result(self._move_client, goal, timeout_s, our_goal_handle)

    def _execute(self, goal_handle):
        started_at = time.monotonic()
        result = PlaceStone.Result()
        try:
            config = self._load_config()
        except StoneConfigError as exc:
            goal_handle.abort()
            result.base = self._result_base(False, ErrorCode.E_INVALID_GOAL, str(exc), started_at)
            return result

        timeout_s = self.get_parameter('node_timeout_s').value
        press_duration = goal_handle.request.press_duration_s
        if press_duration <= 0.0:
            press_duration = self.get_parameter('press_duration_s').value

        def feedback(step, percent):
            message = PlaceStone.Feedback()
            message.step = step
            message.percent = percent
            goal_handle.publish_feedback(message)

        def fail(code, detail):
            if code == 'CANCELLED':
                goal_handle.canceled()
                result.base = self._result_base(False, ErrorCode.E_CANCELLED, detail, started_at)
            else:
                goal_handle.abort()
                result.abort_reason = _abort_reason(code)
                result.base = self._result_base(False, code, detail, started_at)
            return result

        # 0) 핀셋 끝을 리본 파츠 중앙 홈으로 MoveL 접근하고 지정 폭으로 조인다.
        feedback(0, 0.0)
        pick_result, error = self._call_pick_place(
            PickPlace.Goal.MODE_PICK, config['pick_key'], config['pick_approach_key'],
            config['pick_approach_key'], float(config['pinch_width_mm']),
            float(config['pick_speed_ratio']), timeout_s, goal_handle)
        if error is not None:
            return fail(error, f'핀셋 파츠 파지 실패: {error}')

        # 1~2) 파츠 결을 유지한 채 티칭된 대각선 MoveL로 압착 Pose까지 간다.
        feedback(1, 25.0)
        approach_result, error = self._call_move_to_key(
            config['place_approach_key'], float(config['place_speed_ratio']),
            timeout_s, goal_handle)
        if error is not None:
            return fail(error, f'부착 접근 이동 실패: {error}')
        feedback(2, 50.0)
        press_ratio = float(config.get('place_press_speed_ratio', config['place_speed_ratio']))
        press_result, error = self._call_move_to_key(
            config['place_key'], press_ratio, timeout_s, goal_handle)
        if error is not None:
            return fail(error, f'부착 압착 이동 실패: {error}')

        # 3) 힘 제어 없이 티칭 Pose에서만 유지한다.
        deadline = time.monotonic() + press_duration
        while time.monotonic() < deadline:
            if goal_handle.is_cancel_requested:
                return fail('CANCELLED', '사용자 취소')
            if not self._safe_to_move():
                return fail(ErrorCode.E_SAFETY_BLOCKED, 'safe_to_move=false')
            time.sleep(0.05)

        # 4) 현재 압착 Pose에서 핀셋을 열고 티칭된 접근 Pose로 MoveL 이탈한다.
        feedback(3, 75.0)
        release_result, error = self._call_pick_place(
            PickPlace.Goal.MODE_PLACE, config['place_key'], config['place_key'],
            config['place_approach_key'], float(config['release_width_mm']),
            float(config['place_speed_ratio']), timeout_s, goal_handle)
        if error is not None:
            return fail(error, f'파츠 해제 또는 이탈 실패: {error}')

        feedback(4, 100.0)
        goal_handle.succeed()
        result.base = self._result_base(True, ErrorCode.OK, '', started_at)
        result.actual_position = press_result.base.final_pose.position
        return result

    def _result_base(self, success, code, detail, started_at):
        base = ResultBase()
        base.success = success
        base.error.code = code
        base.error.severity = _severity_for(code)
        base.error.detail = detail
        base.duration_s = max(0.0, time.monotonic() - started_at)
        base.completed_at = self.get_clock().now().to_msg()
        return base


def main(args=None):
    rclpy.init(args=args)
    node = StoneNode()
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
