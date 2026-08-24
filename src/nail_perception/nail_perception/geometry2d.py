"""2D 다각형 유틸리티. 단위는 전부 mm, 평면은 `nail_local_frame` 의 XY.

강성 스캔(scan_node)이 폐지되면서 격자 생성·군집 외곽선(convex_hull)·주성분
축 추정 같은 "측정 결과로부터 경계를 만드는" 함수들은 함께 제거됐다. 지금
경계는 측정이 아니라 **설정값**에서 온다 — `nail_boundary_polygon()` 이
그 유일한 출처다.
"""
import math


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
