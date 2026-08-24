# 전 노드 공통 에러 코드. 자유 서술 금지 — 웹이 파싱해 사용자 메시지로 변환한다.

string OK                      = ""

# 공통
string E_INVALID_GOAL          = "E_INVALID_GOAL"
string E_SAFETY_BLOCKED        = "E_SAFETY_BLOCKED"
string E_PRECOND_FAILED        = "E_PRECOND_FAILED"
string E_TIMEOUT               = "E_TIMEOUT"
string E_CANCELLED             = "E_CANCELLED"
string E_COMM_LOST             = "E_COMM_LOST"
string E_MOTION_FAILED         = "E_MOTION_FAILED"

# 접촉 (법선)
string E_NO_CONTACT             = "E_NO_CONTACT"
string E_OVERFORCE              = "E_OVERFORCE"
string E_LOW_STIFFNESS          = "E_LOW_STIFFNESS"

# 접촉 (수평) — 연마 전용
string E_LATERAL_LIMIT          = "E_LATERAL_LIMIT"
string E_LATERAL_JAM            = "E_LATERAL_JAM"

# 툴
string E_TOOL_MISMATCH          = "E_TOOL_MISMATCH"
string E_GRIP_FAILED            = "E_GRIP_FAILED"
string E_TOOL_DROP              = "E_TOOL_DROP"

# 심각도
uint8 SEV_NONE   = 0
uint8 SEV_WARN   = 1     # 기록만 하고 진행
uint8 SEV_RETRY  = 2     # 상한 내 자동 재시도
uint8 SEV_ABORT  = 3     # 공정 중단, 안전 후퇴
uint8 SEV_SAFETY = 4     # 즉시 정지, 자동 복구 금지

string  code
uint8   severity
string  detail          # 사람이 읽을 설명. 코드 대체용 아님
