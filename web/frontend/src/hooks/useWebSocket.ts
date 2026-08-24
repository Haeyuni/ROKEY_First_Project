import { useEffect, useRef, useState } from "react";
import { WS_URL } from "../api";
import type { ProcessState, RunSessionResult, SafetyState, WsEnvelope } from "../types";

const RECONNECT_DELAY_MS = 2000; // IR-06: 연결이 끊기면 2초 간격으로 자동 재연결

interface RosState {
  connected: boolean;
  safety: SafetyState | null;
  processState: ProcessState | null;
  sessionResult: RunSessionResult | null;
}

export function useRosWebSocket(): RosState {
  const [connected, setConnected] = useState(false);
  const [safety, setSafety] = useState<SafetyState | null>(null);
  const [processState, setProcessState] = useState<ProcessState | null>(null);
  const [sessionResult, setSessionResult] = useState<RunSessionResult | null>(null);

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
          case "result":
            setSessionResult(msg.data);
            break;
          default:
            // error — 별도 채널 없음, ProcessState.last_error로 충분
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

  return { connected, safety, processState, sessionResult };
}
