import { useEffect, useRef, useState } from "react";
import { cancelSession, createSession, fetchSessionReport } from "./api";
import { AlertModal } from "./components/AlertModal";
import { ColorStep } from "./components/ColorStep";
import { CompleteStep } from "./components/CompleteStep";
import { PartsStep, type StartSettings } from "./components/PartsStep";
import { ProgressStep } from "./components/ProgressStep";
import { useRosWebSocket } from "./hooks/useWebSocket";
import { CUBICS, DESIGNS, isStoneEnabled, type CubicId, type DesignId } from "./options";
import { SEV_SAFETY, SEV_WARN, type RunSessionResult } from "./types";

const TERMINAL_STAGES = new Set(["FINISH", "ABORTED"]);

type Screen = "color" | "parts" | "progress" | "complete";

// 세션이 진행 중인 것으로 보는 stage 집합(FR-33 uiLocked와 별개로, 새로고침 시
// 서버 상태를 보고 진행 화면으로 복귀하기 위해 씀).
const ACTIVE_STAGES = new Set(["PRECHECK", "SAND", "BRUSH", "COAT", "CURE", "STONE"]);

export default function App() {
  const { connected, safety, processState, sessionResult } = useRosWebSocket();

  const [screen, setScreen] = useState<Screen>("color");
  const [design, setDesign] = useState<DesignId>(DESIGNS[0].id);
  const [cubic, setCubic] = useState<CubicId>(CUBICS[0].id);
  const [startedAt, setStartedAt] = useState<number | null>(null);
  const [finishedAt, setFinishedAt] = useState<number | null>(null);
  const [ackErrorKey, setAckErrorKey] = useState<string | null>(null);
  const [busy, setBusy] = useState(false);
  const [startError, setStartError] = useState<string | null>(null);
  const [fallbackResult, setFallbackResult] = useState<RunSessionResult | null>(null);

  // IR-05: 서버는 "result"를 접속 스냅샷에 안 넣는다(이벤트성). WS가 그
  // 메시지가 나가는 순간 끊겨 있었으면 sessionResult는 영영 안 온다 —
  // 같은 종료를 두 번 처리하지 않도록 세션 id로 dedupe.
  const handledTerminalSessionRef = useRef<string | null>(null);

  const enableStone = isStoneEnabled(cubic);

  const lastError = processState?.last_error;
  const errorKey = lastError && lastError.code ? `${lastError.code}|${lastError.detail}` : null;
  const locked = (lastError?.severity ?? 0) >= SEV_SAFETY; // FR-33
  const showAlert = errorKey !== null && (lastError!.severity ?? 0) >= SEV_WARN && errorKey !== ackErrorKey;

  // 새로고침 등으로 클라이언트 상태 없이 열렸을 때, 서버가 이미 진행 중인
  // 세션을 알려오면 진행 화면으로 복귀한다.
  useEffect(() => {
    if (processState && ACTIVE_STAGES.has(processState.stage) && screen !== "progress" && screen !== "complete") {
      setScreen("progress");
      setStartedAt((prev) => prev ?? Date.now());
    }
  }, [processState, screen]);

  useEffect(() => {
    if (screen !== "progress") return;

    if (sessionResult) {
      setFinishedAt(Date.now());
      setScreen("complete");
      return;
    }

    // sessionResult 없이 ProcessState만 종료 단계(FINISH/ABORTED)로 넘어온
    // 경우 — "result" WS 이벤트를 놓친 것이므로 REST로 마지막 결과를 다시
    // 가져와 화면을 넘긴다.
    if (!processState || !TERMINAL_STAGES.has(processState.stage)) return;
    if (handledTerminalSessionRef.current === processState.session_id) return;
    handledTerminalSessionRef.current = processState.session_id;

    setFinishedAt(Date.now());
    setScreen("complete");
    fetchSessionReport(processState.session_id)
      .then((report) => {
        const code = report.result_code ?? "UNKNOWN";
        setFallbackResult({
          success: code === "COMPLETED" || code === "COMPLETED_WITH_WARN",
          result_code: code,
          warn_count: 0,
          final_error: { code: "", severity: 0, detail: report.abort_reason ?? "" },
        });
      })
      .catch(() => {});
  }, [sessionResult, processState, screen]);

  async function handleStart(settings: StartSettings) {
    setBusy(true);
    setStartError(null);
    try {
      const { session_id } = await createSession({
        shape_profile_id: settings.shapeProfileId,
        target_material: settings.targetMaterial,
        enable_stone: enableStone,
      });
      void session_id; // WS의 ProcessState.session_id로 화면을 갱신하므로 별도 보관은 불필요
      setStartedAt(Date.now());
      setFinishedAt(null);
      setFallbackResult(null);
      setScreen("progress");
    } catch (err) {
      setStartError(String(err));
    } finally {
      setBusy(false);
    }
  }

  function handleCancel() {
    const activeSessionId = processState?.session_id;
    if (!activeSessionId) return;
    cancelSession(activeSessionId).catch(() => {});
  }

  function handleRestart() {
    setScreen("color");
    setDesign(DESIGNS[0].id);
    setCubic(CUBICS[0].id);
    setStartedAt(null);
    setFinishedAt(null);
    setFallbackResult(null);
    setAckErrorKey(null);
    setStartError(null);
  }

  const elapsedMs = startedAt !== null && finishedAt !== null ? finishedAt - startedAt : null;

  return (
    <div className="app-root">
      {screen === "color" && (
        <ColorStep
          safety={safety}
          connected={connected}
          value={design}
          onChange={setDesign}
          onNext={() => setScreen("parts")}
        />
      )}

      {screen === "parts" && (
        <PartsStep
          safety={safety}
          connected={connected}
          design={design}
          cubic={cubic}
          onChangeCubic={setCubic}
          onBack={() => setScreen("color")}
          onStart={handleStart}
          busy={busy}
          safeToMove={safety?.safe_to_move ?? false}
          errorMessage={startError}
        />
      )}

      {screen === "progress" && (
        <ProgressStep
          safety={safety}
          connected={connected}
          processState={processState}
          enableStone={enableStone}
          locked={locked}
          onCancel={handleCancel}
        />
      )}

      {screen === "complete" && (
        <CompleteStep
          safety={safety}
          connected={connected}
          design={design}
          cubic={cubic}
          result={sessionResult ?? fallbackResult}
          elapsedMs={elapsedMs}
          onRestart={handleRestart}
        />
      )}

      {showAlert && lastError && <AlertModal error={lastError} onClose={() => setAckErrorKey(errorKey)} />}
    </div>
  );
}
