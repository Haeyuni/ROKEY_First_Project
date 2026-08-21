"""web.md §5 데이터 요구사항의 4개 테이블. (원문은 SQLite, 팀 결정으로 Postgres 사용)"""

import datetime as dt
import uuid

from sqlalchemy import Boolean, DateTime, Float, ForeignKey, Integer, String, Text
from sqlalchemy.dialects.postgresql import JSONB
from sqlalchemy.orm import Mapped, mapped_column, relationship

from .db import Base


def _new_session_id() -> str:
    return uuid.uuid4().hex


def _utcnow() -> dt.datetime:
    return dt.datetime.now(dt.timezone.utc)


class SessionRecord(Base):
    __tablename__ = "sessions"

    id: Mapped[str] = mapped_column(String, primary_key=True, default=_new_session_id)
    recipe_id: Mapped[str] = mapped_column(String, nullable=False)
    shape_profile_id: Mapped[str | None] = mapped_column(String, nullable=True)
    target_material: Mapped[str] = mapped_column(String, nullable=False)
    layer_total: Mapped[int] = mapped_column(Integer, default=2)

    # RunSession 결과 코드(RunSession.action §7.1): COMPLETED / COMPLETED_WITH_WARN /
    # FAILED / ABORTED_SAFETY / CANCELLED. 진행 중에는 NULL.
    result_code: Mapped[str | None] = mapped_column(String, nullable=True)
    abort_reason: Mapped[str | None] = mapped_column(Text, nullable=True)

    started_at: Mapped[dt.datetime | None] = mapped_column(DateTime(timezone=True), nullable=True)
    finished_at: Mapped[dt.datetime | None] = mapped_column(DateTime(timezone=True), nullable=True)
    created_at: Mapped[dt.datetime] = mapped_column(DateTime(timezone=True), default=_utcnow)

    stiffness_maps: Mapped[list["StiffnessMapRecord"]] = relationship(back_populates="session")
    verdicts: Mapped[list["VerdictRecord"]] = relationship(back_populates="session")
    events: Mapped[list["EventRecord"]] = relationship(back_populates="session")


class StiffnessMapRecord(Base):
    """FR-43: 세션당 1건. StiffnessMap.msg 를 그대로 담는다."""

    __tablename__ = "stiffness_maps"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, autoincrement=True)
    session_id: Mapped[str] = mapped_column(String, ForeignKey("sessions.id"), nullable=False)

    points: Mapped[list] = mapped_column(JSONB, default=list)  # StiffnessPoint[] 직렬화
    boundary: Mapped[list] = mapped_column(JSONB, default=list)     # BoundaryRegion.boundary_polygon
    forbidden: Mapped[list] = mapped_column(JSONB, default=list)    # BoundaryRegion.forbidden_polygon

    threshold_k: Mapped[float | None] = mapped_column(Float, nullable=True)
    separation_margin: Mapped[float | None] = mapped_column(Float, nullable=True)
    valid: Mapped[bool] = mapped_column(Boolean, default=False)

    created_at: Mapped[dt.datetime] = mapped_column(DateTime(timezone=True), default=_utcnow)

    session: Mapped["SessionRecord"] = relationship(back_populates="stiffness_maps")


class VerdictRecord(Base):
    """FR-41/42, DR-01/02: 3점 × 레이어. FAIL 이면 waveform 필수."""

    __tablename__ = "verdicts"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, autoincrement=True)
    session_id: Mapped[str] = mapped_column(String, ForeignKey("sessions.id"), nullable=False)

    layer_index: Mapped[int] = mapped_column(Integer, nullable=False)
    point_label: Mapped[str] = mapped_column(String, nullable=False)  # center/left/right
    x: Mapped[float] = mapped_column(Float, nullable=False)
    y: Mapped[float] = mapped_column(Float, nullable=False)

    release_force_n: Mapped[float] = mapped_column(Float, nullable=False)
    threshold_n: Mapped[float] = mapped_column(Float, nullable=False)
    result: Mapped[str] = mapped_column(String, nullable=False)  # PASS/FAIL/SKIP

    # ForceSample[] 직렬화. FAIL 시 필수(DR-01), PASS/SKIP 이면 NULL 허용.
    waveform: Mapped[list | None] = mapped_column(JSONB, nullable=True)

    measured_at: Mapped[dt.datetime] = mapped_column(DateTime(timezone=True), default=_utcnow)

    session: Mapped["SessionRecord"] = relationship(back_populates="verdicts")


class EventRecord(Base):
    """FR-44, DR-04: 상태 전이·에러 등 시계열 이벤트."""

    __tablename__ = "events"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, autoincrement=True)
    session_id: Mapped[str] = mapped_column(String, ForeignKey("sessions.id"), nullable=False)

    ts: Mapped[dt.datetime] = mapped_column(DateTime(timezone=True), default=_utcnow)
    mtype: Mapped[str] = mapped_column(String, nullable=False)  # 예: "state", "error", "safety"
    detail: Mapped[dict] = mapped_column(JSONB, default=dict)

    session: Mapped["SessionRecord"] = relationship(back_populates="events")
