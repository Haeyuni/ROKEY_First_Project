import { memo } from "react";
import type { RunSessionResult } from "../types";

const RESULT_LABEL_KO: Record<string, string> = {
  COMPLETED: "완료",
  COMPLETED_WITH_WARN: "완료 (경고 있음)",
  FAILED: "실패",
  ABORTED_SAFETY: "안전 중단",
  CANCELLED: "취소됨",
};

interface Props {
  result: RunSessionResult | null;
}

// RunSession.action result — 세션 종료 시 1회 수신(web.md §4.3 "result" 타입).
export const SessionResultBanner = memo(function SessionResultBanner({ result }: Props) {
  if (!result) return null;

  const ok = result.result_code === "COMPLETED";
  const warn = result.result_code === "COMPLETED_WITH_WARN";

  return (
    <div
      className={`session-result ${ok ? "session-result--ok" : warn ? "session-result--warn" : "session-result--fail"}`}
    >
      세션 종료: {RESULT_LABEL_KO[result.result_code] ?? result.result_code}
      {result.total_rework > 0 && ` · 재작업 ${result.total_rework}회`}
      {result.final_error?.code && ` · ${result.final_error.code}: ${result.final_error.detail}`}
    </div>
  );
});
