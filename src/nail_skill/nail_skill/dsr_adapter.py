"""두산 API 얇은 래퍼 (SDS §4.1/§4.3).

두산 API(`DSR_ROBOT2`/`dsr_msgs2`) 호출은 이 파일 안에만 존재한다.
robot_skill_node 의 액션 구현은 이 클래스만 호출하고, 두산 서비스 이름/시그니처를
직접 알 필요가 없다. 드라이버 버전이 바뀌어도 여기만 고치면 된다.

힘 추종 내부 루프는 두산 컨트롤러(~1kHz)가 담당한다 — 이 모듈은 목표값/종료
조건만 던진다 (SDS §1.2 위반 금지 규칙 1).
"""
import threading
import time
from dataclasses import dataclass

import rclpy

from .conversions import TaskPose


class DsrAdapterError(RuntimeError):
    pass


@dataclass
class Wrench:
    fx_n: float
    fy_n: float
    fz_n: float
    tx_nm: float
    ty_nm: float
    tz_nm: float


class DsrAdapter:
    """단일 프로세스당 하나의 로봇 id 로 DR_init 을 구성하는 접속점."""

    def __init__(self, node, robot_id: str, robot_model: str,
                 client_node_name: str = 'robot_skill_dsr_client'):
        try:
            import DR_init
        except ImportError as e:
            raise DsrAdapterError(
                'DR_init 모듈을 찾을 수 없습니다. doosan-robot2 워크스페이스의 '
                'install/setup.bash 를 함께 source 했는지 확인하세요.'
            ) from e

        # setattr() 사용: 클래스 메서드 안에서 `DR_init.__dsr__id = ...` 를 그대로
        # 쓰면 파이썬 name-mangling 때문에 엉뚱한 속성에 대입된다.
        setattr(DR_init, '__dsr__id', robot_id)
        setattr(DR_init, '__dsr__model', robot_model)

        self._node = node
        self._dr_node = rclpy.create_node(client_node_name, namespace=robot_id)
        setattr(DR_init, '__dsr__node', self._dr_node)

        try:
            import DSR_ROBOT2 as dsr
            from DR_common2 import posx
        except ImportError as e:
            raise DsrAdapterError(
                'DSR_ROBOT2 / DR_common2 를 import 할 수 없습니다. dsr_common2 '
                '패키지가 설치된 워크스페이스를 overlay 로 source 했는지 확인하세요.'
            ) from e

        self._wait_for_service_discovery(dsr)

        self._DR_BASE = dsr.DR_BASE
        self._DR_TOOL = dsr.DR_TOOL
        self._DR_MV_MOD_ABS = dsr.DR_MV_MOD_ABS
        self._DR_MV_MOD_REL = dsr.DR_MV_MOD_REL
        self._DR_FC_MOD_ABS = dsr.DR_FC_MOD_ABS
        self._DR_FC_MOD_REL = dsr.DR_FC_MOD_REL
        self._DR_QSTOP = dsr.DR_QSTOP
        self._BUSY_STATES = {dsr.DR_STATE_BUSY, dsr.DR_STATE_BLEND,
                              dsr.DR_STATE_ACC, dsr.DR_STATE_CRZ, dsr.DR_STATE_DEC}

        # amovel/amovejx 는 즉시 반환한다(비동기) — movel/movejx(동기)를 쓰면
        # 이동 완료까지 이 래퍼가 통째로 블록되어, 그 사이 힘/자세 퍼블리시
        # 타이머와 취소/안전 감시 루프가 전부 멈춘다 (스킬 루프 20~50Hz 요구
        # 위반, SDS §1.2). 대신 amove* 로 이동을 시작시키고 check_motion() 을
        # 폴링하는 방식을 쓴다.
        self._amovel = dsr.amovel
        self._amovejx = dsr.amovejx
        self._check_motion = dsr.check_motion
        self._task_compliance_ctrl = dsr.task_compliance_ctrl
        self._release_compliance_ctrl = dsr.release_compliance_ctrl
        self._set_desired_force = dsr.set_desired_force
        self._release_force = dsr.release_force
        self._get_tool_force = dsr.get_tool_force
        self._get_current_posx = dsr.get_current_posx
        self._set_tcp = dsr.set_tcp
        self._get_tcp = dsr.get_tcp
        self._posx = posx
        # DSR_ROBOT2 는 DRL(구 로봇 언어) 함수명을 그대로 옮긴 것이므로
        # get_digital_input(index)/get_robot_state() 이름을 신뢰한다(공식
        # 파이썬 API 목록의 GPIO/System 분류에 실제로 존재 — safety_monitor
        # §7.0 처리 참고). dsr_msgs2 의 RobotState 토픽 정확한 이름은 이
        # 저장소에서 확인할 방법이 없어, 이미 검증된 동기 폴링 함수로
        # 하트비트를 대신한다 — 응답이 오면 통신 생존으로 본다.
        self._get_digital_input = dsr.get_digital_input
        self._get_robot_state = dsr.get_robot_state

        # DSR_ROBOT2 의 각 wrapper 호출은 내부적으로 self._dr_node 를 임시
        # executor 에 물려 spin_until_future_complete 한다. 이 dr_node 를 두
        # 스레드가 동시에 spin 하면(예: 100Hz 힘 퍼블리시 타이머와 액션 실행
        # 스레드가 동시에 두산 API 를 부르는 경우) rclpy executor 상태가
        # 깨진다. 모든 두산 API 호출을 이 락 하나로 직렬화한다.
        self._lock = threading.Lock()

        self._move_stop_client = None
        self._gripper_client = None
        self._robot_id = robot_id
        self._setup_stop_client(robot_id)
        self._setup_gripper_client()

    def _wait_for_service_discovery(self, dsr_module, timeout_sec: float = 15.0):
        """DSR_ROBOT2 wrapper 는 call_async() 전에 wait_for_service() 를 부르지
        않아, 서버-클라이언트 매칭이 끝나기 전에 첫 요청을 보내면 응답이 유실되고
        spin_until_future_complete 가 영원히 대기한다(브링업 재시작 직후 재현됨).
        여기서 한 번에 미리 기다린다.
        """
        from rclpy.client import Client
        deadline = time.monotonic() + timeout_sec
        for name, obj in vars(dsr_module).items():
            if isinstance(obj, Client):
                remaining = max(0.1, deadline - time.monotonic())
                if not obj.wait_for_service(timeout_sec=remaining):
                    self._node.get_logger().warn(
                        f'서비스 디스커버리 타임아웃: {name} (컨트롤러가 아직 기동 중일 수 있음)'
                    )

    def _setup_stop_client(self, robot_id):
        from dsr_msgs2.srv import MoveStop
        self._MoveStop = MoveStop
        self._move_stop_client = self._node.create_client(
            MoveStop, f'/{robot_id}/dsr_controller2/motion/move_stop')

    def _setup_gripper_client(self):
        from onrobot_rg_msgs.srv import SetCommand
        self._SetCommand = SetCommand
        self._gripper_client = self._node.create_client(
            SetCommand, '/onrobot/sendCommand')

    def destroy(self):
        self._dr_node.destroy_node()

    # --- motion (비동기 시작 + 폴링) ------------------------------------------
    def start_move_line(self, pose: TaskPose, vel_mms: float, acc_mms2: float,
                         ref=None, relative: bool = False):
        p = self._posx(pose.x_mm, pose.y_mm, pose.z_mm,
                        pose.rz1_deg, pose.ry_deg, pose.rz2_deg)
        mod = self._DR_MV_MOD_REL if relative else self._DR_MV_MOD_ABS
        _ref = self._DR_TOOL if relative else (self._DR_BASE if ref is None else ref)
        with self._lock:
            self._amovel(p, vel=float(vel_mms), acc=float(acc_mms2), ref=_ref, mod=mod)

    def start_move_joint_to_pose(self, pose: TaskPose, vel_mms: float, acc_mms2: float,
                                  ref=None, sol: int = 0):
        """movejx: 목표는 task pose 지만 관절 보간으로 이동 (MoveTo linear=false)."""
        p = self._posx(pose.x_mm, pose.y_mm, pose.z_mm,
                        pose.rz1_deg, pose.ry_deg, pose.rz2_deg)
        _ref = self._DR_BASE if ref is None else ref
        with self._lock:
            self._amovejx(p, vel=float(vel_mms), acc=float(acc_mms2),
                           ref=_ref, mod=self._DR_MV_MOD_ABS, sol=sol)

    def start_move_rel_tool_z(self, delta_z_mm: float, vel_mms: float, acc_mms2: float):
        self.start_move_line(TaskPose(0.0, 0.0, float(delta_z_mm), 0.0, 0.0, 0.0),
                              vel_mms, acc_mms2, relative=True)

    def start_move_rel_tool_xyz(self, dx_mm: float, dy_mm: float, dz_mm: float,
                                 vel_mms: float, acc_mms2: float):
        self.start_move_line(TaskPose(float(dx_mm), float(dy_mm), float(dz_mm), 0.0, 0.0, 0.0),
                              vel_mms, acc_mms2, relative=True)

    def is_moving(self) -> bool:
        with self._lock:
            status = self._check_motion()
        return status in self._BUSY_STATES

    def wait_motion_done(self, timeout_s: float, poll_hz: float = 20.0,
                          on_tick=None, should_abort=None) -> bool:
        """이동이 끝날 때까지 폴링한다.

        on_tick()        : 매 폴링 주기 호출 (feedback 발행 등)
        should_abort()   : True 를 반환하면 즉시 정지시키고 False 로 리턴
                            (취소 요청 / safe_to_move=false 확인용)
        반환값: 정상 완료면 True, abort 되었거나 타임아웃이면 False
        """
        period = 1.0 / max(poll_hz, 1.0)
        deadline = time.monotonic() + timeout_s
        time.sleep(min(0.05, period))  # BUSY 로 전이되기 전 조기 종료 판정 방지
        while self.is_moving():
            if should_abort is not None and should_abort():
                self.stop()
                return False
            if time.monotonic() > deadline:
                self.stop()
                return False
            if on_tick is not None:
                on_tick()
            time.sleep(period)
        if on_tick is not None:
            on_tick()
        return True

    def stop(self, quick: bool = True) -> bool:
        if self._move_stop_client is None or not self._move_stop_client.service_is_ready():
            self._node.get_logger().error('move_stop 서비스가 준비되지 않음 — 정지 명령 실패')
            return False
        req = self._MoveStop.Request()
        req.stop_mode = self._DR_QSTOP if quick else self._DR_QSTOP
        self._move_stop_client.call_async(req)
        return True

    # --- compliance / force -------------------------------------------------
    def compliance_on(self, stiffness_6d):
        with self._lock:
            self._task_compliance_ctrl(stx=list(stiffness_6d), time=0)

    def compliance_off(self):
        with self._lock:
            self._release_compliance_ctrl()

    def set_desired_force(self, force_6d, axis_mask_6d, relative: bool = False):
        mod = self._DR_FC_MOD_REL if relative else self._DR_FC_MOD_ABS
        with self._lock:
            self._set_desired_force(fd=list(force_6d), dir=list(axis_mask_6d), time=0, mod=mod)

    def release_force(self):
        with self._lock:
            self._release_force(time=0)

    # --- sensing -------------------------------------------------------------
    def read_wrench(self, tool_frame: bool = True) -> Wrench:
        ref = self._DR_TOOL if tool_frame else self._DR_BASE
        with self._lock:
            w = self._get_tool_force(ref=ref)
        if not isinstance(w, (list, tuple)) or len(w) < 6:
            raise DsrAdapterError('get_tool_force 응답 없음')
        return Wrench(w[0], w[1], w[2], w[3], w[4], w[5])

    def get_pose(self) -> TaskPose:
        with self._lock:
            pos, _sol = self._get_current_posx(ref=self._DR_BASE)
        if pos is None:
            raise DsrAdapterError('get_current_posx 응답 없음')
        return TaskPose(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5])

    # --- 안전 감시용 읽기 전용 폴링 (safety_monitor, NIS §7) --------------------
    def read_digital_input(self, channel: int) -> bool:
        with self._lock:
            val = self._get_digital_input(channel)
        if val is None:
            raise DsrAdapterError(f'get_digital_input({channel}) 응답 없음')
        return bool(val)

    def read_robot_state(self):
        """컨트롤러가 응답하는지만 본다 — 반환값 자체는 쓰지 않는다.
        예외 없이 리턴하면 통신 생존, 예외/타임아웃이면 comm_ok=False."""
        with self._lock:
            return self._get_robot_state()

    # --- tool / TCP ------------------------------------------------------------
    def set_tcp(self, name: str):
        with self._lock:
            ret = self._set_tcp(name)
        if ret != 0:
            raise DsrAdapterError(f"set_tcp('{name}') 실패")

    def get_tcp(self) -> str:
        with self._lock:
            return self._get_tcp()

    # --- gripper (OnRobot RG, /onrobot/sendCommand) ---------------------------
    def _send_gripper_command(self, command: str, timeout_sec: float = 10.0) -> bool:
        if self._gripper_client is None or not self._gripper_client.wait_for_service(timeout_sec=5.0):
            self._node.get_logger().error('/onrobot/sendCommand 서비스 연결 실패')
            return False
        req = self._SetCommand.Request()
        req.command = command
        future = self._gripper_client.call_async(req)
        # self._node 는 이미 메인 MultiThreadedExecutor 에 물려 spin 중이므로
        # rclpy.spin_until_future_complete(self._node, ...) 를 다시 부르면 같은
        # 노드를 두 executor 가 동시에 물게 된다. future.done() 을 직접 폴링한다.
        deadline = time.monotonic() + timeout_sec
        while not future.done():
            if time.monotonic() > deadline:
                self._node.get_logger().error('/onrobot/sendCommand 응답 타임아웃')
                return False
            time.sleep(0.02)
        result = future.result()
        return bool(result is not None and result.success)

    def gripper_open(self) -> bool:
        return self._send_gripper_command('o')

    def gripper_set_width(self, width_mm: float) -> bool:
        """폭 명령. 단위는 드라이버 프로토콜(0.1mm)에 맞춰 정수 문자열로 보낸다."""
        return self._send_gripper_command(str(int(round(width_mm * 10))))
