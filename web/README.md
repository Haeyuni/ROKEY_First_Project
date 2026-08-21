# 네일 셀 웹 시스템 — Day1+2+3

`docs/web.md` §8.1 인도 순서 1~7(안전 배너, 강성 히트맵, 세션 시작/취소,
공정 진행 표시, 판정 표시, 저장, 힘 그래프) 전부를 구현했습니다.
축소 순서(§8.2) 대상 중 경계 폴리곤 오버레이(FR-14, S등급)만 남았습니다.

## 아키텍처 (하이브리드)

```
React ──roslibjs, 직접──▶ rosbridge_server (nail_bridge, 포트 9090)
React ──REST + /ws───────▶ FastAPI ──roslibpy(같은 9090)──▶ rosbridge_server
                              │
                              └──▶ Postgres (세션/판정/강성맵/이벤트)
```

- `nail_bridge`(이미 구현됨)가 `rosbridge_server`를 좁은 화이트리스트로 감싸는
  실시간 중계 레이어를 담당합니다. 이 프로젝트는 그걸 대체하지 않습니다.
- `web/backend`(FastAPI)는 별도 rclpy 노드가 아니라 rosbridge에 붙는
  클라이언트(roslibpy)입니다. REST API, `type` envelope 기반 `/ws`,
  Postgres 저장을 담당합니다.
- DB는 `docs/web.md` §5 원문(SQLite)이 아니라 **Postgres**로 결정했습니다
  (사용자 지시).

## 구현 범위

**백엔드** (`web/backend`)
- `GET /api/recipes` (FR-01)
- `POST /api/sessions` — target_material 허용 목록 검증(FR-03, 400), 중복
  세션 거부(FR-04, 409), RunSession goal 전송 + 3초 무응답 시 503(FR-06)
- `POST /api/sessions/{id}/cancel` — ROS2 액션까지 취소 전파(FR-05)
- `GET /api/health`
- `/ws` — `/safety/status`·`/process/status`·`/stiffness/map`·
  `/validation/result`·`/force/data_ui` 중계, 접속 시 safety/state/map
  스냅샷 즉시 전송(IR-05)
- Postgres 스키마: `sessions`/`stiffness_maps`/`verdicts`/`events`
  (web.md §5, `app/models.py`)
- **(Day2)** `app/persistence.py` — `ValidationResult` 수신마다 저장
  (FR-41/42, DR-01/02), `StiffnessMap`을 세션당 1건으로 upsert(FR-43),
  `ProcessState` 전이를 이벤트로 저장(FR-44)
- **(Day2)** RunSession 최종 result 수신 시 `sessions.result_code`/
  `finished_at`/`abort_reason` 반영 + `/ws`에 `result` 브로드캐스트.
  **Day1에는 이 처리가 없어서 세션이 끝나도 DB에서 "진행 중"으로 영원히
  남아 FR-04가 이후 모든 세션 생성을 막는 결함이 있었음 — Day2에서 수정.**
- **(Day3)** `GET /api/sessions/{id}/report` — 세션 1건의 스캔 결과 +
  판정 결과를 반환(§4.4). `report_note` 필드에 FR-22 문구를 고정으로
  포함해 API 응답도 화면과 같은 표현 제약을 받도록 함

**프론트엔드** (`web/frontend`)
- 안전 배너: 상단 상시 표시, `safe_to_move=false` 시 시작 버튼 비활성화,
  `active_faults` 한국어 표시 (FR-30~32)
- UV 상시 점등 경고 배너 (FR-34)
- 강성 히트맵: coarse/fine 크기·테두리 구분, 강성 색상 스케일, 증분 렌더링
  (FR-11~13, NFR-02)
- WebSocket 2초 자동 재연결 (IR-06)
- 세션 시작/취소 UI — 레시피·소재·형상 프로필·레이어 수·스톤 여부 (FR-02)
- **(Day2)** `ProcessStageStepper` — 6단계(스캔/연마/브러싱/도포/경화/검사)
  진행 표시, REWORK/PRECHECK/STONE/FINISH/ABORTED는 배지로 별도 표시(FR-10)
- **(Day2)** `VerdictPanel` — 레이어별 3점(중앙·좌·우) PASS/FAIL/SKIP,
  `threshold_n` 노출(FR-20/21), 재작업 대상 여부 표시(FR-23). 문구는
  전부 "검사한 N개 지점이 기준을 만족함"으로 고정 — "손톱 전체가 경화됨"
  같은 과대 서술 금지(FR-22)
- **(Day2)** `SessionResultBanner` — 세션 종료 결과(`RunSession.result`) 표시
- **(Day2)** FR-33: `last_error.severity >= SEV_SAFETY`면 세션 시작/취소
  UI 전체를 잠금
- **(Day3)** `ForceGraph` — 접촉력 실시간 그래프. 고정 크기 `Float32Array`
  (600점) 링버퍼로 구현해 30분 세션에도 배열이 자라지 않음(NFR-03). 20Hz
  소스 × 600점 = 정확히 30초라서, O2(그래프 표시 구간 미결사항)가 "최근
  30초"로 버퍼 크기 자체에 의해 자연히 확정됨
- **(Day3)** `ErrorBanner` — `ProcessState.last_error`를 한국어로 변환해
  표시(FR-35). 별도 `error` WS 채널을 새로 만들지 않고 이미 오는
  `state` 메시지의 필드를 그대로 씀
- **(Day3)** `SafetyBanner`/`StiffnessHeatmap`/`VerdictPanel`/
  `ProcessStageStepper`/`SessionResultBanner`/`SessionStart`를
  `React.memo`로 감쌈 — `latestForce`가 20Hz로 바뀔 때마다 App 전체가
  리렌더되는데, 이 컴포넌트들의 props는 그 갱신과 무관하므로 불필요한
  재계산을 막기 위함

축소 순서(§8.2) 대상인 FR-14(경계 폴리곤 오버레이)만 구현하지 않았습니다.
O3(에러 코드 한국어 문구 최종본)은 팀 리뷰가 필요해 스텁 상태로 남아
있습니다 — `error_codes.py`/`faultMessages.ts` 두 곳을 함께 바꾸세요.

## 실행

### 0. 사전 준비
```bash
cd web/backend
cp .env.example .env   # 필요 시 값 수정
pip install -r requirements.txt
```

### 1. Postgres
```bash
cd web
docker compose up -d db
```
(docker가 없다면 로컬 Postgres에 `nail`/`nail`/`nail_db`로 맞춰 접속 정보를
`.env`의 `DATABASE_URL`에 넣으세요.)

### 2. ROS2 그래프 + rosbridge
```bash
# nail_msgs 빌드 후
ros2 launch nail_bridge web_bridge.launch.py
# + safety_monitor, session_orchestrator 등 필요한 노드
```

### 3. 백엔드
```bash
cd web/backend
uvicorn app.main:app --reload --port 8000
```

### 4. 프론트엔드
```bash
cd web/frontend
npm install
npm run dev   # http://localhost:5173
```

### (선택) 로봇 없이 웹 파이프라인만 확인
`web/backend/scripts/fake_ros_publisher.py` — `/safety/status`,
`/process/status`, `/stiffness/map`에 더미 데이터를 발행합니다.
NIS §10 `mock_robot_driver`(로봇 동역학까지 흉내내는 정식 mock)를
대체하지 않는, 배선 확인용 최소 스크립트입니다.

## 검증 상태 (중요)

이 샌드박스 환경에는 **docker, rosbridge_server, 빌드된 ROS2 워크스페이스가
없어** 전체 파이프라인을 실제로 붙여서 테스트하지 못했습니다. 대신 아래는
확인했습니다.

- 백엔드: 전체 모듈 import 성공, FastAPI 앱 구성 성공, `/api/recipes`
  라우터 단독 스모크테스트 통과, ORM 모델 → Postgres DDL 컴파일 성공
  (`CreateTable(...).compile(dialect=postgresql.dialect())`) — Day2 이후도
  동일하게 재확인
- 프론트엔드: `npm run build` 성공 (tsc 타입체크 + vite 프로덕션 빌드) —
  Day3 이후도 동일하게 재확인
- **`persistence.py`(verdict/map 저장, 세션 종료 반영), `/ws`의 `verdict`/
  `result`/`force` 처리, `GET /sessions/{id}/report`는 Postgres·rosbridge가
  없어 실제 데이터로 실행 검증은 못 했습니다.** 코드 리뷰 + DDL 호환성
  확인까지만 했습니다.
- `ForceGraph`의 링버퍼 로직은 20Hz 실데이터 없이 정적 코드 리뷰로만
  확인했습니다 — 30분 연속 세션에서의 실측 메모리 프로파일링은 못 했습니다.
- `nail_msgs` colcon 빌드는 이 샌드박스의 anaconda/시스템 python3 충돌로
  실패했습니다 (`ModuleNotFoundError: catkin_pkg`, cmake가 anaconda
  python3를 고정 참조). 웹 개발 범위 밖의 환경 이슈이므로 로컬 개발 머신에서
  `colcon build`를 다시 시도하세요. rclpy 자체는 `/usr/bin/python3` +
  `source /opt/ros/jazzy/setup.bash` 조합으로는 정상 import되는 것을
  확인했습니다 — anaconda python3로는 `GLIBCXX_3.4.30` 누락으로 실패합니다.

## 알려진 근사/제한

- **FR-06 "3초 대기 후 503"**: rosbridge/roslibpy 프로토콜에는 ROS2 액션
  서버의 생존 여부를 직접 조회하는 기능이 없습니다 (`ros_bridge.py`
  `run_session()` docstring 참고). 대신 goal 전송 후 첫 feedback/result가
  timeout 내에 오는지로 판단합니다 — 서버가 정말 없으면 결과가 영영 안 오므로
  실질적으로 같은 효과지만, "정확히 3초"가 보장되는 타임아웃은 아닙니다.
- `recipes.yaml`은 저장소 어디에도 없어 `web/backend/app/data/recipes.yaml`에
  개발용 스텁을 만들었습니다. 실제 레시피 정의가 나오면 교체하세요.
- **RunSession result에는 `session_id` 필드가 없습니다** (IDS §7.1). 그래서
  `SessionResultBanner`는 특정 세션과 엄격히 연결하지 않고 "가장 최근에 온
  결과"를 보여줍니다 — 동시에 여러 세션이 겹칠 일이 없는 이 프로젝트
  전제(단일 운영자, FR-04로 세션 1개 제한)에서는 문제되지 않습니다.
- `GET /api/sessions/{id}/report`가 반환하는 `stiffness_map.points`는
  `StiffnessPoint[]`를 JSONB로 그대로 저장한 것을 그대로 돌려줍니다 —
  대용량 세션에서는 응답이 커질 수 있어, 실사용 전에 페이지네이션이나
  points 생략 옵션이 필요할 수 있습니다(Day3 스코프에서는 다루지 않음).

## 인수 기준 체크리스트 (web.md §9)

인프라(Postgres/rosbridge_server/빌드된 ROS2 워크스페이스)가 없어 이
샌드박스에서는 실행하지 못했습니다. 로컬 개발 환경에서 아래 순서로
확인하세요.

| # | 항목 | 확인 방법 |
|---|---|---|
| 1 | 가짜 퍼블리셔로 세션 시작 → 히트맵 → 판정 → 저장 | `fake_ros_publisher.py` 실행 후 프론트에서 세션 시작, `GET /api/sessions/{id}/report`로 저장 확인 |
| 2 | 안전 fault 주입 시 UI 잠금 + fault 전체 표시 | `/safety/status`에 `active_faults` 채워 발행 → SafetyBanner·SessionStart 잠금 확인 |
| 3 | 브라우저 강제 종료 후 재접속 시 상태 즉시 복원 | 새로고침 후 SafetyBanner/StiffnessHeatmap/ProcessStageStepper가 스냅샷(IR-05)으로 즉시 채워지는지 확인 |
| 4 | 세션 취소가 ROS2 액션까지 전파 | `POST /api/sessions/{id}/cancel` → orchestrator가 실제 CANCELED goal을 받는지 로그 확인 |
| 5 | 웹 종료 시에도 ROS2 세션 정상 진행 | 프론트/백엔드 종료 후에도 orchestrator가 계속 진행하는지 확인 (rosbridge 특성상 설계상 보장, `web/README.md` 상단 아키텍처 참고) |
| 6 | FAIL 판정의 파형과 임계값이 DB에 존재 | `verdicts.waveform`(FAIL 시 NOT NULL), `verdicts.threshold_n` 확인 |
| 7 | 리포트 문구가 FR-22 준수 | `VerdictPanel` 캡션, `SessionReportResponse.report_note` 확인 |
| 8 | 30분 세션에서 브라우저 메모리 누수 없음 | Chrome DevTools Memory 탭에서 30분 세션 전후 힙 비교 (`ForceGraph`는 고정 버퍼라 이론상 안전) |
| 9 | M등급 요구사항 전 항목 충족 | web.md §3의 M 표시 항목을 위 표와 대조 |
