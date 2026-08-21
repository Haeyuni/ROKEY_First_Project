import { SafetyBanner } from "./components/SafetyBanner";
import { SessionStart } from "./components/SessionStart";
import { StiffnessHeatmap } from "./components/StiffnessHeatmap";
import { UvWarningBanner } from "./components/UvWarningBanner";
import { useRosWebSocket } from "./hooks/useWebSocket";

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
  const { connected, safety, processState, map } = useRosWebSocket();

  const activeSessionId =
    processState && ACTIVE_STAGES.has(processState.stage) ? processState.session_id : null;

  return (
    <div className="app">
      <header className="app__header">
        <SafetyBanner safety={safety} connected={connected} />
        <UvWarningBanner />
      </header>

      <main className="app__main">
        <section className="panel">
          <h2>세션 제어</h2>
          <SessionStart safeToMove={safety?.safe_to_move ?? false} activeSessionId={activeSessionId} />
          {processState && (
            <p className="process-state">
              단계: <strong>{processState.stage}</strong> ({processState.stage_percent.toFixed(0)}%)
              · 레이어 {processState.layer_index + 1}/{processState.layer_total}
              {processState.rework_count > 0 && ` · 재작업 ${processState.rework_count}회`}
            </p>
          )}
        </section>

        <section className="panel">
          <h2>강성 히트맵</h2>
          <StiffnessHeatmap map={map} />
        </section>
      </main>
    </div>
  );
}
