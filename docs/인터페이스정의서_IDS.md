# 인터페이스 정의서

**문서 ID** IDS-NAIL-v1.2 · **개정일** 2026-08-26 · **대상** `nail_msgs`

이 문서는 현재 `src/nail_msgs` 파일을 그대로 요약한다. 코드 생성의 최종 입력은
항상 실제 `.msg`, `.srv`, `.action` 파일이다.

## 1. 패키지 구성

```text
nail_msgs/
|-- msg/
|   |-- ErrorCode.msg
|   |-- ResultBase.msg
|   |-- SafetyState.msg
|   |-- ToolState.msg
|   |-- ProcessState.msg
|   |-- TaskPose.msg
|   |-- ProbeMeasurement.msg
|   `-- BoundaryMap.msg
|-- srv/
|   |-- ValidatePrecondition.srv
|   |-- ResetSafety.srv
|   `-- GetToolInfo.srv
`-- action/
    |-- MoveTo.action
    |-- PickPlace.action
    |-- ContactPath.action
    |-- LateralContact.action
    |-- ChangeTool.action
    |-- SandSurface.action
    |-- BrushDust.action
    |-- CoatGel.action
    |-- CureUV.action
    |-- PlaceStone.action
    |-- RunSession.action
    |-- ProbePoint.action
    `-- ScanBoundary.action
```

합계는 msg 8, srv 3, action 13이다. Probe 측정 메시지는 독립 검증용이다.

## 2. 공통 규약

- 움직임은 취소 가능한 Action으로 정의한다.
- 즉시 조회와 검증은 Service로 정의한다.
- 모든 스킬·공정 action result의 첫 필드는 `nail_msgs/ResultBase base`다.
- `geometry_msgs/Pose`와 `Point`는 ROS 표준 단위인 m, rad를 사용한다.
- 이름에 `_mm`, `_mms`, `_s`, `_deg`가 붙은 스칼라는 해당 단위를 사용한다.
- 생산 경로 액션은 티칭 좌표 기반이다. Probe 액션만 계산 외력 측정값을 포함한다.
- `PickPlace`는 그리퍼 명령 계약이며 파지 측정 필드를 포함하지 않는다.

## 3. 메시지

### 3.1 `ErrorCode.msg`

```text
string OK = ""

string E_INVALID_GOAL   = "E_INVALID_GOAL"
string E_SAFETY_BLOCKED = "E_SAFETY_BLOCKED"
string E_PRECOND_FAILED = "E_PRECOND_FAILED"
string E_TIMEOUT         = "E_TIMEOUT"
string E_CANCELLED       = "E_CANCELLED"
string E_COMM_LOST       = "E_COMM_LOST"
string E_MOTION_FAILED   = "E_MOTION_FAILED"
string E_OVERFORCE       = "E_OVERFORCE"
string E_NO_BOUNDARY     = "E_NO_BOUNDARY"
string E_LATERAL_LIMIT   = "E_LATERAL_LIMIT"
string E_TOOL_MISMATCH   = "E_TOOL_MISMATCH"
string E_GRIP_FAILED     = "E_GRIP_FAILED"

uint8 SEV_NONE   = 0
uint8 SEV_WARN   = 1
uint8 SEV_RETRY  = 2
uint8 SEV_ABORT  = 3
uint8 SEV_SAFETY = 4

string code
uint8 severity
string detail
```

`E_GRIP_FAILED`는 그리퍼 명령 호출 실패를 뜻한다. 실제 물체 파지 여부를
측정해서 발생하는 코드는 아니다.

`E_LATERAL_LIMIT`은 인터페이스 상수로 남아 있지만 현재 구현은 티칭 경로를
만들기 전에 기하 한계를 검사하고 `E_INVALID_GOAL`로 중단하므로 발행하지 않는다.

### 3.2 `ResultBase.msg`

```text
bool success
nail_msgs/ErrorCode error
geometry_msgs/Pose final_pose
float64 duration_s
builtin_interfaces/Time completed_at
```

### 3.3 `SafetyState.msg`

```text
std_msgs/Header header
bool safe_to_move
bool estop_released
bool comm_ok

string FAULT_ESTOP     = "FAULT_ESTOP"
string FAULT_COMM_LOST = "FAULT_COMM_LOST"

string[] active_faults
string reason
```

### 3.4 `ToolState.msg`

```text
std_msgs/Header header

string NONE     = "none"
string SANDER   = "sander"
string BRUSH    = "brush"
string COATER   = "coater"
string UV       = "uv"
string TWEEZERS = "tweezers"

string current_tool
string active_tcp
float64 grip_width_mm
```

`grip_width_mm`는 마지막 그리퍼 명령값이며 실측값이 아니다.

### 3.5 `ProcessState.msg`

```text
std_msgs/Header header

string STAGE_IDLE     = "IDLE"
string STAGE_PRECHECK = "PRECHECK"
string STAGE_SAND     = "SAND"
string STAGE_BRUSH    = "BRUSH"
string STAGE_COAT     = "COAT"
string STAGE_CURE     = "CURE"
string STAGE_STONE    = "STONE"
string STAGE_FINISH   = "FINISH"
string STAGE_ABORTED  = "ABORTED"

string session_id
string stage
int32 layer_index
int32 layer_total
float64 stage_percent
float64 session_percent
string current_tool
nail_msgs/ErrorCode last_error
```

### 3.6 Probe 측정 메시지

`ProbeMeasurement`는 요청·정지 위치, 접촉 여부, 이동거리, 공중/실제 압축력,
분리값, 옆힘과 전체 힘을 기록한다. `BoundaryMap`은 모든 측정값과
`boundary_polygon`, coarse/fine 점 수, 접촉 비율 및 유효성 사유를 묶는다.

## 4. 서비스

### 4.1 `ValidatePrecondition.srv`

```text
string STAGE_SAND  = "SAND"
string STAGE_BRUSH = "BRUSH"
string STAGE_COAT  = "COAT"
string STAGE_CURE  = "CURE"
string STAGE_STONE = "STONE"
string STAGE_PROBE = "PROBE"

string stage
string session_id
string required_tool
---
bool ok
string[] blocking_reasons
```

현재 구현은 `safe_to_move`와 `required_tool` 일치 여부를 검사한다.

### 4.2 `ResetSafety.srv`

```text
bool confirm
string operator_note
---
bool ok
string[] remaining_faults
```

### 4.3 `GetToolInfo.srv`

```text
string tool_id
---
bool found
nail_msgs/ToolState state
float64[6] tcp_offset
string slot_frame
```

`tool_id`가 비어 있으면 현재 장착 툴을 조회한다. `tcp_offset`은 x, y, z(mm),
rx, ry, rz(deg) 순서다.

## 5. 스킬 액션

### 5.1 `MoveTo.action`

```text
geometry_msgs/Pose target
string frame_id
string target_key
bool linear
float64 speed_ratio
float64 accel_ratio
float64 timeout_s
---
nail_msgs/ResultBase base
float64 position_error_mm
---
float64 percent
geometry_msgs/Pose current_pose
```

`target_key`가 비어 있지 않으면 `targets.yaml`의 티칭 자세가 `target`과
`frame_id`보다 우선한다.

### 5.2 `PickPlace.action`

```text
string MODE_PICK  = "pick"
string MODE_PLACE = "place"

string mode
string target_key
string frame_id
float64 approach_height_mm
float64 grip_width_mm
bool already_holding
---
nail_msgs/ResultBase base
---
int32 step
float64 percent
```

`step`은 0 이동, 1 하강, 2 그리퍼 명령, 4 상승이다. 파지 여부는 result로
판정하지 않고 운영자가 육안으로 확인한다.

### 5.3 `ContactPath.action`

```text
geometry_msgs/Pose[] waypoints
int32[] circular_via_indices
string frame_id
string reference_key
string session_id
float64 feed_speed_mms
float64 contact_offset_mm
geometry_msgs/Point[] allowed_polygon
int32 passes
float64 max_duration_s
---
nail_msgs/ResultBase base
float64 path_error_mm
int32 passes_done
int32[] missed_segment_indices
string abort_reason
---
float64 percent
int32 current_pass
```

`reference_key`가 있으면 `targets.yaml`의 접촉 자세를 기준으로 waypoint 위치를
상대 오프셋으로 사용한다. 비어 있으면 각 waypoint의 위치와 자세 전체를
`frame_id` 기준 절대 Pose로 사용한다. `circular_via_indices`의 값 `i`는 현재
위치에서 `waypoints[i]`를 지나 `waypoints[i+1]`로 가는 `MoveC`를 뜻하며,
나머지 점은 `MoveL`이다.
`contact_offset_mm`은 티칭점에서 tool +Z 방향 미세 보정이다. 양수는 표면
방향, 음수는 표면에서 멀어지는 방향이다. `allowed_polygon` 밖 waypoint는
이동하지 않고 경로를 중단하며, 종료 시 tool -Z 방향으로 이탈한다.

### 5.4 `LateralContact.action`

```text
geometry_msgs/Vector3 approach_vector
geometry_msgs/Pose[] waypoints
string frame_id
string session_id
float64 work_plane_offset_mm
float64 feed_speed_mms
float64 retreat_mm
int32 passes
float64 max_duration_s
---
nail_msgs/ResultBase base
int32 passes_done
string abort_reason
---
float64 percent
int32 current_pass
```

연마용 티칭 좌표 경로를 추종한다. 종료 시 `approach_vector` 반대 방향으로
`retreat_mm`만큼 후퇴한다. 기하 경계와 진입 깊이는 상위 `sanding_node`가
경로 생성 전에 계산한다.

### 5.5 `ChangeTool.action`

```text
string target_tool
string park_facing
---
nail_msgs/ResultBase base
nail_msgs/ToolState state
---
int32 step
float64 percent
```

현재 구현의 feedback 단계는 0 반납, 1 랙 이동, 2 픽업, 3 TCP, 4 상태 확정이다.
`target_tool="none"`은 반납만 수행한다.

### 5.6 `ProbePoint.action`

입력은 `search_start`, `press_direction`, 공중 Z 오프셋, 최대 깊이·속도,
공중 비교 margin, 전체/옆힘 상한, 연속 확인 수, 접촉 뒤 강성 측정 거리와
timeout이다.
`manual_probe_tool_confirmed=true`가 필수다. result의 `ProbeMeasurement`에서
비접촉은 `base.success=true`, `contact_detected=false`로 표현한다. 힘 상한은
`E_OVERFORCE`다. `stiffness_n_per_mm`은 접촉 뒤 `stiffness_depth_mm` 동안의
압축력 증가율이며 재질 분류용이다.

## 6. 공정 액션

### 6.0 `ScanBoundary.action`

`scan_corners`에는 top-left, top-right, bottom-right, bottom-left 순서의 공중 Pose
네 점을 넣는다. `nail_reference`는 손톱 내부 기준점, `dummy_references`는 손톱
밖 더미손 기준점이다. 누르는 방향은 항상 `base_link (0, 0, -1)`로 고정하며
별도 입력하지 않는다. 기준점의 반복 강성 중앙값 차이가
`material_min_separation_n_per_mm`보다 작으면 `E_NO_BOUNDARY`로 중단한다. 각
격자점은 가까운 강성 기준으로 분류하고, 손톱 기준점에 연결된 영역만 사용한다.
3mm 거친 탐색 뒤 전환 구간 주변을 1mm로 재탐색한다. 결과는 `BoundaryMap`이며
경계가 없으면 `E_NO_BOUNDARY`다. `BoundaryMap`에는 손톱·더미 기준 강성 중앙값과
차이도 반환해 실기 기준값을 조정할 수 있다. 독립 검증용으로 세션 orchestrator에는 연결되지 않는다.

### 6.1 `SandSurface.action`

```text
string session_id

string SIDE_FREE_EDGE = "free_edge"
string SIDE_LEFT      = "left"
string SIDE_RIGHT     = "right"

string approach_side
float64 approach_pitch_deg
float64 work_plane_offset_mm
int32 passes
float64 step_over_mm
float64 feed_speed_mms
float64 max_duration_s
float64 travel_limit_margin_mm
---
nail_msgs/ResultBase base
float64 max_travel_mm
float64 computed_travel_limit_mm
int32 passes_done
string abort_reason
---
float64 percent
int32 current_pass
float64 travel_mm
```

travel 값은 티칭 경계와 설정된 진입 깊이로 계산한 기하 값이다.

### 6.2 `BrushDust.action`

```text
string session_id
int32 passes
float64 path_pitch_mm
float64 feed_speed_mms
float64 coverage_margin_mm
float64 max_duration_s
---
nail_msgs/ResultBase base
int32 passes_done
string abort_reason
---
float64 percent
int32 current_pass
```

### 6.3 `CoatGel.action`

```text
string session_id
int32 layer_index
float64 boundary_offset_mm
float64 path_pitch_mm
float64 feed_speed_mms
int32 passes
float64 max_duration_s
---
nail_msgs/ResultBase base
float64 coverage_ratio
int32 passes_done
string abort_reason
---
float64 percent
int32 current_pass
```

`coverage_ratio`는 좌표 경로 기반 추정치이며 실제 도포 상태 측정값이 아니다.

### 6.4 `CureUV.action`

```text
string session_id
int32 layer_index
float64 standoff_mm
float64 standoff_tolerance_mm
int32 dwell_points
float64 dwell_s_per_point
float64 path_speed_mms
float64 park_distance_mm
float64 max_duration_s
---
nail_msgs/ResultBase base
float64 actual_exposure_s
int32 dwell_completed
float64 mean_standoff_mm
float64 coverage_ratio
bool parked
---
float64 percent
int32 current_dwell_index
float64 elapsed_dwell_s
float64 current_standoff_mm
```

UV 램프는 상시 ON이다. `actual_exposure_s`는 계획된 체류 시간 합계이며 접근과
이탈 중 노출은 포함하지 않는다.

### 6.5 `PlaceStone.action`

```text
string session_id
geometry_msgs/Point target_position
float64 target_yaw_deg
float64 press_duration_s
float64 approach_height_mm
---
nail_msgs/ResultBase base
geometry_msgs/Point actual_position
string abort_reason
---
int32 step
float64 percent
```

`target_position`은 `nail_local_frame` 기준 m 단위다. `actual_position`은 명령
위치이며 측정값이 아니다. 부착 상태는 육안으로 확인한다.

## 7. 세션 액션

### 7.1 `RunSession.action`

```text
string session_id
string recipe_id
string shape_profile_id
string target_material
int32 layer_total
bool enable_brush
bool enable_stone
---
string RESULT_COMPLETED      = "COMPLETED"
string RESULT_COMPLETED_WARN = "COMPLETED_WITH_WARN"
string RESULT_FAILED         = "FAILED"
string RESULT_ABORTED_SAFETY = "ABORTED_SAFETY"
string RESULT_CANCELLED      = "CANCELLED"

bool success
string result_code
nail_msgs/ErrorCode final_error
int32 warn_count
builtin_interfaces/Time started_at
builtin_interfaces/Time finished_at
---
nail_msgs/ProcessState state
```

## 8. 토픽과 QoS

| 토픽 | 타입 | 발행자 | QoS |
|---|---|---|---|
| `/robot/pose` | `geometry_msgs/PoseStamped` | `robot_skill_node` | BEST_EFFORT, depth 1 |
| `/safety/status` | `nail_msgs/SafetyState` | `safety_monitor` | RELIABLE + TRANSIENT_LOCAL |
| `/tool/status` | `nail_msgs/ToolState` | `tool_manager` | RELIABLE + TRANSIENT_LOCAL |
| `/process/status` | `nail_msgs/ProcessState` | `session_orchestrator` | RELIABLE + TRANSIENT_LOCAL |

웹 브리지 화이트리스트에는 `/safety/status`와 `/process/status`만 포함된다.

## 9. 통합 확인

- `CMakeLists.txt`의 5 msg, 3 srv, 11 action이 실제 파일과 일치해야 한다.
- 모든 스킬·공정 action result는 `ResultBase base`로 시작해야 한다.
- 모든 동작 노드는 최신 `/safety/status`를 확인해야 한다.
- 상위 취소와 타임아웃은 현재 하위 action goal로 전파되어야 한다.
- TCP 이름과 `ToolState.active_tcp`가 실제 활성 TCP와 일치해야 한다.
- `PickPlace` 성공을 실제 파지 확인으로 해석하지 않아야 한다.
