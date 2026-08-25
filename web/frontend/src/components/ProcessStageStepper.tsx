import { memo } from "react";
import type { ProcessState } from "../types";

// 센서·검사 단계가 제거된 현재 공정 순서. PRECHECK/STONE/FINISH/ABORTED는
// 스텝퍼 옆 배지로 따로 표시한다.
const CORE_STAGES = ["SAND", "BRUSH", "COAT", "CURE"] as const;

const STAGE_LABEL_KO: Record<(typeof CORE_STAGES)[number], string> = {
  SAND: "연마",
  BRUSH: "브러싱",
  COAT: "도포",
  CURE: "경화",
};

interface Props {
  processState: ProcessState | null;
}

export const ProcessStageStepper = memo(function ProcessStageStepper({ processState }: Props) {
  const stage = processState?.stage ?? "IDLE";
  const currentIndex = CORE_STAGES.indexOf(stage as (typeof CORE_STAGES)[number]);
  const isFinished = stage === "FINISH";
  const isAborted = stage === "ABORTED";

  return (
    <div className="stepper">
      <ol className="stepper__list">
        {CORE_STAGES.map((s, i) => {
          let status: "done" | "active" | "pending" | "aborted";
          if (isAborted) status = i <= currentIndex ? "aborted" : "pending";
          else if (isFinished) status = "done";
          else if (i < currentIndex) status = "done";
          else if (i === currentIndex) status = "active";
          else status = "pending";

          return (
            <li key={s} className={`stepper__item stepper__item--${status}`}>
              <span className="stepper__dot" />
              <span className="stepper__label">{STAGE_LABEL_KO[s]}</span>
            </li>
          );
        })}
      </ol>

      {processState && (
        <div className="stepper__meta">
          {stage === "PRECHECK" && <span className="stepper__badge">사전 점검 중</span>}
          {stage === "STONE" && <span className="stepper__badge">스톤 부착 중</span>}
          {isFinished && <span className="stepper__badge stepper__badge--ok">완료</span>}
          {isAborted && <span className="stepper__badge stepper__badge--error">중단됨</span>}
          <span className="stepper__percent">
            전체 {processState.session_percent.toFixed(0)}% · 현재 단계{" "}
            {processState.stage_percent.toFixed(0)}%
          </span>
        </div>
      )}
    </div>
  );
});
