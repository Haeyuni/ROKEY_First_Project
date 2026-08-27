from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    """web.md §4/§5, NIS §9 을 코드로 옮긴 설정값.

    DB는 web.md §5 원문(SQLite)이 아니라 팀 결정에 따라 Postgres를 쓴다.
    """

    model_config = SettingsConfigDict(env_file=".env", env_file_encoding="utf-8")

    database_url: str = "postgresql+asyncpg://nail:nail@localhost:5432/nail_db"

    # nail_bridge 가 띄우는 rosbridge_websocket 접속 정보. FastAPI는 별도
    # rclpy 노드가 아니라 이 포트로 붙는 rosbridge 클라이언트다 (하이브리드).
    rosbridge_host: str = "localhost"
    rosbridge_port: int = 9090

    # FR-06: "orchestrator 액션 서버가 없으면 시작을 거부한다 / 3초 대기 후 503"
    run_session_timeout_s: float = 3.0

    cors_origins: str = "http://localhost:5173"

    @property
    def cors_origin_list(self) -> list[str]:
        return [o.strip() for o in self.cors_origins.split(",") if o.strip()]


settings = Settings()
