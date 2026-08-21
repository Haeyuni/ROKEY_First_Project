import { useEffect, useRef, useState } from "react";
import { WS_URL } from "../api";
import type {
  ProcessState,
  RunSessionResult,
  SafetyState,
  StiffnessMap,
  ValidationResult,
  WsEnvelope,
} from "../types";

const RECONNECT_DELAY_MS = 2000; // IR-06: 연결이 끊기면 2초 간격으로 자동 재연결

interface RosState {
  connected: boolean;
  safety: SafetyState | null;
  processState: ProcessState | null;
  map: StiffnessMap | null;
  verdicts: ValidationResult[];
  sessionResult: RunSessionResult | null;
}

export function useRosWebSocket(): RosState {
  const [connected, setConnected] = useState(false);
  const [safety, setSafety] = useState<SafetyState | null>(null);
  const [processState, setProcessState] = useState<ProcessState | null>(null);
  const [map, setMap] = useState<StiffnessMap | null>(null);
  const [verdicts, setVerdicts] = useState<ValidationResult[]>([]);
  const [sessionResult, setSessionResult] = useState<RunSessionResult | null>(null);

  // verdict는 이벤트 스트림이라 map처럼 세션 전체를 다시 안 보내준다 —
  // session_id가 바뀌면(새 세션 시작) 프론트에서 직접 리스트를 비운다.
  const verdictSessionRef = useRef<string | null>(null);

  // StrictMode의 effect 이중 실행에도 재연결 타이머가 중복되지 않도록 ref로 관리.
  const reconnectTimer = useRef<number | null>(null);
  const closedByCleanup = useRef(false);

  useEffect(() => {
    closedByCleanup.current = false;
    let ws: WebSocket | null = null;

    function connect(): void {
      ws = new WebSocket(WS_URL);

      ws.onopen = () => setConnected(true);

      ws.onmessage = (event: MessageEvent<string>) => {
        let msg: WsEnvelope;
        try {
          msg = JSON.parse(event.data);
        } catch {
          return;
        }
        // 서버는 접속 즉시 safety/state/map 스냅샷을 보낸다(IR-05) — 새로고침
        // 직후에도 이 핸들러가 그대로 받아 화면을 채운다.
        switch (msg.type) {
          case "safety":
            setSafety(msg.data);
            break;
          case "state":
            setProcessState(msg.data);
            // 새 세션이 PRECHECK로 시작하면 지난 세션의 최종 결과 배너를 지운다.
            if (msg.data.stage === "PRECHECK") setSessionResult(null);
            break;
          case "map":
            setMap(msg.data);
            break;
          case "verdict": {
            const v = msg.data;
            if (verdictSessionRef.current !== v.session_id) {
              verdictSessionRef.current = v.session_id;
              setVerdicts([v]);
            } else {
              setVerdicts((prev) => [...prev, v]);
            }
            break;
          }
          case "result":
            setSessionResult(msg.data);
            break;
          default:
            // force/error — Day3에서 소비 (힘 그래프, 에러 배너)
            break;
        }
      };

      ws.onclose = () => {
        setConnected(false);
        if (closedByCleanup.current) return;
        reconnectTimer.current = window.setTimeout(connect, RECONNECT_DELAY_MS);
      };

      ws.onerror = () => ws?.close();
    }

    connect();

    return () => {
      closedByCleanup.current = true;
      if (reconnectTimer.current !== null) {
        window.clearTimeout(reconnectTimer.current);
      }
      ws?.close();
    };
  }, []);

  return { connected, safety, processState, map, verdicts, sessionResult };
}
