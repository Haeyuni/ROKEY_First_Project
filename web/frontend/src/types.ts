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

export interface ProcessState {
  session_id: string;
  stage: string;
  layer_index: number;
  layer_total: number;
  rework_count: number;
  stage_percent: number;
  session_percent: number;
  current_tool: string;
  last_error: { code: string; severity: number; detail: string };
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

export type WsEnvelope =
  | { type: "safety"; data: SafetyState }
  | { type: "state"; data: ProcessState }
  | { type: "map"; data: StiffnessMap }
  | { type: "verdict"; data: unknown }
  | { type: "force"; data: unknown }
  | { type: "error"; data: unknown }
  | { type: "result"; data: unknown };

export interface Recipe {
  id: string;
  name: string;
  layer_total: number;
  description: string;
}
