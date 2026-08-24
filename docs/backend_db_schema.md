# 백엔드 DB 구조

- 위치: `web/backend/app/models.py`, `db.py`, `persistence.py`
- ORM: SQLAlchemy 2.0 (async), 엔진: `postgresql+asyncpg`
- DB: PostgreSQL (원문 `docs/web.md` §5는 SQLite를 명시했으나 팀 결정으로 Postgres 사용 — `persistence.py`, `config.py` 주석 참고)
- 마이그레이션 도구 없음(Day1 범위): `db.py`의 `init_db()`가 `Base.metadata.create_all`로 스키마를 직접 생성. 스키마 안정화 후 alembic 도입 검토 예정.
- 연결 문자열 기본값: `postgresql+asyncpg://nail:nail@localhost:5432/nail_db` (`.env`의 `DATABASE_URL`로 재정의, `config.py`)

**갱신 2026-08-24**: probe(강성 맵)·검증(3점 판정) 단계 시각화를 웹에서
제거하면서 `stiffness_maps`/`verdicts` 테이블과 관련 저장 로직을
완전히 삭제했다(`docs/web.md` §5 갱신 내역 참고). ROS2 쪽에서는 해당
데이터를 계속 생성하지만, 웹 백엔드는 더 이상 구독·저장하지 않는다.

## ERD 개요

```
sessions (1) ──< events (N)
```

`events.session_id`는 FK로 `sessions.id`를 참조한다. ORM 관계는
`SessionRecord` 쪽에서 `relationship()`으로 정의되어 있고, 자식 → 부모
방향 `back_populates`만 존재한다(양방향).

---

## 1. `sessions`

세션(작업 1회) 마스터 테이블. `SessionRecord` (`models.py:21`)

| 컬럼 | 타입 | 제약 | 설명 |
|---|---|---|---|
| `id` | String | PK, default=`uuid4().hex` | 세션 ID |
| `recipe_id` | String | NOT NULL | 사용한 레시피 ID |
| `shape_profile_id` | String | NULL 허용 | 형상 프로파일 ID |
| `target_material` | String | NOT NULL | 대상 소재 |
| `layer_total` | Integer | default=2 | 총 레이어 수 |
| `result_code` | String | NULL 허용 | `RunSession.action` 결과 코드: `COMPLETED` / `COMPLETED_WITH_WARN` / `FAILED` / `ABORTED_SAFETY` / `CANCELLED`. 진행 중에는 NULL |
| `abort_reason` | Text | NULL 허용 | 실패/중단 사유 (`finalize_session`에서 `"{code}: {detail}"` 형태로 기록) |
| `started_at` | DateTime(tz) | NULL 허용 | 시작 시각 |
| `finished_at` | DateTime(tz) | NULL 허용 | 종료 시각 (`finalize_session` 호출 시 기록) |
| `created_at` | DateTime(tz) | default=now(UTC) | 생성 시각 |

**주의(코드 주석 기반)**: `finalize_session()`이 호출되지 않으면 `result_code`가 계속 NULL로 남아 세션이 "진행 중" 상태로 오인된다(FR-04 중복 세션 거부 로직과 연관된 Day1 결함 이력).

## 2. `events`

세션 시계열 이벤트(상태 전이, 에러, 안전 등). `EventRecord` (`models.py:42`) — FR-44, DR-04

| 컬럼 | 타입 | 제약 | 설명 |
|---|---|---|---|
| `id` | Integer | PK, autoincrement | |
| `session_id` | String | FK → `sessions.id`, NOT NULL | |
| `ts` | DateTime(tz) | default=now(UTC) | 이벤트 시각 |
| `mtype` | String | NOT NULL | 이벤트 종류 예: `state`, `error`, `safety` |
| `detail` | JSONB | default=`{}` | 이벤트 상세 |

---

## 데이터 흐름 (참고)

`persistence.py`는 ROS 브릿지(roslibpy 콜백 → `asyncio.create_task`)에서 호출되어 FastAPI 요청 컨텍스트 밖이므로, `Depends(get_db)` 대신 `SessionLocal()`을 직접 연다.

- `finalize_session(session_id, result)` — `RunSession` 결과 수신 시 `sessions.result_code`/`abort_reason`/`finished_at` 갱신
- `log_event(session_id, mtype, detail)` — 상태 전이 등을 `events`에 insert
