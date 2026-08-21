"""2D 격자·다각형 유틸리티. 단위는 전부 mm, 평면은 스캔 기준 프레임의 XY."""
import math

import numpy as np


def make_grid(area_x_mm, area_y_mm, margin_mm, pitch_mm):
    """중심이 원점인 직사각 격자. {(i, j): (x_mm, y_mm)} 로 반환한다.

    (i, j) 인덱스를 유지하는 이유: 4-이웃 탐색(경계 후보 선정, NIS §6.1 6단계)이
    좌표 근접도 검색 없이 인덱스 ±1 비교만으로 끝나기 때문이다.
    """
    half_x = area_x_mm / 2.0 + margin_mm
    half_y = area_y_mm / 2.0 + margin_mm
    xs = np.arange(-half_x, half_x + 1e-6, pitch_mm)
    ys = np.arange(-half_y, half_y + 1e-6, pitch_mm)
    grid = {}
    for i, x in enumerate(xs):
        for j, y in enumerate(ys):
            grid[(i, j)] = (float(x), float(y))
    return grid


def adjacent_pairs_4(grid):
    """격자의 4-이웃 쌍 (중복 없이) 이터레이터. (i,j) 인덱스 쌍을 낸다."""
    keys = set(grid.keys())
    seen = set()
    for (i, j) in keys:
        for di, dj in ((1, 0), (0, 1)):
            other = (i + di, j + dj)
            if other in keys:
                pair = ((i, j), other)
                if pair not in seen:
                    seen.add(pair)
                    yield pair


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


def convex_hull(points_xy):
    """Andrew's monotone chain. points_xy: [(x,y), ...] -> 반시계 방향 외곽선."""
    pts = sorted(set(points_xy))
    if len(pts) <= 2:
        return pts

    def cross(o, a, b):
        return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])

    lower = []
    for p in pts:
        while len(lower) >= 2 and cross(lower[-2], lower[-1], p) <= 0:
            lower.pop()
        lower.append(p)
    upper = []
    for p in reversed(pts):
        while len(upper) >= 2 and cross(upper[-2], upper[-1], p) <= 0:
            upper.pop()
        upper.append(p)
    return lower[:-1] + upper[:-1]


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


def pca_major_axis_deg(points_xy):
    """점군의 1주성분 방향(도, X축 기준 반시계). 손톱 "길이축" 추정에 쓴다."""
    if len(points_xy) < 2:
        return 0.0
    pts = np.array(points_xy)
    pts = pts - pts.mean(axis=0)
    cov = np.cov(pts.T)
    if cov.shape != (2, 2):
        return 0.0
    eigvals, eigvecs = np.linalg.eigh(cov)
    major = eigvecs[:, np.argmax(eigvals)]
    return math.degrees(math.atan2(major[1], major[0]))
