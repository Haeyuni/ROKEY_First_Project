"""강성값 1차원 군집화 — 손톱(고강성)/피부(저강성) 분리 (SDS §5.2)."""
import numpy as np


def otsu_threshold(values, n_bins=256):
    """분산 최대화 기준 이진 임계값 (표준 Otsu, 1D)."""
    v = np.asarray(values, dtype=float)
    if v.size < 2 or v.max() == v.min():
        return float(v.mean()) if v.size else 0.0
    hist, edges = np.histogram(v, bins=n_bins)
    centers = (edges[:-1] + edges[1:]) / 2.0
    total = hist.sum()
    sum_all = (hist * centers).sum()

    best_var, best_thresh = -1.0, centers[0]
    w0, sum0 = 0.0, 0.0
    for count, center in zip(hist, centers):
        w0 += count
        if w0 == 0 or w0 == total:
            continue
        sum0 += count * center
        w1 = total - w0
        m0 = sum0 / w0
        m1 = (sum_all - sum0) / w1
        between = w0 * w1 * (m0 - m1) ** 2
        if between > best_var:
            best_var = between
            best_thresh = center
    return float(best_thresh)


def kmeans2_1d(values, iters=50):
    """1차원 2-평균. 초기값은 min/max로 잡아 결과가 결정적이다."""
    v = np.asarray(values, dtype=float)
    if v.size < 2:
        return float(v.mean()) if v.size else 0.0
    c0, c1 = float(v.min()), float(v.max())
    if c0 == c1:
        return c0
    for _ in range(iters):
        d0 = np.abs(v - c0)
        d1 = np.abs(v - c1)
        g0 = v[d0 <= d1]
        g1 = v[d0 > d1]
        new_c0 = g0.mean() if g0.size else c0
        new_c1 = g1.mean() if g1.size else c1
        if abs(new_c0 - c0) < 1e-6 and abs(new_c1 - c1) < 1e-6:
            c0, c1 = new_c0, new_c1
            break
        c0, c1 = new_c0, new_c1
    return (c0 + c1) / 2.0


def split_by_threshold(values, threshold):
    v = np.asarray(values, dtype=float)
    hard = v[v >= threshold]
    soft = v[v < threshold]
    return hard, soft


def separation_margin(hard_vals, soft_vals):
    """SDS §5.2: |평균차| / (표준편차 합 + eps). 군집간거리 / 군집내분산."""
    hard = np.asarray(hard_vals, dtype=float)
    soft = np.asarray(soft_vals, dtype=float)
    if hard.size == 0 or soft.size == 0:
        return 0.0
    mh, ms = hard.mean(), soft.mean()
    sh, ss = hard.std(), soft.std()
    return float(abs(mh - ms) / (sh + ss + 1e-9))


def compute_threshold(values, method='otsu'):
    if method == 'kmeans2':
        return kmeans2_1d(values)
    return otsu_threshold(values)
