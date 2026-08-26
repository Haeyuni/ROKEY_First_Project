import type { ProcessState, SafetyState } from "../types";
import { STAGE_ICONS, CloseIcon } from "../icons";
import { MascotFull } from "../mascot";
import { TopBar } from "./TopBar";

// 실제 공정 순서(session_orchestrator_node.py: PRECHECK→SAND→BRUSH→COAT→CURE
// →STONE→FINISH)를 그대로 따른다 — 스톤은 큐빅을 선택했을 때만 CURE 다음에 붙는다.
const STAGE_ORDER = ["SAND", "BRUSH", "COAT", "CURE", "STONE"] as const;
type Stage = (typeof STAGE_ORDER)[number];

const STAGE_LABEL_KO: Record<Stage, string> = {
  SAND: "연마",
  BRUSH: "브러싱",
  COAT: "도포",
  CURE: "경화",
  STONE: "스톤",
};

// TOOL_CHANGE 동안 orchestrator는 아직 current_tool을 새 툴로 바꾸기 전이라
// (session_orchestrator_node.py: emit(TOOL_CHANGE) → _call_change_tool →
// state['current_tool'] 갱신 순서), current_tool은 "방금 끝난 단계에서 쓰던
// 툴"을 가리킨다 — 그걸로 방금까지 어디까지 끝났는지 역산한다.
const TOOL_TO_COMPLETED_STAGE: Partial<Record<string, Stage>> = {
  sander: "SAND",
  brush: "BRUSH",
  coater: "COAT",
  uv: "CURE",
  tweezers: "STONE",
};

type StationStatus = "done" | "active" | "pending";

const TAG_LABEL: Record<StationStatus, string> = {
  done: "완료",
  active: "진행 중",
  pending: "대기 중",
};

interface Props {
  safety: SafetyState | null;
  connected: boolean;
  processState: ProcessState | null;
  enableStone: boolean;
  locked: boolean;
  onCancel: () => void;
}

export function ProgressStep({ safety, connected, processState, enableStone, locked, onCancel }: Props) {
  const stations = STAGE_ORDER.filter((s) => s !== "STONE" || enableStone);
  const stage = processState?.stage ?? "PRECHECK";
  const isAborted = stage === "ABORTED";
  const isFinished = stage === "FINISH";
  const isToolChange = stage === "TOOL_CHANGE";
  const currentIndex = stations.indexOf(stage as Stage);

  const completedStage = isToolChange
    ? TOOL_TO_COMPLETED_STAGE[processState?.current_tool ?? ""]
    : undefined;
  const completedIndex = completedStage ? stations.indexOf(completedStage) : -1;

  const activeLabel = currentIndex >= 0 ? STAGE_LABEL_KO[stations[currentIndex]] : null;
  const stageTitle = isAborted
    ? "공정이 중단됐어요"
    : isFinished
      ? "코팅이 마무리되고 있어요"
      : isToolChange
        ? "툴 교체 중이에요"
        : activeLabel
          ? `${activeLabel} 중이에요`
          : "코팅을 준비하고 있어요";
  const caption = isAborted
    ? "공정이 중단됐어요"
    : isFinished
      ? "코티가 마무리하고 있어요"
      : isToolChange
        ? "코티가 툴을 바꾸고 있어요"
        : activeLabel
          ? `코티가 ${activeLabel} 중이에요`
          : "코티가 준비하고 있어요";

  return (
    <div className="screen screen--peach">
      <div className="blob blob--mint" style={{ top: -110, right: 60 }} />
      <div className="blob blob--lavender" style={{ bottom: -120, left: -70, top: "auto" }} />

      <TopBar
        safety={safety}
        connected={connected}
        extra={
          <button type="button" className="cancel-btn" onClick={onCancel} disabled={locked}>
            <CloseIcon size={13} color="#C97A7A" />
            취소
          </button>
        }
      />

      <div className="title-row">
        <h1>{stageTitle}</h1>
      </div>

      <div className="journey">
        {stations.map((s, i) => {
          let status: StationStatus;
          if (isAborted) status = i < currentIndex ? "done" : "pending";
          else if (isFinished) status = "done";
          else if (isToolChange) status = i <= completedIndex ? "done" : "pending";
          else if (currentIndex < 0) status = "pending";
          else if (i < currentIndex) status = "done";
          else if (i === currentIndex) status = "active";
          else status = "pending";

          const Icon = STAGE_ICONS[s];
          const iconColor = status === "pending" ? "#8B8398" : "#fff";

          return (
            <div key={s} className={`station-col ${i % 2 === 0 ? "station-col--low" : "station-col--high"}`}>
              {status === "active" && (
                <div className="station-col__mascot">
                  <div className="bubble">{activeLabel} 중...</div>
                  <MascotFull pose="working" size={104} />
                </div>
              )}
              <div className={`station station--${status}`}>
                <Icon size={status === "pending" ? 22 : 26} color={iconColor} />
              </div>
              <div className="station-label">
                <div className="station-label__name">{STAGE_LABEL_KO[s]}</div>
                <span className={`tag tag--${status}`}>{TAG_LABEL[status]}</span>
              </div>
            </div>
          );
        })}
      </div>

      <div className="status-card">
        <div className="status-caption">{caption}</div>
        <div className="bar-row">
          <span className="bar-row__label">전체 진행률</span>
          <div className="bar-track">
            <div className="bar-fill bar-fill--mint" style={{ width: `${processState?.session_percent ?? 0}%` }} />
          </div>
          <span className="bar-row__pct">{(processState?.session_percent ?? 0).toFixed(0)}%</span>
        </div>
        <div className="bar-row">
          <span className="bar-row__label">{activeLabel ? `${activeLabel} 단계` : "현재 단계"}</span>
          <div className="bar-track">
            <div className="bar-fill bar-fill--peach" style={{ width: `${processState?.stage_percent ?? 0}%` }} />
          </div>
          <span className="bar-row__pct">{(processState?.stage_percent ?? 0).toFixed(0)}%</span>
        </div>
      </div>
    </div>
  );
}
