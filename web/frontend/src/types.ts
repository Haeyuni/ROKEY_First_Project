// rosbridge/roslibpy는 ROS 메시지 필드명을 그대로 JSON key로 보낸다(camelCase
// 변환 없음) — nail_msgs/*.msg (IDS §3)의 필드명과 1:1로 맞춘다.

export interface SafetyState {
  safe_to_move: boolean;
  estop_released: boolean;
  comm_ok: boolean;
  active_faults: string[];
  reason: string;
}

// ErrorCode.msg 심각도 상수 (IDS §3.1).
export const SEV_NONE = 0;
export const SEV_WARN = 1;
export const SEV_RETRY = 2;
export const SEV_ABORT = 3;
export const SEV_SAFETY = 4; // FR-33: 이 이상이면 진행 UI를 잠근다

export interface ErrorCode {
  code: string;
  severity: number;
  detail: string;
}

export interface ProcessState {
  session_id: string;
  stage: string;
  layer_index: number;
  layer_total: number;
  stage_percent: number;
  session_percent: number;
  current_tool: string;
  last_error: ErrorCode;
}

// RunSession.action result (IDS §7.1).
export interface RunSessionResult {
  success: boolean;
  result_code:
    | "COMPLETED"
    | "COMPLETED_WITH_WARN"
    | "FAILED"
    | "ABORTED_SAFETY"
    | "CANCELLED"
    | string;
  warn_count: number;
  final_error: ErrorCode;
}

export type WsEnvelope =
  | { type: "safety"; data: SafetyState }
  | { type: "state"; data: ProcessState }
  // "error"는 백엔드가 별도로 보내지 않는다 — ProcessState.last_error로
  // 충분해서 중복 채널을 만들지 않았다(App.tsx의 ErrorBanner 참고).
  | { type: "error"; data: unknown }
  | { type: "result"; data: RunSessionResult };

export interface Recipe {
  id: string;
  name: string;
  layer_total: number;
  description: string;
}
