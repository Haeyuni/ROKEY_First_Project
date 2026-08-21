import { ProcessStageStepper } from "./components/ProcessStageStepper";
import { SafetyBanner } from "./components/SafetyBanner";
import { SessionResultBanner } from "./components/SessionResultBanner";
import { SessionStart } from "./components/SessionStart";
import { StiffnessHeatmap } from "./components/StiffnessHeatmap";
import { UvWarningBanner } from "./components/UvWarningBanner";
import { VerdictPanel } from "./components/VerdictPanel";
import { useRosWebSocket } from "./hooks/useWebSocket";
import { SEV_SAFETY } from "./types";

const ACTIVE_STAGES = new Set([
  "PRECHECK",
  "SCAN",
  "SAND",
  "BRUSH",
  "COAT",
  "CURE",
  "INSPECT",
  "REWORK",
  "STONE",
]);

export default function App() {
  const { connected, safety, processState, map, verdicts, sessionResult } = useRosWebSocket();

  const activeSessionId =
    processState && ACTIVE_STAGES.has(processState.stage) ? processState.session_id : null;

  // FR-33: 심각도 SAFETY 에러 수신 시 진행 UI를 잠근다.
  const uiLocked = (processState?.last_error?.severity ?? 0) >= SEV_SAFETY;

  return (
    <div className="app">
      <header className="app__header">
        <SafetyBanner safety={safety} connected={connected} />
        <UvWarningBanner />
      </header>

      <main className="app__main">
        <section className="panel">
          <h2>세션 제어</h2>
          <SessionStart
            safeToMove={safety?.safe_to_move ?? false}
            locked={uiLocked}
            activeSessionId={activeSessionId}
          />
          <ProcessStageStepper processState={processState} />
          <SessionResultBanner result={sessionResult} />
        </section>

        <section className="panel">
          <h2>강성 히트맵</h2>
          <StiffnessHeatmap map={map} />
        </section>

        <section className="panel panel--wide">
          <h2>판정 결과</h2>
          <VerdictPanel verdicts={verdicts} />
        </section>
      </main>
    </div>
  );
}
