import datetime as dt

from pydantic import BaseModel, Field

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
    enable_brush: bool = True
    enable_stone: bool = False


class CreateSessionResponse(BaseModel):
    session_id: str
    status: str = "started"


class HealthResponse(BaseModel):
    status: str
    ros_connected: bool
    db_ok: bool


class SessionReportResponse(BaseModel):
    session_id: str
    recipe_id: str
    target_material: str
    layer_total: int
    result_code: str | None
    abort_reason: str | None
    started_at: dt.datetime | None
    finished_at: dt.datetime | None
