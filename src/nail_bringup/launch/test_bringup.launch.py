"""단위 테스트용 조합 런치 — 필요한 노드만 골라 띄운다.

**`safety`/`skill`/`tool` 은 ws_dsr 를 sourcing 하지 않으면 기동 자체가
죽는다.** `DsrAdapter.__init__` 이 `import DSR_ROBOT2`(`dsr_common2` 제공,
`dsr_msgs2` 를 다시 import)를 하는데 이게 실패하면 `DsrAdapterError` 를 그대로
재던져 노드 프로세스가 즉시 크래시한다 (실측 확인됨). 이 워크스페이스만
source 하면 `ModuleNotFoundError: dsr_msgs2` 로 즉사한다 — 반드시
`source ~/<ws_dsr 경로>/install/setup.bash` 를 이 워크스페이스보다 먼저(또는
같이 overlay로) source 한 뒤 이 launch 를 실행할 것 (project memory: ws_dsr
sibling workspace 참조).

**단, `dsr_bringup2`(실제 컨트롤러 드라이버 프로세스)는 이 launch 가 켜지
않고, 안 켜져 있어도 노드 기동 자체는 성공한다** — import 만 되면 그 다음
`_wait_for_service_discovery` 는 15 초까지 기다리다 실패해도 **WARN만 찍고
계속 진행**한다(로그: "서비스 디스커버리 타임아웃"). 즉 "노드가 뜬 것"과
"액션이 실제로 동작하는 것"은 다르다 — MoveTo/PickPlace 등 액션을 실제로
쏘면 두산 서비스가 응답할 때까지 무한 대기한다. 액션까지 검증하려면
`dsr_bringup2`(+ onrobot 그리퍼 드라이버)까지 띄워야 한다.

**`use_mock_hardware` 는 아직 실제로 배선돼 있지 않다.** robot_skill_node /
tool_manager / safety_monitor 가 파라미터로 선언은 하고 있지만(NIS §3.6),
`DsrAdapter` 안에는 mock 분기가 없다 — 즉 이 파라미터를 true 로 줘도 여전히
진짜 두산 서비스를 호출하려 시도한다. 여기서는 향후를 위해 통과만 시켜두고,
지금 단계의 "하드웨어 없는 단독 실행"은 이 launch 로 A계층(safety/skill/
tool)만 띄우고 실기 없이 노드가 죽지 않는지 확인하는 정도로 이해하면 된다.

## 사용법

```bash
# 기본값: safety_monitor + robot_skill_node + tool_manager (A계층 substrate)
ros2 launch nail_bringup test_bringup.launch.py

# 연마 노드 단위 테스트: substrate + sanding_node
ros2 launch nail_bringup test_bringup.launch.py nodes:=safety,skill,tool,sanding

# 전체 스택
ros2 launch nail_bringup test_bringup.launch.py nodes:=all

# 로그 레벨 낮추기 (힘 데이터 등 DEBUG 로그 보기)
ros2 launch nail_bringup test_bringup.launch.py nodes:=safety,skill,tool,sanding log_level:=debug
```

`nodes` 토큰: `frames` `safety` `skill` `tool` `scan` `sanding` `brushing`
`coating` `curing` `inspection` `stone` `orchestrator` (또는 `all`). 콤마로
여러 개.

각 토큰이 실제로 무엇을 필요로 하는지는 NIS-NAIL-v0.2 §11.1 인터페이스
매트릭스와 docs/노드별_단위테스트_가이드.md 를 참조 — 이 launch 파일은
**의존성을 자동으로 끼워 넣지 않는다** (예: `sanding` 을 넣어도 `scan` 이
자동으로 따라 붙지 않음). 무엇이 켜져 있고 무엇이 꺼져 있는지 항상 명시적으로
알 수 있게 하기 위한 설계다 — REJECT(E_NO_SCAN 등)도 유효한 테스트 케이스다.

## `frames` 토큰 — NIS §11.4 고정 TF (tool_rack_frame/slot_*/handrest_frame/nail_frame)

`config/static_frames.yaml` 을 읽어 프레임마다 `tf2_ros
static_transform_publisher` 를 하나씩 띄운다. **이 좌표는 전부
placeholder다.** `ChangeTool`/슬롯 대상 `PickPlace` 를 동작만 시켜보고
싶을 때 켜되, 실측/CAD 확정 전에는 이 프레임으로 로봇을 실제로 움직이지
말 것 — `static_frames.yaml` 상단 경고를 반드시 읽을 것.
"""
import math

from ament_index_python.packages import get_package_share_directory
import yaml

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

# 토큰 -> (package, executable, node_name, dsr 파라미터 필요 여부)
_NODE_SPECS = {
    'safety':       ('nail_safety',       'safety_monitor_node',         'safety_monitor',        True),
    'skill':        ('nail_skill',        'robot_skill_node',            'robot_skill_node',      True),
    'tool':         ('nail_skill',        'tool_manager',                'tool_manager',          True),
    'scan':         ('nail_perception',   'scan_node',                   'scan_node',              False),
    'sanding':      ('nail_process',      'sanding_node',                'sanding_node',           False),
    'brushing':     ('nail_process',      'brushing_node',               'brushing_node',          False),
    'coating':      ('nail_process',      'coating_node',                'coating_node',           False),
    'curing':       ('nail_process',      'curing_node',                 'curing_node',            False),
    'inspection':   ('nail_process',      'inspection_node',             'inspection_node',        False),
    'stone':        ('nail_process',      'stone_node',                  'stone_node',             False),
    'orchestrator': ('nail_orchestrator', 'session_orchestrator_node',   'session_orchestrator',   False),
}

# NIS §8 동작 순서를 따른 'all' 확장 순서 ('frames' 는 다른 토큰이 TF 를
# 참조하기 전에 뜨도록 맨 앞. 툴 교체가 실제로 일어나진 않지만 로그를
# 읽을 때 위에서 아래로 자연스럽게 보이도록)
_ALL_ORDER = ['frames', 'safety', 'skill', 'tool', 'scan', 'sanding', 'brushing',
              'coating', 'curing', 'inspection', 'stone', 'orchestrator']


# --- frame 값 <-> 3x3 회전행렬 계산 (absolute: true 프레임의 부모 기준 상대값
# 자동 계산용). roll/pitch/yaw 는 tf2 setRPY 와 동일한 고정축(extrinsic)
# 컨벤션 R = Rz(yaw) @ Ry(pitch) @ Rx(roll) 을 따른다.
def _rpy_to_matrix(roll, pitch, yaw):
    cr, sr = math.cos(roll), math.sin(roll)
    cp, sp = math.cos(pitch), math.sin(pitch)
    cy, sy = math.cos(yaw), math.sin(yaw)
    return (
        (cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr),
        (sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr),
        (-sp, cp * sr, cp * cr),
    )


def _zyz_to_matrix(rz1, ry, rz2):
    """두산 posx 의 Task 자세(A,B,C) 컨벤션: R = Rz(rz1) @ Ry(ry) @ Rz(rz2).
    nail_skill.conversions.task_pose_to_ros_pose 와 동일한 식이다 — 티치펜던트
    TASK 화면에 그대로 표시되는 A/B/C 값을 그대로 여기 넣을 수 있게 하기
    위함이다. ⚠️ roll/pitch/yaw(_rpy_to_matrix, tf2 표준 고정축 컨벤션)와는
    다른 회전이다 — 펜던트에서 읽은 A,B,C 를 roll_deg/pitch_deg/yaw_deg 에
    그대로 넣으면 숫자는 맞아도 실제 회전은 달라진다(실측으로 확인됨:
    position_error 는 0에 가까운데 orientation 이 완전히 다르게 나옴)."""
    def rotz(a):
        c, s = math.cos(a), math.sin(a)
        return ((c, -s, 0.0), (s, c, 0.0), (0.0, 0.0, 1.0))

    def roty(a):
        c, s = math.cos(a), math.sin(a)
        return ((c, 0.0, s), (0.0, 1.0, 0.0), (-s, 0.0, c))

    return _mat_mul(_mat_mul(rotz(rz1), roty(ry)), rotz(rz2))


def _mat_to_quat(r):
    m00, m01, m02 = r[0]
    m10, m11, m12 = r[1]
    m20, m21, m22 = r[2]
    tr = m00 + m11 + m22
    if tr > 0:
        s = math.sqrt(tr + 1.0) * 2.0
        w = 0.25 * s
        x = (m21 - m12) / s
        y = (m02 - m20) / s
        z = (m10 - m01) / s
    elif m00 > m11 and m00 > m22:
        s = math.sqrt(1.0 + m00 - m11 - m22) * 2.0
        w = (m21 - m12) / s
        x = 0.25 * s
        y = (m01 + m10) / s
        z = (m02 + m20) / s
    elif m11 > m22:
        s = math.sqrt(1.0 + m11 - m00 - m22) * 2.0
        w = (m02 - m20) / s
        x = (m01 + m10) / s
        y = 0.25 * s
        z = (m12 + m21) / s
    else:
        s = math.sqrt(1.0 + m22 - m00 - m11) * 2.0
        w = (m10 - m01) / s
        x = (m02 + m20) / s
        y = (m12 + m21) / s
        z = 0.25 * s
    return x, y, z, w


def _mat_mul(a, b):
    return tuple(tuple(sum(a[i][k] * b[k][j] for k in range(3)) for j in range(3))
                 for i in range(3))


def _mat_vec(a, v):
    return tuple(sum(a[i][k] * v[k] for k in range(3)) for i in range(3))


def _mat_transpose(a):
    return tuple(tuple(a[j][i] for j in range(3)) for i in range(3))


def _pose_m_rad(frame):
    """프레임 하나의 (회전행렬, 평행이동[m]) 을 만든다.

    rz1_deg/ry_deg/rz2_deg 가 있으면 두산 Task 자세(A,B,C, 티치펜던트 TASK
    화면에 그대로 표시됨) 로 해석한다 — 없으면 기존 roll_deg/pitch_deg/
    yaw_deg(tf2 표준 고정축 RPY) 를 쓴다. 이 둘은 서로 다른 회전 표현이니
    펜던트에서 읽은 A,B,C 는 반드시 rz1_deg/ry_deg/rz2_deg 에 넣을 것.
    """
    if any(k in frame for k in ('rz1_deg', 'ry_deg', 'rz2_deg')):
        r = _zyz_to_matrix(math.radians(frame.get('rz1_deg', 0.0)),
                            math.radians(frame.get('ry_deg', 0.0)),
                            math.radians(frame.get('rz2_deg', 0.0)))
    else:
        r = _rpy_to_matrix(math.radians(frame.get('roll_deg', 0.0)),
                            math.radians(frame.get('pitch_deg', 0.0)),
                            math.radians(frame.get('yaw_deg', 0.0)))
    t = (frame.get('x_mm', 0.0) / 1000.0,
         frame.get('y_mm', 0.0) / 1000.0,
         frame.get('z_mm', 0.0) / 1000.0)
    return r, t


def _compose(parent_pose, child_pose):
    r_p, t_p = parent_pose
    r_c, t_c = child_pose
    r = _mat_mul(r_p, r_c)
    t = tuple(a + b for a, b in zip(_mat_vec(r_p, t_c), t_p))
    return r, t


def _relative_from_absolutes(parent_abs, child_abs):
    """parent_abs, child_abs 모두 base_link 기준 절대 포즈일 때, parent -> child
    상대 포즈(회전행렬, 평행이동)를 역산한다."""
    r_p, t_p = parent_abs
    r_c, t_c = child_abs
    r_pt = _mat_transpose(r_p)
    r_rel = _mat_mul(r_pt, r_c)
    t_rel = _mat_vec(r_pt, tuple(c - p for c, p in zip(t_c, t_p)))
    return r_rel, t_rel


_IDENTITY_POSE = ((1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)), (0.0, 0.0, 0.0)


def _static_frame_nodes(static_frames_file):
    """각 프레임 항목은 기본적으로 parent_frame 기준 상대값(x_mm/y_mm/z_mm/
    roll·pitch·yaw_deg)이다. `absolute: true` 를 추가하면 그 값을 base_link
    기준 절대좌표(예: 티치펜던트 TASK-BASE 로 읽은 값)로 취급하고, 여기서
    parent_frame 기준 상대값으로 자동 환산해 static_transform_publisher 에
    넘긴다 — slot_1~6 처럼 부모(tool_rack_frame)가 있는 프레임도 "베이스
    기준으로 읽은 값을 그대로 입력"할 수 있게 하기 위함이다.
    """
    with open(static_frames_file) as f:
        cfg = yaml.safe_load(f) or {}
    frame_defs = cfg.get('frames') or {}
    absolute_poses = {'base_link': _IDENTITY_POSE}

    def resolve_absolute(name):
        if name in absolute_poses:
            return absolute_poses[name]
        frame = frame_defs[name]
        parent_abs = resolve_absolute(frame['parent_frame'])
        given = _pose_m_rad(frame)
        pose = given if frame.get('absolute', False) else _compose(parent_abs, given)
        absolute_poses[name] = pose
        return pose

    nodes = []
    for name, frame in frame_defs.items():
        abs_pose = resolve_absolute(name)
        if frame.get('absolute', False):
            parent_abs = absolute_poses[frame['parent_frame']]
            r_rel, t_rel = _relative_from_absolutes(parent_abs, abs_pose)
        else:
            r_rel, t_rel = _pose_m_rad(frame)

        x, y, z = t_rel
        qx, qy, qz, qw = _mat_to_quat(r_rel)

        nodes.append(Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name=f'static_tf_{name}',
            output='screen',
            arguments=[
                '--frame-id', str(frame['parent_frame']),
                '--child-frame-id', name,
                '--x', str(x),
                '--y', str(y),
                '--z', str(z),
                # 회전은 쿼터니언으로 직접 넘긴다 — roll/pitch/yaw 로의 역변환은
                # 짐벌락 부근에서 값이 여러 개로 갈라질 수 있어 피한다.
                '--qx', str(qx),
                '--qy', str(qy),
                '--qz', str(qz),
                '--qw', str(qw),
            ],
        ))
    return nodes


def _launch_setup(context, *args, **kwargs):
    raw = LaunchConfiguration('nodes').perform(context).strip()
    tokens = _ALL_ORDER if raw == 'all' else [t.strip() for t in raw.split(',') if t.strip()]

    valid_tokens = set(_NODE_SPECS) | {'frames'}
    unknown = [t for t in tokens if t not in valid_tokens]
    if unknown:
        valid = ', '.join(_ALL_ORDER)
        raise RuntimeError(
            f"알 수 없는 nodes 토큰: {unknown}. 사용 가능: {valid}, all")

    use_mock = LaunchConfiguration('use_mock_hardware').perform(context)
    sync_tcp_on_startup = LaunchConfiguration('sync_tcp_on_startup').perform(context)
    require_handrest = LaunchConfiguration('require_handrest').perform(context)
    heartbeat_timeout_ms = LaunchConfiguration('heartbeat_timeout_ms').perform(context)
    safety_publish_rate_hz = LaunchConfiguration('safety_publish_rate_hz').perform(context)
    dsr_prefix = LaunchConfiguration('dsr_prefix').perform(context)
    robot_model = LaunchConfiguration('robot_model').perform(context)
    rack_config_file = LaunchConfiguration('rack_config_file').perform(context)
    targets_yaml_path = LaunchConfiguration('targets_yaml_path').perform(context)
    base_frame_id = LaunchConfiguration('base_frame_id').perform(context)
    static_frames_file = LaunchConfiguration('static_frames_file').perform(context)
    log_level = LaunchConfiguration('log_level').perform(context)

    actions = [LogInfo(msg=f'[nail_bringup] 기동 대상: {tokens}')]

    for token in tokens:
        if token == 'frames':
            actions.extend(_static_frame_nodes(static_frames_file))
            continue

        package, executable, node_name, needs_dsr = _NODE_SPECS[token]

        params = {}
        if needs_dsr:
            params['dsr_prefix'] = dsr_prefix
            params['robot_model'] = robot_model
            params['use_mock_hardware'] = use_mock == 'true'
        if token == 'tool':
            params['rack_config_file'] = rack_config_file
            params['sync_tcp_on_startup'] = sync_tcp_on_startup == 'true'
        if token == 'skill':
            params['targets_yaml_path'] = targets_yaml_path
            params['base_frame_id'] = base_frame_id
        if token == 'safety':
            params['require_handrest'] = require_handrest == 'true'
            params['heartbeat_timeout_ms'] = int(heartbeat_timeout_ms)
            params['publish_rate_hz'] = int(safety_publish_rate_hz)

        actions.append(Node(
            package=package,
            executable=executable,
            name=node_name,
            output='screen',
            emulate_tty=True,
            parameters=[params] if params else None,
            arguments=['--ros-args', '--log-level', log_level],
        ))

    return actions


def generate_launch_description():
    default_rack_config = get_package_share_directory('nail_skill') + '/config/tool_rack.yaml'
    default_targets = get_package_share_directory('nail_skill') + '/config/targets.yaml'
    default_static_frames = get_package_share_directory('nail_bringup') + '/config/static_frames.yaml'

    return LaunchDescription([
        DeclareLaunchArgument(
            'nodes', default_value='safety,skill,tool',
            description=(
                "쉼표로 구분한 노드 토큰 또는 'all'. 토큰: frames, safety, skill, "
                "tool, scan, sanding, brushing, coating, curing, inspection, "
                "stone, orchestrator. 의존 노드는 자동으로 추가되지 않는다 — "
                "직접 나열할 것.")),
        DeclareLaunchArgument(
            'use_mock_hardware', default_value='false',
            description=(
                'safety/skill/tool 에 전달. 주의: DsrAdapter 에 mock 분기가 아직 '
                '없어 true 로 줘도 실제 dsr 서비스를 호출하려 시도한다.')),
        DeclareLaunchArgument('dsr_prefix', default_value='dsr01',
                               description='두산 드라이버 네임스페이스 (ws_dsr 쪽과 일치해야 함)'),
        DeclareLaunchArgument(
            'heartbeat_timeout_ms', default_value='800',
            description=(
                'safety_monitor 에 전달. 실측: 20Hz 주기로 DI 2개+robot_state 1개를 '
                '순차 호출하는데, robot_skill_node 등 다른 노드도 동시에 같은 두산 '
                '컨트롤러에 서비스를 호출해서 200ms(구 기본값)는 자주 넘겨 '
                'FAULT_COMM_LOST 가 반복적으로 래치됐다. 800으로 실기 검증됨 — '
                '더 심하면 1000까지 올릴 것. (safety_monitor_node.py 자체 기본값과 '
                '별개로 이 launch 인자가 항상 파라미터로 전달되어 덮어쓰므로, 노드 쪽 '
                '기본값만 고쳐서는 여기 값이 안 바뀐다.)')),
        DeclareLaunchArgument(
            'safety_publish_rate_hz', default_value='10',
            description=(
                'safety_monitor 에 전달. 낮출수록(예: 5~10) 두산 컨트롤러 서비스 '
                '호출 빈도가 줄어 다른 노드와의 경합/FAULT_COMM_LOST 오탐이 준다 — '
                '단 안전 상태 갱신도 그만큼 느려진다. 10으로 실기 검증됨.')),
        DeclareLaunchArgument(
            'require_handrest', default_value='false',
            description=(
                'safety_monitor 에 전달. FAULT_NO_HANDREST 는 실시간 결함이라 '
                'ResetSafety 로 안 풀리고 handrest 센서(DI)가 "안착"으로 바뀌어야만 '
                '자동 해제된다. 현재 하드웨어에 안착 센서가 미장착이라 기본값을 '
                'false 로 꺼둠 — 안착 센서가 실제로 달리면 true 로 되돌려서 재검증할 것.')),
        DeclareLaunchArgument(
            'sync_tcp_on_startup', default_value='true',
            description=(
                'tool_manager 기동 시 rack_config_file 의 tcp_offset 을 add_tcp 로 '
                '컨트롤러에 등록 시도할지 여부. 컨트롤러가 config_create_tcp 를 '
                '거부하는 환경(예: 외부 제어 중 config 쓰기 차단)에서는 false 로 꺼서 '
                '매번 뜨는 실패 로그를 없애고, 티치펜던트에 수동 등록된 TCP 를 그대로 쓴다.')),
        DeclareLaunchArgument('robot_model', default_value='m0609'),
        DeclareLaunchArgument('rack_config_file', default_value=default_rack_config,
                               description='tool_manager 슬롯 좌표/TCP 오프셋 yaml (절대경로 권장)'),
        DeclareLaunchArgument('targets_yaml_path', default_value=default_targets,
                               description=(
                                   'robot_skill_node PickPlace target_key 조회 테이블. '
                                   'slot_* 는 여기 없다 — TF 프레임 폴백으로 조회됨(NIS §11.4)')),
        DeclareLaunchArgument(
            'base_frame_id', default_value='base_link',
            description=(
                "robot_skill_node 기본값('base_0')은 이 ws_dsr 드라이버가 실제로 "
                "발행하는 URDF 루트 프레임 이름과 다르다 (실측: world -> base_link "
                "-> link_1..6 -> tool0). 'base_0' 그대로 두면 MoveTo/PickPlace 가 "
                "전부 lookupTransform 에서 target_frame 부재로 즉시 ABORT된다 — "
                "다른 dsr_description 버전을 쓰면 이 값도 다시 확인할 것.")),
        DeclareLaunchArgument(
            'static_frames_file', default_value=default_static_frames,
            description=(
                "'frames' 토큰이 읽는 NIS §11.4 고정 프레임 좌표. 전부 "
                "placeholder — 실측 전 로봇 이동에 쓰지 말 것.")),
        DeclareLaunchArgument('log_level', default_value='info',
                               description='debug 로 주면 §3.5 DEBUG 급 힘 로그도 보임'),
        OpaqueFunction(function=_launch_setup),
    ])
