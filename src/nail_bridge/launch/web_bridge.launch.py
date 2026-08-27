"""web_bridge — ROS2와 백엔드 사이의 유일한 통로 (NIS §9, D계층).

**이 패키지에는 손으로 쓴 rclpy 노드가 없다.** NIS §9 "구성"이 "기본은
`rosbridge_server`(WebSocket)를 그대로 씁니다"라고 명시한다 — 다른 노드들과
달리 web_bridge_node의 정체는 `rosbridge_suite`의 `rosbridge_websocket`
실행 파일 하나이고, 이 launch 파일이 하는 일은 그걸 **"계약을 좁게
유지한다"**(§9 역할)는 요구에 맞게 화이트리스트 파라미터로 감싸 띄우는
것뿐이다. 새 프로토콜을 만들지 않는다.

**화이트리스트가 실제로 막는지 검증한 근거** (RobotWebTools/rosbridge_suite
ros2 브랜치, `rosbridge_server/scripts/rosbridge_websocket.py` 직접 확인):

  - `topics_glob`/`topics_pub_glob`/`topics_sub_glob`/`services_glob`/
    `actions_glob`/`params_glob` 는 전부 실존하는 파라미터다.
  - **빈 문자열("")과 빈 리스트("[]")의 의미가 정반대다** — 이게 이 launch
    파일에서 가장 틀리기 쉬운 지점이다:
      · `""` (빈 문자열) → `parse_glob_string` 이 `None` 반환 → **필터
        없음 = 전부 허용**. rosbridge 기본값이 바로 이거라, 아무 설정 없이
        띄우면 ROS 그래프 전체가 웹에 그대로 노출된다.
      · `"[]"` (빈 리스트 문자열) → 빈 패턴 리스트 → **아무것도 매치 안 됨
        = 전부 차단**.
    그래서 아래 `_glob_str([])` 은 반드시 `"[]"` 를 만들어야 하고, 실수로
    빈 문자열을 넘기면 "차단"이 아니라 "무제한 허용"이 되어 이 launch
    파일의 존재 이유가 사라진다.
  - 문자열 형식 자체도 특이하다 — JSON 배열이 아니라
    `"['/a', '/b']"` 처럼 파이썬 리스트 리터럴처럼 보이는 문자열을
    파싱한다(`parse_glob_string`, 대괄호 벗기고 콤마로 split). `_glob_str()`
    이 이 형식을 만든다.

**`topics_pub_glob=[]`인 이유**: NIS §9 "Web → ROS2" 표에는 Action(실행/
취소)과 Service 뿐, 웹이 토픽에 직접 publish 하는 경로가 없다. 화이트리스트
토픽은 전부 "ROS2 → Web" 방향(중계)이므로, publish 쪽은 통째로 막고
subscribe 쪽만 `relay_topics` 로 연다.

**`max_relay_rate_hz` 를 서버가 강제하지 않는 이유**: rosbridge 의 rate
제한은 구독 요청 시 클라이언트가 보내는 `throttle_rate` 필드를 그대로
따르는 클라이언트-주도 방식이다 — 서버 쪽 상한을 강제하는 파라미터가 없다.
다만 화이트리스트 토픽은 소스 자체가 이미 낮은 주기로 발행하므로
(`/safety/status` 는 safety_monitor 의 `publish_rate_hz` 기본 3(FAULT_COMM_LOST
오탐 대응으로 하향), 나머지는
이벤트/변경 기반으로 더 느리다) 이 상한은 설계상 이미 지켜진다 — 여기서
추가로 스로틀링 코드를 만들지 않는다.

**"웹 연결이 끊겨도 세션은 계속 진행"(§9 에러 표)도 여기서 구현할 게
없다.** rosbridge 는 WebSocket 연결 종료를 액션 취소로 자동 변환하지
않는다 — 연결이 끊긴 클라이언트가 보낸 goal 은 그냥 계속 실행된다. 이미
원하는 동작이라 손댈 필요가 없다.
"""
import os

from ament_index_python.packages import get_package_share_directory
import yaml

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def _glob_str(patterns):
    """parse_glob_string() 이 요구하는 "['a', 'b']" 형식 문자열을 만든다.
    patterns 가 빈 리스트면 "[]" (전부 차단) — "" (전부 허용)이 아니다."""
    return '[' + ', '.join(f"'{p}'" for p in patterns) + ']'


def _launch_setup(context, *args, **kwargs):
    config_path = LaunchConfiguration('config_file').perform(context)
    with open(config_path) as f:
        cfg = (yaml.safe_load(f) or {}).get('web_bridge', {})

    relay_topics = list(cfg.get('relay_topics', []))
    allowed_action = cfg.get('allowed_action', '/session/run')
    allowed_service = cfg.get('allowed_service', '/safety/reset')

    parameters = {
        'port': int(cfg.get('websocket_port', 9090)),
        'topics_glob': _glob_str(relay_topics),
        'topics_pub_glob': _glob_str([]),        # 웹 → 토픽 직접 publish 금지
        'topics_sub_glob': _glob_str(relay_topics),
        'services_glob': _glob_str([allowed_service]),
        'actions_glob': _glob_str([allowed_action]),
        'params_glob': _glob_str([]),            # 파라미터 서버 접근 전부 차단
    }

    return [
        Node(
            package='rosbridge_server',
            executable='rosbridge_websocket',
            name='web_bridge_node',
            output='screen',
            parameters=[parameters],
        ),
    ]


def generate_launch_description():
    default_config = os.path.join(
        get_package_share_directory('nail_bridge'), 'config', 'web_bridge.yaml')

    return LaunchDescription([
        DeclareLaunchArgument(
            'config_file', default_value=default_config,
            description='NIS §9 web_bridge 설정 (config/web_bridge.yaml)'),
        OpaqueFunction(function=_launch_setup),
    ])
