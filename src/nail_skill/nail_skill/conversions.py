"""geometry_msgs(m, quaternion) <-> 두산 TaskPose(mm, ZYZ 오일러 deg) 변환.

IDS §2.1: geometry_msgs/Pose 만 ROS 표준(m, rad)을 쓰고, 나머지 인터페이스는
전부 단위 접미사가 붙은 mm/deg 다. 이 변환은 robot_skill_node 안에서만
일어난다 (SDS §4.1 — 두산 API 호출은 이 노드 밖으로 새어나가지 않는다).
"""
import math
from dataclasses import dataclass


@dataclass
class TaskPose:
    x_mm: float
    y_mm: float
    z_mm: float
    rz1_deg: float
    ry_deg: float
    rz2_deg: float

    def as_list(self):
        return [self.x_mm, self.y_mm, self.z_mm, self.rz1_deg, self.ry_deg, self.rz2_deg]


def pose_to_task_pose(pose_m) -> TaskPose:
    q = pose_m.orientation
    qx, qy, qz, qw = q.x, q.y, q.z, q.w
    n = math.sqrt(qx * qx + qy * qy + qz * qz + qw * qw)
    if n < 1e-9:
        qx, qy, qz, qw = 0.0, 0.0, 0.0, 1.0
    else:
        qx, qy, qz, qw = qx / n, qy / n, qz / n, qw / n

    r00 = 1 - 2 * (qy * qy + qz * qz)
    r01 = 2 * (qx * qy - qz * qw)
    r02 = 2 * (qx * qz + qy * qw)
    r12 = 2 * (qy * qz - qx * qw)
    r20 = 2 * (qx * qz - qy * qw)
    r21 = 2 * (qy * qz + qx * qw)
    r22 = 1 - 2 * (qx * qx + qy * qy)

    sy = math.sqrt(r02 * r02 + r12 * r12)
    if sy > 1e-9:
        ry = math.atan2(sy, r22)
        rz1 = math.atan2(r12, r02)
        rz2 = math.atan2(r21, -r20)
    else:
        ry = math.atan2(sy, r22)
        rz1 = math.atan2(-r01, r00)
        rz2 = 0.0

    return TaskPose(
        x_mm=pose_m.position.x * 1000.0,
        y_mm=pose_m.position.y * 1000.0,
        z_mm=pose_m.position.z * 1000.0,
        rz1_deg=math.degrees(rz1),
        ry_deg=math.degrees(ry),
        rz2_deg=math.degrees(rz2),
    )


def task_pose_to_ros_pose(task_pose: TaskPose):
    from geometry_msgs.msg import Pose

    rz1 = math.radians(task_pose.rz1_deg)
    ry = math.radians(task_pose.ry_deg)
    rz2 = math.radians(task_pose.rz2_deg)

    def rot_z(a):
        c, s = math.cos(a), math.sin(a)
        return ((c, -s, 0.0), (s, c, 0.0), (0.0, 0.0, 1.0))

    def rot_y(a):
        c, s = math.cos(a), math.sin(a)
        return ((c, 0.0, s), (0.0, 1.0, 0.0), (-s, 0.0, c))

    def matmul(a, b):
        return tuple(
            tuple(sum(a[i][k] * b[k][j] for k in range(3)) for j in range(3))
            for i in range(3)
        )

    r = matmul(matmul(rot_z(rz1), rot_y(ry)), rot_z(rz2))

    trace = r[0][0] + r[1][1] + r[2][2]
    if trace > 0.0:
        s = math.sqrt(trace + 1.0) * 2.0
        qw = 0.25 * s
        qx = (r[2][1] - r[1][2]) / s
        qy = (r[0][2] - r[2][0]) / s
        qz = (r[1][0] - r[0][1]) / s
    elif r[0][0] > r[1][1] and r[0][0] > r[2][2]:
        s = math.sqrt(1.0 + r[0][0] - r[1][1] - r[2][2]) * 2.0
        qw = (r[2][1] - r[1][2]) / s
        qx = 0.25 * s
        qy = (r[0][1] + r[1][0]) / s
        qz = (r[0][2] + r[2][0]) / s
    elif r[1][1] > r[2][2]:
        s = math.sqrt(1.0 + r[1][1] - r[0][0] - r[2][2]) * 2.0
        qw = (r[0][2] - r[2][0]) / s
        qx = (r[0][1] + r[1][0]) / s
        qy = 0.25 * s
        qz = (r[1][2] + r[2][1]) / s
    else:
        s = math.sqrt(1.0 + r[2][2] - r[0][0] - r[1][1]) * 2.0
        qw = (r[1][0] - r[0][1]) / s
        qx = (r[0][2] + r[2][0]) / s
        qy = (r[1][2] + r[2][1]) / s
        qz = 0.25 * s

    pose = Pose()
    pose.position.x = task_pose.x_mm / 1000.0
    pose.position.y = task_pose.y_mm / 1000.0
    pose.position.z = task_pose.z_mm / 1000.0
    pose.orientation.x = qx
    pose.orientation.y = qy
    pose.orientation.z = qz
    pose.orientation.w = qw
    return pose


def point_to_mm(point_m):
    return point_m.x * 1000.0, point_m.y * 1000.0, point_m.z * 1000.0


def vector_unit(v):
    n = math.sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
    if n < 1e-9:
        return (0.0, 0.0, 0.0)
    return (v.x / n, v.y / n, v.z / n)


def tool_z_axis_base(task_pose: TaskPose):
    """툴 +Z 축을 base 프레임 단위벡터로. ZYZ 오일러 R = Rz(rz1)Ry(ry)Rz(rz2)
    의 3열이라 rz2(툴 자전)와 무관하다.

    프로브 하강량을 **명령값이 아니라 실측 자세**로 재기 위해 필요하다.
    명령 하강량은 접촉 후 로봇이 밀리는 만큼을 포함하지 않으므로 압입량으로
    쓰면 안 되고, 단순 유클리드 거리는 이탈 구간에서 부호를 잃는다.
    travel = dot(p0 - p, tool_z) 로 재면 하강이 +, 이탈이 - 로 일관된다.
    """
    rz1 = math.radians(task_pose.rz1_deg)
    ry = math.radians(task_pose.ry_deg)
    return (math.cos(rz1) * math.sin(ry), math.sin(rz1) * math.sin(ry), math.cos(ry))
