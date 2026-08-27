# 네일 셀 웹 시스템

현재 웹은 세션 제어, 안전·공정 상태 표시와 PostgreSQL 저장을 제공한다.

## 아키텍처

```text
React -> REST + /ws -> FastAPI -> rosbridge_server (:9090) -> ROS2
                          `-> PostgreSQL
```

- FastAPI는 rclpy 노드가 아니라 `roslibpy` 클라이언트다.
- `nail_bridge`가 ROS 자원 화이트리스트를 적용한다.
- 실시간 중계는 `/safety/status`와 `/process/status`뿐이다.
- ROS 쓰기는 `/session/run` 액션과 `/safety/reset` 서비스만 허용한다.

## 구현 범위

**백엔드**

- `GET /api/recipes`
- `POST /api/sessions`
- `POST /api/sessions/{id}/cancel`
- `GET /api/sessions/{id}/report`
- `GET /api/health`
- `/ws`: `safety`, `state`, `result` 메시지와 접속 시 최신 스냅샷
- PostgreSQL `sessions`, `events` 저장

**프론트엔드**

- E-Stop·컨트롤러 통신 안전 배너
- UV 상시 ON 경고
- 세션 시작·취소
- 연마, 브러싱, 도포, 경화와 선택 스톤 단계 표시
- 표준 에러와 세션 결과 표시

웹에는 카메라·영상 기능, 센서 그래프, 자동 품질 판정, 저수준 로봇 명령이 없다.

## 실행

### 1. 환경과 의존성

```bash
cd web/backend
cp .env.example .env
pip install -r requirements.txt
```

### 2. PostgreSQL

```bash
cd web
docker compose up -d db
```

### 3. ROS 브리지

```bash
ros2 launch nail_bridge web_bridge.launch.py
```

별도 터미널에서 `safety_monitor`, `session_orchestrator`와 필요한 공정 노드를
실행한다.

### 4. 백엔드

```bash
cd web/backend
uvicorn app.main:app --reload --port 8000
```

### 5. 프론트엔드

```bash
cd web/frontend
npm install
npm run dev
```

기본 주소는 `http://localhost:5173`이다.

## 배선 확인용 퍼블리셔

`web/backend/scripts/fake_ros_publisher.py`는 웹 배선 확인을 위해
`/safety/status`와 `/process/status`만 발행한다. 로봇 동작을 모사하지 않는다.

## 현재 제한

- rosbridge의 ROS2 action API에는 액션 서버 생존 여부를 직접 조회하는 기능이
  없어, goal 전송 후 첫 feedback/result가 설정 시간 안에 오는지로 응답 여부를
  판단한다.
- `RunSession` result에는 `session_id`가 없다. 단일 활성 세션 정책에 따라 가장
  최근 result를 현재 세션 결과로 표시한다.
- `ToolState.grip_width_mm`는 명령값이다. 웹은 실제 파지 성공으로 해석하지 않는다.

## 확인 항목

1. 세션 생성, 상태 전이, 종료 결과가 DB와 화면에 반영되는지 확인한다.
2. `safe_to_move=false`에서 시작 버튼과 SAFETY 조작 잠금을 확인한다.
3. 새로고침 후 최신 `safety`와 `state`가 즉시 표시되는지 확인한다.
4. 취소 요청이 ROS2 goal까지 전달되는지 확인한다.
5. 프론트엔드를 닫아도 ROS2 공정이 계속되는지 확인한다.
