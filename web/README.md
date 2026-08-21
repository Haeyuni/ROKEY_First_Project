# 네일 셀 웹 시스템 — Day1

`docs/web.md` §8.1 인도 순서 1~3(안전 배너, 강성 히트맵, 세션 시작/취소)의
축소 불가 항목을 우선 구현했습니다.

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

## 구현 범위 (Day1)

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

**프론트엔드** (`web/frontend`)
- 안전 배너: 상단 상시 표시, `safe_to_move=false` 시 시작 버튼 비활성화,
  `active_faults` 한국어 표시 (FR-30~32)
- UV 상시 점등 경고 배너 (FR-34)
- 강성 히트맵: coarse/fine 크기·테두리 구분, 강성 색상 스케일, 증분 렌더링
  (FR-11~13, NFR-02)
- WebSocket 2초 자동 재연결 (IR-06)
- 세션 시작/취소 최소 UI (레시피·소재 선택 — 전체 폼은 Day2)

Day2/3 항목(6단계 진행 상세 표시, 3점 판정, 힘 그래프, DB 반영 완성,
에러 코드 최종 확정 등)은 범위 밖입니다.

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
  (`CreateTable(...).compile(dialect=postgresql.dialect())`)
- 프론트엔드: `npm run build` 성공 (tsc 타입체크 + vite 프로덕션 빌드)
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
- 세션 종료(RunSession result) → DB 반영, 판정/강성맵/이벤트 저장,
  에러 코드 한국어 문구 최종본은 Day2/3 작업입니다.
