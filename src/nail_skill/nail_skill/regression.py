"""압입 강성 계산 (SDS §5.1).

두 점 차분이 아니라 하강 구간 전체의 선형 회귀를 쓴다 — 노이즈에 훨씬 강하다.
"""
import numpy as np


def compute_stiffness(samples, contact_threshold_n, min_samples):
    """
    samples: [(depth_mm, fz_n), ...] 하강 구간 시계열
    return : (stiffness_n_per_mm, r_squared, n_used)
    """
    contact = [(d, f) for d, f in samples if abs(f) >= contact_threshold_n]
    if len(contact) < min_samples:
        return None, 0.0, len(contact)

    d0 = contact[0][0]
    x = np.array([d - d0 for d, _ in contact])
    y = np.array([abs(f) for _, f in contact])

    slope, intercept = np.polyfit(x, y, 1)
    resid = y - (slope * x + intercept)
    r2 = 1.0 - resid.var() / y.var() if y.var() > 0 else 0.0
    return float(slope), float(r2), len(contact)
