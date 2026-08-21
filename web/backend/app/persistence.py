"""Day2: ROS 릴레이 메시지를 Postgres에 반영.

WS 브로드캐스트나 RunSession 콜백(roslibpy 스레드 → asyncio.create_task)에서
호출되며 FastAPI 요청 컨텍스트 밖이므로, `Depends(get_db)` 대신
`SessionLocal`을 직접 연다.
"""

from __future__ import annotations

import datetime as dt
import logging

from sqlalchemy import select

from .db import SessionLocal
from .models import EventRecord, SessionRecord, StiffnessMapRecord, VerdictRecord

logger = logging.getLogger("nail_web.persistence")


async def save_verdict(data: dict) -> None:
    """FR-41/42, DR-01/02: ValidationResult 수신 시마다 저장."""
    session_id = data.get("session_id")
    if not session_id:
        logger.warning("verdict에 session_id가 없어 저장을 건너뜁니다: %s", data)
        return

    result = data.get("result", "")
    waveform = data.get("waveform") or None
    if result == "FAIL" and not waveform:
        logger.warning(
            "FAIL 판정인데 waveform이 비어 있습니다 (DR-01 위반 가능, session=%s)", session_id
        )

    position = data.get("position") or {}
    record = VerdictRecord(
        session_id=session_id,
        layer_index=data.get("layer_index", 0),
        point_label=data.get("point_label", ""),
        x=position.get("x", 0.0),
        y=position.get("y", 0.0),
        release_force_n=data.get("release_force_n", 0.0),
        threshold_n=data.get("threshold_n", 0.0),
        result=result,
        waveform=waveform,
    )
    async with SessionLocal() as db:
        db.add(record)
        await db.commit()


async def upsert_stiffness_map(data: dict) -> None:
    """FR-43: 세션당 1건 — 있으면 갱신, 없으면 새로 만든다."""
    session_id = data.get("session_id")
    if not session_id:
        logger.warning("map에 session_id가 없어 저장을 건너뜁니다")
        return

    region = data.get("region") or {}
    async with SessionLocal() as db:
        existing = await db.execute(
            select(StiffnessMapRecord).where(StiffnessMapRecord.session_id == session_id)
        )
        record = existing.scalar_one_or_none()
        if record is None:
            record = StiffnessMapRecord(session_id=session_id)
            db.add(record)

        record.points = data.get("points", [])
        record.boundary = region.get("boundary_polygon", [])
        record.forbidden = region.get("forbidden_polygon", [])
        record.threshold_k = data.get("threshold_k_n_per_mm")
        record.separation_margin = data.get("separation_margin")
        record.valid = data.get("valid", False)
        await db.commit()


async def finalize_session(session_id: str, result: dict) -> None:
    """RunSession result 수신 시 세션 종료 반영.

    FR-04(중복 세션 거부)가 정상 동작하려면 이 함수가 반드시 호출되어야
    한다 — 안 부르면 result_code가 NULL로 남아 세션이 영원히 "진행 중"으로
    간주된다 (Day1에서 남아 있던 결함).
    """
    async with SessionLocal() as db:
        record = await db.get(SessionRecord, session_id)
        if record is None:
            logger.warning("finalize_session: 알 수 없는 session_id=%s", session_id)
            return
        record.result_code = result.get("result_code") or "UNKNOWN"
        final_error = result.get("final_error") or {}
        if final_error.get("code"):
            record.abort_reason = f"{final_error['code']}: {final_error.get('detail', '')}"
        record.finished_at = dt.datetime.now(dt.timezone.utc)
        await db.commit()


async def log_event(session_id: str, mtype: str, detail: dict) -> None:
    """FR-44, DR-04: 상태 전이 등 시계열 이벤트."""
    if not session_id:
        return
    async with SessionLocal() as db:
        db.add(EventRecord(session_id=session_id, mtype=mtype, detail=detail))
        await db.commit()
