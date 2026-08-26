"""2D 다각형 유틸리티. 단위는 전부 mm, 평면은 `nail_local_frame` 의 XY.

설정 경계와 독립 Probe 검증에서 사용하는 측정 경계를 함께 지원한다.
"""
import math


def _axis_values(extent_mm, pitch_mm):
    """중앙 0과 양 끝을 포함하는 대칭 격자 축을 만든다."""
    half = extent_mm / 2.0
    values = [0.0]
    value = pitch_mm
    while value < half - 1e-6:
        values.extend((-value, value))
        value += pitch_mm
    if half > 1e-6:
        values.extend((-half, half))
    return sorted(set(round(value, 6) for value in values))


def make_grid(size_x_mm, size_y_mm, pitch_mm, margin_mm=0.0):
    """영역과 바깥 margin을 덮는 (ix, iy, x_mm, y_mm) 지그재그 격자."""
    xs = _axis_values(size_x_mm + 2.0 * margin_mm, pitch_mm)
    ys = _axis_values(size_y_mm + 2.0 * margin_mm, pitch_mm)
    points = []
    for iy, y in enumerate(ys):
        row = [(ix, iy, x, y) for ix, x in enumerate(xs)]
        if iy % 2:
            row.reverse()
        points.extend(row)
    return points


def grid_transition_midpoints(grid, classifications):
    """상하좌우 이웃의 접촉 판정이 바뀌는 구간 중점을 반환한다."""
    by_index = {(ix, iy): (x, y) for ix, iy, x, y in grid}
    result = []
    for (ix, iy), point in by_index.items():
        here = classifications.get((ix, iy))
        if here is None:
            continue
        for neighbor in ((ix + 1, iy), (ix, iy + 1)):
            there = classifications.get(neighbor)
            if there is None or there == here or neighbor not in by_index:
                continue
            other = by_index[neighbor]
            result.append(((point[0] + other[0]) / 2.0,
                           (point[1] + other[1]) / 2.0))
    return result


def central_contact_component(grid, classifications, seed_index=None):
    """기준점(없으면 중심)에 가장 가까운 접촉점과 연결된 성분만 남긴다."""
    by_index = {(ix, iy): (x, y) for ix, iy, x, y in grid}
    contacts = {index for index, value in classifications.items() if value}
    if not contacts:
        return set()
    seed = seed_index if seed_index in contacts else min(
        contacts, key=lambda index: sum(value * value for value in by_index[index]))
    component = {seed}
    pending = [seed]
    while pending:
        ix, iy = pending.pop()
        for neighbor in ((ix - 1, iy), (ix + 1, iy), (ix, iy - 1), (ix, iy + 1)):
            if neighbor in contacts and neighbor not in component:
                component.add(neighbor)
                pending.append(neighbor)
    return component


def convex_hull(points_xy):
    """Andrew monotonic chain. 중복을 제거한 반시계 방향 볼록 외곽선."""
    points = sorted(set(points_xy))
    if len(points) <= 1:
        return points

    def cross(origin, a, b):
        return ((a[0] - origin[0]) * (b[1] - origin[1])
                - (a[1] - origin[1]) * (b[0] - origin[0]))

    lower = []
    for point in points:
        while len(lower) >= 2 and cross(lower[-2], lower[-1], point) <= 0.0:
            lower.pop()
        lower.append(point)
    upper = []
    for point in reversed(points):
        while len(upper) >= 2 and cross(upper[-2], upper[-1], point) <= 0.0:
            upper.pop()
        upper.append(point)
    return lower[:-1] + upper[:-1]


def grid_contour_polygon(grid, classifications):
    """이진 격자 분류의 전환선을 Marching Squares로 연결한 가장 긴 폐곡선."""
    points = {(ix, iy): (x, y) for ix, iy, x, y in grid}
    # 각 cell의 (bottom, right, top, left) 전환 edge 쌍.
    cases = {
        1: ((3, 0),), 2: ((0, 1),), 3: ((3, 1),), 4: ((1, 2),),
        5: ((3, 2), (0, 1)), 6: ((0, 2),), 7: ((3, 2),), 8: ((2, 3),),
        9: ((0, 2),), 10: ((0, 3), (1, 2)), 11: ((1, 2),),
        12: ((1, 3),), 13: ((0, 1),), 14: ((0, 3),),
    }
    adjacency = {}

    def midpoint(a, b):
        return (round((a[0] + b[0]) / 2.0, 6), round((a[1] + b[1]) / 2.0, 6))

    for ix, iy in points:
        indices = ((ix, iy), (ix + 1, iy), (ix + 1, iy + 1), (ix, iy + 1))
        if any(index not in points or index not in classifications for index in indices):
            continue
        values = [classifications[index] for index in indices]
        case = sum((1 << bit) for bit, value in enumerate(values) if value)
        if case in (0, 15):
            continue
        edge_points = (
            midpoint(points[indices[0]], points[indices[1]]),
            midpoint(points[indices[1]], points[indices[2]]),
            midpoint(points[indices[3]], points[indices[2]]),
            midpoint(points[indices[0]], points[indices[3]]),
        )
        for first, second in cases[case]:
            a, b = edge_points[first], edge_points[second]
            adjacency.setdefault(a, []).append(b)
            adjacency.setdefault(b, []).append(a)

    loops = []
    visited_edges = set()
    for start, neighbors in adjacency.items():
        for first in neighbors:
            edge = tuple(sorted((start, first)))
            if edge in visited_edges:
                continue
            loop = [start]
            previous, current = start, first
            while True:
                visited_edges.add(tuple(sorted((previous, current))))
                loop.append(current)
                if current == start:
                    break
                next_points = [point for point in adjacency.get(current, []) if point != previous]
                if len(next_points) != 1:
                    loop = []
                    break
                previous, current = current, next_points[0]
            if len(loop) >= 4:
                loops.append(loop[:-1])

    if not loops:
        return []
    return max(loops, key=polygon_area)


def point_in_polygon(x, y, polygon_xy):
    """레이캐스팅. polygon_xy: [(x,y), ...]."""
    n = len(polygon_xy)
    if n < 3:
        return False
    inside = False
    x1, y1 = polygon_xy[-1]
    for x2, y2 in polygon_xy:
        if ((y1 > y) != (y2 > y)) and \
                (x < (x2 - x1) * (y - y1) / (y2 - y1 + 1e-12) + x1):
            inside = not inside
        x1, y1 = x2, y2
    return inside


def polygon_area(polygon_xy):
    """신발끈 공식(shoelace). 항상 양수로 반환한다."""
    n = len(polygon_xy)
    if n < 3:
        return 0.0
    area = 0.0
    for i in range(n):
        x1, y1 = polygon_xy[i]
        x2, y2 = polygon_xy[(i + 1) % n]
        area += x1 * y2 - x2 * y1
    return abs(area) / 2.0


def centroid(points_xy):
    if not points_xy:
        return (0.0, 0.0)
    xs = [p[0] for p in points_xy]
    ys = [p[1] for p in points_xy]
    return (sum(xs) / len(xs), sum(ys) / len(ys))


def ray_polygon_distance(origin_xy, direction_xy, polygon_xy, want='nearest'):
    """origin_xy 에서 direction_xy(단위벡터 아니어도 됨) 방향으로 쏜 반직선이
    polygon_xy 의 변들과 만나는 거리(t, |direction_xy| 배수 아님 — direction_xy
    가 이미 단위벡터라는 전제 하에 mm 그대로) 중 want 조건에 맞는 값을 반환한다.

    SDS §5.3 `compute_travel_limit` 의 두 용도를 이 하나의 함수로 처리한다:
      - forbidden_polygon 까지: 바깥에서 쏴 처음 닿는 거리 → want='nearest'
      - boundary_polygon 밖으로: 안에서 쏴 빠져나가는 거리 → want='nearest'
        (내부에서 볼록다각형에 쏘면 전방 교차는 하나뿐이라 nearest==farthest)
    없으면 None.
    """
    ox, oy = origin_xy
    dx, dy = direction_xy
    n = len(polygon_xy)
    if n < 3:
        return None
    hits = []
    for i in range(n):
        ax, ay = polygon_xy[i]
        bx, by = polygon_xy[(i + 1) % n]
        ex, ey = bx - ax, by - ay
        denom = ex * dy - ey * dx
        if abs(denom) < 1e-9:
            continue
        t = (ex * (ay - oy) - ey * (ax - ox)) / denom
        u = (dx * (ay - oy) - dy * (ax - ox)) / denom
        if t > 1e-6 and -1e-9 <= u <= 1 + 1e-9:
            hits.append(t)
    if not hits:
        return None
    return min(hits) if want == 'nearest' else max(hits)


def _point_segment_distance(p, a, b):
    px, py = p
    ax, ay = a
    bx, by = b
    dx, dy = bx - ax, by - ay
    if dx == 0.0 and dy == 0.0:
        return math.hypot(px - ax, py - ay)
    t = ((px - ax) * dx + (py - ay) * dy) / (dx * dx + dy * dy)
    t = max(0.0, min(1.0, t))
    cx, cy = ax + t * dx, ay + t * dy
    return math.hypot(px - cx, py - cy)


def point_to_polygon_distance(p, polygon_xy):
    n = len(polygon_xy)
    if n == 0:
        return float('inf')
    return min(_point_segment_distance(p, polygon_xy[i], polygon_xy[(i + 1) % n])
               for i in range(n))


def raster_fill(polygon_xy, pitch_mm, margin_mm=0.0):
    """polygon_xy 를 채우는 지그재그(라운모어) 경로. 한 줄씩 방향을 뒤집어
    왕복 이송 거리를 줄인다.

    margin_mm > 0: 바깥으로 그만큼 확장 (예: brushing 의 coverage_margin_mm).
    margin_mm < 0: 안쪽으로 |margin_mm| 만큼 침식 — 다각형 안쪽이면서 모든
    변에서 |margin_mm| 이상 떨어진 점만 포함한다 (예: coating 의
    boundary_offset_mm, 큐티클 번짐 방지). 진짜 폴리곤 오프셋(Minkowski)이
    아니라 점 단위 거리 필터라 각진 부분에서 살짝 보수적일 수 있다.
    """
    if len(polygon_xy) < 3:
        return []
    xs = [p[0] for p in polygon_xy]
    ys = [p[1] for p in polygon_xy]
    x0, x1 = min(xs) - margin_mm, max(xs) + margin_mm
    y0, y1 = min(ys) - margin_mm, max(ys) + margin_mm

    def included(pt):
        inside = point_in_polygon(pt[0], pt[1], polygon_xy)
        if margin_mm == 0.0:
            return inside
        if margin_mm > 0.0:
            return inside or point_to_polygon_distance(pt, polygon_xy) <= margin_mm
        return inside and point_to_polygon_distance(pt, polygon_xy) >= -margin_mm

    points = []
    y = y0
    row = 0
    while y <= y1 + 1e-6:
        row_pts = []
        x = x0
        while x <= x1 + 1e-6:
            pt = (x, y)
            if included(pt):
                row_pts.append(pt)
            x += pitch_mm
        if row % 2 == 1:
            row_pts.reverse()
        points.extend(row_pts)
        y += pitch_mm
        row += 1
    return points


def nail_boundary_polygon(size_x_mm, size_y_mm, n_points=24):
    """손톱 작업 경계 다각형. `nail_local_frame` 원점(=손톱 중심) 기준 타원을
    n_points 개로 쪼갠 볼록 다각형을 반시계 방향으로 반환한다.

    강성 스캔이 폐지된 뒤 모든 공정 노드(sanding/brushing/coating/curing)가
    작업 영역을 얻는 **유일한 경로**다. size_x_mm/size_y_mm 은 손톱의 전체
    가로/세로 길이(반지름 아님)이며 `nail_bringup/config/static_frames.yaml`
    의 `nail_region` 에 티칭해 둔 실측값이 launch 를 통해 파라미터로 주입된다.

    볼록 다각형인 이유: sanding 의 travel_limit 계산이 쓰는
    `ray_polygon_distance` 가 "안에서 쏘면 전방 교차가 하나뿐"이라는 볼록성을
    전제한다.
    """
    if size_x_mm <= 0.0 or size_y_mm <= 0.0 or n_points < 3:
        return []
    a = size_x_mm / 2.0
    b = size_y_mm / 2.0
    return [(a * math.cos(2.0 * math.pi * i / n_points),
             b * math.sin(2.0 * math.pi * i / n_points))
            for i in range(n_points)]


def oscillating_sweep(points, oscillations):
    """points(임의 튜플/객체 목록 — 2D xy, 3D xyz, geometry_msgs/Pose 등)를
    앞뒤로 oscillations 번 왕복하는 순서로 펼친다. sanding_node/curing_node
    가 공유하는 "수동 지정 waypoints를 N회 왕복"용 헬퍼 — 매 왕복이 시작점
    으로 돌아오므로, 반복 경계에서 좌표가 겹치는(이동거리 0) 항목은 한 번만
    남긴다."""
    if len(points) < 2:
        return list(points)
    backward = list(reversed(points))[1:]
    sweep = list(points)
    for i in range(oscillations):
        if i > 0:
            sweep.extend(points[1:])
        sweep.extend(backward)
    return sweep
