import type { ProcessState } from "../types";

// FR-10: "6단계 공정 진행 상황" — B계층 핵심 공정 6개(NIS §2 인터페이스
// 매트릭스). PRECHECK/REWORK/STONE/FINISH/ABORTED는 이 6단계 밖의 상태라
// 스텝퍼 옆 배지로 따로 표시한다.
const CORE_STAGES = ["SCAN", "SAND", "BRUSH", "COAT", "CURE", "INSPECT"] as const;

const STAGE_LABEL_KO: Record<(typeof CORE_STAGES)[number], string> = {
  SCAN: "스캔",
  SAND: "연마",
  BRUSH: "브러싱",
  COAT: "도포",
  CURE: "경화",
  INSPECT: "검사",
};

interface Props {
  processState: ProcessState | null;
}

export function ProcessStageStepper({ processState }: Props) {
  const stage = processState?.stage ?? "IDLE";
  // REWORK는 INSPECT를 다시 도는 것이므로 INSPECT를 활성 단계로 취급(NIS §8 동작).
  const lookupStage = stage === "REWORK" ? "INSPECT" : stage;
  const currentIndex = CORE_STAGES.indexOf(lookupStage as (typeof CORE_STAGES)[number]);
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
          {stage === "REWORK" && (
            <span className="stepper__badge stepper__badge--warn">
              재작업 중 ({processState.rework_count}회째)
            </span>
          )}
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
}
