# 웹 시스템 개발 요구사항 정의서

**문서 ID** WRD-NAIL-v1.1 · **개정일** 2026-08-24

## 1. 목적과 범위

웹은 ROS2 세션을 시작·취소하고, 공정과 안전 상태를 표시하며 결과와 이벤트를
PostgreSQL에 저장한다. 로봇 제어 판단과 저수준 동작은 ROS2가 소유한다.

```text
React -> REST/WebSocket -> FastAPI -> rosbridge_server -> ROS2
                              `-> PostgreSQL
```

웹 범위는 다음과 같다.

- 레시피 조회와 세션 시작·취소
- `PRECHECK`, `SAND`, `BRUSH`, `COAT`, `CURE`, 선택 `STONE`, 종료 상태 표시
- E-Stop·컨트롤러 통신 기반 안전 상태와 에러 표시
- UV 상시 ON 경고
- 세션 결과와 상태 전이 이벤트 저장

웹에는 카메라 영상, 자동 품질 판정, 센서 그래프 또는 저수준 로봇 조작 화면이
없다.

## 2. 기능 요구사항

| ID | 요구사항 | 인수 기준 |
|---|---|---|
| FR-01 | 레시피 목록 조회 | 선택 목록 표시 |
| FR-02 | 세션 생성 | `RunSession` goal 전송과 DB 레코드 생성 |
| FR-03 | 소재 제한 | `silicone_model`, `artificial_tip` 외 요청은 400 |
| FR-04 | 단일 세션 | 진행 중 세션이 있으면 409 |
| FR-05 | 세션 취소 | ROS2 action cancel까지 전파 |
| FR-06 | 오케스트레이터 응답 확인 | 설정 시간 내 첫 응답이 없으면 503 |
| FR-10 | 공정 상태 표시 | `/process/status`를 1초 내 반영 |
| FR-30 | 안전 배너 | 모든 화면 상단에 표시 |
| FR-31 | 안전 차단 | `safe_to_move=false`이면 시작 비활성화 |
| FR-32 | 결함 표시 | `active_faults` 전체 표시 |
| FR-33 | 심각도 잠금 | `last_error.severity >= SEV_SAFETY`이면 조작 잠금 |
| FR-34 | UV 경고 | 상시 ON 경고 배너 유지 |
| FR-35 | 에러 설명 | 표준 에러를 한국어로 표시 |
| FR-40 | 세션 저장 | 시작·종료·결과·중단 사유 기록 |
| FR-44 | 이벤트 저장 | `ProcessState` 전이 기록 |
| FR-45 | 저장 장애 격리 | DB 오류가 ROS2 공정을 직접 중단하지 않음 |

## 3. ROS와 WebSocket 계약

FastAPI가 구독하고 WebSocket `/ws`로 중계하는 토픽은 두 개다.

| ROS 토픽 | 타입 | WS `type` | 용도 |
|---|---|---|---|
| `/safety/status` | `nail_msgs/SafetyState` | `safety` | 안전 배너와 시작 차단 |
| `/process/status` | `nail_msgs/ProcessState` | `state` | 단계·진행률·현재 툴·에러 |

`ProcessState` 필드(`nail_msgs/ProcessState.msg`):

| 필드 | 타입 | 설명 |
|---|---|---|
| `session_id` | string | 현재 세션 ID |
| `stage` | string | `IDLE`\|`PRECHECK`\|`SAND`\|`BRUSH`\|`COAT`\|`CURE`\|`STONE`\|`FINISH`\|`ABORTED` |
| `layer_index` | int32 | 진행 중인 레이어(0-base) |
| `layer_total` | int32 | 전체 레이어 수 |
| `stage_percent` | float64 | 현재 단계 진행률(0–100), FR-10 |
| `session_percent` | float64 | 전체 세션 진행률(0–100), FR-10 |
| `current_tool` | string | 장착 중인 툴 |
| `last_error` | `nail_msgs/ErrorCode` | `code`/`severity`/`detail` — FR-32·33·35 판단 기준 |

세션 종료 시 백엔드는 `RunSession` result를 `type: result`로 전송한다. 별도
`error` 채널은 필요하지 않으며 현재 에러는 `ProcessState.last_error`를 쓴다.
새 WebSocket 연결에는 최신 `safety`와 `state` 스냅샷을 즉시 보낸다.

허용 ROS 쓰기 계약:

| 종류 | 이름 | 용도 |
|---|---|---|
| Action | `/session/run` | goal, feedback, result, cancel |
| Service | `/safety/reset` | 운영자 확인 후 래치 결함 reset |

웹에서 `MoveTo`, `PickPlace` 또는 공정별 액션을 직접 노출하지 않는다.

## 4. REST 계약

| Method | Path | 설명 |
|---|---|---|
| `POST` | `/api/sessions` | 세션 생성과 시작 |
| `POST` | `/api/sessions/{id}/cancel` | 취소 요청 |
| `GET` | `/api/sessions/{id}/report` | 세션 기본 결과 |
| `GET` | `/api/sessions` | 세션 목록(관리자 대시보드, 페이지네이션·결과 필터) |
| `GET` | `/api/sessions/{id}/events` | 세션 이벤트 로그(관리자 대시보드) |
| `GET` | `/api/health` | ROS·DB 상태 |

레시피 개념은 제거됐다(웹 레이어에서 recipes.yaml·선택 UI·CRUD 전부 삭제).
`RunSession.action`의 `recipe_id` 필드는 로봇팀과의 계약이라 값은 여전히
필요해서, 백엔드가 고정값(`"default"`)을 채워 보낸다.

한 번만 코팅하는 제품이라 레이어 개념도 없다. `RunSession.action`의
`layer_total` 필드는 계약상 남아 있지만 백엔드가 항상 `1`로 고정해서
보낸다 — 프론트는 `layer_total`을 요청 본문에 넣지 않는다.

`POST /api/sessions` 입력은 `shape_profile_id`, `target_material`,
`enable_stone`이다. report는 세션 ID, 소재, 결과 코드, 중단 사유와
시작·종료 시각만 반환한다.

## 5. 저장 계약

PostgreSQL의 현재 저장 단위는 다음과 같다.

- `sessions`: 세션 입력, 시작·종료 시각, 결과 코드, 중단 사유
- `events`: `ProcessState` 전이와 에러 이벤트

로봇 동작의 성공은 계약된 명령이 완료됐다는 뜻이다. 실제 파지, 도포 상태,
경화 상태 또는 스톤 부착 상태를 웹 결과에서 측정 완료로 표현하지 않는다.

## 6. 비기능 요구사항

- 상태 반영 지연 1초 이내
- WebSocket 단절 시 2초 간격 재연결
- 웹 연결이 끊겨도 ROS2 세션은 계속 진행
- 최신 Chrome 지원
- `nail_bridge` 화이트리스트 밖 ROS 자원 접근 금지
- UV 램프는 소프트웨어로 제어하지 않음

## 7. 인수 기준

1. 세션 시작 후 공정 단계와 결과가 화면과 DB에 반영된다.
2. 안전 결함에서 시작이 차단되고 모든 `active_faults`가 표시된다.
3. 브라우저 재접속 시 최신 안전·공정 상태가 즉시 복원된다.
4. 취소가 `/session/run`에서 하위 액션까지 전파된다.
5. 웹 종료 후에도 ROS2 세션이 독립적으로 진행된다.
6. UI와 API가 실제 센서 측정이나 자동 품질 판정을 주장하지 않는다.
