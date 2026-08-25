import type { RunSessionResult, SafetyState } from "../types";
import { cubicName, designName, isStoneEnabled, type CubicId, type DesignId } from "../options";
import { MascotFull } from "../mascot";
import { ArrowRightIcon } from "../icons";
import { translateError } from "../faultMessages";
import { TopBar } from "./TopBar";

const RESULT_LABEL_KO: Record<string, string> = {
  COMPLETED: "코팅이 완료됐어요!",
  COMPLETED_WITH_WARN: "코팅이 완료됐어요 (경고 있었음)",
  FAILED: "코팅이 실패했어요",
  ABORTED_SAFETY: "안전 문제로 중단됐어요",
  CANCELLED: "코팅이 취소됐어요",
};

interface Props {
  safety: SafetyState | null;
  connected: boolean;
  design: DesignId;
  cubic: CubicId;
  result: RunSessionResult | null;
  elapsedMs: number | null;
  onRestart: () => void;
}

function formatElapsed(ms: number | null): string {
  if (ms === null) return "-";
  const totalSec = Math.round(ms / 1000);
  const min = Math.floor(totalSec / 60);
  const sec = totalSec % 60;
  return min > 0 ? `${min}분 ${sec}초` : `${sec}초`;
}

// Step 4 — 코팅 완료 페이지. 팝업이 아니라 전용 화면으로 결과를 보여주고,
// "새 손님 맞이하기"로 Step1부터 다시 시작한다.
export function CompleteStep({ safety, connected, design, cubic, result, elapsedMs, onRestart }: Props) {
  const ok = !result || result.result_code === "COMPLETED";
  const title = result ? (RESULT_LABEL_KO[result.result_code] ?? "코팅이 끝났어요") : "코팅이 완료됐어요!";
  const stoneNote = isStoneEnabled(cubic) ? " · 스톤" : "";

  return (
    <div className="screen screen--pink">
      <div className="blob blob--mint" style={{ top: -120, left: -70 }} />
      <div className="blob blob--lavender" style={{ bottom: -110, right: -80, top: "auto" }} />

      {ok && (
        <>
          <div className="confetti" style={{ left: 120, top: 120, width: 12, height: 12, background: "#FFC2D6" }} />
          <div className="confetti" style={{ left: 220, top: 220, width: 9, height: 9, background: "#B9E8D3", animationDelay: ".3s" }} />
          <div className="confetti" style={{ left: 180, top: 340, width: 10, height: 10, background: "#D9D3F5", animationDelay: ".6s" }} />
          <div className="confetti" style={{ right: 150, top: 130, width: 11, height: 11, background: "#FFD9C2", animationDelay: ".2s" }} />
          <div className="confetti" style={{ right: 240, top: 250, width: 9, height: 9, background: "#BFE0FF", animationDelay: ".5s" }} />
          <div className="confetti" style={{ right: 190, top: 360, width: 10, height: 10, background: "#F0C36A", animationDelay: ".8s" }} />
        </>
      )}

      <TopBar safety={safety} connected={connected} />

      <div className="hero">
        <div className="hero-mascot">
          <MascotFull pose={ok ? "celebrating" : "worried"} size={176} />
        </div>
        <h1>{title}</h1>
        <p>
          {ok
            ? "코티가 예쁘게 마무리했어요"
            : (result?.final_error?.code && translateError(result.final_error.code)) || "확인이 필요해요"}
        </p>
      </div>

      <div className="summary-card">
        <div className="stat">
          <span className="stat__label">컬러</span>
          <span className="stat__value">{designName(design)}</span>
        </div>
        <div className="stat">
          <span className="stat__label">파츠</span>
          <span className="stat__value">
            {cubicName(cubic)}
            {stoneNote}
          </span>
        </div>
        <div className="stat">
          <span className="stat__label">소요 시간</span>
          <span className="stat__value">{formatElapsed(elapsedMs)}</span>
        </div>
        <div className="stat">
          <span className="stat__label">경고</span>
          <span className="stat__value">{result?.warn_count ?? 0}건</span>
        </div>
      </div>

      <div className="screen__footer screen__footer--center">
        <button type="button" className="cta" onClick={onRestart}>
          새 손님 맞이하기
          <ArrowRightIcon />
        </button>
      </div>
    </div>
  );
}
