import type { SafetyState } from "../types";
import { translateFault } from "../faultMessages";

interface Props {
  safety: SafetyState | null;
  connected: boolean;
}

// FR-30: 모든 화면 상단 상시 표시. FR-31: safe_to_move=false면 시작 버튼
// 비활성화(App.tsx가 이 컴포넌트가 넘겨주는 safe 값을 그대로 씀).
// FR-32: active_faults 전부 한국어로, 다중 fault 동시 표시.
export function SafetyBanner({ safety, connected }: Props) {
  if (!connected) {
    return (
      <div className="safety-banner safety-banner--unknown">
        서버 연결 대기 중 — 안전 상태를 알 수 없습니다
      </div>
    );
  }

  if (safety === null) {
    return (
      <div className="safety-banner safety-banner--unknown">
        안전 상태 수신 대기 중...
      </div>
    );
  }

  const safe = safety.safe_to_move;

  return (
    <div className={`safety-banner ${safe ? "safety-banner--ok" : "safety-banner--blocked"}`}>
      <span className="safety-banner__status">
        {safe ? "안전 상태 정상 — 이동 가능" : "정지 상태 — 이동 불가"}
      </span>
      {safety.active_faults.length > 0 && (
        <ul className="safety-banner__faults">
          {safety.active_faults.map((code) => (
            <li key={code}>{translateFault(code)}</li>
          ))}
        </ul>
      )}
    </div>
  );
}
