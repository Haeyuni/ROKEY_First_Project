"""probe_check — ProbePoint 단독 검증 도구 (`ros2 run nail_skill probe_check`).

`ros2 action send_goal` 로도 ProbePoint 를 쏠 수는 있지만, v0.3 측정을
검증하기에는 두 가지가 부족하다.

1. **결과에 파형이 딸려 온다.** 이제 한 점에 200~600 표본이 들어 있어
   터미널이 숫자로 뒤덮인다. 정작 봐야 할 열 몇 개가 묻힌다.
2. **가장 중요한 수치를 한 번 찍어서는 못 얻는다.** 재질이 같은 모형에서
   경계를 가르는 축은 접촉 높이 하나뿐이고, 그 축이 쓸 만한지는
   **같은 점을 여러 번 찍었을 때 높이가 얼마나 흔들리는가**로만 알 수 있다.
   이 값(반복 편차)이 손톱 단차보다 크면 그 뒤 파이프라인은 전부 무의미하다.

그래서 이 도구는 지정한 점(들)을 N회 찍고, 볼 값만 표로 뽑은 뒤 반복
편차를 계산해 "이 설정으로 경계를 가를 수 있는가"를 바로 답한다.

예)
  # 같은 점 5회 — 반복 정밀도 측정 (가장 먼저 할 것)
  ros2 run nail_skill probe_check --points 0,0 --repeat 5 --profile height

  # 손톱 위 / 손 위 각각 3회 — 실제 단차가 얼마인지 측정
  ros2 run nail_skill probe_check --points 0,0 8,0 --repeat 3 --profile height

  # 전 특징 측정 (유지·계단·제하 포함)
  ros2 run nail_skill probe_check --points 0,0 --repeat 3 --profile full
"""
import argparse
import csv
import statistics
import sys

from geometry_msgs.msg import Point
from nail_msgs.action import ProbePoint
import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node


class ProbeCheck(Node):

    def __init__(self, action_name):
        super().__init__('probe_check')
        self._client = ActionClient(self, ProbePoint, action_name)
        self._action_name = action_name

    def wait_for_server(self, timeout_s):
        if not self._client.wait_for_server(timeout_sec=timeout_s):
            self.get_logger().error(
                f'{self._action_name} 액션 서버 없음 — robot_skill_node 가 떠 있는지, '
                'ws_dsr 를 먼저 source 했는지 확인할 것')
            return False
        return True

    def probe_once(self, x_mm, y_mm, args):
        goal = ProbePoint.Goal()
        goal.target = Point(x=x_mm / 1000.0, y=y_mm / 1000.0, z=0.0)
        goal.frame_id = args.frame
        goal.profile = args.profile
        goal.approach_height_mm = args.approach
        goal.max_depth_mm = args.depth
        goal.max_force_n = args.force
        goal.probe_speed_mms = args.speed
        goal.lateral_force_limit_n = args.lateral
        goal.slip_ratio_limit = args.slip_limit
        goal.measure_release = args.measure_release
        goal.source_tag = args.tag

        send = self._client.send_goal_async(goal)
        rclpy.spin_until_future_complete(self, send, timeout_sec=args.timeout)
        if not send.done():
            return None, 'GOAL_SEND_TIMEOUT'
        handle = send.result()
        if handle is None or not handle.accepted:
            return None, 'REJECTED'

        fut = handle.get_result_async()
        rclpy.spin_until_future_complete(self, fut, timeout_sec=args.timeout)
        if not fut.done():
            handle.cancel_goal_async()
            return None, 'RESULT_TIMEOUT'
        return fut.result().result, None


_COLUMNS = (
    ('contact_z_mm', 'z_접촉', '{:9.4f}'),
    ('contact_travel_mm', '하강거리', '{:9.3f}'),
    ('contact_depth_mm', '압입', '{:7.3f}'),
    ('stiffness_n_per_mm', 'k', '{:8.2f}'),
    ('relaxation_ratio', '완화', '{:7.3f}'),
    ('hysteresis_ratio', '이력', '{:7.3f}'),
    ('surface_tilt_deg', '기울기°', '{:8.1f}'),
    ('slip_ratio', '슬립비', '{:7.2f}'),
)


def _row(point, result):
    cells = []
    for attr, _title, fmt in _COLUMNS:
        cells.append(fmt.format(getattr(point, attr, 0.0)))
    flags = ''.join(c for c, ok in (('k', point.stiffness_valid),
                                    ('c', point.curve_valid),
                                    ('h', point.hold_valid)) if ok) or '-'
    return (' '.join(cells) + f' {flags:>4}'
            f' {point.reject_reason or "-":<18}'
            f' {result.tare_noise_n:6.3f} {result.regression_samples:5d}')


def main(argv=None):
    argv = argv if argv is not None else sys.argv[1:]
    ros_args = [a for a in argv if a.startswith('--ros-args')]
    parser = argparse.ArgumentParser(
        prog='probe_check', description='ProbePoint 단독 검증')
    parser.add_argument('--points', nargs='+', default=['0,0'],
                        help='측정할 XY 목록 (mm, frame_id 기준). 예: 0,0 8,0 -8,0')
    parser.add_argument('--repeat', type=int, default=5, help='점마다 반복 횟수')
    parser.add_argument('--profile', default=ProbePoint.Goal.PROFILE_HEIGHT,
                        choices=[ProbePoint.Goal.PROFILE_HEIGHT,
                                 ProbePoint.Goal.PROFILE_FULL],
                        help='height=하강만(빠름·안전), full=유지·계단·제하까지')
    parser.add_argument('--frame', default='nail_frame')
    parser.add_argument('--approach', type=float, default=3.0, help='접근 높이 mm')
    parser.add_argument('--depth', type=float, default=9.0, help='최대 하강 거리 mm')
    parser.add_argument('--force', type=float, default=2.0, help='힘 상한 N')
    parser.add_argument('--speed', type=float, default=2.0, help='하강 속도 mm/s')
    parser.add_argument('--lateral', type=float, default=2.0, help='측면 힘 상한 N')
    parser.add_argument('--slip-limit', type=float, default=0.45, dest='slip_limit')
    parser.add_argument('--measure-release', action='store_true', dest='measure_release')
    parser.add_argument('--action', default='/skill/probe_point')
    parser.add_argument('--tag', default='probe_check')
    parser.add_argument('--timeout', type=float, default=90.0)
    parser.add_argument('--csv', default='', help='결과를 CSV 로도 저장할 경로')
    args = parser.parse_args([a for a in argv if a not in ros_args])

    try:
        targets = [tuple(float(v) for v in p.split(',')) for p in args.points]
        assert all(len(t) == 2 for t in targets)
    except (ValueError, AssertionError):
        parser.error('--points 는 "x,y" 형식이어야 합니다. 예: 0,0 8,0')

    rclpy.init(args=None)
    node = ProbeCheck(args.action)
    rows, per_point = [], {}
    try:
        if not node.wait_for_server(15.0):
            return 1
        print(f'\n프로파일={args.profile}  프레임={args.frame}  '
              f'접근={args.approach}mm  예산={args.depth}mm  힘상한={args.force}N\n')
        head = (' '.join(t.rjust(len(f.format(0.0))) for _a, t, f in _COLUMNS)
                + f' {"유효":>4} {"사유":<18} {"노이즈":>6} {"표본":>5}')
        for (x, y) in targets:
            print(f'── ({x:+.1f}, {y:+.1f}) ' + '─' * max(0, 84 - len(head) // 2))
            print('   ' + head)
            zs = []
            for i in range(args.repeat):
                result, err = node.probe_once(x, y, args)
                if err is not None:
                    print(f'   {i+1:>2} {err}')
                    continue
                pt = result.point
                ok = result.base.success
                print(f'   {i+1:>2} ' + _row(pt, result) +
                      ('' if ok else f'  ← {result.base.error.code}'))
                rows.append(dict(x=x, y=y, i=i + 1, ok=ok,
                                 code=result.base.error.code,
                                 **{a: getattr(pt, a, 0.0) for a, _t, _f in _COLUMNS},
                                 noise_n=result.tare_noise_n,
                                 samples=result.regression_samples))
                if pt.valid:
                    zs.append(pt.contact_z_mm)
            per_point[(x, y)] = zs
            if len(zs) >= 2:
                spread = max(zs) - min(zs)
                sd = statistics.stdev(zs)
                print(f'   → 접촉 높이 평균 {statistics.fmean(zs):.4f}mm  '
                      f'표준편차 {sd:.4f}mm  최대-최소 {spread:.4f}mm')
            print()

        _summarise(per_point)
    finally:
        node.destroy_node()
        rclpy.shutdown()

    if args.csv and rows:
        with open(args.csv, 'w', newline='') as fh:
            w = csv.DictWriter(fh, fieldnames=list(rows[0]))
            w.writeheader()
            w.writerows(rows)
        print(f'CSV 저장: {args.csv}')
    return 0


def _summarise(per_point):
    """반복 편차와 점 사이 단차를 비교해 "이 설정으로 가를 수 있는가"에 답한다.

    경계를 가르는 것은 결국 **단차 / 잡음** 비다. 두 값을 나란히 보여주지
    않으면 "높이가 0.3mm 차이 난다"가 좋은 소식인지 나쁜 소식인지 알 수 없다.
    """
    usable = {k: v for k, v in per_point.items() if len(v) >= 2}
    if not usable:
        return
    noises = [statistics.stdev(v) for v in usable.values()]
    noise = statistics.fmean(noises)
    print('=' * 78)
    print(f'반복 편차(1σ 평균): {noise:.4f} mm')
    if len(usable) >= 2:
        means = {k: statistics.fmean(v) for k, v in usable.items()}
        lo = min(means, key=means.get)
        hi = max(means, key=means.get)
        step = means[hi] - means[lo]
        snr = step / noise if noise > 1e-6 else float('inf')
        print(f'점 사이 최대 높이차: {step:.4f} mm '
              f'({lo} → {hi})')
        print(f'단차 / 반복편차 = {snr:.1f}')
        if snr >= 4.0:
            print('  → 높이 축만으로 경계 판별 가능. scan_node 기본 설정으로 진행할 것.')
        elif snr >= 2.0:
            print('  → 경계는 보이지만 여유가 없다. probe_speed 를 낮추고(0.5~1.0mm/s) '
                  'probe_sample_hz 를 올려 반복 편차부터 줄일 것.')
        else:
            print('  → 이 설정으로는 못 가른다. 반복 편차가 단차만큼 크다. '
                  '툴 고정 상태·TCP·안착을 먼저 의심할 것.')
    else:
        print('  (점을 둘 이상 주면 단차 대비 잡음까지 계산한다 — '
              '예: --points 0,0 8,0)')
    print('=' * 78)


if __name__ == '__main__':
    sys.exit(main())
