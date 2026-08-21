// rosbridge/roslibpy는 ROS 메시지 필드명을 그대로 JSON key로 보낸다(camelCase
// 변환 없음) — nail_msgs/*.msg (IDS §3)의 필드명과 1:1로 맞춘다.

export interface SafetyState {
  safe_to_move: boolean;
  estop_released: boolean;
  comm_ok: boolean;
  handrest_seated: boolean;
  dust_extraction_on: boolean;
  tool_grip_ok: boolean;
  scan_valid: boolean;
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
  rework_count: number;
  stage_percent: number;
  session_percent: number;
  current_tool: string;
  last_error: ErrorCode;
}

export interface Point {
  x: number;
  y: number;
  z: number;
}

export interface StiffnessPoint {
  position: Point;
  stiffness_n_per_mm: number;
  release_force_n: number;
  source: "coarse" | "fine" | "verify" | string;
  valid: boolean;
}

export interface StiffnessMap {
  session_id: string;
  points: StiffnessPoint[];
  valid: boolean;
  threshold_k_n_per_mm: number;
  separation_margin: number;
}

// ValidationResult.msg (IDS §3.10). FR-20/21: 3점 판정 + 판정 시점 임계값.
export interface ValidationResult {
  session_id: string;
  layer_index: number;
  point_label: "center" | "left" | "right" | string;
  position: Point;
  release_force_n: number;
  stiffness_n_per_mm: number;
  threshold_n: number;
  result: "PASS" | "FAIL" | "SKIP" | string;
  measured_at: unknown;
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
  total_rework: number;
  warn_count: number;
  final_error: ErrorCode;
}

// ForceSample.msg (IDS §3.3). /force/data_ui, 20Hz.
export interface ForceSample {
  fx_n: number;
  fy_n: number;
  fz_n: number;
  tx_nm: number;
  ty_nm: number;
  tz_nm: number;
}

export type WsEnvelope =
  | { type: "safety"; data: SafetyState }
  | { type: "state"; data: ProcessState }
  | { type: "map"; data: StiffnessMap }
  | { type: "verdict"; data: ValidationResult }
  | { type: "force"; data: ForceSample }
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
