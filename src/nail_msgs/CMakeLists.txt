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
  "msg/SafetyState.msg"
  "msg/ToolState.msg"
  "msg/ProcessState.msg"
  "msg/TaskPose.msg"
  "srv/ValidatePrecondition.srv"
  "srv/ResetSafety.srv"
  "srv/GetToolInfo.srv"
  "action/MoveTo.action"
  "action/PickPlace.action"
  "action/ContactPath.action"
  "action/LateralContact.action"
  "action/ChangeTool.action"
  "action/SandSurface.action"
  "action/BrushDust.action"
  "action/CoatGel.action"
  "action/CureUV.action"
  "action/PlaceStone.action"
  "action/RunSession.action"
  DEPENDENCIES std_msgs geometry_msgs builtin_interfaces
)

ament_package()
