#!/usr/bin/env python3
"""Day1 수동 스모크테스트용 가짜 퍼블리셔.

NIS §10 `mock_robot_driver`(로봇 동역학까지 흉내내는 정식 mock, 담당 주은/
로봇팀 별도 산출물)와는 다르다. 이 스크립트는 그보다 훨씬 좁게, "웹 파이프라인
(rosbridge → FastAPI → WebSocket → React)이 배선대로 동작하는가"만 확인하기
위해 /safety/status, /process/status 두 토픽에 최소한의 더미 데이터를
흘려보낸다. 실제 안전 로직은 전혀 없다. (probe/검증 단계 시각화 제거로
/stiffness/map 발행은 뺐다.)

사전 조건:
  1. `colcon build --packages-select nail_msgs` 로 nail_msgs가 빌드되어 있을 것
     (이 샌드박스에서는 python3/anaconda 충돌로 빌드가 막혀 있었음 — 실제
     개발 환경에서 다시 시도할 것)
  2. rclpy는 시스템 python3로 실행해야 한다 (아나콘다 python3는
     GLIBCXX 버전 문제로 rclpy import가 실패함, 이 환경에서 확인됨):
       source /opt/ros/jazzy/setup.bash
       source install/setup.bash
       /usr/bin/python3 web/backend/scripts/fake_ros_publisher.py

실행하면:
  - SafetyState: safe_to_move=true 고정 (인터록 주입 테스트는 별도 스크립트로 확장)
  - ProcessState: SCAN → SAND → COAT → CURE → INSPECT → FINISH 를 순환
"""

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSDurabilityPolicy, QoSProfile, QoSReliabilityPolicy

from nail_msgs.msg import ProcessState, SafetyState

TRANSIENT_LOCAL_QOS = QoSProfile(
    depth=1,
    reliability=QoSReliabilityPolicy.RELIABLE,
    durability=QoSDurabilityPolicy.TRANSIENT_LOCAL,
)

STAGES = ["PRECHECK", "SCAN", "SAND", "BRUSH", "COAT", "CURE", "INSPECT", "FINISH"]


class FakeRosPublisher(Node):
    def __init__(self) -> None:
        super().__init__("fake_ros_publisher")

        self._safety_pub = self.create_publisher(SafetyState, "/safety/status", TRANSIENT_LOCAL_QOS)
        self._state_pub = self.create_publisher(ProcessState, "/process/status", TRANSIENT_LOCAL_QOS)

        self._session_id = "fake-session-0001"
        self._stage_index = 0

        self.create_timer(1.0, self._publish_safety)
        self.create_timer(2.0, self._publish_state)

    def _publish_safety(self) -> None:
        msg = SafetyState()
        msg.safe_to_move = True
        msg.estop_released = True
        msg.comm_ok = True
        msg.handrest_seated = True
        msg.dust_extraction_on = True
        msg.tool_grip_ok = True
        msg.scan_valid = True
        msg.active_faults = []
        msg.reason = ""
        self._safety_pub.publish(msg)

    def _publish_state(self) -> None:
        msg = ProcessState()
        msg.session_id = self._session_id
        msg.stage = STAGES[self._stage_index]
        msg.layer_index = 0
        msg.layer_total = 2
        msg.rework_count = 0
        msg.stage_percent = 50.0
        msg.session_percent = (self._stage_index / (len(STAGES) - 1)) * 100.0
        msg.current_tool = "none"
        self._state_pub.publish(msg)
        self.get_logger().info(f"stage={msg.stage}")
        self._stage_index = (self._stage_index + 1) % len(STAGES)


def main() -> None:
    rclpy.init()
    node = FakeRosPublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
