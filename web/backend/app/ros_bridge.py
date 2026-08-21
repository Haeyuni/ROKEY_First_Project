"""FastAPI ↔ rosbridge_server(nail_bridge) 하이브리드 클라이언트.

아키텍처 결정 (2026-08-21, 사용자 확인):
  React ──roslibjs, 직접──▶ rosbridge_server(9090, 기존 nail_bridge)
  React ──REST/WS─────────▶ FastAPI ──roslibpy(같은 9090)──▶ rosbridge_server

FastAPI는 별도 rclpy 노드가 아니다. nail_bridge/launch/web_bridge.launch.py 가
이미 rosbridge_websocket 을 열어주므로, 여기서는 그 화이트리스트 안에 있는
5개 토픽 + `/session/run` 액션 + `/safety/reset` 서비스만 roslibpy로 쓴다
(nail_bridge/config/web_bridge.yaml relay_topics / allowed_action /
allowed_service 와 반드시 같아야 함).

roslibpy는 Twisted 리액터를 백그라운드 스레드에서 돌린다. 토픽 콜백은 그
스레드에서 호출되므로, asyncio 쪽(WS 브로드캐스트)으로 넘길 때는 반드시
``asyncio.run_coroutine_threadsafe`` 를 거친다 — 그냥 코루틴을 만들기만
하고 스케줄하지 않으면 조용히 실행되지 않는다.
"""

from __future__ import annotations

import asyncio
import logging
import threading
import uuid
from collections.abc import Callable

import roslibpy

logger = logging.getLogger("nail_web.ros_bridge")

# 토픽 경로 → (ROS 메시지 타입, WebSocket 프로토콜의 `type` 필드).
# web.md §4.3 표와 정확히 일치해야 한다. `/force/data`(100Hz)는 절대 포함하지
# 않는다 (IR-02, NIS §9 "allow_force_raw 금지").
RELAY_TOPICS: dict[str, tuple[str, str]] = {
    "/safety/status": ("nail_msgs/SafetyState", "safety"),
    "/process/status": ("nail_msgs/ProcessState", "state"),
    "/stiffness/map": ("nail_msgs/StiffnessMap", "map"),
    "/validation/result": ("nail_msgs/ValidationResult", "verdict"),
    "/force/data_ui": ("nail_msgs/ForceSample", "force"),
}

RUN_SESSION_ACTION = "/session/run"
RUN_SESSION_ACTION_TYPE = "nail_msgs/action/RunSession"
RESET_SAFETY_SERVICE = "/safety/reset"
RESET_SAFETY_SERVICE_TYPE = "nail_msgs/ResetSafety"


class RunSessionTimeoutError(Exception):
    """FR-06: orchestrator 액션 서버가 3초 내 응답하지 않음."""


class RosBridgeClient:
    def __init__(
        self,
        host: str,
        port: int,
        loop: asyncio.AbstractEventLoop,
        on_relay_message: Callable[[str, dict], None],
        on_session_result: Callable[[str, dict], None],
    ) -> None:
        self._loop = loop
        self._on_relay_message = on_relay_message
        self._on_session_result = on_session_result
        self.ros = roslibpy.Ros(host=host, port=port)
        self._topics: dict[str, roslibpy.Topic] = {}
        self._action_client: roslibpy.ActionClient | None = None
        self._connected_event = threading.Event()

        # 세션별 최신 상태 스냅샷 (IR-05: 접속 즉시 safety/state/map 전송).
        # roslibpy 콜백 스레드와 FastAPI 이벤트 루프 양쪽에서 읽으므로 lock 보호.
        self._snapshot: dict[str, dict] = {}
        self._snapshot_lock = threading.Lock()

        # RunSession 진행 중인 세션의 goal_id. cancel 시 조회용.
        self._active_goal_ids: dict[str, str] = {}
        self._goal_lock = threading.Lock()

    # ---------------------------------------------------------------- 연결
    def connect(self) -> None:
        self.ros.on_ready(self._on_ready)
        self.ros.on("error", lambda err: logger.error("rosbridge 연결 오류: %s", err))
        self.ros.on("close", lambda *_: logger.warning("rosbridge 연결 끊김"))
        self.ros.run()  # non-blocking, 백그라운드 스레드에서 Twisted reactor 시작

    def close(self) -> None:
        for topic in self._topics.values():
            topic.unsubscribe()
        if self.ros.is_connected:
            self.ros.terminate()

    @property
    def is_connected(self) -> bool:
        return bool(self.ros.is_connected)

    def _on_ready(self) -> None:
        logger.info("rosbridge_server 연결됨 — 릴레이 토픽 %d개 구독 시작", len(RELAY_TOPICS))
        for topic_name, (msg_type, ws_type) in RELAY_TOPICS.items():
            topic = roslibpy.Topic(self.ros, topic_name, msg_type)
            topic.subscribe(self._make_handler(ws_type))
            self._topics[topic_name] = topic
        self._action_client = roslibpy.ActionClient(self.ros, RUN_SESSION_ACTION, RUN_SESSION_ACTION_TYPE)
        self._connected_event.set()

    def _make_handler(self, ws_type: str) -> Callable[[dict], None]:
        def handler(message: dict) -> None:
            with self._snapshot_lock:
                self._snapshot[ws_type] = message
            # roslibpy 콜백 스레드 → FastAPI 이벤트 루프로 안전하게 넘긴다.
            self._loop.call_soon_threadsafe(self._on_relay_message, ws_type, message)

        return handler

    def wait_until_connected(self, timeout_s: float) -> bool:
        return self._connected_event.wait(timeout_s)

    # -------------------------------------------------------------- 스냅샷
    def get_snapshot(self) -> dict[str, dict]:
        """IR-05: 새 WS 클라이언트 접속 시 즉시 보낼 최신 상태."""
        with self._snapshot_lock:
            return dict(self._snapshot)

    # ------------------------------------------------------------ 세션 제어
    def run_session(self, goal: dict, timeout_s: float) -> str:
        """RunSession goal 전송. 성공 시 goal_id 반환, 시간 초과 시 예외.

        FR-06 근사 구현: rosbridge/roslibpy 프로토콜에는 "액션 서버가
        떠 있는가"를 직접 조회하는 API가 없다 (ROS1 actionlib과 달리 ROS2
        액션은 rosbridge에서 wait_for_server 격의 오퍼레이션을 노출하지
        않음). 대신 goal 전송 후 첫 feedback/result 콜백이 `timeout_s` 내에
        오는지로 "서버가 반응하는가"를 판단한다. 서버가 정말 없으면 goal이
        영원히 응답을 안 주므로 이 방식으로도 사실상 같은 효과를 낸다 —
        단, "정확히 3초"가 아니라 "3초 내 첫 응답 없음"이라는 점은 주의.
        """
        if self._action_client is None:
            raise RunSessionTimeoutError("rosbridge 미연결 — 액션 클라이언트 없음")

        session_id = goal["session_id"]
        first_response = threading.Event()

        def _feedback(_msg: dict) -> None:
            first_response.set()

        def _result(msg: dict) -> None:
            # RunSession의 진짜 종료(COMPLETED/FAILED/ABORTED_SAFETY/CANCELLED).
            # 이걸 놓치면 세션이 DB에서 영원히 "진행 중"으로 남아 FR-04
            # (중복 세션 거부)가 이후 모든 세션 생성을 막아버린다.
            first_response.set()
            with self._goal_lock:
                self._active_goal_ids.pop(session_id, None)
            self._loop.call_soon_threadsafe(self._on_session_result, session_id, msg)

        def _err(_msg: dict) -> None:
            first_response.set()

        goal_id = self._action_client.send_goal(roslibpy.Goal(goal), _result, _feedback, _err)
        if goal_id is None:
            raise RunSessionTimeoutError("goal 전송 실패 (액션 클라이언트가 이미 advertise 중)")

        with self._goal_lock:
            self._active_goal_ids[goal["session_id"]] = goal_id

        if not first_response.wait(timeout_s):
            with self._goal_lock:
                self._active_goal_ids.pop(goal["session_id"], None)
            raise RunSessionTimeoutError(
                f"'{RUN_SESSION_ACTION}' 액션 서버 응답 없음 ({timeout_s}s 초과)"
            )

        return goal_id

    def cancel_session(self, session_id: str) -> bool:
        """FR-05: 취소를 ROS2 액션까지 전파."""
        with self._goal_lock:
            goal_id = self._active_goal_ids.get(session_id)
        if goal_id is None or self._action_client is None:
            return False
        self._action_client.cancel_goal(goal_id)
        return True


def make_run_session_goal(
    session_id: str,
    recipe_id: str,
    shape_profile_id: str,
    target_material: str,
    layer_total: int,
    max_rework: int,
    enable_brush: bool,
    enable_stone: bool,
) -> dict:
    """RunSession.action goal 필드 (IDS §7.1) 그대로."""
    return {
        "session_id": session_id,
        "recipe_id": recipe_id,
        "shape_profile_id": shape_profile_id,
        "target_material": target_material,
        "layer_total": layer_total,
        "max_rework": max_rework,
        "enable_brush": enable_brush,
        "enable_stone": enable_stone,
    }


def new_session_id() -> str:
    return uuid.uuid4().hex
