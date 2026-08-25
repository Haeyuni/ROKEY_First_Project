"""여섯 티칭 Pose로 브러싱 곡면 왕복 경로를 만든다 (coater 는 3점 반복,
build_repeat_path 참고)."""
import math
from dataclasses import dataclass

import yaml
from geometry_msgs.msg import Point, Pose


class SurfaceConfigError(ValueError):
    pass


@dataclass
class SurfacePath:
    frame_id: str
    waypoints: list
    circular_via_indices: list
    allowed_polygon: list
    row_count: int


def surface_is_configured(config_path, surface_name):
    try:
        with open(config_path) as stream:
            root = yaml.safe_load(stream) or {}
    except (OSError, yaml.YAMLError):
        return False
    entry = (root.get('surfaces') or {}).get(surface_name) or {}
    return entry.get('configured') is True


def build_surface_path(config_path, surface_name, pitch_mm, inset_mm,
                       arc_segment_length_mm=4.0, arc_min_sagitta_mm=0.05,
                       arc_min_radius_mm=1.0, arc_max_radius_mm=100.0,
                       arc_max_z_change_mm=2.0, arc_max_orientation_change_deg=10.0,
                       lift_between_rungs_mm=0.0):
    """브러시 여섯 티칭 Pose를 순서대로 잇는 곡면 경로를 만든다.

    P1→P2→P3, P4→P5→P6은 각각 실제 티칭한 경유점을 쓰는 MoveC 원호다.
    P3→P4와 반복 시 P6→P1은 별도 경유점이 없어 MoveL로 연결한다. 브러시는
    티칭한 경계까지 쓸어야 하므로 coverage_margin_mm은 0만 허용한다.
    """
    entry = _load_surface(config_path, surface_name)
    poses = [_pose_from_entry(entry['poses'], f'p{i}') for i in range(1, 7)]
    p1, p2, p3, p4, p5, p6 = poses

    pitch_mm = float(pitch_mm)
    inset_mm = float(inset_mm)
    if pitch_mm <= 0.0 or inset_mm < 0.0:
        raise SurfaceConfigError('path_pitch_mm은 양수, 경계 여유는 0 이상이어야 함')
    if inset_mm != 0.0:
        raise SurfaceConfigError(
            '브러시 곡면 경로는 티칭 경계까지 실행하므로 coverage_margin_mm은 0이어야 함')

    boundary = [Point(x=pose.position.x, y=pose.position.y, z=pose.position.z)
                for pose in (p1, p2, p3, p4, p5, p6)]

    waypoints = poses
    circular_via_indices = []
    for start_index, via_index, end_index in ((0, 1, 2), (3, 4, 5)):
        if _arc_is_valid(
                waypoints[start_index], waypoints[via_index], waypoints[end_index],
                float(arc_min_sagitta_mm), float(arc_min_radius_mm),
                float(arc_max_radius_mm), float(arc_max_z_change_mm),
                float(arc_max_orientation_change_deg)):
            circular_via_indices.append(via_index)

    return SurfacePath(
        frame_id=str(entry.get('frame_id') or 'base_link'),
        waypoints=waypoints,
        circular_via_indices=circular_via_indices,
        allowed_polygon=boundary,
        row_count=2,
    )


def build_repeat_path(config_path, surface_name, repeats=1):
    """poses 에 정의된 p1, p2, ... 를 그 순서 그대로 waypoint 로 삼아,
    매번 마지막 점에서 p1으로 되돌아가는 닫힌 루프(p1→p2→...→pN→p1)를
    repeats 번 도는 경로를 만든다 — coater 전용(요청, 2026-08-25; 왕복이
    아니라 항상 p1→...→pN→p1 방향으로만 순환). 점 개수는 p1..pN 이 몇
    개까지 정의됐는지로 자동 결정된다(최소 2개 필요). pitch/inset
    세분화나 곡선 피팅 없이 점들을 직선으로 잇는다."""
    entry = _load_surface(config_path, surface_name)
    poses_dict = entry['poses']
    point_keys = []
    i = 1
    while f'p{i}' in poses_dict:
        point_keys.append(f'p{i}')
        i += 1
    if len(point_keys) < 2:
        raise SurfaceConfigError(
            f'surfaces.{surface_name}.poses 에 p1, p2 이상 최소 2점이 필요함 '
            f'(현재 {len(point_keys)}개)')
    poses = [_pose_from_entry(poses_dict, key) for key in point_keys]
    boundary = [Point(x=pose.position.x, y=pose.position.y, z=pose.position.z)
                for pose in poses]
    repeats = max(1, int(repeats))
    # p1→p2→...→pN 을 반복하되 매 바퀴 p1으로 되돌아간다 — 바퀴 경계에서
    # 이동거리 0인 중복 waypoint가 안 생기게, p1은 각 바퀴 "시작"에만 넣고
    # 맨 끝에 마지막으로 한 번 더 닫아준다.
    waypoints = poses * repeats + [poses[0]]
    return SurfacePath(
        frame_id=str(entry.get('frame_id') or 'base_link'),
        waypoints=waypoints,
        circular_via_indices=[],
        allowed_polygon=boundary,
        row_count=repeats,
    )


def _load_surface(config_path, surface_name):
    try:
        with open(config_path) as stream:
            root = yaml.safe_load(stream) or {}
    except OSError as exc:
        raise SurfaceConfigError(f'티칭 경로 설정 파일을 읽을 수 없음: {exc}') from exc
    except yaml.YAMLError as exc:
        raise SurfaceConfigError(f'티칭 경로 YAML 형식 오류: {exc}') from exc

    entry = (root.get('surfaces') or {}).get(surface_name)
    if not entry:
        raise SurfaceConfigError(f'surfaces.{surface_name} 설정이 없음')
    if entry.get('configured') is not True:
        raise SurfaceConfigError(
            f'surfaces.{surface_name}.configured가 true가 아님 (실기 티칭 전 이동 금지)')
    if not isinstance(entry.get('poses'), dict):
        raise SurfaceConfigError(f'surfaces.{surface_name}.poses 설정이 없음')
    return entry


def _pose_from_entry(entries, key):
    entry = entries.get(key)
    required = ('x_mm', 'y_mm', 'z_mm', 'rz1_deg', 'ry_deg', 'rz2_deg')
    # `x_mm: ` 처럼 키는 있지만 값이 비어(None) 있으면 yaml 파싱은 통과하므로
    # "name not in entry" 만으론 못 걸러낸다 — None 도 함께 검사한다(실기
    # 확인: 안 그러면 float(None) 에서 처리 안 된 TypeError 로 죽음).
    if not isinstance(entry, dict) or any(entry.get(name) is None for name in required):
        raise SurfaceConfigError(f'{key}에 X/Y/Z/A/B/C 여섯 값이 모두 필요함')

    pose = Pose()
    pose.position.x = float(entry['x_mm']) / 1000.0
    pose.position.y = float(entry['y_mm']) / 1000.0
    pose.position.z = float(entry['z_mm']) / 1000.0
    qx, qy, qz, qw = _zyz_quaternion(
        float(entry['rz1_deg']), float(entry['ry_deg']), float(entry['rz2_deg']))
    pose.orientation.x = qx
    pose.orientation.y = qy
    pose.orientation.z = qz
    pose.orientation.w = qw
    return pose


def _zyz_quaternion(rz1_deg, ry_deg, rz2_deg):
    a = math.radians(rz1_deg) / 2.0
    b = math.radians(ry_deg) / 2.0
    c = math.radians(rz2_deg) / 2.0
    return (
        -math.sin(b) * math.sin(a - c),
        math.sin(b) * math.cos(a - c),
        math.cos(b) * math.sin(a + c),
        math.cos(b) * math.cos(a + c),
    )


def _lift_pose(pose, lift_mm):
    """pose 를 표면 반대 방향(local -Z, robot_skill_node 의 "tool +Z가
    표면 쪽" 관례와 동일)으로 lift_mm 만큼 들어올린 Pose. 자세는 그대로."""
    axis = _local_z_axis(pose.orientation)
    lifted = Pose()
    lifted.position.x = pose.position.x - axis[0] * lift_mm / 1000.0
    lifted.position.y = pose.position.y - axis[1] * lift_mm / 1000.0
    lifted.position.z = pose.position.z - axis[2] * lift_mm / 1000.0
    lifted.orientation = pose.orientation
    return lifted


def _local_z_axis(q):
    """quaternion 의 로컬 +Z 축을 world(base) 좌표 단위벡터로 변환."""
    return (
        2.0 * (q.x * q.z + q.y * q.w),
        2.0 * (q.y * q.z - q.x * q.w),
        1.0 - 2.0 * (q.x * q.x + q.y * q.y),
    )


def _blend_pose(first, second, t):
    pose = Pose()
    pose.position.x = first.position.x + (second.position.x - first.position.x) * t
    pose.position.y = first.position.y + (second.position.y - first.position.y) * t
    pose.position.z = first.position.z + (second.position.z - first.position.z) * t
    pose.orientation = _slerp(first.orientation, second.orientation, t)
    return pose


def _slerp(first, second, t):
    from geometry_msgs.msg import Quaternion

    a = [first.x, first.y, first.z, first.w]
    b = [second.x, second.y, second.z, second.w]
    dot = sum(x * y for x, y in zip(a, b))
    if dot < 0.0:
        b = [-value for value in b]
        dot = -dot
    dot = max(-1.0, min(1.0, dot))
    if dot > 0.9995:
        values = [x + t * (y - x) for x, y in zip(a, b)]
        norm = math.sqrt(sum(value * value for value in values)) or 1.0
        values = [value / norm for value in values]
    else:
        angle = math.acos(dot)
        scale = math.sin(angle)
        left = math.sin((1.0 - t) * angle) / scale
        right = math.sin(t * angle) / scale
        values = [left * x + right * y for x, y in zip(a, b)]
    return Quaternion(x=values[0], y=values[1], z=values[2], w=values[3])


def _position_distance_mm(first, second):
    return math.dist(
        (first.position.x, first.position.y, first.position.z),
        (second.position.x, second.position.y, second.position.z)) * 1000.0


def _arc_is_valid(start, via, end, min_sagitta_mm, min_radius_mm, max_radius_mm,
                  max_z_change_mm, max_orientation_change_deg):
    a = _vector_mm(start, via)
    b = _vector_mm(start, end)
    c = _vector_mm(via, end)
    len_a = _norm(a)
    len_b = _norm(b)
    len_c = _norm(c)
    cross = (a[1] * b[2] - a[2] * b[1],
             a[2] * b[0] - a[0] * b[2],
             a[0] * b[1] - a[1] * b[0])
    cross_norm = _norm(cross)
    if min(len_a, len_b, len_c) < 0.05 or cross_norm < 1e-6:
        return False
    sagitta_mm = cross_norm / len_b
    radius_mm = len_a * len_b * len_c / (2.0 * cross_norm)
    z_values = [pose.position.z * 1000.0 for pose in (start, via, end)]
    orientation_change = max(
        _orientation_angle_deg(start.orientation, via.orientation),
        _orientation_angle_deg(via.orientation, end.orientation))
    return (sagitta_mm >= min_sagitta_mm
            and min_radius_mm <= radius_mm <= max_radius_mm
            and max(z_values) - min(z_values) <= max_z_change_mm
            and orientation_change <= max_orientation_change_deg)


def _vector_mm(first, second):
    return ((second.position.x - first.position.x) * 1000.0,
            (second.position.y - first.position.y) * 1000.0,
            (second.position.z - first.position.z) * 1000.0)


def _norm(vector):
    return math.sqrt(sum(value * value for value in vector))


def _orientation_angle_deg(first, second):
    dot = abs(first.x * second.x + first.y * second.y
              + first.z * second.z + first.w * second.w)
    return math.degrees(2.0 * math.acos(max(-1.0, min(1.0, dot))))
