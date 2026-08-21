// FR-32/35: active_faults 전부 한국어 문구로 표시. 백엔드 app/error_codes.py의
// FAULT_CODE_KO와 같은 내용 — 프론트는 배너 렌더링에 네트워크 왕복 없이 즉시
// 써야 하므로 여기 정적으로 둔다. 문구 확정(O3, Day3)되면 두 곳을 함께 바꾼다.
export const FAULT_CODE_KO: Record<string, string> = {
  FAULT_ESTOP: "비상정지가 눌렸습니다",
  FAULT_COMM_LOST: "로봇과의 통신이 끊겼습니다",
  FAULT_TOOL_DROP: "툴 낙하가 감지되었습니다",
  FAULT_NO_HANDREST: "손 거치대가 안착되지 않았습니다",
  FAULT_NO_DUST: "집진기가 꺼져 있습니다",
};

export function translateFault(code: string): string {
  return FAULT_CODE_KO[code] ?? `알 수 없는 결함 (${code})`;
}
