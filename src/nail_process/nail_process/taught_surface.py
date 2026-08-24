"""여섯 티칭 Pose로 브러싱/코팅 공용 곡면 왕복 경로를 만든다."""
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
                       arc_max_z_change_mm=2.0, arc_max_orientation_change_deg=10.0):
    entry = _load_surface(config_path, surface_name)
    poses = [_pose_from_entry(entry['poses'], f'p{i}') for i in range(1, 7)]
    p1, p2, p3, p4, p5, p6 = poses

    top = lambda t: _curve_pose(p1, p2, p3, t)
    bottom = lambda t: _curve_pose(p6, p5, p4, t)
    boundary = _boundary_polygon(top, bottom, p3, p4, p6, p1)

    cross_width_mm = sum(
        _position_distance_mm(top(t), bottom(t)) for t in (0.0, 0.5, 1.0)
    ) / 3.0
    top_length_mm = _curve_length_mm(top)
    bottom_length_mm = _curve_length_mm(bottom)
    longitudinal_mm = (top_length_mm + bottom_length_mm) / 2.0

    pitch_mm = float(pitch_mm)
    inset_mm = float(inset_mm)
    if pitch_mm <= 0.0 or inset_mm < 0.0:
        raise SurfaceConfigError('path_pitch_mm은 양수, 경계 여유는 0 이상이어야 함')
    if cross_width_mm <= 2.0 * inset_mm or longitudinal_mm <= 2.0 * inset_mm:
        raise SurfaceConfigError(
            f'경계 여유 {inset_mm:.2f}mm가 티칭 영역 '
            f'({longitudinal_mm:.2f} x {cross_width_mm:.2f}mm)에 비해 너무 큼')

    v0 = inset_mm / cross_width_mm
    v1 = 1.0 - v0
    t0 = inset_mm / longitudinal_mm
    t1 = 1.0 - t0
    work_width_mm = cross_width_mm - 2.0 * inset_mm
    row_count = max(2, int(math.ceil(work_width_mm / pitch_mm)) + 1)

    waypoints = []
    circular_via_indices = []
    for row_index in range(row_count):
        u = row_index / (row_count - 1)
        v = v0 + (v1 - v0) * u

        def row_pose(t):
            return _blend_pose(top(t), bottom(t), v)

        row_length_mm = _curve_length_mm(row_pose, t0, t1)
        segment_count = max(1, int(math.ceil(
            row_length_mm / max(float(arc_segment_length_mm), 0.1))))
        ts = [t0 + (t1 - t0) * i / segment_count for i in range(segment_count + 1)]
        if row_index % 2:
            ts.reverse()

        waypoints.append(row_pose(ts[0]))
        for start_t, end_t in zip(ts, ts[1:]):
            middle_t = (start_t + end_t) / 2.0
            start = waypoints[-1]
            via = row_pose(middle_t)
            end = row_pose(end_t)
            via_index = len(waypoints)
            waypoints.extend((via, end))
            if _arc_is_valid(
                    start, via, end,
                    float(arc_min_sagitta_mm), float(arc_min_radius_mm),
                    float(arc_max_radius_mm), float(arc_max_z_change_mm),
                    float(arc_max_orientation_change_deg)):
                circular_via_indices.append(via_index)

    return SurfacePath(
        frame_id=str(entry.get('frame_id') or 'base_link'),
        waypoints=waypoints,
        circular_via_indices=circular_via_indices,
        allowed_polygon=boundary,
        row_count=row_count,
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
    if not isinstance(entry, dict) or any(name not in entry for name in required):
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


def _curve_pose(start, middle, end, t):
    # 세 점을 각각 t=0, 0.5, 1에서 통과하는 2차 Lagrange 곡선이다.
    weights = (2.0 * (t - 0.5) * (t - 1.0),
               -4.0 * t * (t - 1.0),
               2.0 * t * (t - 0.5))
    pose = Pose()
    for axis in ('x', 'y', 'z'):
        value = sum(w * getattr(p.position, axis)
                    for w, p in zip(weights, (start, middle, end)))
        setattr(pose.position, axis, value)
    if t <= 0.5:
        pose.orientation = _slerp(start.orientation, middle.orientation, t * 2.0)
    else:
        pose.orientation = _slerp(middle.orientation, end.orientation, (t - 0.5) * 2.0)
    return pose


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


def _curve_length_mm(curve, start=0.0, end=1.0, samples=24):
    points = [curve(start + (end - start) * i / samples) for i in range(samples + 1)]
    return sum(_position_distance_mm(a, b) for a, b in zip(points, points[1:]))


def _position_distance_mm(first, second):
    return math.dist(
        (first.position.x, first.position.y, first.position.z),
        (second.position.x, second.position.y, second.position.z)) * 1000.0


def _boundary_polygon(top, bottom, p3, p4, p6, p1, samples=24):
    poses = [top(i / samples) for i in range(samples + 1)]
    poses.extend(_blend_pose(p3, p4, i / samples) for i in range(1, samples + 1))
    poses.extend(bottom(1.0 - i / samples) for i in range(1, samples + 1))
    poses.extend(_blend_pose(p6, p1, i / samples) for i in range(1, samples))
    return [Point(x=pose.position.x, y=pose.position.y, z=pose.position.z)
            for pose in poses]


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
