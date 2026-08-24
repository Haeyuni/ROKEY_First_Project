# 티치펜던트 TASK 화면의 X/Y/Z/A/B/C 그대로 — quaternion 변환 없음.
# ry가 ±180° 근처(이 워크스페이스의 정상 자세)일 때 quaternion 왕복 변환이
# 다른 자세로 튀는 문제(robot_skill_node.py 참고)를 피하기 위해, 사람이
# 직접 좌표를 넣는 액션 필드(SandSurface/CureUV 의 waypoints 등)는
# geometry_msgs/Pose 대신 이 타입을 쓴다.

float64 x_mm
float64 y_mm
float64 z_mm
float64 rz1_deg    # A
float64 ry_deg     # B
float64 rz2_deg    # C
