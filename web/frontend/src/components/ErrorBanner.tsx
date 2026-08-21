import { translateError } from "../faultMessages";
import type { ErrorCode } from "../types";
import { SEV_ABORT, SEV_SAFETY, SEV_WARN } from "../types";

interface Props {
  error: ErrorCode | undefined | null;
}

// FR-35: 에러 코드를 한국어 문구로 변환해 표시. ProcessState.last_error를
// 그대로 쓴다 — 별도 "error" WS 채널 없이도 충분하다(types.ts 주석 참고).
export function ErrorBanner({ error }: Props) {
  if (!error || !error.code) return null;

  const severityClass =
    error.severity >= SEV_SAFETY
      ? "error-banner--safety"
      : error.severity >= SEV_ABORT
        ? "error-banner--abort"
        : error.severity >= SEV_WARN
          ? "error-banner--warn"
          : "error-banner--info";

  return (
    <div className={`error-banner ${severityClass}`}>
      {translateError(error.code)}
      {error.detail && <span className="error-banner__detail"> — {error.detail}</span>}
    </div>
  );
}
