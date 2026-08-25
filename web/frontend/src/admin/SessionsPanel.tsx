import { Fragment, useEffect, useState } from "react";
import { fetchSessionEvents, fetchSessions } from "../api";
import type { SessionEvent, SessionListItem } from "../types";

const PAGE_SIZE = 20;

const RESULT_LABEL_KO: Record<string, string> = {
  COMPLETED: "완료",
  COMPLETED_WITH_WARN: "완료(경고)",
  FAILED: "실패",
  ABORTED_SAFETY: "안전중단",
  CANCELLED: "취소됨",
};

const RESULT_TONE: Record<string, string> = {
  COMPLETED: "ok",
  COMPLETED_WITH_WARN: "warn",
  FAILED: "danger",
  ABORTED_SAFETY: "danger",
  CANCELLED: "muted",
};

function resultBadge(code: string | null) {
  if (code === null) return <span className="badge badge--info">진행 중</span>;
  const label = RESULT_LABEL_KO[code] ?? code;
  const tone = RESULT_TONE[code] ?? "muted";
  return <span className={`badge badge--${tone}`}>{label}</span>;
}

function fmtTime(s: string | null): string {
  if (!s) return "—";
  return new Date(s).toLocaleString("ko-KR", { hour12: false });
}

function fmtDuration(start: string | null, end: string | null): string {
  if (!start || !end) return "—";
  const ms = new Date(end).getTime() - new Date(start).getTime();
  if (!Number.isFinite(ms) || ms < 0) return "—";
  const totalSec = Math.round(ms / 1000);
  const min = Math.floor(totalSec / 60);
  const sec = totalSec % 60;
  return min > 0 ? `${min}분 ${sec}초` : `${sec}초`;
}

function summarizeEvent(e: SessionEvent): string {
  const d = e.detail as Record<string, unknown>;
  if (e.mtype === "state" && typeof d.stage === "string") {
    const parts = [`stage=${d.stage}`];
    if (typeof d.session_percent === "number") parts.push(`session=${d.session_percent.toFixed(0)}%`);
    if (typeof d.stage_percent === "number") parts.push(`stage=${d.stage_percent.toFixed(0)}%`);
    const err = d.last_error as { code?: string; detail?: string } | undefined;
    if (err && err.code) parts.push(`error=${err.code}${err.detail ? `(${err.detail})` : ""}`);
    return parts.join(" · ");
  }
  return JSON.stringify(d);
}

export function SessionsPanel() {
  const [sessions, setSessions] = useState<SessionListItem[] | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [resultFilter, setResultFilter] = useState<string>("");
  const [offset, setOffset] = useState(0);

  const [expandedId, setExpandedId] = useState<string | null>(null);
  const [eventsCache, setEventsCache] = useState<Record<string, SessionEvent[]>>({});
  const [eventsError, setEventsError] = useState<string | null>(null);

  function load() {
    setError(null);
    fetchSessions({ limit: PAGE_SIZE, offset, resultCode: resultFilter || undefined })
      .then(setSessions)
      .catch((err) => setError(String(err)));
  }

  useEffect(load, [offset, resultFilter]);

  async function toggleEvents(id: string) {
    if (expandedId === id) {
      setExpandedId(null);
      return;
    }
    setExpandedId(id);
    if (!eventsCache[id]) {
      setEventsError(null);
      try {
        const events = await fetchSessionEvents(id);
        setEventsCache((prev) => ({ ...prev, [id]: events }));
      } catch (err) {
        setEventsError(String(err));
      }
    }
  }

  return (
    <section className="panel">
      <div className="panel__head">
        <div>
          <h2>세션 이력</h2>
          <p className="panel__desc">Postgres에 저장된 세션 + 상태 이벤트 로그입니다.</p>
        </div>
        <label className="filter">
          결과
          <select
            className="input"
            value={resultFilter}
            onChange={(e) => {
              setOffset(0);
              setResultFilter(e.target.value);
            }}
          >
            <option value="">전체</option>
            {Object.entries(RESULT_LABEL_KO).map(([code, label]) => (
              <option key={code} value={code}>
                {label}
              </option>
            ))}
          </select>
        </label>
      </div>

      {error && <div className="alert alert--danger">{error}</div>}

      <table className="table">
        <thead>
          <tr>
            <th>세션</th>
            <th>레시피</th>
            <th>소재</th>
            <th>레이어</th>
            <th>결과</th>
            <th>시작</th>
            <th>소요</th>
            <th aria-label="로그" />
          </tr>
        </thead>
        <tbody>
          {sessions === null && (
            <tr>
              <td colSpan={8} className="table__empty">
                불러오는 중...
              </td>
            </tr>
          )}
          {sessions !== null && sessions.length === 0 && (
            <tr>
              <td colSpan={8} className="table__empty">
                세션 기록이 없습니다.
              </td>
            </tr>
          )}
          {sessions?.map((s) => (
            <Fragment key={s.id}>
              <tr>
                <td className="mono" title={s.id}>
                  {s.id.slice(0, 8)}…
                </td>
                <td className="mono">{s.recipe_id}</td>
                <td>{s.target_material}</td>
                <td>{s.layer_total}</td>
                <td>{resultBadge(s.result_code)}</td>
                <td className="table__muted">{fmtTime(s.started_at)}</td>
                <td className="table__muted">{fmtDuration(s.started_at, s.finished_at)}</td>
                <td className="table__actions">
                  <button type="button" className="btn btn--ghost btn--sm" onClick={() => toggleEvents(s.id)}>
                    {expandedId === s.id ? "로그 닫기" : "로그 보기"}
                  </button>
                </td>
              </tr>
              {expandedId === s.id && (
                <tr className="events-row">
                  <td colSpan={8}>
                    {s.abort_reason && <div className="events-row__abort">중단 사유: {s.abort_reason}</div>}
                    {eventsError && <div className="alert alert--danger">{eventsError}</div>}
                    {!eventsCache[s.id] && !eventsError && <div className="table__muted">이벤트 불러오는 중...</div>}
                    {eventsCache[s.id] && eventsCache[s.id].length === 0 && (
                      <div className="table__muted">기록된 이벤트가 없습니다.</div>
                    )}
                    {eventsCache[s.id] && eventsCache[s.id].length > 0 && (
                      <ul className="event-list">
                        {eventsCache[s.id].map((e) => (
                          <li key={e.id} className="event-list__item">
                            <span className="mono event-list__ts">{fmtTime(e.ts)}</span>
                            <span className="badge badge--info">{e.mtype}</span>
                            <span className="event-list__detail">{summarizeEvent(e)}</span>
                          </li>
                        ))}
                      </ul>
                    )}
                  </td>
                </tr>
              )}
            </Fragment>
          ))}
        </tbody>
      </table>

      <div className="pager">
        <button type="button" className="btn btn--ghost btn--sm" onClick={() => setOffset((o) => Math.max(0, o - PAGE_SIZE))} disabled={offset === 0}>
          이전
        </button>
        <span className="pager__label">{offset + 1}–{offset + (sessions?.length ?? 0)}</span>
        <button
          type="button"
          className="btn btn--ghost btn--sm"
          onClick={() => setOffset((o) => o + PAGE_SIZE)}
          disabled={!sessions || sessions.length < PAGE_SIZE}
        >
          다음
        </button>
      </div>
    </section>
  );
}
