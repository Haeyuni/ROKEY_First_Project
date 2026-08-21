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


def _static_frame_nodes(static_frames_file):
    with open(static_frames_file) as f:
        cfg = yaml.safe_load(f) or {}
    nodes = []
    for name, frame in (cfg.get('frames') or {}).items():
        nodes.append(Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name=f'static_tf_{name}',
            output='screen',
            arguments=[
                '--frame-id', str(frame['parent_frame']),
                '--child-frame-id', name,
                '--x', str(frame.get('x_mm', 0.0) / 1000.0),
                '--y', str(frame.get('y_mm', 0.0) / 1000.0),
                '--z', str(frame.get('z_mm', 0.0) / 1000.0),
                '--roll', str(math.radians(frame.get('roll_deg', 0.0))),
                '--pitch', str(math.radians(frame.get('pitch_deg', 0.0))),
                '--yaw', str(math.radians(frame.get('yaw_deg', 0.0))),
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
        if token == 'skill':
            params['targets_yaml_path'] = targets_yaml_path
            params['base_frame_id'] = base_frame_id

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
