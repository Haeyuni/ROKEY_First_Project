"""web.md §4.3: 단일 채널 `/ws`, `type` 필드로 분기.

IR-05: 클라이언트 접속 시 서버는 최신 safety/state/map을 즉시 전송한다
(스냅샷). IR-06(2초 재연결)은 프론트 책임이지만, 서버는 매 연결마다 스냅샷을
다시 보내는 것으로 그 전제를 충족시킨다.
"""

from __future__ import annotations

import asyncio
import logging

from fastapi import WebSocket

logger = logging.getLogger("nail_web.ws_manager")

# IR-05 스냅샷 대상 3종. verdict/force/error/result는 이벤트성이라 스냅샷
# 대상이 아니다(web.md §4.3 표 "빈도" 열 참고).
SNAPSHOT_TYPES = ("safety", "state", "map")


class WsConnectionManager:
    def __init__(self) -> None:
        self._clients: set[WebSocket] = set()
        self._lock = asyncio.Lock()

    async def connect(self, ws: WebSocket, snapshot: dict[str, dict]) -> None:
        await ws.accept()
        async with self._lock:
            self._clients.add(ws)
        for ws_type in SNAPSHOT_TYPES:
            data = snapshot.get(ws_type)
            if data is not None:
                await self._send_safe(ws, {"type": ws_type, "data": data})

    async def disconnect(self, ws: WebSocket) -> None:
        async with self._lock:
            self._clients.discard(ws)

    async def broadcast(self, message: dict) -> None:
        async with self._lock:
            clients = list(self._clients)
        for ws in clients:
            await self._send_safe(ws, message)

    async def _send_safe(self, ws: WebSocket, message: dict) -> None:
        try:
            await ws.send_json(message)
        except Exception:
            logger.debug("WS 전송 실패 — 연결 해제된 클라이언트로 간주", exc_info=True)
            async with self._lock:
                self._clients.discard(ws)
