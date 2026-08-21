import datetime as dt

from pydantic import BaseModel, ConfigDict, Field

# FR-03: target_material 허용 목록. IDS RunSession.action 주석(BR-029: 사람
# 신체 금지)과 web.md FR-03 이 같은 값을 가리킨다.
ALLOWED_TARGET_MATERIALS = ("silicone_model", "artificial_tip")


class RecipeSummary(BaseModel):
    id: str
    name: str
    layer_total: int
    description: str = ""


class CreateSessionRequest(BaseModel):
    recipe_id: str
    shape_profile_id: str
    target_material: str
    layer_total: int = Field(default=2, ge=1, le=5)
    max_rework: int = Field(default=2, ge=0, le=10)
    enable_brush: bool = True
    enable_stone: bool = False


class CreateSessionResponse(BaseModel):
    session_id: str
    status: str = "started"


class HealthResponse(BaseModel):
    status: str
    ros_connected: bool
    db_ok: bool


class VerdictOut(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    layer_index: int
    point_label: str
    x: float
    y: float
    release_force_n: float
    threshold_n: float
    result: str
    measured_at: dt.datetime


class StiffnessMapOut(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    points: list
    boundary: list
    forbidden: list
    threshold_k: float | None
    separation_margin: float | None
    valid: bool


# FR-22: API 응답도 화면과 같은 표현 제약을 받는다 — "손톱 전체가 경화됨"이
# 아니라 "검사한 3개 지점"으로 한정해서 서술한다(web.md §3.3 경고).
REPORT_SCOPE_NOTE = (
    "이 리포트의 판정은 레이어별로 검사한 3개 지점(중앙·좌·우)의 결과입니다. "
    "손톱 표면 전체의 경화를 보증하지 않습니다."
)


class SessionReportResponse(BaseModel):
    session_id: str
    recipe_id: str
    target_material: str
    layer_total: int
    result_code: str | None
    abort_reason: str | None
    started_at: dt.datetime | None
    finished_at: dt.datetime | None
    stiffness_map: StiffnessMapOut | None
    verdicts: list[VerdictOut]
    report_note: str = REPORT_SCOPE_NOTE
