# Nailbot — 자동 네일아트 로봇 시스템

두산로보틱스 ROKEY 9기 그룹A 1조 · Doosan M0609 기반 자동 네일아트 로봇

---

## 1. 시스템 설계

![시스템 아키텍처](docs/diagrams/nailbot_system_architecture.png)

### 1.1 계층 구조

| 계층 | 패키지 | 노드 | 역할 |
|---|---|---|---|
| 웹 | — | React / FastAPI | 키오스크·관리자 화면, 세션 REST + WebSocket 중계, 이력 저장 |
| D · 게이트웨이 | `nail_bridge` | `web_bridge_node` | rosbridge_websocket을 화이트리스트로 감싼 웹↔ROS 유일 통로 |
| C · 세션 | `nail_orchestrator` | `session_orchestrator` | 단계 순서·단계별 타임아웃·취소 전파 상태머신 |
| B · 공정 | `nail_process` | `sanding` `brushing` `coating` `curing` `stone` | 공정별 ActionServer |
| A · 스킬 | `nail_skill` | `robot_skill_node` `tool_manager` | 로봇 원자 스킬(MoveTo/PickPlace/ContactPath/LateralContact), 툴 교체·TCP |
| 안전 | `nail_safety` | `safety_monitor` | E-Stop·통신두절 래치, `/safety/status` 발행, 전제조건 검증 |
| 공통 | `nail_msgs` / `nail_perception` | (노드 없음) | 인터페이스 정의(msg 5 · srv 3 · action 11), 2D 기하 유틸 |
| 기동 | `nail_bringup` | (노드 없음) | 고정 TF + 노드 조합 launch |

### 1.2 통신 인터페이스

```text
Topic     /process/status (ProcessState) · /safety/status (SafetyState)
          /tool/status (ToolState) · /robot/pose (PoseStamped, 50 Hz)
Service   /safety/validate · /safety/reset · /tool/get_info
Action    /session/run
          /process/{sand, brush, coat, cure, place_stone}
          /skill/{move_to, pick_place, contact_path, lateral_contact} · /tool/change
```

웹에 노출되는 것은 **구독 2개(`/process/status`, `/safety/status`)와
쓰기 2개(`/session/run` 액션, `/safety/reset` 서비스)뿐**이다. 나머지 ROS
자원은 `nail_bridge/config/web_bridge.yaml` 화이트리스트 밖이라 접근할 수 없다.

### 1.3 네트워크 구성

![네트워크 구성도](docs/diagrams/nailbot_network.png)

| 프로세스 | 포트 | 비고 |
|---|---|---|
| Vite 개발 서버 | 5173 | React 정적 파일 (`/` 키오스크, `/admin` 관리자) |
| FastAPI (uvicorn) | 8000 | REST + `/ws` |
| rosbridge_websocket | 9090 | `web_bridge_node` |
| PostgreSQL | 5432 | docker compose 로컬 또는 팀 공용 원격 DB |

웹·ROS 노드·DSR 드라이버가 **로봇 PC 한 대**에서 전부 돈다.

---

## 2. 플로우 차트

### 2.1 세션 진행 시퀀스

![세션 진행 시퀀스](docs/diagrams/nailbot_session_flow.png)

```text
PRECHECK → SAND → BRUSH → (COAT → CURE) × layer_total → [STONE] → FINISH
```

- `BRUSH`는 항상 실행, `STONE`은 주문의 `enable_stone` 옵션일 때만 실행한다.
- 각 단계 앞에 `TOOL_CHANGE`가 들어간다 (툴 반납 → 픽업 → TCP 활성화).
- 모든 전이에서 `ProcessState`를 `/process/status`와 `RunSession` feedback으로
  동시에 발행한다.

### 2.2 동작 순서도

![동작 순서도](docs/diagrams/nailbot_operation_flow.png)

중단 처리는 ① 툴 반납 → ② HOME(`rack_transit`) 복귀 → ③ `ABORTED` 발행
순서다. 단 **툴 반납에 실패했고 들고 있는 툴이 UV 램프면 HOME 복귀를 생략하고
현재 위치에 정지**한다 — 켜진 램프를 든 채로 이동시키지 않기 위해서다.

결과 코드는 `COMPLETED` / `COMPLETED_WITH_WARN` / `FAILED` / `ABORTED_SAFETY` /
`CANCELLED` 다섯 가지다.

---

## 3. 운영체제 환경

| 항목 | 값 |
|---|---|
| OS | Ubuntu (로봇 PC 1대에 웹·ROS·드라이버 전부 상주) |
| ROS 배포판 | **ROS 2 Jazzy** (`/opt/ros/jazzy`) |
| Python | 3.12 |
| 빌드 | colcon (`ament_python`, `nail_msgs`만 `ament_cmake`) |
| Node.js | 18+ (Vite 6 / TypeScript 5.6) |
| 브라우저 | Chrome 최신 1종만 지원 (폴리필 없음) |
| 컨테이너 | Docker Compose (PostgreSQL 16-alpine) |

### 워크스페이스 배치

이 저장소는 **DSR 드라이버와 별도의 워크스페이스**다. 두 워크스페이스를
overlay로 함께 source 해야 한다.

```text
~/ws_cobot_pjt/
├── ws_dsr/        # 두산 dsr_bringup2 · dsr_common2 · dsr_msgs2 · onrobot 드라이버
└── ws_cobot1/     # 이 저장소 (src/nail_*)
```

> `safety` · `skill` · `tool` 노드는 `DSR_ROBOT2` import에 실패하면 프로세스가
> 즉시 죽는다. **`ws_dsr`를 먼저 source 하지 않으면 `ModuleNotFoundError:
> dsr_msgs2`로 기동 자체가 안 된다.**

---

## 4. 사용한 장비 목록

![장비 연결 구성](docs/diagrams/nailbot_hardware_stack.png)

### 4.1 로봇 · 제어

| 장비 | 모델 | 비고 |
|---|---|---|
| 협동로봇 | Doosan M0609 (6축) | 이더넷으로 로봇 PC와 연결 |
| 그리퍼 | OnRobot RG2 | Modbus, `/onrobot/sendCommand` |
| 티치펜던트 | 두산 순정 | **E-Stop 내장** — 별도 비상정지 박스 없음 |
| 로봇 PC | Ubuntu · ROS 2 Jazzy | 웹·ROS·드라이버 동시 구동 |

### 4.2 툴 · 작업물

![작업대 평면도](docs/diagrams/nailbot_cell_layout.png)

| 순서 | 구성품 | ROS 키 | 보관 · 특이사항 |
|---|---|---|---|
| 1 | 네일 파일 | `sander` | 작업대 위 브래킷에 눕혀 보관, 손톱면을 곡선으로 연마 |
| 2 | 분진 제거용 붓 | `brush` | 작업대 **바깥 구멍**에 수직으로 꽂아 보관, 경유점 없이 직행 |
| 3 | 젤 네일 | `coater` | 뚜껑이 곧 붓 — `unscrew: true`(돌려 열고 반납 시 잠금) |
| 4 | UV LED 램프 | `uv` | USB 상시 전원 — **소프트웨어로 끌 수 없음** |
| 5 | 핀셋 | `tweezers` | 네일 파츠 중앙 홈을 파지, 그리퍼 폭을 코드와 일치시켜야 함 |
| — | 네일 파츠 트레이 | `stone_tray` | 받침 블록 위 투명 접시(고정 필수) |
| — | 인조 네일팁 (작업물) | `nail_local_frame` | 실리콘 손가락 모형 + 지그로 높이 확보 |

- 툴 랙과 작업물 지그는 전용 치구가 아니라 **레고 브릭**으로 구성했다.
- 작업대는 베이스플레이트 + 랩(오염 시 랩만 교체).

### 4.3 계획했으나 사용하지 않은 장비

F/T 힘·토크 센서(장착됐으나 미작동) · 손 안착 감지 센서(미장착) ·
더스트 컬렉터(미장착) · 비전 카메라(미도입).
→ 접촉 검출을 포기하고 **티칭 좌표 + `travel_limit_mm` 안전 마진** 방식으로 전환했다.

---

## 5. 의존성

### 5.1 ROS 2 (rosdep)

각 패키지 `package.xml`에 선언돼 있다. 한 번에 설치하려면:

```bash
cd ~/ws_cobot_pjt/ws_cobot1
rosdep install --from-paths src --ignore-src -r -y
```

| 패키지 | 주요 의존 |
|---|---|
| `nail_msgs` | `rosidl_default_generators`, `std_msgs`, `geometry_msgs`, `builtin_interfaces` |
| `nail_skill` | `rclpy`, `tf2_ros`, `tf2_geometry_msgs`, `onrobot_rg_msgs`, `nail_perception` |
| `nail_process` | `rclpy`, `nail_skill`, `nail_perception`, `python3-yaml` |
| `nail_safety` / `nail_orchestrator` | `rclpy`, `nail_msgs`, `std_msgs` |
| `nail_bridge` | `rosbridge_server`, `python3-yaml` |
| `nail_bringup` | `launch`, `launch_ros`, `tf2_ros`, `python3-yaml` |

외부 워크스페이스(`ws_dsr`)에서 오는 의존: `dsr_common2`(`DSR_ROBOT2`),
`dsr_msgs2`, `dsr_bringup2`, `onrobot_rg_msgs`.

### 5.2 백엔드 — `web/backend/requirements.txt`

```text
fastapi==0.115.6          uvicorn[standard]==0.34.0
sqlalchemy==2.0.36        asyncpg==0.30.0
greenlet==3.5.5           pydantic==2.10.4
pydantic-settings==2.7.0  roslibpy==2.1.0
PyYAML==6.0.2
```

> FastAPI는 rclpy 노드가 아니라 **`roslibpy` 클라이언트**로 rosbridge에 붙는다.
> 백엔드에 ROS 2 설치가 필요 없는 이유다.

### 5.3 프론트엔드 — `web/frontend/package.json`

```text
react 18.3 · react-dom 18.3
vite 6.0 · typescript 5.6 · @vitejs/plugin-react 4.3
```

---

## 6. 실행 순서

터미널마다 아래 환경을 먼저 적용한다. **순서가 중요하다.**

```bash
source /opt/ros/jazzy/setup.bash
source ~/ws_cobot_pjt/ws_dsr/install/setup.bash
source ~/ws_cobot_pjt/ws_cobot1/install/setup.bash
```

### 0. 빌드 (최초 1회 · 인터페이스 변경 시)

```bash
cd ~/ws_cobot_pjt/ws_cobot1
colcon build --packages-select nail_msgs nail_perception nail_skill \
  nail_process nail_orchestrator nail_safety nail_bridge nail_bringup
source install/setup.bash
```

설정 YAML(`targets.yaml`, `tool_rack.yaml`, `taught_*.yaml`,
`static_frames.yaml`)을 고친 뒤에도 **재빌드 + 노드 재시작**이 필요하다.

### 1. 두산 드라이버 — 터미널 A

이 저장소의 launch는 드라이버를 켜지 않는다. 실기 또는 에뮬레이터에 맞는
`dsr_bringup2`(+ OnRobot 그리퍼 드라이버)를 **먼저** 띄운다.

```bash
# ws_dsr 쪽 launch — 실제 파일명·인자는 사용 중인 dsr_bringup2 버전에 맞춘다
ros2 launch dsr_bringup2 dsr_bringup2.launch.py \
  mode:=real  name:=dsr01  model:=m0609      # 에뮬레이터는 mode:=virtual
```

- `name`(= `dsr_prefix`)은 이후 launch 인자와 동일해야 한다 (기본 `dsr01`).
- 노드가 뜬 것과 액션이 실제로 도는 것은 다르다 — 드라이버가 없어도 `nail_*`
  노드는 기동되지만(서비스 디스커버리 타임아웃 WARN만 출력), `MoveTo`·
  `PickPlace`를 쏘면 두산 서비스 응답을 무한 대기한다.

### 2. ROS 2 공정 스택 — 터미널 B

```bash
ros2 launch nail_bringup integration_bringup.launch.py
```

`frames` + 안전 + 스킬 + 툴 + 공정 5종 + 오케스트레이터를 전부 띄운다.
주요 인자:

| 인자 | 기본값 | 설명 |
|---|---|---|
| `dsr_prefix` | `dsr01` | 두산 드라이버 네임스페이스 |
| `home_target_key` | `rack_transit` | 실패·취소 시 복귀할 티칭 경유점 |
| `sanding_path_y_offset_mm` | `0.5` | 샌딩 3-Pose 전체 Y 보정 |
| `log_level` | `info` | `debug`로 주면 상세 로그 |

<details>
<summary>노드 단위로 골라 띄우기 (단위 테스트용)</summary>

```bash
# 기본: safety + skill + tool
ros2 launch nail_bringup test_bringup.launch.py

# 연마 노드만 추가 — 공정 노드는 frames 를 반드시 같이 넣을 것
ros2 launch nail_bringup test_bringup.launch.py nodes:=frames,safety,skill,tool,sanding
```

토큰: `frames` `safety` `skill` `tool` `sanding` `brushing` `coating`
`curing` `stone` `orchestrator` (또는 `all`).
**의존 노드를 자동으로 끼워 넣지 않는다** — 무엇이 켜져 있는지 항상 명시적으로
보이게 하려는 설계다.
</details>

### 3. ROS ↔ 웹 게이트웨이 — 터미널 C

```bash
ros2 launch nail_bridge web_bridge.launch.py
```

`ws://localhost:9090`에 화이트리스트가 적용된 rosbridge가 뜬다.

### 4. PostgreSQL — 터미널 D

```bash
cd web && docker compose up -d db
```

팀 공용 원격 DB를 쓰면 이 단계를 건너뛰고 `.env`의 `DATABASE_URL`만 바꾼다.

### 5. 백엔드 — 터미널 E

```bash
cd web/backend
cp .env.example .env          # 최초 1회, DATABASE_URL·ROSBRIDGE_* 확인
pip install -r requirements.txt
uvicorn app.main:app --reload --port 8000
```

### 6. 프론트엔드 — 터미널 F

```bash
cd web/frontend
npm install
npm run dev
```

| 화면 | 주소 |
|---|---|
| 고객 키오스크 | http://localhost:5173 |
| 관리자 대시보드 | http://localhost:5173/admin |
| 백엔드 헬스체크 | http://localhost:8000/api/health |

### 실행 순서 요약

```text
① dsr_bringup2 (드라이버)  →  ② integration_bringup (ROS 공정 스택)
   →  ③ web_bridge (rosbridge)  →  ④ docker compose db
   →  ⑤ uvicorn (FastAPI)  →  ⑥ npm run dev (React)
```

로봇 없이 웹 배선만 확인하려면 ①②를 건너뛰고
`web/backend/scripts/fake_ros_publisher.py`로 `/safety/status`,
`/process/status`만 발행하면 된다 (로봇 동작은 모사하지 않는다).
