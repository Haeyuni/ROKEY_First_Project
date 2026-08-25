import { memo, useState } from "react";
import type { SafetyState } from "../types";
import { translateFault } from "../faultMessages";
import { ChevronDownIcon, SunIcon } from "../icons";

interface Props {
  safety: SafetyState | null;
  connected: boolean;
}

// FR-30/31/32/34: 상단 상시 캡슐. UV 경고 문구는 permit 구조가 없는 안전 통제의
// 일부이므로(FR-34) 펼쳐야만 보이는 게 아니라 캡슐에 항상 짧은 형태로 노출하고,
// 클릭 시 전체 문구 + 결함 목록만 추가로 펼친다.
export const SafetyPill = memo(function SafetyPill({ safety, connected }: Props) {
  const [open, setOpen] = useState(false);

  const tone = !connected || safety === null ? "unknown" : safety.safe_to_move ? "ok" : "blocked";
  const label = !connected
    ? "서버 연결 대기 중"
    : safety === null
      ? "안전 상태 수신 대기 중"
      : safety.safe_to_move
        ? "안전 상태 정상 · 이동 가능"
        : "정지 상태 · 이동 불가";

  return (
    <div className="safety-pill-wrap">
      <button
        type="button"
        className={`safety-pill safety-pill--${tone}`}
        onClick={() => setOpen((o) => !o)}
        aria-expanded={open}
      >
        <span className="safety-pill__dot" />
        <span className="safety-pill__text">{label}</span>
        <span className="safety-pill__uv">
          <SunIcon size={13} color="#B8860B" />
          UV 상시 점등
        </span>
        <ChevronDownIcon size={13} />
      </button>

      {open && (
        <div className="safety-panel" role="region" aria-label="안전 상세 정보">
          <p className="safety-panel__uv">
            UV 램프는 항상 켜져 있습니다 (소프트웨어로 끌 수 없음) — 차단 고글 착용, 직접 응시 금지
          </p>
          {safety && safety.active_faults.length > 0 && (
            <ul className="safety-panel__faults">
              {safety.active_faults.map((code) => (
                <li key={code}>{translateFault(code)}</li>
              ))}
            </ul>
          )}
        </div>
      )}
    </div>
  );
});
