"""FR-02~06: 세션 생성/취소.

세션 시작 판단(레시피 유효성, 형상, 안전 전제조건 등)은 전적으로
session_orchestrator가 소유한다(web.md §1.3 "웹은 관측자다"). 여기서 하는
검증은 orchestrator에 보내기 *전에* 걸러도 되는, 웹 계층에서 확정 가능한
두 가지(FR-03 허용 소재, FR-04 중복 세션) 뿐이다.
"""

from __future__ import annotations

import asyncio
import datetime as dt
import logging

from fastapi import APIRouter, Depends, HTTPException, Request
from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession

from ..db import get_db
from ..models import EventRecord, SessionRecord
from ..ros_bridge import RunSessionTimeoutError, make_run_session_goal, new_session_id
from ..schemas import (
    ALLOWED_TARGET_MATERIALS,
    CreateSessionRequest,
    CreateSessionResponse,
    EventItem,
    SessionListItem,
    SessionReportResponse,
)

logger = logging.getLogger("nail_web.sessions")

router = APIRouter(prefix="/api/sessions", tags=["sessions"])

# 레시피 개념은 웹에서 제거됐지만(레시피 CRUD·선택 UI 전부 삭제),
# RunSession.action의 recipe_id 필드는 orchestrator 쪽 계약이라 여전히
# 값이 필요하다 — orchestrator는 분기에 쓰지 않는 메타데이터라 고정값으로 채운다.
DEFAULT_RECIPE_ID = "default"

# layer_total은 orchestrator의 COAT→CURE 레이어 루프 반복 횟수를 그대로
# 결정한다(session_orchestrator_node.py: for layer_index in range(layer_total)).
# 항상 1회만 도포/경화하도록 웹에서 사용자 선택 없이 고정한다.
FIXED_LAYER_TOTAL = 1


# 관리자 대시보드: 세션 이력 목록(최신순, 페이지네이션 + 결과 코드 필터).
@router.get("", response_model=list[SessionListItem])
async def list_sessions(
    db: AsyncSession = Depends(get_db),
    limit: int = 50,
    offset: int = 0,
    result_code: str | None = None,
) -> list[SessionListItem]:
    stmt = select(SessionRecord).order_by(SessionRecord.created_at.desc()).limit(limit).offset(offset)
    if result_code is not None:
        stmt = stmt.where(SessionRecord.result_code == result_code)
    rows = (await db.execute(stmt)).scalars().all()
    return [
        SessionListItem(
            id=r.id,
            target_material=r.target_material,
            layer_total=r.layer_total,
            result_code=r.result_code,
            abort_reason=r.abort_reason,
            started_at=r.started_at,
            finished_at=r.finished_at,
            created_at=r.created_at,
        )
        for r in rows
    ]


@router.post("", response_model=CreateSessionResponse, status_code=201)
async def create_session(
    body: CreateSessionRequest,
    request: Request,
    db: AsyncSession = Depends(get_db),
) -> CreateSessionResponse:
    # FR-03: 허용 목록 밖 target_material은 400.
    if body.target_material not in ALLOWED_TARGET_MATERIALS:
        raise HTTPException(
            status_code=400,
            detail=f"target_material은 {ALLOWED_TARGET_MATERIALS} 중 하나여야 합니다",
        )

    # FR-04: 진행 중(result_code IS NULL) 세션이 있으면 409.
    active = await db.execute(
        select(SessionRecord).where(SessionRecord.result_code.is_(None)).limit(1)
    )
    if active.scalar_one_or_none() is not None:
        raise HTTPException(status_code=409, detail="이미 진행 중인 세션이 있습니다")

    session_id = new_session_id()
    record = SessionRecord(
        id=session_id,
        recipe_id=DEFAULT_RECIPE_ID,
        shape_profile_id=body.shape_profile_id,
        target_material=body.target_material,
        layer_total=FIXED_LAYER_TOTAL,
        started_at=dt.datetime.now(dt.timezone.utc),
    )
    db.add(record)
    await db.commit()

    goal = make_run_session_goal(
        session_id=session_id,
        recipe_id=DEFAULT_RECIPE_ID,
        shape_profile_id=body.shape_profile_id,
        target_material=body.target_material,
        layer_total=FIXED_LAYER_TOTAL,
        enable_stone=body.enable_stone,
    )

    ros_bridge = request.app.state.ros_bridge
    timeout_s = request.app.state.settings.run_session_timeout_s
    try:
        # roslibpy.send_goal은 threading.Event로 블로킹 대기하므로 이벤트
        # 루프를 막지 않게 스레드로 넘긴다.
        await asyncio.to_thread(ros_bridge.run_session, goal, timeout_s)
    except RunSessionTimeoutError as exc:
        logger.error("RunSession goal 응답 없음 (session_id=%s): %s", session_id, exc)
        record.result_code = "FAILED"
        record.abort_reason = f"ORCH_UNAVAILABLE: {exc}"
        record.finished_at = dt.datetime.now(dt.timezone.utc)
        await db.commit()
        # FR-06: 액션 서버 미기동/무응답 시 503.
        raise HTTPException(status_code=503, detail="세션 오케스트레이터가 응답하지 않습니다") from exc

    return CreateSessionResponse(session_id=session_id)


@router.post("/{session_id}/cancel", status_code=202)
async def cancel_session(
    session_id: str,
    request: Request,
    db: AsyncSession = Depends(get_db),
) -> dict:
    record = await db.get(SessionRecord, session_id)
    if record is None:
        raise HTTPException(status_code=404, detail="세션을 찾을 수 없습니다")
    if record.result_code is not None:
        raise HTTPException(status_code=409, detail="이미 종료된 세션입니다")

    ros_bridge = request.app.state.ros_bridge
    cancelled = await asyncio.to_thread(ros_bridge.cancel_session, session_id)
    if not cancelled:
        raise HTTPException(status_code=409, detail="취소할 진행 중인 goal을 찾지 못했습니다")

    # 실제 CANCELLED 확정은 RunSession result 수신 시점(persistence.finalize_session).
    return {"session_id": session_id, "status": "cancel_requested"}


@router.get("/{session_id}/report", response_model=SessionReportResponse)
async def get_session_report(
    session_id: str,
    db: AsyncSession = Depends(get_db),
) -> SessionReportResponse:
    """§4.4 GET /api/sessions/{id}/report.

    web.md §2.2가 이력 조회 화면 자체는 범위 밖("SQLite 직접 조회"로
    대체)으로 뒀지만, 이 엔드포인트는 그 대체 수단이 아니라 세션 하나의
    기본 정보를 API로 확인하기 위한 것이다. probe(강성 맵)·검증(판정) 단계
    제거로 그 결과 필드는 더 이상 없다.
    """
    session = await db.get(SessionRecord, session_id)
    if session is None:
        raise HTTPException(status_code=404, detail="세션을 찾을 수 없습니다")

    return SessionReportResponse(
        session_id=session.id,
        target_material=session.target_material,
        layer_total=session.layer_total,
        result_code=session.result_code,
        abort_reason=session.abort_reason,
        started_at=session.started_at,
        finished_at=session.finished_at,
    )


# 관리자 대시보드: 세션 하나의 시계열 이벤트(state/error/safety) 로그.
@router.get("/{session_id}/events", response_model=list[EventItem])
async def get_session_events(
    session_id: str,
    db: AsyncSession = Depends(get_db),
) -> list[EventItem]:
    session = await db.get(SessionRecord, session_id)
    if session is None:
        raise HTTPException(status_code=404, detail="세션을 찾을 수 없습니다")

    stmt = select(EventRecord).where(EventRecord.session_id == session_id).order_by(EventRecord.ts.asc())
    rows = (await db.execute(stmt)).scalars().all()
    return [EventItem(id=r.id, ts=r.ts, mtype=r.mtype, detail=r.detail) for r in rows]
