"""Day2: ROS 릴레이 메시지를 Postgres에 반영.

WS 브로드캐스트나 RunSession 콜백(roslibpy 스레드 → asyncio.create_task)에서
호출되며 FastAPI 요청 컨텍스트 밖이므로, `Depends(get_db)` 대신
`SessionLocal`을 직접 연다.
"""

from __future__ import annotations

import datetime as dt
import logging

from .db import SessionLocal
from .models import EventRecord, SessionRecord

logger = logging.getLogger("nail_web.persistence")


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
