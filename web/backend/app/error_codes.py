"""FR-35: 에러 코드 → 한국어 문구. 최종 확정은 O3(Day3, web.md §10 미결사항).

Day1 스텁: nail_msgs/ErrorCode.msg 의 코드 전체를 우선 커버해 파싱 누락이
없게 한다. 문구는 팀 리뷰 후 Day3에 다듬는다.
"""

ERROR_CODE_KO: dict[str, str] = {
    "": "정상",
    "E_INVALID_GOAL": "요청값이 허용 범위를 벗어났습니다",
    "E_SAFETY_BLOCKED": "안전 조건이 충족되지 않아 진행할 수 없습니다",
    "E_PRECOND_FAILED": "사전 조건을 만족하지 못했습니다",
    "E_TIMEOUT": "작업이 제한 시간 내에 끝나지 않았습니다",
    "E_CANCELLED": "사용자가 취소했습니다",
    "E_COMM_LOST": "로봇과의 통신이 끊겼습니다",
    "E_MOTION_FAILED": "로봇 동작이 실패했습니다",
    "E_LATERAL_LIMIT": "연마 이동 한계를 초과했습니다 (피부 접촉 방지)",
    "E_TOOL_MISMATCH": "요구한 툴과 장착된 툴이 다릅니다",
    "E_GRIP_FAILED": "툴을 파지하지 못했습니다",
}

FAULT_CODE_KO: dict[str, str] = {
    "FAULT_ESTOP": "비상정지가 눌렸습니다",
    "FAULT_COMM_LOST": "로봇과의 통신이 끊겼습니다",
}


def translate_error(code: str) -> str:
    return ERROR_CODE_KO.get(code, f"알 수 없는 오류 ({code})")


def translate_fault(code: str) -> str:
    return FAULT_CODE_KO.get(code, f"알 수 없는 결함 ({code})")
