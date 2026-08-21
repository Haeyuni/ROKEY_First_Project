# 개발 명세서 (Software Development Specification)

**문서 ID** SDS-NAIL-v1.0 · **작성일** 2026-08-21
**하위 문서** IDS-NAIL-v1.0 (인터페이스 정의서)

> 인터페이스 정의서가 "**무엇을 주고받는가**"라면, 이 문서는 "**그것을 어떻게 구현하는가**"입니다.
> 알고리즘, 두산 API 매핑, 공통 규약, 테스트 절차를 다룹니다.

---

## 1. 개요

### 1.1 시스템 목적

비전 센서 없이 로봇 내장 F/T 센서의 접촉 정보만으로 대상 표면의 형상과 경계를 인식하고, 연마·도포·경화를 수행한 뒤 결과를 물리적으로 자가 검증하는 협동로봇 셀.

### 1.2 제어 계층과 주기

| 계층 | 주기 | 구현 위치 | 책임 |
|---|---|---|---|
| 힘 루프 | ~1 kHz | 두산 컨트롤러 | 목표 접촉력 추종 |
| 스킬 루프 | 20~50 Hz | `robot_skill_node` | 종료 조건 감시, 궤적 추종 |
| 공정 루프 | 1~10 Hz | 공정 노드 | 경로 생성, 판정 |
| 세션 루프 | 초~분 | `session_orchestrator` | 시퀀스, 재작업 분기 |

**위반 금지 규칙**

1. **힘 추종 내부 루프를 ROS2 로 올리지 않는다.** F/T 를 토픽으로 받아 보정량을 계산해 다시 명령하면 DDS 지연·지터로 접촉이 발산한다. 순응제어는 컨트롤러가 수행하고, ROS2 는 목표값과 종료 조건만 준다.
2. **공정 시퀀스를 DRL 스크립트나 백엔드에 두지 않는다.** `session_orchestrator` 가 단일 결정권자다.
3. **웹은 세션 단위 명령만 보낸다.** 저수준 동작 명령을 웹에 노출하지 않는다.

### 1.3 명시적 비목표

아래는 구현하지 않으며, 결과 필드에도 넣지 않는다.

| 항목 | 사유 |
|---|---|
| 도포 두께 측정 | 힘 제어로 점성 액체 두께 제어는 이 일정에서 불가능. 요구사항은 "빈 영역 없음"으로 축소 |
| 분진 제거 검증 | 힘으로 측정 불가. 성공 기준은 경로 커버리지뿐 |
| 손톱 전체 경화 보증 | 3점 검사로는 국소 미경화 반점 검출 불가 |
| UV 소프트웨어 제어 | 상시 ON 정책 (§9.3) |
| 사람 신체 대상 절삭·연마 | 안전 전제 (§9.1) |

---

## 2. 개발 환경

### 2.1 요구 스택

| 항목 | 버전 |
|---|---|
| OS | Ubuntu 24.04 |
| ROS2 | Jazzy Jalisco |
| 로봇 드라이버 | `doosan-robot2` (jazzy 브랜치) |
| 언어 | Python 3.12 (`rclpy`) — 공정·조정 계층 |
| 웹 브리지 | `rosbridge_suite` |
| 빌드 | `colcon` |

### 2.2 워크스페이스 구성

```
nail_ws/
└── src/
    ├── nail_msgs/          # 인터페이스 (IDS 참조)
    ├── nail_skill/         # robot_skill_node, tool_manager
    ├── nail_perception/    # scan_node
    ├── nail_process/       # sanding, brushing, coating, curing, inspection, stone
    ├── nail_safety/        # safety_monitor
    ├── nail_orchestrator/  # session_orchestrator
    ├── nail_gateway/       # web bridge 설정
    └── nail_bringup/       # launch, config
```

**빌드 순서**: `nail_msgs` → 나머지. 인터페이스가 바뀌면 전체 재빌드가 필요하므로, 인터페이스 변경은 팀 전체에 공지합니다.

```bash
cd ~/nail_ws
colcon build --packages-select nail_msgs
source install/setup.bash
colcon build --symlink-install
```

> `--symlink-install` 을 쓰면 Python 노드 수정 시 재빌드가 불필요합니다. `nail_msgs` 변경 시에는 여전히 재빌드해야 합니다.

### 2.3 로봇 없이 개발하기 — 두산 가상 모드

**별도의 mock 드라이버를 만들지 않습니다.** 두산 드라이버가 제공하는 가상 모드를 사용합니다.

```bash
# 실기
ros2 launch dsr_bringup2 dsr_bringup2_rviz.launch.py mode:=real host:=192.168.137.100

# 가상 (에뮬레이터)
ros2 launch dsr_bringup2 dsr_bringup2_rviz.launch.py mode:=virtual
```

> 정확한 인자명과 기본값은 설치된 브랜치에 따라 다를 수 있습니다. `ros2 launch dsr_bringup2 dsr_bringup2_rviz.launch.py --show-args` 로 확인하고 팀 위키에 확정본을 적어두세요.

**가상 모드로 검증 가능한 것 / 불가능한 것**

| 검증 가능 | 검증 불가 |
|---|---|
| 액션 수명주기, REJECT/ABORT/CANCEL 경로 | **접촉력 응답, 강성 측정** |
| 상태 머신 전이, 재작업 루프 분기 | **택프리 인장력** |
| 툴 교체 시퀀스, TCP 전환 | 그리퍼 폭 피드백 실측 |
| 경로 생성, TF 변환, 특이점 회피 | 순응제어 거동 |
| 웹 UI 전 화면, WebSocket 중계 | 임계값 캘리브레이션 |
| 타임아웃·취소 전파 | 표면 조도, 도포 상태 |

> **가상 모드는 "로직 검증"만 커버합니다.** 이 프로젝트의 핵심인 강성 판별과 택프리 판정은 가상으로 검증할 수 없습니다.
>
> 따라서 **실기 시간을 반드시 확보해야 하는 작업**은 다음 셋으로 좁혀집니다.
> 1. `ProbePoint` 파라미터 튜닝 및 강성 실측 (C-02)
> 2. `LateralContact` 접촉력 튜닝 (연마)
> 3. 택프리 임계값 캘리브레이션 (C-01)
>
> 나머지 전부는 가상 모드에서 완성한 뒤 실기에 들어갑니다. **실기에서 상태 머신을 디버깅하고 있으면 일정이 무너집니다.**

### 2.4 실행

```bash
# 하드웨어 계층
ros2 launch nail_bringup hardware.launch.py mode:=virtual

# 스킬 + 공정 노드
ros2 launch nail_bringup nodes.launch.py

# 조정 + 웹
ros2 launch nail_bringup app.launch.py

# 전체 한 번에
ros2 launch nail_bringup nail_cell.launch.py mode:=virtual
```

`mode` 인자 하나가 전 계층에 전파되어야 합니다.

---

## 3. 공통 구현 규약

**이 장을 어기는 코드는 리뷰에서 반려합니다.**

### 3.1 액션 서버 표준 수명주기

```
① goal 수신
      ↓
② 파라미터 유효성 검사             → 실패 REJECT (E_INVALID_GOAL)
      ↓
③ /safety/status.safe_to_move 확인 → false REJECT (E_SAFETY_BLOCKED)
      ↓
④ ValidatePrecondition 호출        → ok=false REJECT (E_PRECOND_FAILED)
      ↓
⑤ ACCEPT → 실행
      ↓
⑥ 매 주기: feedback 발행 / 취소 확인 / safe_to_move 재확인
      ↓
⑦ 종료 처리 (성공·실패·취소 공통):
      · 툴을 표면에서 이탈           ← 이탈 방향 주의 (§3.2)
      · 필요 시 HOME 복귀
      · ResultBase 채워 반환
```

**③④를 goal 수락 전에 하는 이유**: 수락한 goal 을 도중에 죽이는 것보다 애초에 받지 않는 편이 로봇이 움직이지 않아 안전하고 로그도 깔끔합니다.

### 3.2 이탈 방향 — 접근 방식에 따라 다름

| 접근 | 이탈 방향 |
|---|---|
| `ContactPath`, `ProbePoint` (법선) | **+Z** (표면 법선 방향) |
| `LateralContact` (수평) | **접근 벡터 역방향** |

ABORT 와 CANCEL 의 뒷정리는 완전히 같으므로 **공통 함수 하나**로 구현하되, 이탈 방향만 접근 방식에 따라 분기합니다.

```python
def _cleanup(self, approach_mode, approach_vec=None):
    if approach_mode == 'normal':
        self._retreat_along([0, 0, 1], self.retreat_mm)
    else:
        self._retreat_along([-v for v in approach_vec], self.retreat_mm)
    self._release_compliance()
    if self.abort_return_home:
        self._move_home()
```

### 3.3 콜백 데드락 회피 ★

**이 프로젝트에서 가장 나오기 쉬운 버그입니다.** 액션 서버의 콜백 안에서 서비스를 동기 호출하면 실행기가 잠깁니다.

```python
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor

class SandingNode(Node):
    def __init__(self):
        super().__init__('sanding_node')
        self._cb_action = MutuallyExclusiveCallbackGroup()
        self._cb_client = MutuallyExclusiveCallbackGroup()   # 반드시 분리

        self._srv = ActionServer(
            self, SandSurface, '/process/sand',
            execute_callback=self._execute,
            callback_group=self._cb_action)

        self._validate = self.create_client(
            ValidatePrecondition, '/safety/validate',
            callback_group=self._cb_client)                  # 다른 그룹

def main():
    rclpy.init()
    node = SandingNode()
    executor = MultiThreadedExecutor()                       # 필수
    executor.add_node(node)
    executor.spin()
```

**`MultiThreadedExecutor` 없이 `rclpy.spin()` 을 쓰면 콜백 그룹을 나눠도 소용없습니다.** 전 노드가 `MultiThreadedExecutor` 를 씁니다.

### 3.4 취소 전파

```
RunSession cancel
   → 진행 중인 공정 액션 cancel
      → 진행 중인 스킬 액션 cancel
         → 컨트롤러 정지
```

공정 노드는 자기가 호출한 스킬 액션의 `goal_handle` 을 **반드시 멤버로 보관**합니다. 없으면 취소를 아래로 전달할 수 없습니다.

```python
self._skill_handle = None   # 멤버

async def _execute(self, goal_handle):
    send = self._lateral_client.send_goal_async(req, feedback_callback=self._on_fb)
    self._skill_handle = await send
    ...

def _on_cancel(self, goal_handle):
    if self._skill_handle is not None:
        self._skill_handle.cancel_goal_async()
    return CancelResponse.ACCEPT
```

### 3.5 안전 상태 구독 의무

**모든 노드가 `/safety/status` 를 구독합니다. 예외 없습니다.**

`safe_to_move == false` 관측 시:
- 실행 중 goal → `ABORT(E_SAFETY_BLOCKED)`
- 신규 goal → `REJECT`
- `curing_node` → 추가로 **즉시 조사 영역 밖으로 이탈** (permit 이 없으므로 이것이 유일한 대응)

### 3.6 파라미터

하드코딩 상수를 두지 않습니다. 전 노드 공통 파라미터:

| 이름 | 타입 | 기본 | 설명 |
|---|---|---|---|
| `mode` | string | `virtual` | `real` / `virtual` |
| `node_timeout_s` | double | `120.0` | 액션 타임아웃 |
| `retreat_mm` | double | `10.0` | 이탈 거리 |
| `abort_return_home` | bool | `true` | ABORT 시 HOME 복귀 |
| `log_force_data` | bool | `false` | DEBUG 힘 로그 |

파라미터 파일은 `nail_bringup/config/<node_name>.yaml` 하나씩 두고 launch 에서 주입합니다. 노드별 파라미터 전문은 §11 참조.

### 3.7 로깅

| 레벨 | 사용 |
|---|---|
| `DEBUG` | 매 주기 힘 값 등 대량 데이터 |
| `INFO` | goal 수락/완료, 상태 전이 |
| `WARN` | 재시도, 임계값 근접 |
| `ERROR` | ABORT, 통신 실패, 안전 결함 |

**모든 ABORT 로그는 에러 코드와 좌표·측정값을 포함합니다.**

```
[E_LATERAL_LIMIT] sanding aborted: travel=6.2mm limit=6.0mm at (12.3, 4.5, 0.0)
```

특히 `E_SEPARATION_LOW`, `E_NO_SCAN`, `E_LOW_STIFFNESS`, `E_LATERAL_LIMIT` 네 개는 **반드시 수치를 함께 남기세요.** 사후 원인 분석의 유일한 근거입니다.

---

## 4. 두산 API 매핑

### 4.1 추상화 원칙

**두산 API 호출은 `robot_skill_node` 안에만 존재합니다.** 다른 노드가 `dsr_msgs2` 를 import 하면 리뷰 반려입니다. 드라이버 버전이 바뀌어도 이 노드만 고치면 되도록 가둡니다.

### 4.2 기능 매핑

| 스킬 동작 | 두산 기능 | 비고 |
|---|---|---|
| 직선 이동 | `movel` 계열 서비스 | `MoveTo(linear=true)` |
| 관절 이동 | `movej` 계열 서비스 | `MoveTo(linear=false)` |
| 순응제어 시작 | `task_compliance_ctrl` | 축별 강성 벡터 지정 |
| 순응제어 해제 | `release_compliance_ctrl` | **종료 경로마다 반드시 호출** |
| 목표 힘 설정 | `set_desired_force` | 축 선택 + 목표값 |
| 힘 해제 | `release_force` | |
| 힘 조건 확인 | `check_force_condition` | 접촉 감지에 사용 |
| 외력 읽기 | 툴 외력 조회 | `/force/data` 발행 원천 |
| 그리퍼 개폐 | 디지털 출력 서비스 | RG2 제어 |
| TCP 설정 | TCP 설정 서비스 | 툴 교체 시 |
| 정지 | stop 서비스 | 안전 정지 |

> **⚠️ 서비스 이름과 타입은 설치된 브랜치에서 직접 확인하세요.**
> `doosan-robot2` 는 버전에 따라 네임스페이스와 서비스명이 달라집니다. 아래로 실물을 확인하고 팀 위키에 확정본을 기록한 뒤 구현에 들어갑니다.
> ```bash
> ros2 service list | grep dsr
> ros2 service type /dsr01/motion/move_line
> ros2 interface show <타입>
> ```
> 이 표는 **기능 대응만** 보증하며 정확한 이름은 보증하지 않습니다.

### 4.3 내부 추상화 인터페이스

`robot_skill_node` 내부에 얇은 래퍼를 두고, 액션 구현은 이 래퍼만 호출합니다.

```python
class DsrAdapter:
    def move_line(self, pose, vel, acc) -> bool: ...
    def move_joint(self, pose, vel, acc) -> bool: ...
    def compliance_on(self, stiffness_6d) -> bool: ...
    def compliance_off(self) -> bool: ...
    def set_desired_force(self, force_6d, axis_mask) -> bool: ...
    def release_force(self) -> bool: ...
    def read_wrench(self) -> ForceSample: ...
    def set_tcp(self, offset_6d) -> bool: ...
    def gripper(self, open_close, width_mm) -> float: ...   # 실측 폭 반환
    def stop(self) -> bool: ...
```

가상/실기 차이도 이 클래스 안에서 흡수합니다. 상위 코드는 `mode` 를 알 필요가 없습니다.

---

## 5. 핵심 알고리즘

### 5.1 압입 강성 계산 (`ProbePoint`)

**두 점 차분으로 계산하지 마세요.** 하강 구간 전체의 선형 회귀를 씁니다. 노이즈에 훨씬 강합니다.

```python
def compute_stiffness(samples, contact_threshold_n, min_samples):
    """
    samples: [(depth_mm, fz_n), ...] 하강 구간 시계열
    return : (stiffness_n_per_mm, r_squared, n_used)
    """
    contact = [(d, f) for d, f in samples if abs(f) >= contact_threshold_n]
    if len(contact) < min_samples:
        return None, 0.0, len(contact)

    d0 = contact[0][0]
    x = np.array([d - d0 for d, _ in contact])       # 접촉점 기준 압입량
    y = np.array([abs(f) for _, f in contact])

    slope, intercept = np.polyfit(x, y, 1)
    resid = y - (slope * x + intercept)
    r2 = 1.0 - resid.var() / y.var() if y.var() > 0 else 0.0
    return slope, r2, len(contact)
```

**`r_squared` 를 함께 반환하고 낮으면 그 점을 `valid=false` 로 버리세요.** 선형성이 무너진 점은 프로브가 비스듬히 닿았거나 미끄러진 경우입니다. 그런 점이 군집화에 섞이면 `separation_margin` 이 오염됩니다.

### 5.2 2단계 스캔 (`scan_node`) ★

```python
def execute_scan(goal):
    # ---------- 1단계: 거친 스캔 ----------
    coarse_grid = make_grid(area, pitch=goal.coarse_pitch_mm, margin=goal.margin_mm)
    coarse = [probe(p, source='coarse') for p in coarse_grid]      # feedback 매 점
    valid = [p for p in coarse if p.valid]

    if len(valid) < goal.coarse_min_valid_points:
        return fail(E_COARSE_INSUFFICIENT)

    # 군집화
    k_values = [p.stiffness_n_per_mm for p in valid]
    threshold = otsu_threshold(k_values)
    hard = [p for p in valid if p.stiffness_n_per_mm >= threshold]
    soft = [p for p in valid if p.stiffness_n_per_mm <  threshold]

    if len(hard) < goal.coarse_min_per_cluster or \
       len(soft) < goal.coarse_min_per_cluster:
        return fail(E_SEPARATION_LOW)       # ★ 여기서 멈춘다. 2단계로 안 감

    margin = separation_margin(hard, soft)
    if margin < goal.separation_margin_min:
        return fail(E_SEPARATION_LOW)

    # ---------- 경계 후보 선정 ----------
    candidates = []
    for a, b in adjacent_pairs(coarse_grid):        # 4-이웃
        ka, kb = k_of(a), k_of(b)
        if ka is None or kb is None:
            continue
        if (ka >= threshold) != (kb >= threshold):  # 임계를 사이에 둔 쌍
            candidates.append(midpoint(a, b))

    # ---------- 2단계: 정밀 스캔 ----------
    band = union_of_disks(candidates, radius=goal.boundary_band_mm / 2)
    fine_grid = [p for p in make_grid(area, pitch=goal.fine_pitch_mm)
                 if p in band and not already_measured(p, coarse_grid)]

    if len(fine_grid) > goal.fine_max_points:
        fine_grid = sorted(fine_grid, key=dist_to_nearest_candidate)[:goal.fine_max_points]
        warn("fine grid truncated")

    fine = [probe(p, source='fine') for p in fine_grid]

    # ---------- 최종 판정 ----------
    allpts = valid + [p for p in fine if p.valid]
    threshold = otsu_threshold([p.stiffness_n_per_mm for p in allpts])
    margin = separation_margin_all(allpts, threshold)
    if margin < goal.separation_margin_min:
        return fail(E_SEPARATION_LOW)       # 정밀 데이터로 재확인

    region = extract_polygons(allpts, threshold)    # fine 점 우선
    publish_map(allpts, threshold, margin, region)
    broadcast_tf('nail_local_frame', origin_from(region))
    return ok()
```

**`separation_margin` 정의**

```python
def separation_margin(hard, soft):
    mh, ms = mean(k(hard)), mean(k(soft))
    sh, ss = std(k(hard)),  std(k(soft))
    return abs(mh - ms) / (sh + ss + 1e-9)
```

> **⚠️ 1단계 군집 점 수 미달 시 절대 2단계로 넘어가지 마세요.**
> 3 mm 격자면 폭 13 mm 손톱에 격자선이 4~5줄만 걸칩니다. 안착 위치가 어긋나면 고강성 점 수가 급감합니다. 잘못된 후보 위에서 정밀 스캔을 돌리면 시간만 쓰고 결과는 쓰레기입니다.
>
> **완화책**: 군집 점 수가 `coarse_min_per_cluster × 1.5` 미만이면 `coarse_pitch_mm` 를 2 mm 로 낮춰 1단계를 **한 번 더** 돌리는 재시도를 넣으세요. 정밀 스캔 시간을 아끼는 것보다 1단계 신뢰도가 우선입니다.

**소요 시간 추정**

| 단계 | 점 수 | 점당 | 소요 |
|---|---|---|---|
| 거친 (3 mm) | 약 42 | 1.5 s | 약 1.1 분 |
| 정밀 (1 mm) | 약 60~110 | 1.5 s | 약 1.5~2.8 분 |
| **합계** | **약 100~150** | | **약 2.6~3.9 분** |

### 5.3 수평 연마 진행 한계 (`sanding_node`) ★★

**이 함수가 수평 연마의 유일한 피부 접촉 방어선입니다.**

```python
def compute_travel_limit(start_point, approach_vec, region, margin_mm):
    """
    접근 시작점에서 approach_vec 방향으로 진행할 때
    금지 영역에 닿기까지의 거리에서 안전 마진을 뺀 값.
    """
    d_forbidden = ray_polygon_distance(start_point, approach_vec,
                                       region.forbidden_polygon)
    d_boundary  = ray_polygon_distance(start_point, approach_vec,
                                       region.boundary_polygon, exit_side=True)
    d = min(x for x in (d_forbidden, d_boundary) if x is not None)
    return d - margin_mm
```

```python
limit = compute_travel_limit(start, vec, region, goal.travel_limit_margin_mm)
if limit <= 0:
    return reject(E_INVALID_GOAL, "travel limit <= 0; check approach side")
```

> **⚠️ 수평 접근에서는 `E_LOW_STIFFNESS` 가 작동하지 않습니다.**
> `ContactPath` 는 압입을 하므로 `ΔF/Δz` 로 강성을 계속 측정할 수 있고, 강성이 떨어지면 "피부에 닿았다"고 판정할 수 있습니다. **수평 접근은 접근축으로 밀고 있을 뿐 압입이 아니므로 강성을 측정할 수 없습니다.**
>
> 따라서 피부 접촉을 막는 것은 위 함수가 계산한 `travel_limit_mm` **하나뿐**입니다. 이 값을 상수로 하드코딩하거나, `margin_mm` 을 0 에 가깝게 잡거나, 계산을 생략하면 **이 공정에는 안전장치가 아예 없어집니다.**

**수평 접근을 쓰는 이유**: 샌딩 비트를 법선 방향으로 누르면 압력이 한 점에 집중되어 표면이 파입니다. 표면에 평행하게 대고 옆으로 밀면 접촉이 선으로 분산되어 균일하게 깎입니다. 실제 시술에서 파일을 눕혀 쓰는 것과 같은 원리입니다.

**순응제어 축 설정**

```python
# 접근축만 힘 제어, 나머지는 위치 제어
stiffness = [3000.0] * 6          # 기본 강성 (위치 제어에 가깝게)
axis = approach_axis_index(approach_vec)   # 0=X 1=Y 2=Z
stiffness[axis] = 200.0                    # 접근축만 낮춤
adapter.compliance_on(stiffness)
adapter.set_desired_force(force_6d, axis_mask=only(axis))
```

### 5.4 3점 검사 좌표 산출 (`inspection_node`)

```python
def make_inspect_points(region, cfg):
    origin = polygon_centroid(region.boundary_polygon)
    cx = origin.x + cfg.center_offset_x_ratio * nail_half_length(region)

    pts = [('center', Point(cx, origin.y))]

    for label, sign in (('left', +1), ('right', -1)):
        d_edge = distance_to_boundary(Point(cx, origin.y),
                                      direction=(0, sign), region=region)
        offset = d_edge * cfg.side_offset_y_ratio
        # 경계 이격 확보
        while d_edge - offset < cfg.min_edge_clearance_mm and offset > 0:
            offset -= 0.2
        if offset <= 0:
            pts.append((label, None))       # SKIP
        else:
            pts.append((label, Point(cx, origin.y + sign * offset)))
    return pts
```

**판정**

```python
if abs(point.release_force_n) > cfg.tack_threshold_n:
    result = 'FAIL'      # 이탈 시 점착 = 미경화
else:
    result = 'PASS'
```

> **왜 중앙 + 좌우인가**
>
> | 점 | 노리는 것 |
> |---|---|
> | 중앙 | 기준값. 여기가 FAIL 이면 조사 자체가 실패 |
> | 좌·우 | **사이드월 음영 사각지대** — 이 프로젝트가 해결하겠다고 주장한 부위 |
>
> 중앙만 검사하면 일괄 조사 방식과 결과가 같아서 국소 조사의 우위를 보일 수 없습니다. **좌우 두 점이 프로젝트의 주장을 검증합니다.**

> **⚠️ 3점의 한계를 리포트에 명시하세요.** 3점으로는 국소적인 작은 미경화 반점을 검출할 수 없습니다. `passed=true` 는 "손톱 전체가 경화됨"이 아니라 **"검사한 3개 지점이 기준을 만족함"** 입니다.

### 5.5 산소 저해층 대응

일반 탑젤은 **정상 경화되어도 표면에 미경화 점착층이 남습니다.** 공기 중 산소가 라디칼 중합을 방해하기 때문이며, 시술 후 클렌저로 닦아내는 것이 표준 공정입니다. **이 상태로 택프리 검사를 하면 완전 경화품도 FAIL 로 판정됩니다.**

| 대응 | 방법 | 조건 |
|---|---|---|
| A (권장) | 논와이프(Non-wipe) 탑젤 사용 | 소재 확보 가능 시 |
| B | 판정 시점을 레이어 간(베이스 경화 직후)으로 이동 | A 불가 시 |
| C | 점착층 존재를 전제로 `tack_threshold_n` 상향 재보정 | 최후 수단, 판별력 저하 |

**이 결정은 코드가 아니라 소재 확보 결과에 달려 있습니다. Day 1 에 확인하세요.** 코드는 세 경우 모두 파라미터 변경만으로 대응 가능하도록 작성합니다.

---

## 6. 노드별 구현 요점

### 6.1 robot_skill_node

**제공**: `MoveTo` · `PickPlace` · `ContactPath` · `LateralContact` · `ProbePoint` (Action) / `/force/data` `/force/data_ui` `/robot/pose` (Topic)

**구현 순서**: `MoveTo` → `ProbePoint` → `PickPlace` → `ContactPath` → `LateralContact`

`ProbePoint` 를 두 번째로 두는 이유는 `scan_node` 와 `inspection_node` 가 모두 여기 의존하기 때문입니다. 이것만 되면 두 노드가 병렬로 시작할 수 있습니다.

**필수 확인**
- 종료 경로마다 `compliance_off()` + `release_force()` 가 호출되는가 (`try/finally`)
- 이탈 방향이 접근 방식에 맞는가 (§3.2)
- `/force/data` 100 Hz 와 `/force/data_ui` 20 Hz 를 **별도 타이머**로 발행하는가

### 6.2 tool_manager

**툴 6종**: `probe` · `sander` · `brush` · `coater` · `uv` · `tweezers`

`config/tool_rack.yaml`:
```yaml
tools:
  sander:
    slot_frame: slot_2
    expected_grip_width_mm: 18.0
    tcp_offset: [0, 0, 92.5, 0, 0, 0]
  uv:
    slot_frame: slot_5
    expected_grip_width_mm: 22.0
    tcp_offset: [0, 0, 110.0, 0, 0, 0]
    park_facing: into_rack        # UV 상시 ON — 광축을 랙 안쪽으로
```

> **툴 낙하는 자동 복구하지 마세요.** 어디 떨어졌는지 모르는 상태로 로봇을 움직이면 밟습니다. `FAULT_TOOL_DROP` 등록 후 사람이 치울 때까지 정지합니다.
> **UV 툴 낙하 시에는 램프가 임의 방향을 비추게 되므로 즉시 전원을 차단하고 수습하세요.**

### 6.3 scan_node

알고리즘은 §5.2. 추가 구현 요점:

- **feedback 을 점마다 발행합니다.** 이 스트림이 그대로 웹의 강성 히트맵이 됩니다. `stage` 필드로 coarse/fine 을 구분해 색을 다르게 렌더링하면, 듬성듬성 찍히다 경계선을 따라 촘촘해지는 그림이 나옵니다.
- `nail_local_frame` 은 X=길이축, Z=표면 법선으로 잡습니다. **이후 모든 공정의 좌표 기준**이므로 부호 규약을 팀에서 먼저 합의하세요.
- 맵 발행 시 `session_id` 를 반드시 채웁니다.

### 6.4 sanding_node

알고리즘은 §5.3. **`LateralContact` 를 쓰는 유일한 노드입니다.**

전제조건: 안착 · `sander` 툴 · **더스트 컬렉터 ON**

> 분진은 안전 이슈이자 품질 이슈입니다. 젤 표면에 내려앉으면 경화 품질과 검사 판정이 동시에 오염됩니다. `require_dust_for_sanding` 을 `false` 로 두지 마세요.

### 6.5 coating_node

`boundary_polygon` 을 `boundary_offset_mm` 만큼 안쪽으로 축소한 영역에만 도포합니다 (큐티클 번짐 방지).

`coverage_ratio` 는 궤적 기반 추정치이며 **판정 기준이 아닙니다.** 결과에 두께 필드를 넣지 마세요.

### 6.6 curing_node

**UV 램프는 상시 ON 입니다.** 이 노드는 "언제 켜는가"가 아니라 **"얼마나 오래 그 자리에 머무는가"** 로 조사량을 만듭니다.

```
1. 표면 법선을 따라 standoff_mm 유지하는 dwell 지점 생성
2. 각 지점에서 dwell_s_per_point 만큼 정지 체류   ← 정지 = 조사
3. 총 체류 시간 집계 → actual_exposure_s
4. park_distance_mm 밖으로 이탈 → parked = true
```

> **⚠️ 상시 ON 의 부작용: 접근·이탈 경로에서도 조사됩니다.**
> 켠 채로 이동하므로 손톱으로 접근하는 동안과 빠져나가는 동안에도 UV 가 젤에 닿습니다. 결과적으로 `actual_exposure_s` 가 실제 조사량보다 작게 기록됩니다.
>
> **대응 (권장)**: 램프 헤드에 3D 프린팅 원뿔형 차광 슈라우드를 씌워 빔을 좁히세요. 사전 노출과 주변 산란이 동시에 줄고, 원가는 필라멘트 값뿐입니다. 정량 데이터의 신뢰도와 작업자 노출이 함께 개선됩니다.

**안전 결함 시 대응은 이탈뿐입니다.** permit 이 없으므로 "소등"이라는 선택지가 없습니다. `parked` 가 확실히 true 로 종료되는지 확인하세요.

### 6.7 inspection_node

알고리즘은 §5.4. `require_all_pass=true` 이면 하나라도 FAIL 시 `passed=false`, FAIL 좌표를 `fail_points` 에 담아 REWORK 대상으로 넘깁니다.

### 6.8 safety_monitor

**`session_orchestrator` 와 반드시 분리합니다.** orchestrator 는 로직이 복잡해 비정상 종료 가능성이 상대적으로 높고, 안전 감시가 그 안에 있으면 함께 죽습니다. safety_monitor 는 **다른 노드에 의존하지 않고** 드라이버 토픽과 DI 만 보고 판단합니다.

```
20 Hz 루프:
  E-Stop DI / 안착 DI / 더스트 DI 읽기
  /tool/status 최신값 반영
  하트비트 타임아웃 확인 → comm_ok
  safe_to_move = (not estop) and comm_ok and (active_faults 비어 있음)
  → SafetyState 발행
```

**`ResetSafety` 를 무조건 성공시키지 마세요.** 물리적 원인이 남아 있는데 리셋되면 인터록이 무의미해집니다. `remaining_faults` 가 비었을 때만 `ok=true` 입니다.

### 6.9 session_orchestrator

```
PRECHECK → SCAN → SAND → BRUSH
   ↓
┌── 레이어 루프 (layer_index = 0 .. layer_total-1) ──┐
│ COAT → CURE → INSPECT                              │
│   PASS → 다음 레이어                                │
│   FAIL → REWORK: fail_points 만 CURE 재실행         │
│           (exposure × rework_exposure_scale)        │
│           rework_count > max_rework → ABORT         │
└─────────────────────────────────────────────────────┘
   ↓
STONE → FINISH → REPORT
```

**ABORT 공통 처리**
```
① 진행 중인 하위 액션 전부 cancel (완료 대기)
② 툴 반납 시도 (ChangeTool("none"))
③ HOME 복귀
④ ProcessState(stage=ABORTED) 발행
⑤ 자동 재시작 안 함
```

> **②에서 반납이 실패해도 ③은 진행합니다. 단 UV 툴은 예외입니다.**
> 켜진 램프를 든 채 HOME 으로 가면 **이동 경로 전체가 조사됩니다.** UV 반납 실패 시에는 HOME 복귀 대신 **현재 위치에서 정지하고 수동 개입을 요청**하세요. 반납 실패는 `active_faults` 에 남깁니다.

**툴 교체 비용**: 레이어당 `coater → uv → probe` 3회. `layer_total=2` 면 스캔·연마·브러시 포함 총 9회, 교체당 15초면 **약 2분 15초**가 교체에만 쓰입니다.

> **UV 를 손목에 상시 장착하는 방안은 권장하지 않습니다.** 교체는 줄지만, 켜진 램프를 달고 연마·도포를 도는 것이 되어 **방금 바른 젤이 의도치 않게 굳습니다.** 랙 반납 방식을 유지하세요.

### 6.10 web_bridge

`rosbridge_server` 를 그대로 사용합니다. 중계 화이트리스트:

`/process/status` · `/safety/status` · `/validation/result` · `/stiffness/map` · `/force/data_ui`

> **`/force/data`(100 Hz)를 중계하지 마세요.** WebSocket 이 버티지 못합니다.

> **웹이 끊겼다고 로봇을 멈추지 마세요.** 경화 중에 브라우저를 닫았다고 로봇이 손톱 위에서 멈추면 더 위험합니다. 중단은 명시적 cancel 이나 E-Stop 으로만 합니다. 웹은 관측자이며 중단 권한이 없습니다.

---

## 7. 에러 처리 정책

### 7.1 심각도별 동작

| 등급 | 동작 | 세션 결과 |
|---|---|---|
| `SEV_WARN` | 기록만 하고 진행 | 정상 (경고 포함) |
| `SEV_RETRY` | 상한 내 자동 재시도, 초과 시 ABORT 로 승격 | 성공 시 정상 |
| `SEV_ABORT` | 공정 중단, 안전 후퇴, 세션 종료 | FAILED |
| `SEV_SAFETY` | 즉시 정지, **자동 복구 금지**, 수동 확인 필요 | ABORTED_SAFETY |

### 7.2 코드별 정책

| 코드 | 등급 | 복구 |
|---|---|---|
| `E_COARSE_INSUFFICIENT` | ABORT | 안착 위치 재확인 후 재스캔 |
| `E_SEPARATION_LOW` | ABORT | C-02 재검토. 자동 재시도 금지 |
| `E_NO_SCAN` | ABORT | REJECT 로 처리 — 로봇 무동작 |
| `E_NO_CONTACT` | RETRY(2) | 탐색 범위 확대 후 재시도 |
| `E_OVERFORCE` | ABORT | 즉시 후퇴 |
| `E_LOW_STIFFNESS` | **SAFETY** | 즉시 후퇴 + 좌표 로그. 자동 복구 금지 |
| `E_LATERAL_LIMIT` | **SAFETY** | 즉시 역방향 후퇴 + 좌표 로그 |
| `E_LATERAL_JAM` | RETRY(1) | 접근각 변경 후 재시도 |
| `E_GRIP_FAILED` | RETRY(2) | 재파지 |
| `E_TOOL_DROP` | **SAFETY** | 정지. 사람이 치울 때까지 대기 |
| `E_MAP_SESSION_MISMATCH` | **SAFETY** | goal 거부 |
| `E_REWORK_EXCEEDED` | ABORT | 소재·광원 문제로 판단, 수동 개입 |
| `E_COMM_LOST` | ABORT | 안전 종료 후 세션 중단 |
| `E_SAFETY_BLOCKED` | ABORT/SAFETY | 원인 fault 등급 따름 |

### 7.3 재시도 상한

```yaml
retry:
  probe_no_contact: 2
  lateral_jam: 1
  grip: 2
  rework_per_layer: 2
  rework_per_session: 5
```

**모든 재시도 루프에 상한이 있어야 합니다.** 상한 없는 재시도는 소재가 근본적으로 반응하지 않는 상황(파장 불일치, 소재 변질)에서 무한 반복하며 대상을 손상시킵니다.

---

## 8. 웹 · 데이터

### 8.1 백엔드 책임 범위

Spring Boot 의 책임은 **세션 요청 중계, 이력 저장, UI 브로드캐스트** 세 가지로 한정합니다. 공정 판단, 재시도 결정, 안전 판정을 백엔드에 구현하지 않습니다.

이유는 단순합니다. 네트워크가 끊기거나 백엔드가 재시작될 때 **로봇이 접촉 상태로 방치되면 안 됩니다.** `session_orchestrator` 가 웹 연결과 무관하게 현재 공정을 안전하게 종료할 수 있어야 하고, 그러려면 시퀀스 판단이 ROS2 쪽에 있어야 합니다.

### 8.2 저장 원칙

- **FAIL 판정의 힘 파형(`waveform`)을 반드시 저장합니다.** 임계값 재보정 시 유일한 근거입니다.
- `ValidationResult.threshold_n` 을 함께 저장합니다. 판정 시점의 임계값이 없으면 나중에 데이터를 다시 해석할 수 없습니다.
- `StiffnessMap` 은 세션 단위로 보존합니다.

---

## 9. 안전 구현 요구사항

### 9.1 대상 제한 (협상 불가)

**절삭 및 연마 공정은 실리콘 모델·인조 팁 전용입니다. 사람 신체를 대상으로 실행하지 않습니다.**

절차뿐 아니라 코드로도 확인합니다.

```python
ALLOWED_MATERIALS = {'silicone_model', 'artificial_tip'}

if goal.target_material not in ALLOWED_MATERIALS:
    return reject(E_INVALID_GOAL, f"material not allowed: {goal.target_material}")
```

### 9.2 3중 방어선

| 선 | 위치 | 응답 | 담당 |
|---|---|---|---|
| 1선 | 두산 컨트롤러 | ~1 ms | 힘 상한 초과 시 자체 정지 |
| 2선 | 스킬/공정 노드 | 20~50 Hz | 강성 하한·진행 한계 감지 시 ABORT |
| 3선 | safety_monitor | 20 Hz | 인터록·알람 감시 |

### 9.3 UV 정책 — 소프트웨어 제어 없음 ★

**UV 램프는 상시 ON 이며, 소프트웨어는 램프를 끄지 못합니다.**

즉:
- E-Stop 을 눌러도 소프트웨어는 소등할 수 없습니다
- 툴을 파지한 채 이동하는 동안에도 램프는 켜져 있습니다
- 노드가 죽거나 통신이 끊겨도 램프는 계속 조사합니다

**따라서 아래 물리적 통제가 유일한 방어선입니다. 하나라도 빠지면 방어선이 없습니다.**

| # | 통제 | 상태 |
|---|---|---|
| 1 | 작업 영역을 UV 차단 아크릴(황색)로 차폐 | **필수** |
| 2 | 작업자·촬영자 전원 UV 차단 고글 착용 | **필수** |
| 3 | 램프 헤드 차광 슈라우드 (§6.6) | **강력 권장** |
| 4 | 램프 전원선을 **E-Stop 회로에 직렬 결선**(하드웨어) | **강력 권장** |
| 5 | 수동 마스터 스위치를 작업자 손 닿는 곳에 배치 | **필수** |
| 6 | 미사용 시 UV 툴을 광축이 랙 안쪽을 향하도록 반납 | **필수** |

> **4번은 permit 폐지와 모순되지 않습니다.** permit 은 소프트웨어 승인 절차이고, 4번은 전원 라인 물리 결선입니다. 배선 하나로 "E-Stop 누르면 램프도 꺼진다"가 복원되므로 가능하면 반드시 하세요. **소프트웨어를 못 믿는 상황일수록 하드웨어 인터록의 값이 올라갑니다.**
>
> 365/405 nm 는 통증이 지연되어 나타나 노출을 인지하기 어렵습니다. 실습 중 "잠깐인데 괜찮겠지"가 가장 위험합니다.

### 9.4 분진 관리

연마 중 케라틴 분진이 발생합니다. 국소 배기 또는 더스트 컬렉터가 동작하지 않으면 연마 공정을 시작하지 않습니다. **안전 요구사항이자 품질 요구사항입니다** — 분진이 젤 표면에 내려앉으면 경화 품질과 검사 판정이 동시에 오염됩니다.

---

## 10. 개발 순서 및 테스트

### 10.1 Phase

| Phase | 산출물 | 완료 기준 |
|---|---|---|
| **0** | `nail_msgs` 확정 · 가상 모드 기동 · launch 골격 | 빈 노드들이 가상 모드에서 뜨고 서로 discovery |
| **1** | `robot_skill_node`(MoveTo, ProbePoint) · `safety_monitor` · `scan_node` | 가상에서 2단계 스캔 완주, 실기에서 강성 실측 |
| **2** | `tool_manager` · `sanding_node` · `curing_node` · `inspection_node` | 연마→경화→검증 폐루프 |
| **3** | `coating_node` · `brushing_node` · `orchestrator` · 웹 | 전 공정 무개입 완주 |
| **4** | `stone_node` · 리포트 · 이력 | 축소 가능 |

### 10.2 실기 사용 원칙

**실기 시간은 §2.3 의 세 작업에만 씁니다.** 상태 머신·UI·시퀀스 디버깅은 가상 모드에서 끝낸 뒤 실기에 들어갑니다.

### 10.3 캘리브레이션 절차

| ID | 실험 | 확정 파라미터 | 시점 |
|---|---|---|---|
| **C-02** | 재질별 압입 강성 분포 측정 | `separation_margin_min`, 강성 임계 | **1주차 최우선** |
| **C-01** | 노출시간별 택프리 인장력 측정 | `tack_threshold_n`, `exposure_s` | 1주차 |
| C-03 | 접촉력별 표면 조도·두께 감소 | `target_force_n`(sanding) | 1주차 |
| C-04 | 도포 번짐 한계 | `boundary_offset_mm` | 2주차 |
| C-05 | 젤 관통 한계 | `probe_max_force_n`(inspection) | 1주차 |

> **C-02 를 가장 먼저 하세요.** 재질별 강성 구간이 유의미하게 분리되지 않으면 이 프로젝트의 전제 자체가 성립하지 않습니다. 다른 어떤 개발보다 우선합니다.

### 10.4 테스트

**단위** — 로봇 불필요
- 강성 회귀 (§5.1): 합성 데이터로 기울기·R² 검증
- 경계 후보 선정 (§5.2): 가상 격자에서 후보가 실제 경계에 놓이는지
- 진행 한계 계산 (§5.3): 다각형·광선 교차 케이스
- 3점 좌표 산출 (§5.4): 마진 부족 시 SKIP 처리

**통합** — 가상 모드
- 액션 REJECT/ABORT/CANCEL 경로 전수
- 취소 전파 (RunSession → 공정 → 스킬)
- 상태 머신 전이, REWORK 루프
- `safe_to_move=false` 주입 시 전 노드 반응

**인터록** — 실기
| ID | 시험 |
|---|---|
| I-01 | 안착 센서 OFF 상태로 공정 시작 → 전 단계 REJECT |
| I-02 | 잘못된 툴 파지 상태로 goal → REJECT |
| I-03 | 스캔 없이 연마 goal → REJECT, **로봇 무동작** |
| I-04 | 이전 세션 맵으로 goal → REJECT |
| I-05 | 더스트 컬렉터 OFF → 연마 REJECT |
| I-06 | 연마 중 E-Stop → 정지 + 래치 |
| I-07 | 물리 원인 남긴 채 ResetSafety → 실패 |
| I-08 | 통신 차단 → 안전 종료 |
| I-09 | 진행 한계 축소 설정 → `E_LATERAL_LIMIT` 발생 확인 |
| I-10 | 노출 0초 조사 후 검사 → 3점 FAIL 확인 |

**안전 회귀** — 코드 변경 시마다 재실행
1. E-Stop → 정지 확인
2. `tool_manager` 강제 종료 → 툴 낙하 여부
3. 가공 금지 영역 침범 시도 → 정지 확인 (**누적 0건 유지**)
4. 통신 두절 → 안전 후퇴 후 세션 종료
5. `curing_node` ABORT → `parked=true` 확인

---

## 11. 파라미터 전문

### 11.1 캘리브레이션 필요 값

**실측 전에는 신뢰하지 마세요.**

| 파라미터 | 노드 | 실험 |
|---|---|---|
| `separation_margin_min` | scan | C-02 |
| `target_force_n` / `max_force_n` | sanding | C-03 |
| `jam_force_n` | sanding | C-03 |
| `boundary_offset_mm` | coating | C-04 |
| `standoff_mm` / `dwell_s_per_point` | curing | C-01 |
| `tack_threshold_n` | inspection | C-01 |
| `probe_max_force_n` | inspection | C-05 |
| `press_force_n` | stone | — |

### 11.2 안전 파라미터 — 변경 시 리뷰 필수

**개인이 임의로 바꾸지 않습니다.** 변경하려면 근거 데이터와 함께 팀 리뷰를 거칩니다.

| 파라미터 | 노드 | 지키는 것 |
|---|---|---|
| `travel_limit_margin_mm` ★ | sanding | **수평 연마의 유일한 피부 접촉 방어선** |
| `forbidden_margin_mm` | sanding | 금지 영역 여유 |
| `max_force_n` (전 노드) | 전체 | 과압 방지 |
| `probe_max_force_n` | inspection | 도포면 관통 방지 |
| `require_dust_for_sanding` | safety | 분진 인터록 |
| `require_handrest` | safety | 안착 인터록 |
| `auto_reset` | safety | **`true` 금지** |
| `ALLOWED_MATERIALS` | 전체 | 대상 제한 (§9.1) |

### 11.3 설정 파일 배치

```
nail_bringup/config/
├── common.yaml
├── robot_skill.yaml
├── tool_manager.yaml
├── tool_rack.yaml
├── scan.yaml
├── sanding.yaml
├── brushing.yaml
├── coating.yaml
├── curing.yaml
├── inspection.yaml
├── stone.yaml
├── safety.yaml
├── orchestrator.yaml
└── web_bridge.yaml
```

---

## 12. 구현 체크리스트

노드 완료를 선언하기 전에 확인합니다.

**전 노드 공통**
- [ ] `/safety/status` 를 구독하고 `safe_to_move=false` 에 반응한다
- [ ] goal 수락 전에 `ValidatePrecondition` 을 호출한다
- [ ] `MultiThreadedExecutor` + 콜백 그룹 분리로 데드락이 없다
- [ ] 취소 요청이 하위 액션까지 전파된다
- [ ] ABORT / CANCEL 뒷정리가 같은 함수로 처리된다
- [ ] **이탈 방향이 접근 방식에 맞다** (법선 = +Z, 수평 = 접근 역벡터)
- [ ] `result.error.code` 에 `ErrorCode.msg` 상수만 넣는다
- [ ] 모든 ABORT 로그에 코드와 좌표·측정값이 있다
- [ ] 하드코딩 상수 없이 전부 `declare_parameter` 로 선언했다
- [ ] 가상 모드에서 단독 실행된다

**담당자별**
- [ ] **robot_skill**: 종료 경로마다 `compliance_off()` + `release_force()` 가 호출된다
- [ ] **scan**: 1단계 군집 점 수 미달 시 2단계로 넘어가지 않는다
- [ ] **sanding**: `travel_limit_mm` 를 강성 맵에서 계산한다 (상수 금지) ★
- [ ] **coating**: result 에 두께 필드가 없다
- [ ] **curing**: 모든 종료 경로에서 `parked=true` 가 된다
- [ ] **inspection**: 좌우 검사점이 `min_edge_clearance_mm` 를 지킨다
- [ ] **safety**: `ResetSafety` 가 `remaining_faults` 비었을 때만 성공한다
- [ ] **orchestrator**: UV 툴 반납 실패 시 HOME 복귀하지 않고 정지한다
- [ ] **web_bridge**: `/force/data`(100 Hz)를 중계하지 않는다
