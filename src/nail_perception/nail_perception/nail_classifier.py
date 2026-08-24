"""다특징 융합 손톱/피부 분류 (기존 강성 1차원 Otsu 대체).

`clustering.py` 는 강성값 하나를 Otsu 로 자른다. 실기에서 이게 안 갈리는
이유는 임계값 알고리즘이 나빠서가 아니라 **축 하나로는 원래 안 갈리기
때문**이다.

  * 위치제어 로봇으로 단단한 면을 누르면 측정되는 기울기는 손톱 강성이
    아니라 로봇·툴 컴플라이언스라 상한에서 포화한다. 눌려 굳은 피부와
    값이 겹친다.
  * 게다가 단단한 점일수록 힘 상한에 빨리 닿아 회귀할 표본이 없다.
    강성이 안 나오는 게 곧 손톱이라는 신호인데, 그 점을 버리면 손톱만
    사라지고 피부만 남는다.

여기서는 서로 **다른 물리에 기대는** 축 여럿을 모아 한 축으로 투영한다.
축마다 자기 혼자서 얼마나 갈리는지(1차원 분리도)를 재고, 그 값을 그대로
가중치로 쓴다 — 오늘 조명·재질·툴 상태에서 실제로 작동하는 축이 알아서
발언권을 갖고, 죽은 축은 0 이 된다. 사전 학습도, 라벨도 필요 없다.

마지막으로 **공간 일관성**을 한 번 먹인다. 손톱은 연결된 한 덩어리지
체스판 무늬가 아니다. 임계값 근처에서 흔들린 점을 이웃 다수결로 정리하면
경계선이 눈에 띄게 매끄러워진다.
"""
import math
from dataclasses import dataclass, field

import numpy as np

from .clustering import compute_threshold, separation_margin

_EPS = 1e-9


# (StiffnessPoint 속성명, 방향, 설명)
#   방향 +1: 값이 클수록 손톱 / -1: 값이 클수록 피부
FEATURE_SPECS = (
    ('stiffness_n_per_mm', +1, '압입 강성. 포화하지만 여전히 유효한 축'),
    ('linearity_r2',       +1, '선형 적합도. 손톱은 선형, 피부는 변형경화'),
    ('power_exponent',     -1, 'f ∝ d^p 의 p. 피부 2~3, 손톱 1 근처'),
    ('relaxation_ratio',   -1, '유지 중 힘 감소율. 피부(점탄성)가 크다'),
    ('hysteresis_ratio',   -1, '적재/제하 면적비. 피부가 크다'),
    ('height_residual_mm', +1, '작업면 대비 높이. 강성과 독립인 기하 증거'),
)

# 특징마다 "그 점에서 실제로 측정됐는지"를 말해주는 플래그가 다르다.
#
# 0.0 을 그냥 값으로 읽으면 안 된다. 단단한 손톱은 힘 상한에 즉시 닿아
# 회귀할 표본이 없고, 그러면 linearity_r2 가 0.0 으로 남는다. 그 0.0 을
# 측정값으로 읽으면 "손톱은 선형성이 나쁘다"가 되어 라벨이 통째로 뒤집힌다
# (시뮬레이션에서 재현됨). 측정 안 된 값과 0 인 값은 반드시 구분한다.
FEATURE_GATE = {
    'stiffness_n_per_mm': 'stiffness_valid',   # 회귀 또는 계단 압입
    'linearity_r2':       'curve_valid',       # 회귀가 실제로 돈 경우만
    'power_exponent':     'curve_valid',
    'relaxation_ratio':   'hold_valid',        # 유지 중 미끄러지지 않은 경우만
    'hysteresis_ratio':   'hold_valid',
    'height_residual_mm': None,                # 접촉만 잡히면 항상 유효
}

# 1차원 분리도 상한. 한 군집이 사실상 상수면 분모가 0 으로 가서 분리도가
# 발산하고, 그 축 하나가 가중치를 독식한다.
MARGIN_CAP = 12.0

# 표준화 z 값 절단 한계. 배경 모델이 못 따라간 몇 점이 손톱보다 큰 잔차를
# 내는 일이 실제로 있고, 그러면 Otsu 가 그 이상점을 잘라 "거의 전부가
# 손톱"이라는 결론이 나온다.
_Z_CLIP = 4.0


@dataclass
class Classification:
    threshold: float = 0.0                  # 융합 판별축 상의 임계값
    scores: np.ndarray = field(default_factory=lambda: np.zeros(0))
    labels: np.ndarray = field(default_factory=lambda: np.zeros(0, dtype=bool))
    margin: float = 0.0                     # 융합축의 분리도
    method: str = 'fused'                   # 'fused' | 'stiffness'
    weights: dict = field(default_factory=dict)
    feature_margins: dict = field(default_factory=dict)
    feature_coherences: dict = field(default_factory=dict)
    used_features: tuple = ()
    spatial_flips: int = 0
    stiffness_threshold: float = 0.0        # 하위 호환 보고용
    height_step_mm: float = 0.0             # 손톱/피부 평균 높이차
    height_snr: float = 0.0                 # 높이 단차 / 배경 잔차. 기하 축의 신뢰도
    detail: str = ''

    @property
    def hard_count(self):
        return int(self.labels.sum())

    @property
    def soft_count(self):
        return int((~self.labels).sum())


def _robust_z(values):
    """중앙값/MAD 표준화. 이상치 몇 점이 스케일을 끌고 가지 않게 한다."""
    v = np.asarray(values, dtype=float)
    finite = v[np.isfinite(v)]
    if finite.size < 2:
        return np.zeros_like(v)
    med = float(np.median(finite))
    mad = float(np.median(np.abs(finite - med)))
    scale = 1.4826 * mad
    if scale < _EPS:
        scale = float(finite.std())
    if scale < _EPS:
        return np.zeros_like(v)
    z = (v - med) / scale
    # 이상점 한 줌이 Otsu 임계값을 통째로 끌고 가지 못하게 자른다. 순서는
    # 그대로 유지되므로 판별력은 잃지 않고, "손톱 대신 이상점을 잘라내는"
    # 실패만 막힌다.
    return np.clip(z, -_Z_CLIP, _Z_CLIP)


def _one_dim_margin(values, method='otsu'):
    """한 축을 Otsu 로 자른 뒤의 분리도. 그 축의 발언권 자체가 된다.

    `clustering.separation_margin` 을 그대로 쓰지 않는 이유: 한 군집이 거의
    상수면 분모(표준편차 합)가 0 으로 가서 분리도가 수백까지 튀고, 그 축
    하나가 융합 가중치를 독식한다. 여기서는 분모에 전체 범위 기준 하한을
    두고 상한도 씌운다.
    """
    v = np.asarray(values, dtype=float)
    v = v[np.isfinite(v)]
    span = float(v.max() - v.min()) if v.size else 0.0
    if v.size < 4 or span < _EPS:
        return 0.0, 0.0
    thr = compute_threshold(v, method)
    hi, lo = v[v >= thr], v[v < thr]
    if hi.size < 2 or lo.size < 2:
        return 0.0, float(thr)
    denom = max(float(hi.std() + lo.std()), 0.05 * span, _EPS)
    margin = abs(float(hi.mean() - lo.mean())) / denom
    return float(min(margin, MARGIN_CAP)), float(thr)


def _collect(points, extra):
    """점 목록 → {특징명: 값 배열}. 못 쓰는 값은 NaN 으로 비워 둔다."""
    cols = {}
    n = len(points)
    for name, _direction, _doc in FEATURE_SPECS:
        col = np.full(n, np.nan, dtype=float)
        for i, pt in enumerate(points):
            if name in extra:
                val = extra[name][i]
            else:
                val = getattr(pt, name, None)
            if val is None:
                continue
            val = float(val)
            if not math.isfinite(val):
                continue
            if not _feature_available(pt, name):
                continue
            col[i] = val
        cols[name] = col
    return cols


def _feature_available(pt, name):
    """그 점에서 이 특징이 **실제로 측정됐는지**.

    측정 안 된 값(기본값 0.0)을 측정값으로 읽지 않기 위한 관문이다.
    플래그가 아예 없는 옛 메시지는 강성 축만 살려 하위 호환을 유지한다.
    """
    if getattr(pt, 'slip_events', 0):
        return False
    gate = FEATURE_GATE.get(name)
    if gate is None:
        return True
    flag = getattr(pt, gate, None)
    if flag is None:
        return name == 'stiffness_n_per_mm'
    return bool(flag)


def _log1p_signed(v):
    """강성처럼 수십 배 차이 나는 축은 로그로 눌러야 한 축이 판을 안 먹는다."""
    return np.sign(v) * np.log1p(np.abs(v))


def feature_coherence(xy, labels, radius_mm, min_neighbors=3):
    """이 축이 만든 라벨이 **공간적으로 뭉쳐 있는가**. 0(무작위) ~ 1(완벽).

    허위 축을 걸러내는 관문이다. Otsu 는 순수한 노이즈도 잘라내고, 잘라낸
    두 덩어리는 분리도가 1.5~2 씩 나온다 — 분리도 하한만으로는 노이즈 축과
    진짜 축이 구분되지 않는다(재질이 동일한 모형 시뮬레이션에서 강성·선형성
    축이 가중치를 받아버렸다).

    손톱은 연결된 한 덩어리다. 진짜로 재질/기하를 보는 축은 이웃끼리 라벨이
    맞고, 노이즈를 보는 축은 이웃과 무관하게 흩어진다. 우연히 맞을 확률
    (라벨 비율로 계산)을 빼고 정규화하므로, 한쪽 라벨이 압도적일 때 값이
    부풀지 않는다.
    """
    pts = np.asarray(xy, dtype=float)
    lab = np.asarray(labels, dtype=bool)
    if pts.shape[0] != lab.size or lab.size < 4 or radius_mm <= 0.0:
        return 1.0
    d2 = ((pts[:, None, :] - pts[None, :, :]) ** 2).sum(axis=2)
    np.fill_diagonal(d2, np.inf)
    near = d2 <= radius_mm ** 2

    agree_total, pair_total = 0, 0
    for i in range(lab.size):
        idx = np.flatnonzero(near[i])
        if idx.size < min_neighbors:
            continue
        agree_total += int((lab[idx] == lab[i]).sum())
        pair_total += int(idx.size)
    if pair_total == 0:
        return 1.0
    observed = agree_total / pair_total
    p_hi = float(lab.mean())
    chance = p_hi ** 2 + (1.0 - p_hi) ** 2
    if chance >= 1.0 - _EPS:
        return 1.0
    return float(max(0.0, min(1.0, (observed - chance) / (1.0 - chance))))


def classify(points, extra=None, *, min_feature_margin=1.0,
             coherence_radius_mm=0.0, flip_band_sigma=0.75,
             coherence_passes=2, min_feature_coherence=0.15,
             threshold_method='otsu', xy=None):
    """점 목록 → Classification.

    points : StiffnessPoint (또는 같은 속성을 가진 객체) 목록
    extra  : {특징명: 값 배열} — 메시지에 없는 파생 특징(height_residual_mm 등)
    xy     : [(x_mm, y_mm), ...]. 주면 공간 일관성 보정을 수행한다.
    min_feature_margin : 1차원 분리도가 이 값 이하인 축은 가중치 0.
    """
    extra = extra or {}
    n = len(points)
    result = Classification(scores=np.zeros(n), labels=np.zeros(n, dtype=bool))
    if n < 4:
        result.detail = f'표본 부족 ({n}점)'
        return result

    cols = _collect(points, extra)

    weights, zcols, margins, coherences = {}, {}, {}, {}
    for name, direction, _doc in FEATURE_SPECS:
        col = cols[name]
        avail = np.isfinite(col)
        if avail.sum() < max(4, int(0.3 * n)):
            margins[name] = 0.0
            continue
        raw = col.copy()
        if name == 'stiffness_n_per_mm':
            raw = np.log1p(np.abs(np.nan_to_num(raw))) * np.sign(np.nan_to_num(raw))
            raw[~avail] = np.nan
        margin, thr = _one_dim_margin(raw[avail], threshold_method)
        margins[name] = float(margin)
        w = max(0.0, margin - min_feature_margin)
        if w <= 0.0:
            continue
        # 이 축 혼자 자른 라벨이 공간적으로 뭉쳐 있는지 본다. 흩어져 있으면
        # 노이즈를 자른 것이므로 발언권을 준다.
        if xy is not None and coherence_radius_mm > 0.0:
            own = np.zeros(n, dtype=bool)
            own[avail] = raw[avail] >= thr
            if direction < 0:
                own = ~own
            coh = feature_coherence(xy, own, coherence_radius_mm)
            coherences[name] = coh
            if coh < min_feature_coherence:
                continue
            w *= coh
            if w <= 0.0:
                continue
        z = np.full(n, np.nan)
        z[avail] = _robust_z(raw[avail]) * direction
        weights[name] = w
        zcols[name] = z

    if not weights:
        # 어느 축도 혼자서는 못 가른다 — 가장 나은 축만이라도 살려서
        # 등가중으로 투영한다. 그래도 분리도 판정은 아래에서 다시 한다.
        best = max(margins, key=lambda k: margins[k]) if margins else None
        if best is None or margins[best] <= 0.0:
            result.detail = '유효한 판별 축 없음 — 전 특징의 1차원 분리도 0'
            return result
        col = cols[best]
        avail = np.isfinite(col)
        direction = next(d for nm, d, _ in FEATURE_SPECS if nm == best)
        z = np.full(n, np.nan)
        z[avail] = _robust_z(col[avail]) * direction
        weights, zcols = {best: 1.0}, {best: z}

    # 점마다 "쓸 수 있는 축들만" 가중평균한다. 축이 하나도 없는 점은 0.
    total = np.zeros(n)
    wsum = np.zeros(n)
    for name, w in weights.items():
        z = zcols[name]
        ok = np.isfinite(z)
        total[ok] += w * z[ok]
        wsum[ok] += w
    scores = np.divide(total, wsum, out=np.zeros(n), where=wsum > 0)

    fused_margin, fused_thr = _one_dim_margin(scores, threshold_method)

    # 융합이 강성 단독보다 나쁘면 쓰지 않는다 — 개선하러 온 코드가 개악하고
    # 조용히 넘어가는 일은 없어야 한다.
    #
    # 단, 되돌아갈 자격은 분리도만으로 주지 않는다. Otsu 는 순수 노이즈도
    # 잘라내고 그 분리도가 10 을 넘기도 한다 — 재질이 같은 모형에서 강성
    # 축이 정확히 그렇다. 공간적으로 뭉치지 않는 축은 아무리 분리도가 높아도
    # 손톱을 보고 있는 게 아니므로 폴백 대상에서 뺀다.
    stiff_raw = cols['stiffness_n_per_mm']
    stiff_ok = np.isfinite(stiff_raw)
    stiff_margin, stiff_thr = (_one_dim_margin(stiff_raw[stiff_ok], threshold_method)
                               if stiff_ok.sum() >= 4 else (0.0, 0.0))
    stiff_usable = stiff_ok.sum() >= 4
    if stiff_usable and xy is not None and coherence_radius_mm > 0.0:
        own = np.zeros(n, dtype=bool)
        own[stiff_ok] = stiff_raw[stiff_ok] >= stiff_thr
        stiff_coh = feature_coherence(xy, own, coherence_radius_mm)
        coherences.setdefault('stiffness_n_per_mm', stiff_coh)
        if stiff_coh < min_feature_coherence:
            stiff_usable = False

    if fused_margin >= stiff_margin or not stiff_usable:
        result.method = 'fused'
        result.scores = scores
        result.threshold = fused_thr
        result.margin = fused_margin
    else:
        result.method = 'stiffness'
        s = np.zeros(n)
        s[stiff_ok] = stiff_raw[stiff_ok]
        result.scores = s
        result.threshold = stiff_thr
        result.margin = stiff_margin
        result.detail = (f'융합 분리도({fused_margin:.2f}) < 강성 단독'
                         f'({stiff_margin:.2f}) — 강성 단독으로 되돌림')

    result.weights = {k: float(v) for k, v in weights.items()}
    result.feature_margins = {k: float(v) for k, v in margins.items()}
    result.feature_coherences = {k: float(v) for k, v in coherences.items()}
    result.used_features = tuple(weights.keys())
    result.stiffness_threshold = float(stiff_thr)
    result.labels = result.scores >= result.threshold

    if xy is not None and coherence_radius_mm > 0.0:
        result.labels, result.spatial_flips = enforce_spatial_coherence(
            xy, result.labels, result.scores, result.threshold,
            radius_mm=coherence_radius_mm, band_sigma=flip_band_sigma,
            passes=coherence_passes)
        # 라벨이 바뀌었으니 분리도도 바뀐 라벨 기준으로 다시 잰다.
        hi = result.scores[result.labels]
        lo = result.scores[~result.labels]
        if hi.size >= 2 and lo.size >= 2:
            result.margin = separation_margin(hi, lo)

    hr = extra.get('height_residual_mm')
    if hr is not None:
        hr = np.asarray(hr, dtype=float)
        hi = hr[result.labels & np.isfinite(hr)]
        lo = hr[(~result.labels) & np.isfinite(hr)]
        if hi.size and lo.size:
            result.height_step_mm = float(np.median(hi) - np.median(lo))

    return result


def enforce_spatial_coherence(xy, labels, scores, threshold, *,
                              radius_mm, band_sigma=0.75, passes=2,
                              min_neighbors=3, majority=0.67):
    """이웃 다수결로 임계값 근처의 흔들린 라벨만 뒤집는다.

    확신이 있는 점(임계값에서 멀리 떨어진 점)은 건드리지 않는다 — 다수결로
    전부 밀어버리면 폭이 좁은 손톱 끝이 통째로 지워진다.
    """
    pts = np.asarray(xy, dtype=float)
    labels = np.asarray(labels, dtype=bool).copy()
    scores = np.asarray(scores, dtype=float)
    if pts.shape[0] != labels.size or labels.size < 4:
        return labels, 0

    d2 = ((pts[:, None, :] - pts[None, :, :]) ** 2).sum(axis=2)
    np.fill_diagonal(d2, np.inf)
    neighbor = d2 <= radius_mm ** 2

    spread = float(scores.std())
    band = band_sigma * spread if spread > _EPS else _EPS
    flips = 0
    for _ in range(passes):
        changed = 0
        for i in range(labels.size):
            idx = np.flatnonzero(neighbor[i])
            if idx.size < min_neighbors:
                continue
            if abs(scores[i] - threshold) > band:
                continue
            agree = int((labels[idx] == labels[i]).sum())
            if agree / idx.size <= 1.0 - majority:
                labels[i] = not labels[i]
                changed += 1
        flips += changed
        if changed == 0:
            break
    return labels, flips


def height_driven(result, dominance=0.5):
    """판별을 실제로 이끈 축이 높이인가.

    재질이 같은 모형에서는 힘 축이 전부 죽고 높이만 남는다. 그때는 판정
    기준도 높이에 맞는 것을 써야 한다 — 아래 `describe` 가 찍는 가중치를
    보면 어느 축이 일했는지 그대로 보인다.
    """
    total = sum(result.weights.values())
    if total <= _EPS:
        return False
    return result.weights.get('height_residual_mm', 0.0) / total >= dominance


def describe(result):
    """로그 한 줄 요약. 어느 축이 실제로 일했는지가 튜닝의 전부다."""
    if not result.feature_margins:
        return f'method={result.method} margin={result.margin:.2f} (특징 없음)'
    ranked = sorted(result.feature_margins.items(), key=lambda kv: -kv[1])
    def tag(k, v):
        bits = f'{k}={v:.2f}'
        if k in result.feature_coherences:
            bits += f'/c{result.feature_coherences[k]:.2f}'
        if k in result.weights:
            bits += f'→w{result.weights[k]:.2f}'
        return bits
    axes = ' '.join(tag(k, v) for k, v in ranked)
    return (f'method={result.method} margin={result.margin:.2f} '
            f'hard={result.hard_count} soft={result.soft_count} '
            f'flips={result.spatial_flips} step={result.height_step_mm:+.3f}mm '
            f'(snr {result.height_snr:.1f}) | {axes}')
