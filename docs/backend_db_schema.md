# 백엔드 DB 구조

- 위치: `web/backend/app/models.py`, `db.py`, `persistence.py`
- ORM: SQLAlchemy 2.0 (async), 엔진: `postgresql+asyncpg`
- DB: PostgreSQL (원문 `docs/web.md` §5는 SQLite를 명시했으나 팀 결정으로 Postgres 사용 — `persistence.py`, `config.py` 주석 참고)
- 마이그레이션 도구 없음(Day1 범위): `db.py`의 `init_db()`가 `Base.metadata.create_all`로 스키마를 직접 생성. 스키마 안정화 후 alembic 도입 검토 예정.
- 연결 문자열 기본값: `postgresql+asyncpg://nail:nail@localhost:5432/nail_db` (`.env`의 `DATABASE_URL`로 재정의, `config.py`)

## ERD 개요

```
sessions (1) ──< stiffness_maps (N, 실질 1:1)
    │
    ├──< verdicts (N)
    │
    └──< events (N)
```

모든 자식 테이블은 `session_id` FK로 `sessions.id`를 참조한다. ORM 관계는 `SessionRecord` 쪽에서만 `relationship()`으로 정의되어 있고, 자식 → 부모 방향 `back_populates`만 존재한다(양방향).

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

## 2. `stiffness_maps`

세션당 1건(강성 맵). `StiffnessMapRecord` (`models.py:44`) — FR-43

| 컬럼 | 타입 | 제약 | 설명 |
|---|---|---|---|
| `id` | Integer | PK, autoincrement | |
| `session_id` | String | FK → `sessions.id`, NOT NULL | |
| `points` | JSONB | default=`[]` | `StiffnessPoint[]` 직렬화 |
| `boundary` | JSONB | default=`[]` | `BoundaryRegion.boundary_polygon` |
| `forbidden` | JSONB | default=`[]` | `BoundaryRegion.forbidden_polygon` |
| `threshold_k` | Float | NULL 허용 | 강성 임계값 (N/mm) |
| `separation_margin` | Float | NULL 허용 | 분리 마진 |
| `valid` | Boolean | default=False | 유효성 판정 |
| `created_at` | DateTime(tz) | default=now(UTC) | |

저장은 upsert 방식(`persistence.upsert_stiffness_map`): `session_id`로 기존 레코드를 찾아 있으면 갱신, 없으면 새로 생성.

## 3. `verdicts`

레이어 × 지점별 판정 결과. `VerdictRecord` (`models.py:65`) — FR-41/42, DR-01/02

| 컬럼 | 타입 | 제약 | 설명 |
|---|---|---|---|
| `id` | Integer | PK, autoincrement | |
| `session_id` | String | FK → `sessions.id`, NOT NULL | |
| `layer_index` | Integer | NOT NULL | 레이어 번호 |
| `point_label` | String | NOT NULL | `center` / `left` / `right` |
| `x`, `y` | Float | NOT NULL | 측정 위치 좌표 |
| `release_force_n` | Float | NOT NULL | 이탈력(N) |
| `threshold_n` | Float | NOT NULL | 판정 임계값(N) — DR-02: 판정과 함께 반드시 저장 |
| `result` | String | NOT NULL | `PASS` / `FAIL` / `SKIP` |
| `waveform` | JSONB | NULL 허용 | `ForceSample[]` 직렬화. **DR-01: `result=FAIL`이면 필수**, PASS/SKIP이면 NULL 허용 |
| `measured_at` | DateTime(tz) | default=now(UTC) | |

`persistence.save_verdict()`는 `result == "FAIL"`인데 `waveform`이 비어 있으면 DR-01 위반 가능성으로 경고 로그만 남기고(하드 제약 아님) 저장은 진행한다.

## 4. `events`

세션 시계열 이벤트(상태 전이, 에러, 안전 등). `EventRecord` (`models.py:90`) — FR-44, DR-04

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

- `save_verdict(data)` — `ValidationResult` 수신 시마다 `verdicts`에 insert
- `upsert_stiffness_map(data)` — 강성 맵 수신 시 `stiffness_maps` upsert
- `finalize_session(session_id, result)` — `RunSession` 결과 수신 시 `sessions.result_code`/`abort_reason`/`finished_at` 갱신
- `log_event(session_id, mtype, detail)` — 상태 전이 등을 `events`에 insert
