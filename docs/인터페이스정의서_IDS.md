# 인터페이스 정의서 (Interface Definition Specification)

**문서 ID** IDS-NAIL-v1.0 · **작성일** 2026-08-21
**대상 패키지** `nail_msgs`

> 이 문서는 `nail_msgs` 패키지에 그대로 배치할 인터페이스 정의 전문입니다.
> **가장 먼저 확정하고 병합하세요.** 나머지 모든 패키지가 여기에 의존하므로, 확정 전에는 병렬 개발이 불가능합니다.

---

## 0. 변경 이력

| 버전 | 변경 |
|---|---|
| v1.0 | 초판. `LateralContact` 신설 · UV permit 폐지 · 2단계 스캔 · 3점 검사 반영 · `mock_robot_driver` 제거 |

**폐지된 인터페이스** (v0.x 문서에 있었다면 삭제)

| 항목 | 사유 |
|---|---|
| `RequestUvPermit.srv` | UV 상시 ON 정책으로 permit 구조 폐지 |
| `E_UV_DENIED`, `E_UV_TIMEOUT` | 위와 동일 |
| `FAULT_UV_TIMEOUT` | 위와 동일 |
| mock 전용 인터페이스 일체 | `mock_robot_driver` 제거. 대체는 두산 공식 가상 모드 (SDS §2.3) |

---

## 1. 패키지 구조

```
nail_msgs/
├── CMakeLists.txt
├── package.xml
├── msg/
│   ├── ErrorCode.msg
│   ├── ResultBase.msg
│   ├── ForceSample.msg
│   ├── StiffnessPoint.msg
│   ├── StiffnessMap.msg
│   ├── BoundaryRegion.msg
│   ├── SafetyState.msg
│   ├── ToolState.msg
│   ├── ProcessState.msg
│   └── ValidationResult.msg
├── srv/
│   ├── ValidatePrecondition.srv
│   ├── ResetSafety.srv
│   ├── GetStiffnessMap.srv
│   └── GetToolInfo.srv
└── action/
    ├── MoveTo.action
    ├── PickPlace.action
    ├── ContactPath.action
    ├── LateralContact.action
    ├── ProbePoint.action
    ├── ChangeTool.action
    ├── ScanBoundary.action
    ├── SandSurface.action
    ├── BrushDust.action
    ├── CoatGel.action
    ├── CureUV.action
    ├── InspectCure.action
    ├── PlaceStone.action
    └── RunSession.action
```

**합계**: msg 10 · srv 4 · action 14

### 1.1 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.8)
project(nail_msgs)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/ErrorCode.msg"
  "msg/ResultBase.msg"
  "msg/ForceSample.msg"
  "msg/StiffnessPoint.msg"
  "msg/StiffnessMap.msg"
  "msg/BoundaryRegion.msg"
  "msg/SafetyState.msg"
  "msg/ToolState.msg"
  "msg/ProcessState.msg"
  "msg/ValidationResult.msg"
  "srv/ValidatePrecondition.srv"
  "srv/ResetSafety.srv"
  "srv/GetStiffnessMap.srv"
  "srv/GetToolInfo.srv"
  "action/MoveTo.action"
  "action/PickPlace.action"
  "action/ContactPath.action"
  "action/LateralContact.action"
  "action/ProbePoint.action"
  "action/ChangeTool.action"
  "action/ScanBoundary.action"
  "action/SandSurface.action"
  "action/BrushDust.action"
  "action/CoatGel.action"
  "action/CureUV.action"
  "action/InspectCure.action"
  "action/PlaceStone.action"
  "action/RunSession.action"
  DEPENDENCIES std_msgs geometry_msgs builtin_interfaces
)

ament_package()
```

### 1.2 package.xml

```xml
<?xml version="1.0"?>
<package format="3">
  <name>nail_msgs</name>
  <version>1.0.0</version>
  <description>Interface definitions for the nail cell</description>
  <maintainer email="team@example.com">nail team</maintainer>
  <license>Apache-2.0</license>

  <buildtool_depend>ament_cmake</buildtool_depend>
  <buildtool_depend>rosidl_default_generators</buildtool_depend>

  <depend>std_msgs</depend>
  <depend>geometry_msgs</depend>
  <depend>builtin_interfaces</depend>

  <exec_depend>rosidl_default_runtime</exec_depend>
  <member_of_group>rosidl_interface_packages</member_of_group>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
```

---

## 2. 설계 규약

### 2.1 단위

| 물리량 | 단위 | 접미사 |
|---|---|---|
| 길이 | mm | `_mm` |
| 힘 | N | `_n` |
| 토크 | Nm | `_nm` |
| 강성 | N/mm | `_n_per_mm` |
| 시간 | s | `_s` |
| 속도 | mm/s | `_mms` |
| 각도 | deg | `_deg` |

**필드명에 단위 접미사를 반드시 붙입니다.** `geometry_msgs/Pose`만 예외로 ROS 표준(m, rad)을 따르며, 변환은 `robot_skill_node`가 담당합니다.

### 2.2 공통 필드

- 모든 액션 result의 **첫 필드는 `ResultBase base`** 입니다. 호출자는 `base.error.severity` 하나만 보고 복구 정책을 적용할 수 있어야 합니다.
- 세션 컨텍스트가 필요한 모든 인터페이스는 `session_id`를 **필수**로 받습니다.

### 2.3 Action vs Service

| | 사용 |
|---|---|
| **Action** | 로봇이 움직이는 모든 동작. 길고, 취소 가능해야 하고, 진행률이 필요 |
| **Service** | 계산·조회만 하고 즉시 반환 (목표 5 ms) |

**로봇 동작을 서비스로 만들지 않습니다.** 서비스는 취소할 수 없어 안전 경로에 쓸 수 없습니다.

---

## 3. msg

### 3.1 ErrorCode.msg

```
# 전 노드 공통 에러 코드. 자유 서술 금지 — 웹이 파싱해 사용자 메시지로 변환한다.

string OK                      = ""

# 공통
string E_INVALID_GOAL          = "E_INVALID_GOAL"
string E_SAFETY_BLOCKED        = "E_SAFETY_BLOCKED"
string E_PRECOND_FAILED        = "E_PRECOND_FAILED"
string E_TIMEOUT               = "E_TIMEOUT"
string E_CANCELLED             = "E_CANCELLED"
string E_COMM_LOST             = "E_COMM_LOST"
string E_MOTION_FAILED         = "E_MOTION_FAILED"

# 스캔 / 경계
string E_COARSE_INSUFFICIENT   = "E_COARSE_INSUFFICIENT"
string E_SEPARATION_LOW        = "E_SEPARATION_LOW"
string E_NO_SCAN               = "E_NO_SCAN"

# 접촉 (법선)
string E_NO_CONTACT            = "E_NO_CONTACT"
string E_OVERFORCE             = "E_OVERFORCE"
string E_LOW_STIFFNESS         = "E_LOW_STIFFNESS"

# 접촉 (수평) — 연마 전용
string E_LATERAL_LIMIT         = "E_LATERAL_LIMIT"
string E_LATERAL_JAM           = "E_LATERAL_JAM"

# 툴
string E_TOOL_MISMATCH         = "E_TOOL_MISMATCH"
string E_GRIP_FAILED           = "E_GRIP_FAILED"
string E_TOOL_DROP             = "E_TOOL_DROP"

# 세션
string E_REWORK_EXCEEDED       = "E_REWORK_EXCEEDED"
string E_MAP_SESSION_MISMATCH  = "E_MAP_SESSION_MISMATCH"
string E_STONE_MISS            = "E_STONE_MISS"

# 심각도
uint8 SEV_NONE   = 0
uint8 SEV_WARN   = 1     # 기록만 하고 진행
uint8 SEV_RETRY  = 2     # 상한 내 자동 재시도
uint8 SEV_ABORT  = 3     # 공정 중단, 안전 후퇴
uint8 SEV_SAFETY = 4     # 즉시 정지, 자동 복구 금지

string  code
uint8   severity
string  detail          # 사람이 읽을 설명. 코드 대체용 아님
```

> **`code`를 문자열로 둔 이유**: 로그 grep과 웹 파싱이 숫자보다 쉽고, 상수 정의를 여기 한 곳에 모아두면 오타가 컴파일/런타임에 드러납니다. 각 노드가 리터럴 문자열을 직접 쓰지 말고 반드시 이 상수를 참조하세요.

### 3.2 ResultBase.msg

```
# 모든 액션 result 의 첫 필드.

bool                     success
nail_msgs/ErrorCode      error
geometry_msgs/Pose       final_pose
float64                  final_fz_n
float64                  duration_s
builtin_interfaces/Time  completed_at
```

### 3.3 ForceSample.msg

```
builtin_interfaces/Time stamp
float64 fx_n
float64 fy_n
float64 fz_n
float64 tx_nm
float64 ty_nm
float64 tz_nm
```

### 3.4 StiffnessPoint.msg

```
# 한 점의 압입 측정 결과. 스캔과 택프리 검사가 공유한다.

string SRC_COARSE = "coarse"
string SRC_FINE   = "fine"
string SRC_VERIFY = "verify"

geometry_msgs/Point position          # nail_frame 또는 nail_local_frame 기준, mm
float64 stiffness_n_per_mm            # 하강 구간 선형회귀 기울기
float64 release_force_n               # 이탈 시 최저 Fz. 음수면 점착
float64 hysteresis_ratio              # 로딩/언로딩 면적비. 연조직 보조 판별
float64 contact_depth_mm              # 접촉 후 압입 깊이
float64 lateral_force_n               # 접촉 시 측면 힘. 비스듬 접촉 검출
string  source                        # SRC_*
bool    valid                         # false면 미접촉 또는 측정 실패
```

### 3.5 StiffnessMap.msg

```
std_msgs/Header header

string  session_id                    # 필수. E_MAP_SESSION_MISMATCH 검증용
string  frame_id                      # 좌표 기준 프레임

nail_msgs/StiffnessPoint[] points     # coarse + fine 통합

# --- 2단계 스캔 메타 ---
float64 coarse_pitch_mm
float64 fine_pitch_mm
int32   coarse_point_count
int32   fine_point_count
int32   candidate_count               # 경계 후보 쌍 개수

# --- 판정 결과 ---
bool    valid                         # false면 후속 공정 전부 진입 불가
float64 threshold_k_n_per_mm          # 손톱/피부 분리 임계 강성
float64 separation_margin             # 군집간거리 / 군집내분산
int32   cluster_hard_count
int32   cluster_soft_count
string  reject_reason                 # valid=false 사유

nail_msgs/BoundaryRegion region

builtin_interfaces/Time created_at
```

### 3.6 BoundaryRegion.msg

```
geometry_msgs/Point[] boundary_polygon    # 고강성(가공 허용) 외곽선
geometry_msgs/Point[] forbidden_polygon   # 저강성(가공 금지)
geometry_msgs/Point[] coat_polygon        # boundary에서 offset 안쪽

float64 boundary_offset_mm
float64 repeat_deviation_mm               # 반복 측정 편차
bool    reliable
```

### 3.7 SafetyState.msg

```
std_msgs/Header header

bool safe_to_move                     # 전 노드가 이 값 하나로 판단

bool estop_released
bool comm_ok
bool handrest_seated
bool dust_extraction_on
bool tool_grip_ok
bool scan_valid

string FAULT_ESTOP        = "FAULT_ESTOP"
string FAULT_COMM_LOST    = "FAULT_COMM_LOST"
string FAULT_TOOL_DROP    = "FAULT_TOOL_DROP"
string FAULT_NO_HANDREST  = "FAULT_NO_HANDREST"
string FAULT_NO_DUST      = "FAULT_NO_DUST"

string[] active_faults                # 비어 있어야 safe_to_move 가능
string   reason                       # 사람이 읽을 설명
```

> **UV 관련 필드가 없습니다.** 상시 ON 정책이므로 소프트웨어가 UV 상태를 소유하지 않습니다. UV 차단은 물리 결선(SDS §9.3)이 담당합니다.

### 3.8 ToolState.msg

```
std_msgs/Header header

string NONE    = "none"
string PROBE   = "probe"
string SANDER  = "sander"
string BRUSH   = "brush"
string COATER  = "coater"
string UV      = "uv"
string TWEEZERS= "tweezers"

string  current_tool
string  active_tcp                    # tcp_probe, tcp_sander, …
float64 grip_width_mm
float64 expected_width_mm
bool    grip_verified
```

### 3.9 ProcessState.msg

```
std_msgs/Header header

string STAGE_IDLE     = "IDLE"
string STAGE_PRECHECK = "PRECHECK"
string STAGE_SCAN     = "SCAN"
string STAGE_SAND     = "SAND"
string STAGE_BRUSH    = "BRUSH"
string STAGE_COAT     = "COAT"
string STAGE_CURE     = "CURE"
string STAGE_INSPECT  = "INSPECT"
string STAGE_REWORK   = "REWORK"
string STAGE_STONE    = "STONE"
string STAGE_FINISH   = "FINISH"
string STAGE_ABORTED  = "ABORTED"

string  session_id
string  stage
int32   layer_index
int32   layer_total
int32   rework_count
float64 stage_percent
float64 session_percent
string  current_tool
nail_msgs/ErrorCode last_error
```

### 3.10 ValidationResult.msg

```
string POINT_CENTER = "center"
string POINT_LEFT   = "left"
string POINT_RIGHT  = "right"

string RESULT_PASS  = "PASS"
string RESULT_FAIL  = "FAIL"
string RESULT_SKIP  = "SKIP"     # 마진 부족으로 측정 안 함

string  session_id
int32   layer_index
string  point_label              # POINT_*
geometry_msgs/Point position

float64 release_force_n
float64 stiffness_n_per_mm
float64 threshold_n              # 판정에 쓴 임계값 (사후 재해석용)
string  result

nail_msgs/ForceSample[] waveform # FAIL 시 필수. PASS 시 생략 가능
builtin_interfaces/Time measured_at
```

> **`threshold_n`을 결과에 함께 저장하는 이유**: 임계값은 캘리브레이션으로 계속 바뀝니다. 판정 시점의 임계값이 없으면 나중에 데이터를 다시 해석할 수 없습니다.

---

## 4. srv

### 4.1 ValidatePrecondition.srv

```
# 모든 공정 노드가 goal 수락 전에 호출한다.

string STAGE_SCAN    = "SCAN"
string STAGE_SAND    = "SAND"
string STAGE_BRUSH   = "BRUSH"
string STAGE_COAT    = "COAT"
string STAGE_CURE    = "CURE"
string STAGE_INSPECT = "INSPECT"
string STAGE_STONE   = "STONE"

string stage
string session_id
string required_tool
---
bool     ok
string[] blocking_reasons      # 막힌 이유 전부. 하나만 주지 말 것
```

### 4.2 ResetSafety.srv

```
bool confirm                   # true 일 때만 처리
string operator_note
---
bool     ok                    # remaining_faults 가 비었을 때만 true
string[] remaining_faults
```

### 4.3 GetStiffnessMap.srv

```
string session_id              # 불일치 시 found=false + E_MAP_SESSION_MISMATCH
---
bool                    found
nail_msgs/StiffnessMap  map
nail_msgs/ErrorCode     error
```

### 4.4 GetToolInfo.srv

```
string tool_id                 # 빈 문자열이면 현재 장착 툴
---
bool                 found
nail_msgs/ToolState  state
float64[6]           tcp_offset      # x,y,z(mm) rx,ry,rz(deg)
string               slot_frame
```

---

## 5. action — A계층 (스킬 프리미티브)

### 5.1 MoveTo.action

```
geometry_msgs/Pose target
string  frame_id
bool    linear                 # true=movel, false=movej
float64 speed_ratio            # 0.0~1.0
float64 accel_ratio
float64 timeout_s
---
nail_msgs/ResultBase base
float64 position_error_mm
---
float64 percent
geometry_msgs/Pose current_pose
```

### 5.2 PickPlace.action

```
string MODE_PICK  = "pick"
string MODE_PLACE = "place"

string  mode
string  target_key             # targets.yaml 키
string  frame_id
float64 approach_height_mm
float64 grip_width_mm          # PICK 시 목표 폭
float64 expected_width_mm
float64 width_tolerance_mm
bool    verify_grip
---
nail_msgs/ResultBase base
float64 measured_width_mm
bool    grip_verified
---
int32   step                   # 0=이동 1=하강 2=그립 3=검증 4=상승
float64 percent
```

### 5.3 ContactPath.action — 법선 접근

```
# 브러싱 · 도포용. 표면 법선(-Z) 방향으로 접근하고 +Z로 이탈한다.

geometry_msgs/Pose[] waypoints
string  frame_id
string  session_id

float64 target_force_n
float64 max_force_n
float64 feed_speed_mms
bool    use_compliance

# 강성 감시 (법선 접근에서만 유효)
bool    abort_on_low_stiffness
float64 min_stiffness_n_per_mm

geometry_msgs/Point[] allowed_polygon   # TCP 이탈 감시
int32   passes
float64 max_duration_s
---
nail_msgs/ResultBase base
float64 mean_force_n
float64 max_force_measured_n
float64 min_stiffness_measured_n_per_mm
float64 path_error_mm
int32   passes_done
int32[] missed_segment_indices
string  abort_reason           # ABORT_LOW_STIFFNESS / ABORT_OVERFORCE / …
nail_msgs/ForceSample[] force_log
---
float64 percent
int32   current_pass
nail_msgs/ForceSample current_wrench
```

### 5.4 LateralContact.action — 수평 접근 ★

```
# 연마 전용. 표면에 평행하게 접근하여 접근축으로만 힘을 유지한다.
#
# ⚠️ ContactPath 와 통합하지 말 것.
#    수평 접근은 압입이 없어 강성을 측정할 수 없으므로
#    E_LOW_STIFFNESS 가 작동하지 않는다.
#    피부 접촉을 막는 유일한 수단은 travel_limit_mm 이다.

geometry_msgs/Vector3 approach_vector   # 단위벡터. frame_id 기준 수평
geometry_msgs/Pose[]  waypoints         # 접근축에 수직인 평면 내 경로
string  frame_id
string  session_id

float64 work_plane_offset_mm            # 표면 기준 작업 평면 높이
float64 target_force_n                  # 접근축 유지력
float64 max_force_n                     # 접근축 상한
float64 jam_force_n                     # 진행축 걸림 판정
float64 feed_speed_mms

float64 travel_limit_mm      ★          # 접근축 최대 진행 거리. 0 이하면 REJECT
float64 retreat_mm                      # 이탈 시 역방향 후퇴 거리

int32   passes
float64 max_duration_s
---
nail_msgs/ResultBase base
float64 mean_force_n
float64 max_force_measured_n
float64 max_travel_mm        ★          # 실제 최대 진행. 한계 대비 여유 확인
float64 applied_travel_limit_mm         # 적용된 한계값 (리포트 기록)
float64 max_jam_force_n
int32   passes_done
string  abort_reason                    # ABORT_LATERAL_LIMIT / ABORT_LATERAL_JAM / …
nail_msgs/ForceSample[] force_log
---
float64 percent
int32   current_pass
float64 travel_mm                       # 현재 진행 거리 → UI 게이지
nail_msgs/ForceSample current_wrench
```

### 5.5 ProbePoint.action

```
# 스캔과 택프리 검사가 공유하는 최소 단위.

geometry_msgs/Point target
string  frame_id
float64 approach_height_mm
float64 probe_speed_mms
float64 contact_threshold_n
float64 max_depth_mm
float64 max_force_n
float64 lateral_force_limit_n

bool    measure_release                 # true면 이탈 중 Fz 측정 (택프리)
float64 release_speed_mms
string  source_tag                      # StiffnessPoint.source 에 기록
---
nail_msgs/ResultBase base
nail_msgs/StiffnessPoint point
nail_msgs/ForceSample[] waveform        # 하강+이탈 전체 파형
int32   regression_samples              # 회귀에 쓴 표본 수
---
float64 current_depth_mm
float64 current_force_n
```

### 5.6 ChangeTool.action

```
string  target_tool                     # ToolState 상수. "none"이면 반납만
float64 expected_width_mm
float64 width_tolerance_mm
bool    verify_after_grip
string  park_facing                     # UV 반납 자세 지정
---
nail_msgs/ResultBase base
nail_msgs/ToolState state
float64 measured_width_mm
---
int32   step                            # 0=반납 1=랙이동 2=픽업 3=TCP 4=검증
float64 percent
```

---

## 6. action — B계층 (공정)

### 6.1 ScanBoundary.action — 2단계 ★

```
string  session_id
string  frame_id

# 스캔 영역
float64 area_x_mm
float64 area_y_mm
float64 margin_mm

# 1단계
float64 coarse_pitch_mm                 # 기본 3.0
int32   coarse_min_valid_points
int32   coarse_min_per_cluster

# 2단계
float64 fine_pitch_mm                   # 기본 1.0
float64 boundary_band_mm                # 후보 중점 기준 밴드 폭
int32   fine_max_points                 # 시간 폭주 방지

# 판정
float64 separation_margin_min
float64 invalid_point_max_ratio
---
nail_msgs/ResultBase base
nail_msgs/StiffnessMap map
---
string STAGE_COARSE    = "COARSE"
string STAGE_CANDIDATE = "CANDIDATE"
string STAGE_FINE      = "FINE"

string  stage
nail_msgs/StiffnessPoint last_point
int32   points_done
int32   points_total
int32   candidate_count
float64 stage_percent
float64 overall_percent                 # coarse 30% + fine 70% 가중
```

### 6.2 SandSurface.action

```
string  session_id

string SIDE_FREE_EDGE = "free_edge"
string SIDE_LEFT      = "left"
string SIDE_RIGHT     = "right"

string  approach_side
float64 approach_pitch_deg              # 0 = 완전 수평
float64 work_plane_offset_mm

float64 target_force_n
float64 max_force_n
float64 jam_force_n

int32   passes
float64 step_over_mm
float64 feed_speed_mms
float64 max_duration_s

float64 travel_limit_margin_mm  ★       # 경계까지 거리에서 뺄 안전 마진
float64 forbidden_margin_mm
---
nail_msgs/ResultBase base
float64 mean_force_n
float64 max_force_measured_n
float64 max_travel_mm
float64 computed_travel_limit_mm ★      # 이번 세션에 계산된 한계값
float64 max_jam_force_n
int32   passes_done
string  abort_reason
---
float64 percent
int32   current_pass
float64 travel_mm
nail_msgs/ForceSample current_wrench
```

### 6.3 BrushDust.action

```
string  session_id
float64 target_force_n
float64 max_force_n
int32   passes
float64 path_pitch_mm
float64 feed_speed_mms
float64 coverage_margin_mm
float64 max_duration_s
---
nail_msgs/ResultBase base
int32   passes_done
string  abort_reason
---
float64 percent
int32   current_pass
```

### 6.4 CoatGel.action

```
string  session_id
int32   layer_index

float64 boundary_offset_mm
float64 target_force_n
float64 max_force_n
float64 path_pitch_mm
float64 feed_speed_mms
int32   passes
bool    use_compliance
float64 max_duration_s
---
nail_msgs/ResultBase base
float64 mean_force_n
float64 coverage_ratio                  # 궤적 기반 추정. 판정 기준 아님
int32   passes_done
string  abort_reason
---
float64 percent
int32   current_pass
```

> **`coverage_ratio`는 기록용입니다.** 도포 두께는 측정하지 않으며, 요구사항은 "빈 영역 없이 덮였는가"로 한정됩니다. result 에 두께 필드를 추가하지 마세요.

### 6.5 CureUV.action — permit 없음

```
# UV 램프는 상시 ON 이다. 이 액션은 "언제 켜는가"가 아니라
# "얼마나 오래 그 자리에 머무는가"로 조사량을 만든다.

string  session_id
int32   layer_index

geometry_msgs/Point[] target_regions    # 비어 있으면 전체. 차 있으면 부분 재조사
float64 standoff_mm
float64 standoff_tolerance_mm
int32   dwell_points
float64 dwell_s_per_point
float64 exposure_scale                  # 재조사 시 1.5 등
float64 path_speed_mms
float64 park_distance_mm
float64 max_duration_s                  # 모션 타임아웃 (램프 아님)
---
nail_msgs/ResultBase base
float64 actual_exposure_s               # 체류 시간 합계. 접근/이탈 노출 미포함
int32   dwell_completed
float64 mean_standoff_mm
float64 coverage_ratio                  # 궤적 로그. 판정 기준 아님
bool    parked                          # 대기 위치 이탈 완료
---
float64 percent
int32   current_dwell_index
float64 elapsed_dwell_s
float64 current_standoff_mm
```

> **`parked`를 반드시 확인하세요.** permit이 없으므로 안전 결함 시 유일한 대응은 물리적 이탈입니다. `parked=false`로 종료되는 경로가 있다면 그것 자체가 안전 결함입니다.

### 6.6 InspectCure.action — 3점 고정

```
# 검사점은 중앙 · 좌 · 우 3점으로 고정한다.

string  session_id
int32   layer_index

float64 center_offset_x_ratio           # 0 = 손톱 중심
float64 side_offset_y_ratio             # 경계까지 거리 대비 비율
float64 min_edge_clearance_mm           # 좌우점 최소 경계 이격

float64 tack_threshold_n
bool    require_all_pass

float64 probe_depth_mm
float64 probe_max_force_n               # 젤 관통 방지 상한
float64 release_speed_mms
float64 point_timeout_s
---
nail_msgs/ResultBase base
bool    passed
nail_msgs/ValidationResult[] results    # 3개 (마진 부족 시 SKIP 포함)
geometry_msgs/Point[] fail_points       # REWORK 대상 좌표
int32   points_measured
string  abort_reason
---
float64 percent
nail_msgs/ValidationResult last_result
```

> **3점의 한계를 결과 해석에 반영하세요.** `passed=true`는 "손톱 전체가 경화됨"이 아니라 **"검사한 3개 지점이 기준을 만족함"**입니다. 리포트 문구를 그렇게 쓰세요.

### 6.7 PlaceStone.action

```
string  session_id
geometry_msgs/Point target_position
float64 target_yaw_deg

float64 press_force_n
float64 press_duration_s
float64 position_tolerance_mm
int32   max_retry

bool    verify_enabled                  # 기본 false
int32   verify_probe_count
float64 approach_height_mm
---
nail_msgs/ResultBase base
geometry_msgs/Point actual_position
float64 position_error_mm               # verify_enabled=false 면 -1.0
int32   retry_count
string  abort_reason
---
int32   step
float64 percent
```

---

## 7. action — C계층

### 7.1 RunSession.action

```
string  session_id
string  recipe_id
string  shape_profile_id
string  target_material                 # 안전 검증용. 사람 신체 금지 (BR-029)

int32   layer_total
int32   max_rework
bool    enable_brush
bool    enable_stone
---
string RESULT_COMPLETED       = "COMPLETED"
string RESULT_COMPLETED_WARN  = "COMPLETED_WITH_WARN"
string RESULT_FAILED          = "FAILED"
string RESULT_ABORTED_SAFETY  = "ABORTED_SAFETY"
string RESULT_CANCELLED       = "CANCELLED"

bool    success
string  result_code
nail_msgs/StiffnessMap scan_result
nail_msgs/ValidationResult[] all_results
nail_msgs/ErrorCode final_error
int32   total_rework
int32   warn_count
builtin_interfaces/Time started_at
builtin_interfaces/Time finished_at
---
nail_msgs/ProcessState state            # ProcessState 를 통째로 전달
```

> feedback 으로 `ProcessState`를 그대로 싣고, **동일 내용을 `/process/status` 토픽에도 발행**합니다. 액션 feedback 은 goal 을 보낸 클라이언트만 받으므로, 여러 웹 화면이 동시에 보려면 토픽이 필요합니다.

---

## 8. 토픽 · QoS 계약

| 토픽 | 타입 | 발행 | 주기 | QoS |
|---|---|---|---|---|
| `/force/data` | `ForceSample` | robot_skill | 100 Hz | BEST_EFFORT, depth 1 |
| `/force/data_ui` | `ForceSample` | robot_skill | 20 Hz | BEST_EFFORT, depth 1 |
| `/robot/pose` | `PoseStamped` | robot_skill | 50 Hz | BEST_EFFORT, depth 1 |
| `/safety/status` | `SafetyState` | safety_monitor | 20 Hz | **RELIABLE + TRANSIENT_LOCAL** |
| `/tool/status` | `ToolState` | tool_manager | 변경 시 | **RELIABLE + TRANSIENT_LOCAL** |
| `/stiffness/map` | `StiffnessMap` | scan_node | 완료 시 | **RELIABLE + TRANSIENT_LOCAL** |
| `/process/status` | `ProcessState` | orchestrator | 변경 + 1 Hz | **RELIABLE + TRANSIENT_LOCAL** |
| `/validation/result` | `ValidationResult` | inspection, stone | 이벤트 | RELIABLE, depth 20 |

**TRANSIENT_LOCAL 을 쓴 4개는 "늦게 뜬 노드도 즉시 현재 상태를 받아야 하는" 것들입니다.** 특히 `/safety/status`가 VOLATILE 이면, 재시작한 노드가 안전 상태를 모르는 창(window)이 생깁니다.

**`/force/data`(100 Hz)는 웹으로 중계하지 마세요.** WebSocket 이 버티지 못합니다.

---

## 9. 통합 시 확인 사항

인터페이스를 병합하기 전에 아래를 확인합니다.

- [ ] `colcon build --packages-select nail_msgs` 성공
- [ ] `ros2 interface show nail_msgs/action/LateralContact` 출력 정상
- [ ] 모든 action result 의 첫 필드가 `ResultBase base`
- [ ] 모든 길이/힘 필드에 단위 접미사가 있음
- [ ] 에러 코드 문자열이 `ErrorCode.msg` 에만 정의됨 (노드에 중복 정의 없음)
- [ ] `session_id` 가 필요한 인터페이스에 빠짐없이 있음
- [ ] UV permit 관련 잔재가 없음 (`grep -ri "permit" nail_msgs/` 무결과)
- [ ] mock 관련 잔재가 없음 (`grep -ri "mock" nail_msgs/` 무결과)
