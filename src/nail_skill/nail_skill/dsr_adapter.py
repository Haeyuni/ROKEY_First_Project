"""두산 API 얇은 래퍼 (SDS §4.1/§4.3).

두산 API(`DSR_ROBOT2`/`dsr_msgs2`) 호출은 이 파일 안에만 존재한다.
robot_skill_node 의 액션 구현은 이 클래스만 호출하고, 두산 서비스 이름/시그니처를
직접 알 필요가 없다. 드라이버 버전이 바뀌어도 여기만 고치면 된다.

이동, 로봇 상태, TCP, 그리퍼와 두산 컨트롤러의 외력 조회 API를 제공한다.
"""
import threading
import time

import rclpy

from .conversions import TaskPose


class DsrAdapterError(RuntimeError):
    pass


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
        self._amovec = dsr.amovec
        self._check_motion = dsr.check_motion
        self._get_current_posx = dsr.get_current_posx
        # 나사 뚜껑 풀기에서 손목(J6) 잔여 가동범위를 확인하는 데만 쓴다.
        # 드라이버 버전에 따라 없을 수 있어 없으면 None 으로 두고, 호출부가
        # "가동범위 확인 불가"로 처리한다.
        self._get_current_posj = getattr(dsr, 'get_current_posj', None)
        self._set_tcp = dsr.set_tcp
        self._get_tcp = dsr.get_tcp
        self._add_tcp = dsr.add_tcp
        self._del_tcp = dsr.del_tcp
        self._set_robot_mode = dsr.set_robot_mode
        self._get_robot_mode = dsr.get_robot_mode
        self._ROBOT_MODE_MANUAL = dsr.ROBOT_MODE_MANUAL
        self._ROBOT_MODE_AUTONOMOUS = dsr.ROBOT_MODE_AUTONOMOUS
        self._posx = posx
        # E-Stop과 통신 상태는 외부 센서가 아니라 컨트롤러 robot_state로 확인한다.
        self._get_robot_state = dsr.get_robot_state
        self._get_tool_force = dsr.get_tool_force

        # DSR_ROBOT2 의 각 wrapper 호출은 내부적으로 self._dr_node 를 임시
        # executor 에 물려 spin_until_future_complete 한다. 이 dr_node 를 두
        # 스레드가 동시에 spin 하면 rclpy executor 상태가 깨질 수 있으므로
        # 모든 두산 API 호출을 이 락 하나로 직렬화한다.
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
        from sensor_msgs.msg import JointState
        self._SetCommand = SetCommand
        self._gripper_client = self._node.create_client(
            SetCommand, '/onrobot/sendCommand')

        # /onrobot/sendCommand 는 Modbus 전송 성공만 확인하고 즉시
        # success=True 를 반환한다(OnRobotRGControllerServer.sendCommandCallback,
        # ws_dsr) — 실제 모션 완료를 보장하지 않는다. 게다가 RG2 자체
        # 프로토콜상 그리퍼가 busy(모션 중)일 때 온 새 명령은 조용히
        # 무시된다. 그 결과 직전 사이클의 grip 이 아직 안 끝난 채로 다음
        # open 명령이 도착하면, 명령은 무시되고 서비스는 성공을 반환하는데
        # 그리퍼는 실제로 열리지 않은 채 하강해버리는 문제가 실기에서
        # 재현됨. /joint_states 의 그리퍼 조인트 effort 는 busy 일 때만
        # 0 이 아니게 채워지므로(같은 파일 getStatus 참고) 이걸로 실제
        # busy 상태를 직접 확인한다.
        self._gripper_joint_state = None
        self._gripper_joint_state_lock = threading.Lock()
        self._node.create_subscription(
            JointState, '/joint_states', self._on_joint_states, 10)

    def _on_joint_states(self, msg):
        for name, effort in zip(msg.name, msg.effort):
            if 'finger_joint' in name:
                with self._gripper_joint_state_lock:
                    self._gripper_joint_state = (name, effort)
                return

    def _wait_gripper_idle(self, timeout_sec: float) -> bool:
        """그리퍼가 busy 가 아닐 때까지 대기. /joint_states 에 그리퍼 조인트가
        아직 안 잡히면(조인트 merge 노드 미기동 등) 판단 불가로 보고 True 를
        반환한다 — 이 경우 호출부의 gripper_settle_s sleep 이 유일한
        안전장치가 된다."""
        deadline = time.monotonic() + timeout_sec
        while time.monotonic() < deadline:
            with self._gripper_joint_state_lock:
                state = self._gripper_joint_state
            if state is None:
                return True
            _, effort = state
            if effort == 0.0:
                return True
            time.sleep(0.02)
        return False

    def destroy(self):
        self._dr_node.destroy_node()

    # --- motion (비동기 시작 + 폴링) ------------------------------------------
    def start_move_line(self, pose: TaskPose, vel_mms: float, acc_mms2: float,
                         ref=None, relative: bool = False,
                         vel_degs: float = None, acc_degs2: float = None):
        """vel_degs/acc_degs2 를 주면 amovel 에 [병진, 회전] 쌍으로 넘긴다.

        스칼라만 주면 두산 컨트롤러가 회전 속도를 자기 기본값으로 정하는데,
        자세 변화가 큰 이동(뚜껑 풀기의 tool Z 축 회전)에서는 그 기본값이
        너무 빨라 제어가 안 된다 — 그럴 때만 회전 쪽을 따로 지정한다.
        """
        p = self._posx(pose.x_mm, pose.y_mm, pose.z_mm,
                        pose.rz1_deg, pose.ry_deg, pose.rz2_deg)
        mod = self._DR_MV_MOD_REL if relative else self._DR_MV_MOD_ABS
        _ref = (self._DR_TOOL if ref is None else ref) if relative \
            else (self._DR_BASE if ref is None else ref)
        vel = float(vel_mms) if vel_degs is None else [float(vel_mms), float(vel_degs)]
        acc = float(acc_mms2) if acc_degs2 is None else [float(acc_mms2), float(acc_degs2)]
        with self._lock:
            self._amovel(p, vel=vel, acc=acc, ref=_ref, mod=mod)

    def start_move_joint_to_pose(self, pose: TaskPose, vel_mms: float, acc_mms2: float,
                                  ref=None, sol: int = 2):
        """movejx: 목표는 task pose 지만 관절 보간으로 이동 (MoveTo linear=false).

        sol(관절 해 분기) 기본값을 0에서 2로 변경 — 이 로봇/셀은
        get_current_posx 로 실측할 때마다 항상 분기 2가 나왔다(실기 확인,
        2026-08-24/25). sol=0 으로 두면 로봇이 목표 바로 옆에 있어도
        그 분기에서 IK 가 안 풀려 amovejx 가 즉시(수십 ms) 조용히 실패한다
        (반환값을 확인 안 해 예외도 안 남) — ContactPath 접근(movejx) 이
        NOT REACHABLE 로 즉시 ABORT 되는 문제의 원인이었다.
        """
        p = self._posx(pose.x_mm, pose.y_mm, pose.z_mm,
                        pose.rz1_deg, pose.ry_deg, pose.rz2_deg)
        _ref = self._DR_BASE if ref is None else ref
        with self._lock:
            self._amovejx(p, vel=float(vel_mms), acc=float(acc_mms2),
                           ref=_ref, mod=self._DR_MV_MOD_ABS, sol=sol)

    def start_move_circle(self, via: TaskPose, end: TaskPose,
                          vel_mms: float, acc_mms2: float, ref=None):
        """현재 위치에서 via를 지나 end로 가는 비동기 MoveC를 시작한다."""
        via_pos = self._posx(via.x_mm, via.y_mm, via.z_mm,
                              via.rz1_deg, via.ry_deg, via.rz2_deg)
        end_pos = self._posx(end.x_mm, end.y_mm, end.z_mm,
                             end.rz1_deg, end.ry_deg, end.rz2_deg)
        _ref = self._DR_BASE if ref is None else ref
        try:
            with self._lock:
                ret = self._amovec(via_pos, end_pos, vel=float(vel_mms),
                                    acc=float(acc_mms2), ref=_ref,
                                    mod=self._DR_MV_MOD_ABS)
        except Exception as exc:
            raise DsrAdapterError(f'amovec 호출 실패: {exc}') from exc
        if ret != 0:
            raise DsrAdapterError(f'amovec 명령 거부(ret={ret})')

    def start_move_rel_tool_z(self, delta_z_mm: float, vel_mms: float, acc_mms2: float):
        self.start_move_line(TaskPose(0.0, 0.0, float(delta_z_mm), 0.0, 0.0, 0.0),
                              vel_mms, acc_mms2, relative=True)

    def start_move_rel_tool_xyz(self, dx_mm: float, dy_mm: float, dz_mm: float,
                                 vel_mms: float, acc_mms2: float):
        self.start_move_line(TaskPose(float(dx_mm), float(dy_mm), float(dz_mm), 0.0, 0.0, 0.0),
                               vel_mms, acc_mms2, relative=True)

    def start_rotate_tool_z(self, delta_deg: float, along_z_mm: float,
                             vel_mms: float, acc_mms2: float,
                             vel_degs: float, acc_degs2: float):
        """tool +Z 축 둘레로 delta_deg 만큼 돌면서 동시에 tool Z 로 along_z_mm 이동.

        나사를 푸는 움직임(회전 + 피치만큼의 축방향 이동)이 정확히 이 형태다.
        posx 의 자세 3개는 ZYZ(A,B,C) 라서 (delta,0,0) 은 Rz(delta)·Ry(0)·Rz(0)
        = Z축 회전 하나만 남는다. ref=DR_TOOL + mod=REL 이므로 그 Z 는 현재
        tool 의 Z 다.

        두산에는 `move_spiral` 이 따로 있지만 그건 TCP **위치**만 나선으로
        움직이고 자세는 그대로 둔다 — 뚜껑을 옆으로 끌고 다닐 뿐 풀지 못한다.
        나사에서 말하는 나선(회전+축방향 전진)은 이 함수 쪽이다.
        """
        self.start_move_line(
            TaskPose(0.0, 0.0, float(along_z_mm), float(delta_deg), 0.0, 0.0),
            vel_mms, acc_mms2, relative=True,
            vel_degs=vel_degs, acc_degs2=acc_degs2)

    def start_move_rel_base_xyz(self, dx_mm: float, dy_mm: float, dz_mm: float,
                                 vel_mms: float, acc_mms2: float):
        self.start_move_line(
            TaskPose(float(dx_mm), float(dy_mm), float(dz_mm), 0.0, 0.0, 0.0),
            vel_mms, acc_mms2, ref=self._DR_BASE, relative=True)

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

    def get_pose(self) -> TaskPose:
        with self._lock:
            pos, _sol = self._get_current_posx(ref=self._DR_BASE)
        if pos is None:
            raise DsrAdapterError('get_current_posx 응답 없음')
        return TaskPose(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5])

    def get_joints(self):
        """관절 6개의 현재 각도(deg) 리스트. 드라이버가 안 주면 None.

        None 을 예외 대신 돌려주는 이유: 이 값을 쓰는 곳(뚜껑 풀기의 J6 잔여
        가동범위 계산)은 값이 없으면 "확인 불가"로 물러설 뿐 실패가 아니다.
        """
        if self._get_current_posj is None:
            return None
        with self._lock:
            joints = self._get_current_posj()
        # 드라이버 버전에 따라 (posj, ...) 튜플로 감싸 오는 경우가 있다.
        if isinstance(joints, tuple) and joints and not isinstance(joints[0], float):
            joints = joints[0]
        if joints is None or len(joints) < 6:
            return None
        return [float(v) for v in joints[:6]]

    # --- 안전 감시용 읽기 전용 폴링 -------------------------------------------
    def read_robot_state(self):
        """컨트롤러가 응답하는지만 본다 — 반환값 자체는 쓰지 않는다.
        예외 없이 리턴하면 통신 생존, 예외/타임아웃이면 comm_ok=False."""
        with self._lock:
            return self._get_robot_state()

    def get_tool_force(self, ref=None):
        """두산 컨트롤러가 계산한 외력/토크 6축 값을 읽는다."""
        force_ref = self._DR_BASE if ref is None else ref
        with self._lock:
            values = self._get_tool_force(ref=force_ref)
        if not isinstance(values, (list, tuple)) or len(values) != 6:
            raise DsrAdapterError(f'get_tool_force 응답 오류: {values!r}')
        return [float(value) for value in values]

    # --- tool / TCP ------------------------------------------------------------
    def set_tcp(self, name: str):
        """add_tcp 와 마찬가지로 AUTONOMOUS(ROS 외부제어) 상태에서는 거부되는
        것으로 보여 MANUAL <-> AUTONOMOUS 를 오간다 (실측 기반, add_tcp 쪽
        docstring 참고). 실패해도 반드시 AUTONOMOUS 로 복귀시킨다."""
        with self._lock:
            self._set_robot_mode(self._ROBOT_MODE_MANUAL)
            try:
                ret = self._set_tcp(name)
            finally:
                self._set_robot_mode(self._ROBOT_MODE_AUTONOMOUS)
        if ret != 0:
            raise DsrAdapterError(f"set_tcp('{name}') 실패")

    def get_tcp(self) -> str:
        with self._lock:
            return self._get_tcp()

    def add_tcp(self, name: str, offset):
        """컨트롤러에 TCP 좌표계를 (재)등록한다 — 값의 출처는 호출자(랙 설정
        파일 등)이며, 이 함수는 그대로 두산 컨트롤러에 반영만 한다.

        같은 이름이 이미 있으면 두산 쪽 add_tcp 가 실패로 응답하는 것으로
        보여 먼저 del_tcp 로 지운다 — 없는 이름을 지우는 건 실패해도 무해하니
        결과를 무시한다. 이렇게 해야 설정 파일의 값이 바뀐 뒤 노드를 재시작할
        때마다 컨트롤러 쪽 값도 항상 최신으로 갱신된다.

        config_create_tcp/config_delete_tcp 는 로봇이 ROS(AUTONOMOUS) 제어
        상태에서는 거부되는 것으로 보여(실측: 동일 호출이 MANUAL 에서만
        성공), 호출 전후로 MANUAL <-> AUTONOMOUS 를 오간다. 실패해도 반드시
        AUTONOMOUS 로 복귀시켜야 한다 — 그대로 두면 이후 이동 명령(amovel 등)이
        전부 막힌다.
        """
        with self._lock:
            self._set_robot_mode(self._ROBOT_MODE_MANUAL)
            try:
                self._del_tcp(name)
                ret = self._add_tcp(name, offset)
            finally:
                self._set_robot_mode(self._ROBOT_MODE_AUTONOMOUS)
        if ret != 0:
            raise DsrAdapterError(f"add_tcp('{name}') 실패")

    # --- gripper (OnRobot RG, /onrobot/sendCommand) ---------------------------
    def _send_gripper_command(self, command: str, timeout_sec: float = 10.0) -> bool:
        if self._gripper_client is None or not self._gripper_client.wait_for_service(timeout_sec=5.0):
            self._node.get_logger().error('/onrobot/sendCommand 서비스 연결 실패')
            return False
        # RG2 는 busy 상태에서 온 새 명령을 조용히 무시한다 — 직전 모션이
        # 아직 안 끝났으면 먼저 기다려 이번 명령이 씹히는 걸 막는다.
        if not self._wait_gripper_idle(timeout_sec):
            self._node.get_logger().warn(
                '/onrobot/sendCommand: 직전 그리퍼 모션이 아직 안 끝남(busy) '
                '— 이번 명령이 무시될 수 있음')
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
        if not (result is not None and result.success):
            return False
        # 서비스는 Modbus 전송 성공만 의미할 뿐 모션 완료를 보장하지 않는다
        # (위 주석 참고) — busy 플래그가 뜰 시간(퍼블리시 50Hz 최소 1주기)을
        # 준 뒤 실제로 idle 로 돌아올 때까지 기다려 진짜 완료를 확인한다.
        time.sleep(0.05)
        if not self._wait_gripper_idle(timeout_sec):
            self._node.get_logger().error('/onrobot/sendCommand: 그리퍼 모션 완료 대기 타임아웃')
            return False
        return True

    def gripper_open(self) -> bool:
        return self._send_gripper_command('o')

    def gripper_set_width(self, width_mm: float) -> bool:
        """폭 명령. 단위는 드라이버 프로토콜(0.1mm)에 맞춰 정수 문자열로 보낸다."""
        return self._send_gripper_command(str(int(round(width_mm * 10))))
