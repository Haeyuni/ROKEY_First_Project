"""web.md 백엔드 진입점.

하이브리드 아키텍처: rosbridge_server(nail_bridge, 포트 9090)를 실시간
중계 레이어로 그대로 쓰고, 이 FastAPI는
  1) roslibpy로 rosbridge에 붙어 릴레이 토픽을 구독 + `/ws`로 재중계
     (type-envelope, 접속 시 스냅샷 — web.md §4.3, IR-05)
  2) REST(sessions/health)와 Postgres 저장을 담당한다.

실행 전제: nail_bridge의 rosbridge_websocket이 먼저 떠 있어야 한다.
  ros2 launch nail_bridge web_bridge.launch.py
"""

from __future__ import annotations

import asyncio
import logging
from contextlib import asynccontextmanager

from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware

from . import persistence
from .config import settings
from .db import init_db
from .ros_bridge import RosBridgeClient
from .routers import health, sessions
from .ws_manager import WsConnectionManager

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger("nail_web.main")


@asynccontextmanager
async def lifespan(app: FastAPI):
    await init_db()

    ws_manager = WsConnectionManager()
    loop = asyncio.get_event_loop()

    def on_relay_message(ws_type: str, data: dict) -> None:
        # roslibpy 콜백 스레드에서 call_soon_threadsafe로 넘어온 뒤,
        # 여기서부터는 이벤트 루프 위이므로 안전하게 태스크를 만들 수 있다.
        asyncio.create_task(ws_manager.broadcast({"type": ws_type, "data": data}))
        # Day2: 실시간 중계와 별개로 상태전이를 Postgres에 반영한다(FR-44).
        # 저장 실패가 공정에 영향을 주면 안 되므로(FR-45) 예외를 여기서
        # 흡수한다 — WS 브로드캐스트는 저장 성패와 무관하게 이미 끝났다.
        if ws_type == "state":
            session_id = data.get("session_id", "")
            asyncio.create_task(_safe(persistence.log_event(session_id, "state", data)))

    async def _safe(coro) -> None:
        try:
            await coro
        except Exception:
            logger.exception("DB 저장 실패 — 공정/중계에는 영향 없음 (FR-45)")

    def on_session_result(session_id: str, result: dict) -> None:
        asyncio.create_task(ws_manager.broadcast({"type": "result", "data": result}))
        asyncio.create_task(_safe(persistence.finalize_session(session_id, result)))

    ros_bridge = RosBridgeClient(
        host=settings.rosbridge_host,
        port=settings.rosbridge_port,
        loop=loop,
        on_relay_message=on_relay_message,
        on_session_result=on_session_result,
    )
    ros_bridge.connect()

    app.state.settings = settings
    app.state.ros_bridge = ros_bridge
    app.state.ws_manager = ws_manager

    if not await asyncio.to_thread(ros_bridge.wait_until_connected, 5.0):
        logger.warning(
            "rosbridge_server(%s:%s)에 5초 내 연결되지 않았습니다. "
            "nail_bridge가 떠 있는지 확인하세요 — REST API는 동작하지만 "
            "실시간 중계와 세션 시작은 실패합니다.",
            settings.rosbridge_host,
            settings.rosbridge_port,
        )

    yield

    ros_bridge.close()


app = FastAPI(title="nail-web-backend", lifespan=lifespan)

app.add_middleware(
    CORSMiddleware,
    allow_origins=settings.cors_origin_list,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(sessions.router)
app.include_router(health.router)


@app.websocket("/ws")
async def ws_endpoint(ws: WebSocket) -> None:
    ws_manager: WsConnectionManager = ws.app.state.ws_manager
    ros_bridge: RosBridgeClient = ws.app.state.ros_bridge

    await ws_manager.connect(ws, ros_bridge.get_snapshot())
    try:
        while True:
            # web.md §4.3은 서버→클라이언트 단방향 채널이다. 클라이언트가
            # 뭘 보내든 무시하고 연결 유지 목적으로만 받는다.
            await ws.receive_text()
    except WebSocketDisconnect:
        pass
    finally:
        await ws_manager.disconnect(ws)
