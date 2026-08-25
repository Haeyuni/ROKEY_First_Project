"""테스트 컴퓨터의 외부 두산/RG2 드라이버에 연결하는 전체 공정 launch.

드라이버는 이 launch에 넣지 않는다. 테스트 컴퓨터에서 실제 로봇 또는 에뮬레이터에
맞는 ws_dsr 드라이버를 먼저 실행하고, 이 launch가 같은 dsr_prefix의 서비스를
사용한다. 공정 전체는 /session/run Action 하나로 시작한다.
"""
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    test_bringup = os.path.join(
        get_package_share_directory('nail_bringup'), 'launch', 'test_bringup.launch.py')

    return LaunchDescription([
        DeclareLaunchArgument(
            'dsr_prefix', default_value='dsr01',
            description='외부 두산/RG2 드라이버와 같은 ROS 네임스페이스'),
        DeclareLaunchArgument(
            'home_target_key', default_value='rack_transit',
            description='실패·취소 시 복귀할 targets.yaml의 티칭 target_key'),
        DeclareLaunchArgument(
            'sanding_path_y_offset_mm', default_value='0.0',
            description='샌딩 기본 3-Pose 전체에 적용할 base_link Y 보정(mm)'),
        DeclareLaunchArgument(
            'log_level', default_value='info',
            description='ROS 로그 레벨'),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(test_bringup),
            launch_arguments={
                'nodes': 'all',
                'dsr_prefix': LaunchConfiguration('dsr_prefix'),
                'home_target_key': LaunchConfiguration('home_target_key'),
                'sanding_path_y_offset_mm': LaunchConfiguration('sanding_path_y_offset_mm'),
                'log_level': LaunchConfiguration('log_level'),
            }.items()),
    ])
