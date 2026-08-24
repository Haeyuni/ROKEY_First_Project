"""접촉점 구름 → 작업면 평면 추정 (mm 단위, 스캔 기준 프레임).

손톱 모형이 수평이 아니라 비스듬하면 두 가지가 동시에 깨진다.

1. **접근 높이가 점마다 달라진다.** 고정된 z 평면에서 하강을 시작하면
   기울어진 면의 한쪽 끝은 한참 내려가야 닿고 반대쪽 끝은 시작부터 눌려
   있다. `max_depth_mm` 예산을 그 편차가 통째로 먹어버려, 정작 압입에
   쓸 여유가 남지 않는다.
2. **접촉 높이를 그대로 쓰면 경사가 재질처럼 보인다.** contact_z 는
   손톱/피부를 가르는 가장 좋은 기하 특징인데, 15° 기울어진 13mm 영역이면
   경사만으로 3.5mm 의 높이차가 생긴다 — 손톱과 피부의 실제 단차(0.1~0.5mm)
   보다 열 배 크다. 경사 성분을 빼지 않으면 높이 특징이 통째로 무용지물이
   된다.

그래서 스캔 시작 전에 서너 점만 찍어 평면을 구해두고, 이후 모든 점의
목표 z 를 그 평면 위로 얹는다. 접근 높이가 일정해지므로 `max_depth_mm`
을 바짝 조일 수 있고(= 미끄러질 기회도 줄고), 남은 잔차가 곧 재질의
높이 단차가 된다.
"""
import math

import numpy as np

_EPS = 1e-9


def fit_plane(points_xyz):
    """z = a·x + b·y + c 최소자승 적합.

    반환: (a, b, c, rms_residual_mm, normal_unit_xyz)
    """
    pts = np.asarray(points_xyz, dtype=float)
    if pts.ndim != 2 or pts.shape[0] < 3:
        c = float(pts[:, 2].mean()) if pts.size else 0.0
        return 0.0, 0.0, c, 0.0, (0.0, 0.0, 1.0)
    a_mat = np.column_stack([pts[:, 0], pts[:, 1], np.ones(pts.shape[0])])
    coef, *_ = np.linalg.lstsq(a_mat, pts[:, 2], rcond=None)
    a, b, c = (float(v) for v in coef)
    resid = pts[:, 2] - a_mat @ coef
    rms = float(np.sqrt((resid ** 2).mean()))
    nv = np.array([-a, -b, 1.0])
    nv /= max(float(np.linalg.norm(nv)), _EPS)
    return a, b, c, rms, (float(nv[0]), float(nv[1]), float(nv[2]))


def fit_plane_robust(points_xyz, trim_ratio=0.25, passes=2):
    """잔차 상위 trim_ratio 를 잘라내고 다시 적합한다.

    스캔 점 구름에는 손톱(솟은 쪽)과 피부(꺼진 쪽)가 섞여 있어 그대로
    적합하면 평면이 손톱 쪽으로 들린다. 재질 단차를 잔차로 남기려면
    다수파(보통 피부·작업대)에 평면을 맞춰야 한다.
    """
    pts = np.asarray(points_xyz, dtype=float)
    if pts.shape[0] < 4:
        return fit_plane(pts)
    a, b, c, rms, nv = fit_plane(pts)
    for _ in range(passes):
        resid = np.abs(pts[:, 2] - (a * pts[:, 0] + b * pts[:, 1] + c))
        keep_n = max(3, int(round(pts.shape[0] * (1.0 - trim_ratio))))
        keep = np.argsort(resid)[:keep_n]
        if keep.size < 3 or keep.size == pts.shape[0]:
            break
        a, b, c, rms, nv = fit_plane(pts[keep])
    return a, b, c, rms, nv


def plane_z(plane, x_mm, y_mm):
    return plane[0] * x_mm + plane[1] * y_mm + plane[2]


def plane_tilt_deg(plane):
    return math.degrees(math.atan(math.hypot(plane[0], plane[1])))


def residual_heights(plane, points_xyz):
    """각 점의 평면 대비 높이 [mm]. 양수면 평면보다 솟아 있다."""
    pts = np.asarray(points_xyz, dtype=float)
    if pts.size == 0:
        return np.zeros(0)
    return pts[:, 2] - (plane[0] * pts[:, 0] + plane[1] * pts[:, 1] + plane[2])


def normal_to_zyz_deg(normal_xyz, yaw_ref_deg=0.0):
    """단위 법선 → 두산 ZYZ 오일러 (rz1, ry, rz2) [deg].

    툴 -Z 가 표면 안쪽(-normal)을 향하도록, 즉 툴 +Z 가 normal 과 같은
    방향이 되도록 만든다. rz2 는 툴 자전이라 자유도가 남으므로 호출자가
    준 yaw_ref_deg 를 그대로 넘긴다(기존 자세의 자전을 유지하는 용도).

    ⚠️ 이 자세로 실제로 움직이려면 **TCP 가 프로브 팁에 맞춰져 있어야
    한다.** TCP 오프셋이 0(=플랜지)인 상태에서 자세만 기울이면 팁이 툴
    길이만큼 옆으로 휩쓸린다. robot_skill_node 는 그래서 이 경로를
    기본적으로 끄고, 파라미터로 명시 허용했을 때만 쓴다.
    """
    nx, ny, nz = normal_xyz
    n = math.sqrt(nx * nx + ny * ny + nz * nz)
    if n < _EPS:
        return 0.0, 0.0, yaw_ref_deg
    nx, ny, nz = nx / n, ny / n, nz / n
    ry = math.degrees(math.acos(max(-1.0, min(1.0, nz))))
    rz1 = math.degrees(math.atan2(ny, nx))
    return rz1, ry, yaw_ref_deg


def fit_quadric(points_xyz, trim_ratio=0.25, passes=2):
    """z = a·x² + b·xy + c·y² + d·x + e·y + f 로버스트 적합.

    반환: (coef6, rms_residual_mm)

    손가락은 평면이 아니라 원통에 가깝고 손톱판도 볼록하다. 평면만 빼면
    그 곡률이 잔차에 그대로 남아 "가운데가 솟았다"가 되는데, 그건 재질이
    아니라 손가락 모양이다. 재질이 같은 모형에서는 높이가 유일한 판별 축이
    되므로, 그 축을 오염시키는 곡률은 반드시 먼저 빼야 한다.
    """
    pts = np.asarray(points_xyz, dtype=float)
    if pts.shape[0] < 8:
        a, b, c, rms, _ = fit_plane_robust(pts)
        return np.array([0.0, 0.0, 0.0, a, b, c]), rms

    def design(p):
        x, y = p[:, 0], p[:, 1]
        return np.column_stack([x * x, x * y, y * y, x, y, np.ones(p.shape[0])])

    work = pts
    coef = None
    for _ in range(passes + 1):
        a_mat = design(work)
        coef, *_ = np.linalg.lstsq(a_mat, work[:, 2], rcond=None)
        resid = np.abs(pts[:, 2] - design(pts) @ coef)
        keep_n = max(8, int(round(pts.shape[0] * (1.0 - trim_ratio))))
        if keep_n >= pts.shape[0]:
            break
        work = pts[np.argsort(resid)[:keep_n]]
    resid = pts[:, 2] - design(pts) @ coef
    return coef, float(np.sqrt((resid ** 2).mean()))


def quadric_z(coef, x_mm, y_mm):
    a, b, c, d, e, f = coef
    return a * x_mm * x_mm + b * x_mm * y_mm + c * y_mm * y_mm + d * x_mm + e * y_mm + f


def residuals_quadric(coef, points_xyz):
    pts = np.asarray(points_xyz, dtype=float)
    if pts.size == 0:
        return np.zeros(0)
    return pts[:, 2] - quadric_z(coef, pts[:, 0], pts[:, 1])


def local_slope(xy, z, radius_mm):
    """각 점 주변에 국소 평면을 맞춰 |∇z| [mm/mm] 를 낸다.

    재질이 같은 모형에서 손톱 경계는 **높이가 급히 꺾이는 능선**으로 나타난다.
    높이 자체보다 그 기울기가 경계 위치를 훨씬 날카롭게 짚어주므로, 정밀
    스캔 대상을 고를 때 이 값이 큰 쪽을 우선한다.
    """
    pts = np.asarray(xy, dtype=float)
    zz = np.asarray(z, dtype=float)
    n = pts.shape[0]
    out = np.zeros(n)
    if n < 4:
        return out
    d2 = ((pts[:, None, :] - pts[None, :, :]) ** 2).sum(axis=2)
    for i in range(n):
        idx = np.flatnonzero(d2[i] <= radius_mm ** 2)
        if idx.size < 4:
            continue
        a_mat = np.column_stack([pts[idx, 0], pts[idx, 1], np.ones(idx.size)])
        try:
            coef, *_ = np.linalg.lstsq(a_mat, zz[idx], rcond=None)
        except np.linalg.LinAlgError:
            continue
        out[i] = math.hypot(float(coef[0]), float(coef[1]))
    return out


class SurfaceModel:
    """작업면 형상 모델. 평면 또는 이차곡면을 같은 인터페이스로 감싼다.

    같은 코드가 두 곳에서 필요하다.
      * **접근 높이 보정** — 하강 거리 지도에 맞춰, 어느 점에서든 같은 거리만
        내려가 접촉하게 만든다.
      * **재질 단차 추출** — 접촉 높이에서 작업면 성분을 빼고 잔차만 남긴다.
    둘 다 "손가락은 평면이 아니라 원통"이라는 사실에 걸린다. 평면만 빼면
    그 곡률이 통째로 남아, 보정 쪽에서는 하강 예산을 잡아먹고 판별 쪽에서는
    재질 단차인 척한다.
    """

    def __init__(self, kind, coef, rms):
        self.kind = kind
        self.coef = coef
        self.rms = float(rms)

    def z(self, x_mm, y_mm):
        if self.kind == 'quadric':
            return quadric_z(self.coef, x_mm, y_mm)
        return plane_z(self.coef, x_mm, y_mm)

    def residuals(self, points_xyz):
        if self.kind == 'quadric':
            return residuals_quadric(self.coef, points_xyz)
        return residual_heights(self.coef, points_xyz)

    @property
    def tilt_deg(self):
        if self.kind == 'quadric':
            return math.degrees(math.atan(math.hypot(self.coef[3], self.coef[4])))
        return plane_tilt_deg(self.coef)


def fit_surface(points_xyz, mode='auto', min_quadric_points=8, gain=0.8,
                bandwidth_mm=None, min_local_points=16):
    """작업면 적합. mode: 'plane' | 'quadric' | 'local' | 'auto'.

    'auto' 는 표본이 충분하면 국소 회귀를 쓰고, 아니면 이차곡면이 **뚜렷하게**
    더 맞을 때만 갈아탄다 — 자유도가 많으면 항상 조금은 더 잘 맞으므로,
    그 정도로는 바꾸지 않는다.
    """
    pts = np.asarray(points_xyz, dtype=float)
    if mode == 'local' or (mode == 'auto' and pts.shape[0] >= min_local_points):
        h = bandwidth_mm
        if h is None:
            span = max(float(np.ptp(pts[:, 0])), float(np.ptp(pts[:, 1])), 1.0)
            h = span / 3.0
        return LocalSurface(pts[:, :2], pts[:, 2], h)

    a, b, c, rms, _nv = fit_plane_robust(pts)
    plane = SurfaceModel('plane', (a, b, c), rms)
    if mode == 'plane' or pts.shape[0] < min_quadric_points:
        return plane
    coef, q_rms = fit_quadric(pts)
    quad = SurfaceModel('quadric', coef, q_rms)
    if mode == 'quadric':
        return quad
    return quad if q_rms < rms * gain else plane


def local_background(query_xy, background_xy, background_z, bandwidth_mm,
                     degree=2, min_effective=6.0):
    """국소 가중 회귀(LOESS)로 배경 높이를 추정한다.

    전역 다항식으로는 손가락 곡면을 못 따라간다. 반경 9mm 원통을 ±7mm 구간에
    이차곡면으로 맞추면 끝단에서 0.6mm 가 어긋나는데, 이는 손톱 단차(0.3mm)
    보다 크다 — 그 어긋남이 이상점이 되고, Otsu 가 손톱 대신 그 이상점을
    잘라낸다(시뮬레이션에서 110점 중 105점이 "손톱"으로 분류됐다).

    국소 회귀는 질의점 근처의 배경 표본에만 가중치를 줘 임의의 매끄러운
    형상을 따라간다. bandwidth 를 아주 크게 잡으면 전역 적합과 같아지므로,
    전역 방식을 일반화한 것이지 다른 방식이 아니다.

    background_* 는 "손톱이 아님이 확실한" 점들이어야 한다 — 손톱을 포함해
    맞추면 배경이 손톱을 따라 들려 단차가 그만큼 깎인다.
    """
    q = np.asarray(query_xy, dtype=float)
    b = np.asarray(background_xy, dtype=float)
    bz = np.asarray(background_z, dtype=float)
    if q.size == 0:
        return np.zeros(0)
    if b.shape[0] < 4:
        return np.full(q.shape[0], float(bz.mean()) if bz.size else 0.0)

    h = max(float(bandwidth_mm), 1e-3)
    out = np.empty(q.shape[0])
    for i in range(q.shape[0]):
        dx = b[:, 0] - q[i, 0]
        dy = b[:, 1] - q[i, 1]
        w = np.exp(-(dx * dx + dy * dy) / (2.0 * h * h))
        # 유효 표본 수가 부족하면 차수를 낮춰 과적합을 막는다.
        n_eff = float(w.sum())
        deg = degree if n_eff >= min_effective and b.shape[0] >= 8 else 1
        if deg >= 2:
            a_mat = np.column_stack([dx * dx, dx * dy, dy * dy, dx, dy, np.ones(b.shape[0])])
        else:
            a_mat = np.column_stack([dx, dy, np.ones(b.shape[0])])
        sw = np.sqrt(w)[:, None]
        try:
            coef, *_ = np.linalg.lstsq(a_mat * sw, bz * sw[:, 0], rcond=None)
        except np.linalg.LinAlgError:
            out[i] = float(np.average(bz, weights=w))
            continue
        out[i] = float(coef[-1])   # dx=dy=0 에서의 값 = 상수항
    return out


class LocalSurface:
    """`SurfaceModel` 과 같은 인터페이스의 국소 회귀 배경."""

    kind = 'local'

    def __init__(self, background_xy, background_z, bandwidth_mm, degree=2):
        self._bxy = np.asarray(background_xy, dtype=float)
        self._bz = np.asarray(background_z, dtype=float)
        self._h = float(bandwidth_mm)
        self._degree = degree
        fitted = local_background(self._bxy, self._bxy, self._bz, self._h, degree)
        resid = self._bz - fitted
        self.rms = float(np.sqrt((resid ** 2).mean())) if resid.size else 0.0
        a, b, _c, _rms, _nv = fit_plane_robust(
            np.column_stack([self._bxy, self._bz])) if self._bxy.shape[0] >= 3 \
            else (0.0, 0.0, 0.0, 0.0, None)
        self._tilt = math.degrees(math.atan(math.hypot(a, b)))

    def z(self, x_mm, y_mm):
        return float(local_background([(x_mm, y_mm)], self._bxy, self._bz,
                                      self._h, self._degree)[0])

    def residuals(self, points_xyz):
        pts = np.asarray(points_xyz, dtype=float)
        if pts.size == 0:
            return np.zeros(0)
        return pts[:, 2] - local_background(pts[:, :2], self._bxy, self._bz,
                                            self._h, self._degree)

    @property
    def tilt_deg(self):
        return self._tilt
