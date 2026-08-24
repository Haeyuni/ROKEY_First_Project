"""ProbePoint 파형 → 손톱/피부 판별 특징 추출 (순수 함수, ROS 비의존).

압입 강성 하나로는 손톱과 피부가 갈리지 않는다 — 실기에서 확인된 사실이고,
이유가 둘 있다.

1. **강성은 포화한다.** 위치제어 로봇으로 단단한 면을 누르면 측정되는
   기울기 ΔF/Δz 는 손톱의 강성이 아니라 로봇·툴·F/T 센서 자신의
   컴플라이언스다. 손톱이 아무리 단단해도 기울기는 그 상한에서 멈추므로,
   눌려서 굳은 피부 구간과 값이 겹친다.
2. **명령 하강량은 압입량이 아니다.** 고정 문턱값(예: 0.3N)으로 접촉을
   판정하면 문턱을 넘기까지 하강한 만큼이 통째로 압입량에 섞여 들어간다.
   센서 바이어스가 크면(실측 Fz ≈ -12N) 그 오차가 압입량보다 커진다.

그래서 여기서는 크기가 아니라 **모양**을 본다. 아래 특징들은 전부
무차원이거나 자기 자신으로 정규화돼 있어 바이어스와 로봇 강성에 둔감하다.

  relaxation_ratio   유지(dwell) 중 힘 감소율. 피부는 점탄성이라 크고
                     손톱은 거의 0 이다. **가장 잘 갈리는 축이고, 위치를
                     고정한 채 재므로 미끄러짐의 영향도 가장 적다.**
  hysteresis_ratio   적재/제하 면적비. 피부는 에너지를 먹고 손톱은 되돌린다.
  power_exponent     f ∝ d^p 의 p. 피부(변형경화) 2~3, 손톱 1 근처.
  linearity_r2       선형 적합도. 손톱 높고 피부 낮다.
  contact_z_mm       접촉 시작 높이. 강성과 **독립인** 기하학적 증거라
                     힘 특징이 전부 애매해도 이 축 하나로 갈리는 경우가 있다.

접촉 시점은 고정 문턱값이 아니라 **꺾임점 탐색**(bilinear change-point)으로
잡는다. "접촉 전 평평 + 접촉 후 상승" 두 구간으로 나눴을 때 잔차가 최소가
되는 지점이고, 두 직선의 교점을 취하므로 샘플 간격보다 촘촘한 해상도가
나온다. 센서 노이즈 수준이 바뀌어도 문턱값을 다시 튜닝할 필요가 없다.

경사면 대응은 두 갈래다.
  * 접촉력 **벡터**의 방향으로 국소 표면 기울기를 추정한다 — 가벼운 접촉에서
    반력은 대체로 표면 법선을 향한다. (마찰이 있으면 마찰원뿔 안쪽으로
    들어와 기울기를 과소평가한다. 하한으로만 쓸 것.)
  * 접선/법선 힘 비가 마찰계수를 넘거나 스틱-슬립 급락이 보이면 그 점의
    힘 특징을 **버린다** — 미끄러진 파형으로 계산한 강성은 의미가 없다.
    (contact_z_mm 은 접촉 직후 값이라 살려 쓴다.)
"""
import math
from dataclasses import dataclass

import numpy as np

_EPS = 1e-9

# numpy 2.0 에서 trapz → trapezoid 로 이름이 바뀌었다. ROS Humble 이 얹어주는
# numpy 는 1.x 라 여기서 한 번 흡수한다.
_trapz = getattr(np, 'trapezoid', None) or np.trapz


@dataclass
class ProbeSample:
    """한 시점의 관측. 힘은 접근 자세에서 뜬 tare 를 뺀 상대값(tool frame)."""
    t_s: float
    travel_mm: float      # 접근 시작점 기준 이동 거리 (하강 방향이 +)
    z_mm: float           # base 프레임 절대 높이
    fx_n: float
    fy_n: float
    fz_n: float


@dataclass
class ProbeFeatures:
    """한 점의 측정 결과.

    `valid` 와 `stiffness_valid` 를 일부러 나눠 둔다. 단단한 손톱은 힘 상한
    3N 까지 눌러도 압입이 0.1mm 도 안 돼서 **회귀할 구간 자체가 없다** —
    강성이 안 나오는 게 고장이 아니라 그게 손톱이라는 뜻이다. 여기서 점을
    통째로 버리면 정작 손톱만 전부 사라지고 피부만 남는다.

    그래서 `valid` 는 "접촉을 찾았고 미끄러지지 않았다"(= 기하 특징을 믿을
    수 있다)만 뜻하고, 힘-곡선 특징을 믿어도 되는지는 `stiffness_valid` 가
    따로 말한다. 분류기는 두 그룹을 나눠서 쓴다.
    """
    valid: bool = False                # 기하 특징(contact_z) 신뢰 가능
    stiffness_valid: bool = False      # 강성값(회귀 또는 계단 압입) 신뢰 가능
    curve_valid: bool = False          # 곡선 모양 특징(r2, p) 신뢰 가능
    hold_valid: bool = False           # 유지·제하 특징(완화, 이력) 신뢰 가능
    reject_reason: str = ''

    # 기하
    contact_travel_mm: float = 0.0     # touchdown 시점의 travel
    contact_z_mm: float = 0.0          # touchdown 시점의 절대 높이 (경계 판별용)
    indentation_mm: float = 0.0        # touchdown 이후 실제 압입량

    # 힘
    peak_force_n: float = 0.0
    baseline_n: float = 0.0
    noise_n: float = 0.0
    axial_sign: float = 1.0            # 하강 시 Fz 가 증가/감소하는 방향

    # 판별 특징
    stiffness_n_per_mm: float = 0.0        # 하강 구간 회귀 기울기
    incremental_stiffness_n_per_mm: float = 0.0  # 계단 압입 ΔF/Δd. 회귀보다 강건
    linearity_r2: float = 0.0
    power_exponent: float = 0.0
    relaxation_ratio: float = 0.0
    hysteresis_ratio: float = 0.0
    adhesion_force_n: float = 0.0      # 이탈 중 최저 축력. 음수면 점착(미경화)

    # 경사·미끄러짐
    surface_tilt_deg: float = 0.0
    lateral_force_n: float = 0.0
    slip_ratio: float = 0.0            # |F_tangential| / |F_normal|
    slip_events: int = 0

    n_load_samples: int = 0
    n_contact_samples: int = 0


# --- 저수준 적합 헬퍼 -----------------------------------------------------------
def _lin(x, y):
    """최소자승 1차 적합. (slope, intercept, r2). polyfit 대비 경고/오버헤드 없음."""
    n = x.size
    if n < 2:
        return 0.0, float(y.mean()) if n else 0.0, 0.0
    mx, my = x.mean(), y.mean()
    sxx = float(((x - mx) ** 2).sum())
    if sxx < _EPS:
        return 0.0, float(my), 0.0
    sxy = float(((x - mx) * (y - my)).sum())
    slope = sxy / sxx
    intercept = float(my - slope * mx)
    syy = float(((y - my) ** 2).sum())
    resid = y - (slope * x + intercept)
    sse = float((resid ** 2).sum())
    r2 = 1.0 - sse / syy if syy > _EPS else 0.0
    return float(slope), intercept, float(r2)


def robust_line(x, y, trim_sigma=2.5, passes=2):
    """잔차가 큰 표본을 잘라내고 다시 적합한다.

    압입 파형에는 접촉 순간의 오버슈트와 스틱-슬립 스파이크가 섞이는데,
    그냥 최소자승으로 밀면 그 몇 점이 기울기를 통째로 끌고 간다.
    """
    x = np.asarray(x, float)
    y = np.asarray(y, float)
    slope, intercept, r2 = _lin(x, y)
    for _ in range(passes):
        if x.size < 6:
            break
        resid = y - (slope * x + intercept)
        sigma = float(resid.std())
        if sigma < _EPS:
            break
        keep = np.abs(resid) <= trim_sigma * sigma
        if keep.sum() < max(4, int(0.6 * x.size)):
            break
        x, y = x[keep], y[keep]
        slope, intercept, r2 = _lin(x, y)
    return slope, intercept, r2, x.size


def axial_sign(fz_series, head_fraction=0.25, n_extreme=5):
    """하강 중 Fz 가 어느 쪽으로 자라는지 파형 자체에서 판정한다.

    실측에서 Fz 는 무접촉 -12N → 접촉 -9N 으로 **커지는** 쪽이었지만, TCP
    설정이나 센서 장착 방향이 바뀌면 부호가 뒤집힌다. 부호를 파라미터로
    박아두면 그때마다 조용히 오작동하므로 매 점 새로 뽑는다.

    "끝부분의 중앙값"은 쓰면 안 된다 — 단단한 손톱은 힘 상한에 순식간에
    도달해 접촉 표본이 전체의 10% 도 안 되므로, 끝 25% 조차 대부분 무접촉
    노이즈다. 중앙값 부호가 노이즈에 따라 뒤집히고, 그러면 그 점은 통째로
    NO_CONTACT 이 된다(재현됨). 대신 접촉 전 기준선에서 **가장 크게 벗어난
    표본 몇 개**의 부호를 쓴다 — 그 표본들이 곧 접촉 구간이다.
    """
    f = np.asarray(fz_series, float)
    if f.size == 0:
        return 1.0
    k = max(1, int(f.size * head_fraction))
    base = float(np.median(f[:k]))
    dev = f - base
    n = min(max(1, n_extreme), dev.size)
    extreme = dev[np.argsort(np.abs(dev))[-n:]]
    return -1.0 if float(extreme.mean()) < 0.0 else 1.0


# --- touchdown 탐색 ---------------------------------------------------------------
def detect_touchdown(travel, force, noise_n=0.0, min_rise_n=0.0,
                     min_pre=3, min_post=4):
    """꺾임점 탐색으로 접촉 시작 지점을 찾는다.

    travel: 단조 증가하는 하강 거리 [mm]
    force : 축 방향 힘 [N]. 접촉 전 ≈ 0, 접촉 후 증가.
    반환  : (index, contact_travel_mm, baseline_n, slope_n_per_mm) 또는
            접촉이 없으면 (None, 0.0, 0.0, 0.0)

    index 는 "접촉 후" 구간의 첫 표본이고, contact_travel_mm 은 두 직선의
    교점이라 표본 간격보다 정밀하다.
    """
    d = np.asarray(travel, float)
    f = np.asarray(force, float)
    n = d.size
    if n < min_pre + min_post:
        return None, 0.0, 0.0, 0.0

    rise_gate = max(min_rise_n, 4.0 * noise_n, _EPS)
    baseline_guess = float(np.median(f[:min_pre]))
    if float(f.max()) - baseline_guess < rise_gate:
        return None, 0.0, 0.0, 0.0

    best_sse, best = np.inf, None
    for k in range(min_pre, n - min_post + 1):
        pre = f[:k]
        c = float(pre.mean())
        sse = float(((pre - c) ** 2).sum())
        slope, intercept, _ = _lin(d[k:], f[k:])
        if slope <= _EPS:
            continue
        resid = f[k:] - (slope * d[k:] + intercept)
        sse += float((resid ** 2).sum())
        if sse < best_sse:
            best_sse, best = sse, (k, c, slope, intercept)

    if best is None:
        # 꺾임점 적합이 전부 실패(예: 처음부터 눌린 상태) — 문턱값으로 대체
        idx = int(np.argmax(f - baseline_guess >= rise_gate))
        if f[idx] - baseline_guess < rise_gate:
            return None, 0.0, 0.0, 0.0
        return idx, float(d[idx]), baseline_guess, 0.0

    k, c, slope, intercept = best
    d0 = (c - intercept) / slope
    # 교점은 "접촉 직전 평평 구간"과 "상승 구간" 사이에 있어야 한다. 밖으로
    # 나가면 적합이 신뢰할 수 없다는 뜻이므로 구간 경계로 자른다.
    d0 = float(min(max(d0, float(d[0])), float(d[k])))
    return k, d0, c, float(slope)


# --- 개별 특징 --------------------------------------------------------------------
def power_exponent(depth_mm, force_n, force_floor_ratio=0.2):
    """f ∝ d^p 의 p. 무차원이라 센서 스케일·로봇 강성에 둔감하다.

    Hertz 접촉(구형 팁 vs 탄성 반무한체)은 p = 1.5, 변형경화하는 연조직은
    2~3, 압입이 거의 없어 로봇 컴플라이언스만 보이는 단단한 면은 1 근처다.
    """
    d = np.asarray(depth_mm, float)
    f = np.asarray(force_n, float)
    if d.size < 4:
        return 0.0
    peak = float(f.max())
    if peak <= _EPS:
        return 0.0
    keep = (d > 1e-3) & (f > force_floor_ratio * peak)
    if keep.sum() < 4:
        return 0.0
    slope, _, _ = _lin(np.log(d[keep]), np.log(f[keep]))
    return float(slope)


def relaxation_ratio(dwell_force, edge_fraction=0.25):
    """유지 구간의 힘 감소율 (f0 - f1) / f0.

    위치를 고정한 채 재므로 압입량 오차·경사면 미끄러짐의 영향이 가장 적다.
    피부(점탄성)는 0.15~0.4, 손톱은 0.05 아래로 나온다.
    """
    f = np.asarray(dwell_force, float)
    if f.size < 4:
        return 0.0
    k = max(1, int(f.size * edge_fraction))
    f0 = float(f[:k].mean())
    f1 = float(f[-k:].mean())
    if f0 <= _EPS:
        return 0.0
    return float(max(0.0, min(1.0, (f0 - f1) / f0)))


def hysteresis_ratio(load_depth, load_force, unload_depth, unload_force):
    """1 - (제하 면적 / 적재 면적). 두 곡선이 겹치는 깊이 구간에서만 잰다.

    부분 제하(unload_fraction < 1)를 쓰므로 적재 곡선 전체와 비교하면 안 된다.
    """
    ld = np.asarray(load_depth, float)
    lf = np.asarray(load_force, float)
    ud = np.asarray(unload_depth, float)
    uf = np.asarray(unload_force, float)
    if ld.size < 3 or ud.size < 3:
        return 0.0

    lo = max(float(ud.min()), float(ld.min()))
    hi = min(float(ud.max()), float(ld.max()))
    if hi - lo < 1e-3:
        return 0.0

    order = np.argsort(ud)
    ud, uf = ud[order], uf[order]
    lmask = (ld >= lo) & (ld <= hi)
    umask = (ud >= lo) & (ud <= hi)
    if lmask.sum() < 3 or umask.sum() < 3:
        return 0.0

    a_load = float(_trapz(lf[lmask], ld[lmask]))
    a_unload = float(_trapz(uf[umask], ud[umask]))
    if a_load <= _EPS:
        return 0.0
    return float(max(0.0, min(1.0, (a_load - a_unload) / a_load)))


def _median_filter(v, window=5):
    """홀수 창 이동 중앙값. 한 표본짜리 스파이크를 지운다."""
    v = np.asarray(v, float)
    if v.size < 3 or window < 3:
        return v
    w = min(window if window % 2 else window + 1, v.size if v.size % 2 else v.size - 1)
    if w < 3:
        return v
    half = w // 2
    padded = np.pad(v, half, mode='edge')
    return np.array([np.median(padded[i:i + w]) for i in range(v.size)])


def slip_metrics(axial, lateral, peak_n, drop_n=0.15, noise_n=0.0,
                 active_ratio=0.3, min_persist=2):
    """(slip_ratio, slip_events, tilt_deg, lateral_n).

    slip_ratio 는 접선/법선 힘 비다. 경사면에 붙어만 있어도 tan θ 만큼은
    나오므로 이 값 자체가 곧 미끄러짐은 아니다 — 마찰계수를 넘으면 그렇다.
    실제로 미끄러진 순간은 접선력이 **급락**하는 스틱-슬립 패턴으로 잡는다.

    "인접 두 표본의 차이"로 급락을 세면 안 된다. 표본 하나가 노이즈로 튄 것도
    급락으로 세므로, 표본이 많은 점(= 부드러워서 깊이 눌린 점)일수록 오탐이
    쌓인다 — 시뮬레이션에서 정상 파형의 19% 가 미끄러진 것으로 잡혔다.
    대신 (1) 이동 중앙값으로 스파이크를 지우고, (2) 지금까지의 최대치 대비
    떨어진 정도를 보고, (3) 그 상태가 몇 표본 이상 **유지**될 때만 1회로 센다.
    문턱값도 노이즈 수준에 따라 자동으로 올라간다.
    """
    a = np.asarray(axial, float)
    lat = np.asarray(lateral, float)
    if a.size == 0:
        return 0.0, 0, 0.0, 0.0
    active = a > max(active_ratio * peak_n, _EPS)
    if active.sum() < 2:
        return 0.0, 0, 0.0, float(lat.max() if lat.size else 0.0)

    aa, ll = a[active], lat[active]
    ratio = float(np.median(ll / np.maximum(aa, _EPS)))
    tilt = math.degrees(math.atan2(float(np.median(ll)), max(float(np.median(aa)), _EPS)))

    smooth = _median_filter(ll)
    gate = max(abs(drop_n), 4.0 * noise_n)
    running_max = np.maximum.accumulate(smooth)
    below = smooth < (running_max - gate)

    events, run = 0, 0
    for flag in below:
        if flag:
            run += 1
            if run == min_persist:
                events += 1
        else:
            run = 0
    return ratio, int(events), float(tilt), float(np.median(ll))


def estimate_surface_normal(fx, fy, fz, sign=1.0):
    """접촉력 벡터에서 국소 표면 법선(tool frame 단위벡터)을 추정한다.

    가벼운 접촉·구형 팁이면 반력은 대체로 법선을 향한다. 마찰이 있으면
    마찰원뿔 안으로 들어와 기울기를 **과소평가**하므로 하한으로만 쓴다.
    """
    v = np.array([float(fx), float(fy), float(fz)], dtype=float) * float(sign)
    n = float(np.linalg.norm(v))
    if n < _EPS:
        return (0.0, 0.0, 1.0)
    v /= n
    return (float(v[0]), float(v[1]), float(v[2]))


# --- 전체 파형 → 특징 --------------------------------------------------------------
def _tail_mean(samples, attr, fraction=0.25):
    if not samples:
        return None
    k = max(1, int(len(samples) * fraction))
    return float(np.mean([getattr(x, attr) for x in samples[-k:]]))


def compute_features(load, dwell=(), dither=(), unload=(), *,
                     noise_n=0.0,
                     min_rise_n=0.0,
                     min_stiffness_samples=6,
                     stiffness_window=(0.25, 1.0),
                     slip_ratio_limit=0.45,
                     slip_drop_n=0.15,
                     sign=None):
    """ProbeSample 시퀀스 → ProbeFeatures.

    load   : 하강 구간 (필수)
    dwell  : 접촉 힘에 도달한 뒤 위치를 고정한 채 유지한 구간 (선택)
    dither : dwell 뒤에 아주 조금(0.05mm 정도) 더 눌러 다시 유지한 구간 (선택).
             여기서 나오는 ΔF/Δd 가 **계단 압입 강성**이다. 하강 곡선 회귀와
             달리 압입 구간이 짧아도 되고, touchdown 추정 오차가 그대로
             빠지며, 이동 거리가 짧아 경사면에서 미끄러질 틈도 적다.
             단단한 표면에서 회귀가 실패해도 이 값은 살아 있다.
    unload : 부분 제하 구간 (선택)

    stiffness_window: 최대 힘 대비 어느 구간으로 회귀 기울기를 낼지. 접촉
             직후 저하중 구간은 팁이 자리를 잡는 중이라 왜곡되므로 기본값은
             상위 25~100% 만 쓴다.
    """
    feat = ProbeFeatures()
    # 센서가 한 표본 걸렀거나 자세 조회가 튀면 NaN 이 섞여 들어온다. 여기서
    # 걸러내지 않으면 이후 회귀·적분이 조용히 NaN 을 뱉는다.
    load = [s for s in load
            if math.isfinite(s.travel_mm) and math.isfinite(s.z_mm)
            and math.isfinite(s.fx_n) and math.isfinite(s.fy_n) and math.isfinite(s.fz_n)]
    dwell = [s for s in dwell if math.isfinite(s.fz_n) and math.isfinite(s.travel_mm)]
    dither = [s for s in dither if math.isfinite(s.fz_n) and math.isfinite(s.travel_mm)]
    unload = [s for s in unload if math.isfinite(s.fz_n) and math.isfinite(s.travel_mm)]
    if not load:
        feat.reject_reason = 'NO_SAMPLES'
        return feat

    d = np.array([s.travel_mm for s in load], dtype=float)
    z = np.array([s.z_mm for s in load], dtype=float)
    fx = np.array([s.fx_n for s in load], dtype=float)
    fy = np.array([s.fy_n for s in load], dtype=float)
    fz = np.array([s.fz_n for s in load], dtype=float)

    s_ax = float(sign) if sign is not None else axial_sign(fz)
    feat.axial_sign = s_ax
    f_ax = s_ax * fz
    f_lat = np.hypot(fx, fy)
    feat.n_load_samples = int(d.size)
    feat.noise_n = float(noise_n)

    idx, d0, baseline, _ = detect_touchdown(d, f_ax, noise_n=noise_n, min_rise_n=min_rise_n)
    if idx is None:
        # 시작부터 이미 눌려 있으면 꺾임이 없어 접촉 시점을 잡을 수 없다.
        # "표면이 없다"와는 원인도 대책도 정반대다 — 전자는 접근 높이를
        # 올려야 하고 후자는 내려야 한다. 구분해서 남긴다.
        head = f_ax[:max(3, int(f_ax.size * 0.25))]
        gate = max(min_rise_n, 4.0 * noise_n, _EPS)
        feat.reject_reason = ('ALREADY_IN_CONTACT'
                              if float(np.median(head)) >= gate else 'NO_CONTACT')
        return feat

    feat.contact_travel_mm = d0
    feat.baseline_n = float(baseline)
    # 접촉 높이: 교점 d0 을 감싸는 두 표본 사이에서 선형 보간한다. 이 값이
    # 사실상 손톱 표면의 높이 지도가 되므로 정밀도가 그대로 경계 품질이 된다.
    feat.contact_z_mm = float(np.interp(d0, d, z))

    contact = np.arange(d.size) >= idx
    dc = d[contact] - d0
    fc = f_ax[contact] - baseline
    lc = f_lat[contact]
    keep = dc > 0.0
    dc, fc, lc = dc[keep], fc[keep], lc[keep]
    feat.n_contact_samples = int(dc.size)

    peak = float(fc.max()) if dc.size else 0.0
    feat.peak_force_n = peak
    feat.indentation_mm = float(dc.max()) if dc.size else 0.0

    # --- 힘-곡선 특징: 표본이 충분할 때만. 단단한 면에서는 여기가 비어도 정상 ---
    if dc.size >= min_stiffness_samples and peak > _EPS:
        lo, hi = stiffness_window
        band = (fc >= lo * peak) & (fc <= hi * peak)
        if band.sum() >= 4:
            slope, _, r2, _ = robust_line(dc[band], fc[band])
        else:
            slope, _, r2, _ = robust_line(dc, fc)
        feat.stiffness_n_per_mm = float(max(0.0, slope))
        feat.linearity_r2 = float(max(0.0, r2))
        feat.power_exponent = power_exponent(dc, fc)
        feat.stiffness_valid = feat.stiffness_n_per_mm > _EPS
        # 곡선 모양은 회귀가 실제로 돌았을 때만 의미가 있다. 여기서 안 채운
        # 채로 0.0 을 남기면 분류기가 그 0.0 을 "선형성이 나쁘다"는 측정값으로
        # 읽는다 — 단단한 손톱일수록 표본이 없어 0.0 이 되므로, 라벨이 통째로
        # 뒤집힌다(시뮬레이션에서 재현됨).
        feat.curve_valid = feat.stiffness_valid
    else:
        feat.reject_reason = 'SHALLOW_INDENT'

    if dc.size:
        ratio, events, tilt, lat_n = slip_metrics(fc, lc, peak, drop_n=slip_drop_n,
                                                  noise_n=noise_n)
        feat.slip_ratio = ratio
        feat.slip_events = events
        feat.surface_tilt_deg = tilt
        feat.lateral_force_n = lat_n

    # --- 유지 구간: 응력 완화 --------------------------------------------------
    if dwell:
        fzd = np.array([x.fz_n for x in dwell], dtype=float) * s_ax - baseline
        lat_d = np.hypot(np.array([x.fx_n for x in dwell], dtype=float),
                         np.array([x.fy_n for x in dwell], dtype=float))
        # 유지 중 접선력까지 같이 빠졌다면 응력완화가 아니라 미끄러진 것이다.
        # 그런 파형으로 낸 relaxation 은 피부처럼 보일 뿐이라 버린다.
        if lat_d.size >= 4:
            k_edge = max(1, int(lat_d.size * 0.25))
            lat_head = float(lat_d[:k_edge].mean())
            lat_drop = lat_head - float(lat_d[-k_edge:].mean())
        else:
            lat_head = float(lat_d[0]) if lat_d.size else 0.0
            lat_drop = lat_head - float(lat_d[-1]) if lat_d.size >= 2 else 0.0
        lat_gate = max(slip_drop_n, 4.0 * noise_n, 0.25 * abs(lat_head))
        if lat_drop <= lat_gate:
            feat.relaxation_ratio = relaxation_ratio(fzd)
            feat.hold_valid = True
        else:
            feat.slip_events += 1

    # --- 계단 압입: ΔF/Δd -------------------------------------------------------
    if dwell and dither:
        f_a = _tail_mean(list(dwell), 'fz_n')
        f_b = _tail_mean(list(dither), 'fz_n')
        d_a = _tail_mean(list(dwell), 'travel_mm')
        d_b = _tail_mean(list(dither), 'travel_mm')
        if None not in (f_a, f_b, d_a, d_b):
            delta_d = d_b - d_a
            delta_f = (f_b - f_a) * s_ax
            if delta_d > 1e-3 and delta_f > 0.0:
                feat.incremental_stiffness_n_per_mm = float(delta_f / delta_d)
                if not feat.stiffness_valid:
                    # 회귀는 못 했지만 계단 압입은 나왔다 — 이쪽을 대표값으로 쓴다.
                    feat.stiffness_n_per_mm = feat.incremental_stiffness_n_per_mm
                    feat.stiffness_valid = True
                    feat.reject_reason = ''

    # --- 제하 구간: 이력·점착 ---------------------------------------------------
    if unload:
        du = np.array([x.travel_mm for x in unload], dtype=float) - d0
        fu = np.array([x.fz_n for x in unload], dtype=float) * s_ax - baseline
        feat.adhesion_force_n = float(fu.min())
        pos = du > 0.0
        if pos.sum() >= 3 and dc.size >= 3:
            feat.hysteresis_ratio = hysteresis_ratio(dc, fc, du[pos], fu[pos])
            feat.hold_valid = True

    if feat.slip_ratio > slip_ratio_limit or feat.slip_events > 0:
        # 힘 특징은 못 믿지만 contact_z_mm 은 접촉 직후 값이라 살아 있다.
        # 기하 특징만 골라 쓸 수 있도록 valid 는 유지하고 사유만 남긴다.
        feat.stiffness_valid = False
        feat.curve_valid = False
        feat.hold_valid = False
        feat.reject_reason = 'SLIPPED'

    feat.valid = True
    return feat
