"""새 Probe 도구의 플랜지 기준 3점 힘 감지 시험."""
import math
import os
import statistics
import time

import rclpy
import yaml
from ament_index_python.packages import get_package_share_directory
from rclpy.node import Node

from nail_msgs.msg import SafetyState

from .conversions import TaskPose
from .dsr_adapter import DsrAdapter, DsrAdapterError


class ProbeForceTestNode(Node):
    def __init__(self):
        super().__init__('probe_force_test_node')
        d = self.declare_parameter
        default_config = os.path.join(
            get_package_share_directory('nail_skill'), 'config', 'probe_force_test.yaml')
        d('config_path', default_config)
        d('execute', False)
        d('dsr_prefix', 'dsr01')
        d('robot_model', 'm0609')
        d('safety_topic', '/safety/status')
        d('approach_speed_mms', 5.0)
        d('press_speed_mms', 0.7)
        d('press_accel_mms2', 1.0)
        d('max_depth_mm', 10.0)
        d('max_force_n', 5.0)
        d('air_offset_z_mm', 60.0)
        d('comparison_margin_n', 0.3)
        d('baseline_samples', 30)
        d('sample_hz', 10.0)
        d('noise_sigma', 6.0)
        d('min_detect_force_n', 0.15)
        d('confirm_samples', 3)
        d('motion_timeout_s', 40.0)
        d('move_pose_tolerance_mm', 1.0)
        d('move_axis_tolerance_deg', 3.0)

        self._latest_safety = None
        self._last_safety_rx = None
        self.create_subscription(
            SafetyState, self.get_parameter('safety_topic').value,
            self._on_safety_status, 10)
        self._adapter = DsrAdapter(
            self,
            self.get_parameter('dsr_prefix').value,
            self.get_parameter('robot_model').value,
            client_node_name='probe_force_test_dsr_client')

    def destroy_node(self):
        self._adapter.destroy()
        super().destroy_node()

    def _on_safety_status(self, msg):
        self._latest_safety = msg
        self._last_safety_rx = time.monotonic()

    def _safe_to_move(self):
        # get_tool_force()는 동기 서비스라 응답 대기 중 subscription 처리가
        # 잠시 밀릴 수 있다. 여기서 별도 1초 freshness 제한까지 적용하면
        # safety_monitor가 정상이어도 baseline 중 오탐 정지한다. 실제 E-Stop과
        # 통신 결함 판정은 safety_monitor가 safe_to_move에 반영하므로 그 값을 쓴다.
        return self._latest_safety is not None and self._latest_safety.safe_to_move

    def _spin_and_check_safety(self):
        """이동 대기 중에도 safety subscription을 계속 처리한다."""
        rclpy.spin_once(self, timeout_sec=0.0)
        return self._safe_to_move()

    def _safety_detail(self):
        if self._latest_safety is None or self._last_safety_rx is None:
            return '안전 상태 메시지를 아직 받지 못함'
        msg = self._latest_safety
        age = time.monotonic() - self._last_safety_rx
        return (
            f'safe_to_move={msg.safe_to_move}, estop_released={msg.estop_released}, '
            f'comm_ok={msg.comm_ok}, active_faults={list(msg.active_faults)}, '
            f'reason={msg.reason!r}, age={age:.3f}s')

    @staticmethod
    def _tool_z_axis(pose):
        rz1 = math.radians(pose.rz1_deg)
        ry = math.radians(pose.ry_deg)
        return (
            math.cos(rz1) * math.sin(ry),
            math.sin(rz1) * math.sin(ry),
            math.cos(ry),
        )

    @staticmethod
    def _sub(a, b):
        return [x - y for x, y in zip(a, b)]

    @staticmethod
    def _norm(values):
        return math.sqrt(sum(value * value for value in values))

    @staticmethod
    def _mean(samples):
        return [statistics.fmean(values) for values in zip(*samples)]

    def _load_points(self):
        path = self.get_parameter('config_path').value
        try:
            with open(path) as stream:
                config = yaml.safe_load(stream) or {}
        except (OSError, yaml.YAMLError) as exc:
            raise ValueError(f'시험 설정을 읽을 수 없음: {exc}') from exc
        if config.get('configured') is not True:
            raise ValueError('probe_force_test.yaml의 configured가 true가 아님')
        points = config.get('points') or {}
        result = []
        required = ('x_mm', 'y_mm', 'z_mm', 'rz1_deg', 'ry_deg', 'rz2_deg')
        for name in ('center', 'left', 'right'):
            entry = points.get(name)
            if not isinstance(entry, dict) or any(entry.get(key) is None for key in required):
                raise ValueError(f'points.{name}에 X/Y/Z/A/B/C 값이 모두 필요함')
            pose = TaskPose(*(float(entry[key]) for key in required))
            result.append((name, pose, int(entry.get('sol', 2))))
        return result

    def _sample_force(self):
        rclpy.spin_once(self, timeout_sec=0.0)
        return self._adapter.get_tool_force()

    def _baseline(self, press_axis):
        samples = []
        period = 1.0 / max(1.0, self.get_parameter('sample_hz').value)
        for _ in range(max(2, int(self.get_parameter('baseline_samples').value))):
            if not self._spin_and_check_safety():
                raise RuntimeError(
                    'baseline 측정 중 안전 상태가 이동을 차단함: '
                    + self._safety_detail())
            samples.append(self._sample_force())
            time.sleep(period)
        mean = self._mean(samples)
        normal_samples = [sum(sample[i] * press_axis[i] for i in range(3))
                          for sample in samples]
        normal_noise = statistics.pstdev(normal_samples)
        force_noise = max(self._norm(self._sub(sample[:3], mean[:3])) for sample in samples)
        torque_noise = max(self._norm(self._sub(sample[3:], mean[3:])) for sample in samples)
        return mean, normal_noise, force_noise, torque_noise

    def _verify_reached(self, target, context):
        actual = self._adapter.get_pose()
        position_error = self._norm((
            actual.x_mm - target.x_mm,
            actual.y_mm - target.y_mm,
            actual.z_mm - target.z_mm,
        ))
        actual_axis = self._tool_z_axis(actual)
        target_axis = self._tool_z_axis(target)
        dot = max(-1.0, min(1.0, sum(a * b for a, b in zip(actual_axis, target_axis))))
        axis_error = math.degrees(math.acos(dot))
        if position_error > self.get_parameter('move_pose_tolerance_mm').value:
            raise RuntimeError(
                f'{context} 위치 오차 {position_error:.2f}mm가 허용값을 초과함')
        if axis_error > self.get_parameter('move_axis_tolerance_deg').value:
            raise RuntimeError(
                f'{context} 작업축 오차 {axis_error:.2f}deg가 허용값을 초과함')
        self.get_logger().info(
            f'{context} 도달 확인: position_error={position_error:.2f}mm '
            f'axis_error={axis_error:.2f}deg')

    def _wait_until_stopped(self):
        deadline = time.monotonic() + 5.0
        while self._adapter.is_moving():
            if time.monotonic() > deadline:
                raise RuntimeError('정지 명령 뒤에도 로봇 모션이 끝나지 않음')
            time.sleep(0.05)

    def _move_and_wait(self, pose, speed, accel, sol=2, on_tick=None, linear=False):
        if linear:
            self._adapter.start_move_line(pose, speed, accel)
        else:
            self._adapter.start_move_joint_to_pose(pose, speed, accel, sol=sol)
        if not self._adapter.wait_motion_done(
                self.get_parameter('motion_timeout_s').value,
                poll_hz=self.get_parameter('sample_hz').value,
                on_tick=on_tick, should_abort=lambda: not self._spin_and_check_safety()):
            raise RuntimeError('이동 실패, 타임아웃 또는 안전 차단')
        self._verify_reached(pose, '티칭 Pose')

    def _run_profile(self, name, phase, pose, press_axis, sol, compression_threshold=None):
        label = f'{name}/{phase}'
        self._move_and_wait(
            pose, self.get_parameter('approach_speed_mms').value,
            self.get_parameter('approach_speed_mms').value * 2.0, sol)
        baseline, normal_noise, force_noise, torque_noise = self._baseline(press_axis)
        self.get_logger().info(
            f'[{label}] baseline force={baseline[:3]} torque={baseline[3:]} '
            f'normal_noise={normal_noise:.4f}N')

        depth = self.get_parameter('max_depth_mm').value
        target = TaskPose(
            pose.x_mm + press_axis[0] * depth,
            pose.y_mm + press_axis[1] * depth,
            pose.z_mm + press_axis[2] * depth,
            pose.rz1_deg, pose.ry_deg, pose.rz2_deg)
        peak = {'compression': 0.0, 'normal': 0.0, 'lateral': 0.0,
                'force': 0.0, 'torque': 0.0}
        state = {'result': None, 'last': None, 'confirmed_samples': 0, 'sample_count': 0}

        def monitor_force():
            force = self._sample_force()
            delta = self._sub(force, baseline)
            normal = sum(delta[i] * press_axis[i] for i in range(3))
            # 접촉 반력은 누르는 방향의 반대이므로 -normal만 압축력으로 쓴다.
            compression = max(0.0, -normal)
            lateral = self._norm([
                delta[i] - normal * press_axis[i] for i in range(3)])
            total_force = self._norm(delta[:3])
            torque = self._norm(delta[3:])
            state['last'] = (force, normal, compression, lateral, total_force, torque)
            state['sample_count'] += 1
            peak['compression'] = max(peak['compression'], compression)
            peak['normal'] = max(peak['normal'], abs(normal))
            peak['lateral'] = max(peak['lateral'], lateral)
            peak['force'] = max(peak['force'], total_force)
            peak['torque'] = max(peak['torque'], torque)
            if state['sample_count'] % max(1, int(self.get_parameter('sample_hz').value / 4.0)) == 0:
                self.get_logger().info(
                    f'[{label}] force={force[:3]} torque={force[3:]} '
                    f'normal={normal:.4f}N compression={compression:.4f}N '
                    f'lateral={lateral:.4f}N total={total_force:.4f}N')
            # 공중 경로는 표면보다 base +Z 60mm에서 끝나므로 접촉 제한을
            # 적용하지 않고 이동 자체의 힘을 끝까지 기록한다.
            if phase != 'air' and total_force >= self.get_parameter('max_force_n').value:
                state['result'] = f'{phase.upper()}_FORCE_LIMIT'
                return True
            if compression_threshold is not None and compression >= compression_threshold:
                state['confirmed_samples'] += 1
                if state['confirmed_samples'] >= self.get_parameter('confirm_samples').value:
                    state['result'] = 'CONTACT_CONFIRMED'
                    return True
            else:
                state['confirmed_samples'] = 0
            return False

        self._adapter.start_move_line(
            target, self.get_parameter('press_speed_mms').value,
            self.get_parameter('press_accel_mms2').value)
        completed = self._adapter.wait_motion_done(
            self.get_parameter('motion_timeout_s').value,
            poll_hz=self.get_parameter('sample_hz').value,
            should_abort=lambda: not self._spin_and_check_safety() or monitor_force())
        if not completed:
            self._wait_until_stopped()
        elif state['result'] is None:
            self._verify_reached(target, f'[{label}] 최대 하강점')
        if not self._safe_to_move():
            raise RuntimeError(f'[{label}] 안전 차단 뒤에는 자동 이탈 이동을 실행하지 않음')

        stopped_pose = self._adapter.get_pose()
        traveled_mm = sum((value - start) * axis for value, start, axis in zip(
            (stopped_pose.x_mm, stopped_pose.y_mm, stopped_pose.z_mm),
            (pose.x_mm, pose.y_mm, pose.z_mm), press_axis))
        try:
            self._move_and_wait(
                pose, self.get_parameter('press_speed_mms').value,
                self.get_parameter('press_accel_mms2').value, sol, linear=True)
        except DsrAdapterError as exc:
            self.get_logger().error(f'[{label}] 이탈 실패: {exc}')

        result = state['result'] or (
            f'{phase.upper()}_COMPLETE' if completed else 'MOTION_ABORTED')
        lateral_warning = peak['lateral'] > peak['normal']
        torque_warning = peak['torque'] > torque_noise * self.get_parameter('noise_sigma').value
        last_force = state['last'][0] if state['last'] is not None else baseline
        last_delta = self._sub(last_force, baseline)
        self.get_logger().info(
            f'[{label}] result={result} traveled={traveled_mm:.3f}mm '
            f'peak_compression={peak["compression"]:.4f}N '
            f'peak_normal={peak["normal"]:.4f}N '
            f'peak_lateral={peak["lateral"]:.4f}N peak_force={peak["force"]:.4f}N '
            f'peak_torque={peak["torque"]:.4f} lateral_warning={lateral_warning} '
            f'torque_warning={torque_warning} baseline_force_noise={force_noise:.4f}N '
            f'confirmed_samples={state["confirmed_samples"]} last_force={last_force} '
            f'last_delta={last_delta}')
        return {
            'result': result,
            'peak_compression': peak['compression'],
            'peak_lateral': peak['lateral'],
            'peak_force': peak['force'],
            'traveled_mm': traveled_mm,
            'normal_noise': normal_noise,
        }

    def _run_point(self, name, pose, sol):
        tool_z = self._tool_z_axis(pose)
        # TCP가 없는 시험이므로 각 플랜지 자세에서 base_link 아래를 향하는 축을 고른다.
        press_axis = tool_z if tool_z[2] < 0.0 else tuple(-value for value in tool_z)
        axis_name = '+Z' if tool_z[2] < 0.0 else '-Z'
        self.get_logger().info(
            f'[{name}] press_axis={axis_name} base=({press_axis[0]:.3f}, '
            f'{press_axis[1]:.3f}, {press_axis[2]:.3f})')

        air_pose = TaskPose(
            pose.x_mm, pose.y_mm,
            pose.z_mm + self.get_parameter('air_offset_z_mm').value,
            pose.rz1_deg, pose.ry_deg, pose.rz2_deg)
        air = self._run_profile(name, 'air', air_pose, press_axis, sol)

        threshold = max(
            self.get_parameter('min_detect_force_n').value,
            air['peak_compression'] + self.get_parameter('comparison_margin_n').value)
        self.get_logger().info(
            f'[{name}] 공중 peak_compression={air["peak_compression"]:.4f}N '
            f'→ 실제 접촉 threshold={threshold:.4f}N')
        contact = self._run_profile(
            name, 'contact', pose, press_axis, sol, compression_threshold=threshold)
        residual = contact['peak_compression'] - air['peak_compression']
        separated = contact['result'] == 'CONTACT_CONFIRMED'
        self.get_logger().info(
            f'[{name}] COMPARISON air={air["peak_compression"]:.4f}N '
            f'contact={contact["peak_compression"]:.4f}N residual={residual:.4f}N '
            f'separated={separated} contact_result={contact["result"]}')
        return 'SEPARATED' if separated else 'NOT_SEPARATED'

    def run(self):
        points = self._load_points()
        for name, pose, _ in points:
            tool_z = self._tool_z_axis(pose)
            axis_name = '+Z' if tool_z[2] < 0.0 else '-Z'
            self.get_logger().info(
                f'[{name}] pose=({pose.x_mm:.2f}, {pose.y_mm:.2f}, {pose.z_mm:.2f}, '
                f'{pose.rz1_deg:.2f}, {pose.ry_deg:.2f}, {pose.rz2_deg:.2f}), '
                f'downward_axis={axis_name}, '
                f'air_start_z={pose.z_mm + self.get_parameter("air_offset_z_mm").value:.2f}mm')
        if not self.get_parameter('execute').value:
            self.get_logger().warn('dry run 완료. 실제 이동은 execute:=true일 때만 시작함')
            return
        if not self._safe_to_move():
            raise RuntimeError('/safety/status가 safe_to_move=true 상태가 아님: '
                               + self._safety_detail())
        results = []
        for name, pose, sol in points:
            if not self._safe_to_move():
                raise RuntimeError('다음 점 시작 전 안전 상태가 이동을 차단함: '
                                   + self._safety_detail())
            results.append((name, self._run_point(name, pose, sol)))
        self.get_logger().info(f'3점 Probe 힘 시험 완료: {results}')


def main(args=None):
    rclpy.init(args=args)
    node = None
    try:
        node = ProbeForceTestNode()
        # safety status 수신을 기다린 뒤 실행한다.
        deadline = time.monotonic() + 2.0
        while time.monotonic() < deadline and node._latest_safety is None:
            rclpy.spin_once(node, timeout_sec=0.1)
        node.run()
    except (DsrAdapterError, RuntimeError, ValueError) as exc:
        if node is not None:
            node.get_logger().error(f'Probe 힘 시험 중단: {exc}')
        else:
            print(f'Probe 힘 시험 초기화 실패: {exc}')
    finally:
        if node is not None:
            node.destroy_node()
        rclpy.shutdown()
