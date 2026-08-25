import { useEffect, useState } from "react";
import { fetchHealth } from "../api";
import { translateError, translateFault } from "../faultMessages";
import { useRosWebSocket } from "../hooks/useWebSocket";
import type { HealthResponse } from "../types";

const HEALTH_POLL_MS = 5000;

const STAGE_LABEL_KO: Record<string, string> = {
  PRECHECK: "사전 점검",
  SAND: "연마",
  BRUSH: "브러싱",
  COAT: "도포",
  CURE: "경화",
  STONE: "스톤",
  FINISH: "완료",
  ABORTED: "중단됨",
};

function connDot(state: boolean | null): string {
  if (state === null) return "muted";
  return state ? "ok" : "danger";
}

// 로봇 상태 — 고객 화면과 같은 /ws 채널(useRosWebSocket)을 그대로 재사용하고,
// 백엔드→ROS/DB 연결은 WS에 없는 정보라 GET /api/health를 주기적으로 폴링한다.
export function RobotStatusPanel() {
  const { connected, safety, processState, sessionResult } = useRosWebSocket();
  const [health, setHealth] = useState<HealthResponse | null>(null);
  const [healthError, setHealthError] = useState<string | null>(null);

  useEffect(() => {
    let cancelled = false;
    function poll() {
      fetchHealth()
        .then((h) => {
          if (!cancelled) {
            setHealth(h);
            setHealthError(null);
          }
        })
        .catch((err) => {
          if (!cancelled) setHealthError(String(err));
        });
    }
    poll();
    const timer = window.setInterval(poll, HEALTH_POLL_MS);
    return () => {
      cancelled = true;
      window.clearInterval(timer);
    };
  }, []);

  return (
    <section className="panel">
      <div className="panel__head">
        <div>
          <h2>로봇 상태</h2>
          <p className="panel__desc">실시간 안전 상태 · 공정 진행 상태 · 연결 상태 (5초마다 헬스체크 갱신)</p>
        </div>
      </div>

      <div className="status-grid">
        <div className="status-card">
          <span className="status-card__label">관리자 화면 ↔ 백엔드</span>
          <span className="status-card__value">
            <span className={`status-dot status-dot--${connDot(connected)}`} />
            {connected ? "연결됨" : "끊김"}
          </span>
        </div>
        <div className="status-card">
          <span className="status-card__label">백엔드 ↔ ROS bridge</span>
          <span className="status-card__value">
            <span className={`status-dot status-dot--${connDot(health?.ros_connected ?? null)}`} />
            {health ? (health.ros_connected ? "연결됨" : "끊김") : "확인 중"}
          </span>
        </div>
        <div className="status-card">
          <span className="status-card__label">백엔드 ↔ DB</span>
          <span className="status-card__value">
            <span className={`status-dot status-dot--${connDot(health?.db_ok ?? null)}`} />
            {health ? (health.db_ok ? "정상" : "오류") : "확인 중"}
          </span>
        </div>
      </div>
      {healthError && <div className="alert alert--danger">헬스체크 조회 실패: {healthError}</div>}

      <div className="status-block">
        <h3>안전 상태</h3>
        {safety === null ? (
          <p className="table__muted">수신 대기 중...</p>
        ) : (
          <>
            <div className="status-row">
              <span className={`badge badge--${safety.safe_to_move ? "ok" : "danger"}`}>
                {safety.safe_to_move ? "이동 가능" : "정지 상태"}
              </span>
              {!safety.estop_released && <span className="badge badge--danger">비상정지 눌림</span>}
              {!safety.comm_ok && <span className="badge badge--danger">로봇 통신 이상</span>}
            </div>
            {safety.active_faults.length > 0 && (
              <ul className="event-list">
                {safety.active_faults.map((code) => (
                  <li key={code} className="event-list__item">
                    <span className="badge badge--danger">FAULT</span>
                    <span className="event-list__detail">
                      {translateFault(code)} ({code})
                    </span>
                  </li>
                ))}
              </ul>
            )}
          </>
        )}
      </div>

      <div className="status-block">
        <h3>공정 상태</h3>
        {processState === null ? (
          <p className="table__muted">아직 수신된 공정 상태가 없습니다.</p>
        ) : (
          <>
            <div className="status-row">
              <span className="table__muted">세션</span>
              <span className="mono">{processState.session_id || "—"}</span>
            </div>
            <div className="status-row">
              <span className="table__muted">단계</span>
              <span className="badge badge--info">{STAGE_LABEL_KO[processState.stage] ?? processState.stage}</span>
            </div>
            <div className="status-row">
              <span className="table__muted">현재 툴</span>
              <span>{processState.current_tool || "—"}</span>
            </div>
            <div className="bar-row">
              <span className="bar-row__label">전체 진행률</span>
              <div className="bar-track">
                <div className="bar-fill bar-fill--a" style={{ width: `${processState.session_percent}%` }} />
              </div>
              <span className="bar-row__pct">{processState.session_percent.toFixed(0)}%</span>
            </div>
            <div className="bar-row">
              <span className="bar-row__label">단계 진행률</span>
              <div className="bar-track">
                <div className="bar-fill bar-fill--b" style={{ width: `${processState.stage_percent}%` }} />
              </div>
              <span className="bar-row__pct">{processState.stage_percent.toFixed(0)}%</span>
            </div>
            {processState.last_error.code && (
              <div className="alert alert--danger">
                {translateError(processState.last_error.code)}
                {processState.last_error.detail && ` — ${processState.last_error.detail}`}
              </div>
            )}
          </>
        )}
      </div>

      {sessionResult && (
        <div className="status-block">
          <h3>마지막 세션 결과</h3>
          <div className="status-row">
            <span
              className={`badge badge--${
                sessionResult.result_code === "COMPLETED"
                  ? "ok"
                  : sessionResult.result_code === "COMPLETED_WITH_WARN"
                    ? "warn"
                    : "danger"
              }`}
            >
              {sessionResult.result_code}
            </span>
            <span className="table__muted">경고 {sessionResult.warn_count}건</span>
          </div>
        </div>
      )}
    </section>
  );
}
