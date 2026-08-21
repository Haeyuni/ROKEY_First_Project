// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:action/FollowContour.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/action/detail/follow_contour__functions.h"
#include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `mode`
// Member `frame_id`
// Member `session_id`
// Member `required_tool`
#include "rosidl_runtime_c/string_functions.h"
// Member `trajectory`
#include "geometry_msgs/msg/pose.h"
// Member `trajectory`
#include "geometry_msgs/msg/detail/pose__rosidl_typesupport_introspection_c.h"
// Member `allowed_polygon`
#include "geometry_msgs/msg/point.h"
// Member `allowed_polygon`
#include "geometry_msgs/msg/detail/point__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_Goal__init(message_memory);
}

void nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_Goal__fini(message_memory);
}

size_t nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__size_function__FollowContour_Goal__trajectory(
  const void * untyped_member)
{
  const geometry_msgs__msg__Pose__Sequence * member =
    (const geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Goal__trajectory(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Pose__Sequence * member =
    (const geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_function__FollowContour_Goal__trajectory(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Pose__Sequence * member =
    (geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Goal__trajectory(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Pose * item =
    ((const geometry_msgs__msg__Pose *)
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Goal__trajectory(untyped_member, index));
  geometry_msgs__msg__Pose * value =
    (geometry_msgs__msg__Pose *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__assign_function__FollowContour_Goal__trajectory(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Pose * item =
    ((geometry_msgs__msg__Pose *)
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_function__FollowContour_Goal__trajectory(untyped_member, index));
  const geometry_msgs__msg__Pose * value =
    (const geometry_msgs__msg__Pose *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__resize_function__FollowContour_Goal__trajectory(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Pose__Sequence * member =
    (geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  geometry_msgs__msg__Pose__Sequence__fini(member);
  return geometry_msgs__msg__Pose__Sequence__init(member, size);
}

size_t nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__size_function__FollowContour_Goal__allowed_polygon(
  const void * untyped_member)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Goal__allowed_polygon(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_function__FollowContour_Goal__allowed_polygon(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Goal__allowed_polygon(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Point * item =
    ((const geometry_msgs__msg__Point *)
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Goal__allowed_polygon(untyped_member, index));
  geometry_msgs__msg__Point * value =
    (geometry_msgs__msg__Point *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__assign_function__FollowContour_Goal__allowed_polygon(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Point * item =
    ((geometry_msgs__msg__Point *)
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_function__FollowContour_Goal__allowed_polygon(untyped_member, index));
  const geometry_msgs__msg__Point * value =
    (const geometry_msgs__msg__Point *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__resize_function__FollowContour_Goal__allowed_polygon(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  geometry_msgs__msg__Point__Sequence__fini(member);
  return geometry_msgs__msg__Point__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_member_array[14] = {
  {
    "mode",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, mode),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "trajectory",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, trajectory),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__size_function__FollowContour_Goal__trajectory,  // size() function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Goal__trajectory,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_function__FollowContour_Goal__trajectory,  // get(index) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Goal__trajectory,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__assign_function__FollowContour_Goal__trajectory,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__resize_function__FollowContour_Goal__trajectory  // resize(index) function pointer
  },
  {
    "frame_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, frame_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "session_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, session_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "required_tool",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, required_tool),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "target_force_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, target_force_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "force_upper_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, force_upper_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "force_lower_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, force_lower_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "lateral_force_limit_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, lateral_force_limit_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "allowed_polygon",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, allowed_polygon),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__size_function__FollowContour_Goal__allowed_polygon,  // size() function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Goal__allowed_polygon,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__get_function__FollowContour_Goal__allowed_polygon,  // get(index) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Goal__allowed_polygon,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__assign_function__FollowContour_Goal__allowed_polygon,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__resize_function__FollowContour_Goal__allowed_polygon  // resize(index) function pointer
  },
  {
    "boundary_offset_mm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, boundary_offset_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_passes",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, max_passes),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_duration_s",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, max_duration_s),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "feed_speed_mms",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Goal, feed_speed_mms),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_Goal",  // message name
  14,  // number of fields
  sizeof(nail_msgs__action__FollowContour_Goal),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_member_array,  // message members
  nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_Goal__get_type_hash,
  &nail_msgs__action__FollowContour_Goal__get_type_description,
  &nail_msgs__action__FollowContour_Goal__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_Goal)() {
  nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_member_array[9].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  if (!nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_Goal__rosidl_typesupport_introspection_c__FollowContour_Goal_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `base`
#include "nail_msgs/msg/action_result_base.h"
// Member `base`
#include "nail_msgs/msg/detail/action_result_base__rosidl_typesupport_introspection_c.h"
// Member `missed_segment_indices`
#include "rosidl_runtime_c/primitives_sequence_functions.h"
// Member `force_log`
#include "nail_msgs/msg/force_sample.h"
// Member `force_log`
#include "nail_msgs/msg/detail/force_sample__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_Result__init(message_memory);
}

void nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_Result__fini(message_memory);
}

size_t nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__size_function__FollowContour_Result__missed_segment_indices(
  const void * untyped_member)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Result__missed_segment_indices(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_function__FollowContour_Result__missed_segment_indices(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Result__missed_segment_indices(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int32_t * item =
    ((const int32_t *)
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Result__missed_segment_indices(untyped_member, index));
  int32_t * value =
    (int32_t *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__assign_function__FollowContour_Result__missed_segment_indices(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int32_t * item =
    ((int32_t *)
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_function__FollowContour_Result__missed_segment_indices(untyped_member, index));
  const int32_t * value =
    (const int32_t *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__resize_function__FollowContour_Result__missed_segment_indices(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  rosidl_runtime_c__int32__Sequence__fini(member);
  return rosidl_runtime_c__int32__Sequence__init(member, size);
}

size_t nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__size_function__FollowContour_Result__force_log(
  const void * untyped_member)
{
  const nail_msgs__msg__ForceSample__Sequence * member =
    (const nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Result__force_log(
  const void * untyped_member, size_t index)
{
  const nail_msgs__msg__ForceSample__Sequence * member =
    (const nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_function__FollowContour_Result__force_log(
  void * untyped_member, size_t index)
{
  nail_msgs__msg__ForceSample__Sequence * member =
    (nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Result__force_log(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__msg__ForceSample * item =
    ((const nail_msgs__msg__ForceSample *)
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Result__force_log(untyped_member, index));
  nail_msgs__msg__ForceSample * value =
    (nail_msgs__msg__ForceSample *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__assign_function__FollowContour_Result__force_log(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__msg__ForceSample * item =
    ((nail_msgs__msg__ForceSample *)
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_function__FollowContour_Result__force_log(untyped_member, index));
  const nail_msgs__msg__ForceSample * value =
    (const nail_msgs__msg__ForceSample *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__resize_function__FollowContour_Result__force_log(
  void * untyped_member, size_t size)
{
  nail_msgs__msg__ForceSample__Sequence * member =
    (nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  nail_msgs__msg__ForceSample__Sequence__fini(member);
  return nail_msgs__msg__ForceSample__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_member_array[6] = {
  {
    "base",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Result, base),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "passes_done",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Result, passes_done),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "duration_s",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Result, duration_s),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "missed_segment_indices",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Result, missed_segment_indices),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__size_function__FollowContour_Result__missed_segment_indices,  // size() function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Result__missed_segment_indices,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_function__FollowContour_Result__missed_segment_indices,  // get(index) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Result__missed_segment_indices,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__assign_function__FollowContour_Result__missed_segment_indices,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__resize_function__FollowContour_Result__missed_segment_indices  // resize(index) function pointer
  },
  {
    "max_lateral_force_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Result, max_lateral_force_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "force_log",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Result, force_log),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__size_function__FollowContour_Result__force_log,  // size() function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_const_function__FollowContour_Result__force_log,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__get_function__FollowContour_Result__force_log,  // get(index) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__fetch_function__FollowContour_Result__force_log,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__assign_function__FollowContour_Result__force_log,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__resize_function__FollowContour_Result__force_log  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_Result",  // message name
  6,  // number of fields
  sizeof(nail_msgs__action__FollowContour_Result),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_member_array,  // message members
  nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_Result__get_type_hash,
  &nail_msgs__action__FollowContour_Result__get_type_description,
  &nail_msgs__action__FollowContour_Result__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_Result)() {
  nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ActionResultBase)();
  nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_member_array[5].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ForceSample)();
  if (!nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_Result__rosidl_typesupport_introspection_c__FollowContour_Result_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_Feedback__init(message_memory);
}

void nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_Feedback__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_member_array[4] = {
  {
    "percent",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Feedback, percent),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_pass",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Feedback, current_pass),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_fz",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Feedback, current_fz),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_lateral_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_Feedback, current_lateral_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_Feedback",  // message name
  4,  // number of fields
  sizeof(nail_msgs__action__FollowContour_Feedback),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_member_array,  // message members
  nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_Feedback__get_type_hash,
  &nail_msgs__action__FollowContour_Feedback__get_type_description,
  &nail_msgs__action__FollowContour_Feedback__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_Feedback)() {
  if (!nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_Feedback__rosidl_typesupport_introspection_c__FollowContour_Feedback_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `goal`
#include "nail_msgs/action/follow_contour.h"
// Member `goal`
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_SendGoal_Request__init(message_memory);
}

void nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_SendGoal_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_SendGoal_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "goal",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_SendGoal_Request, goal),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_SendGoal_Request",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__FollowContour_SendGoal_Request),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_member_array,  // message members
  nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_SendGoal_Request__get_type_hash,
  &nail_msgs__action__FollowContour_SendGoal_Request__get_type_description,
  &nail_msgs__action__FollowContour_SendGoal_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Request)() {
  nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_Goal)();
  if (!nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/time.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_SendGoal_Response__init(message_memory);
}

void nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_SendGoal_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_member_array[2] = {
  {
    "accepted",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_SendGoal_Response, accepted),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_SendGoal_Response, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_SendGoal_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__FollowContour_SendGoal_Response),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_member_array,  // message members
  nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_SendGoal_Response__get_type_hash,
  &nail_msgs__action__FollowContour_SendGoal_Response__get_type_description,
  &nail_msgs__action__FollowContour_SendGoal_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Response)() {
  nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/follow_contour.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_SendGoal_Event__init(message_memory);
}

void nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_SendGoal_Event__fini(message_memory);
}

size_t nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_SendGoal_Event__request(
  const void * untyped_member)
{
  const nail_msgs__action__FollowContour_SendGoal_Request__Sequence * member =
    (const nail_msgs__action__FollowContour_SendGoal_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_SendGoal_Event__request(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__FollowContour_SendGoal_Request__Sequence * member =
    (const nail_msgs__action__FollowContour_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_SendGoal_Event__request(
  void * untyped_member, size_t index)
{
  nail_msgs__action__FollowContour_SendGoal_Request__Sequence * member =
    (nail_msgs__action__FollowContour_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_SendGoal_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__FollowContour_SendGoal_Request * item =
    ((const nail_msgs__action__FollowContour_SendGoal_Request *)
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_SendGoal_Event__request(untyped_member, index));
  nail_msgs__action__FollowContour_SendGoal_Request * value =
    (nail_msgs__action__FollowContour_SendGoal_Request *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_SendGoal_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__FollowContour_SendGoal_Request * item =
    ((nail_msgs__action__FollowContour_SendGoal_Request *)
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_SendGoal_Event__request(untyped_member, index));
  const nail_msgs__action__FollowContour_SendGoal_Request * value =
    (const nail_msgs__action__FollowContour_SendGoal_Request *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_SendGoal_Event__request(
  void * untyped_member, size_t size)
{
  nail_msgs__action__FollowContour_SendGoal_Request__Sequence * member =
    (nail_msgs__action__FollowContour_SendGoal_Request__Sequence *)(untyped_member);
  nail_msgs__action__FollowContour_SendGoal_Request__Sequence__fini(member);
  return nail_msgs__action__FollowContour_SendGoal_Request__Sequence__init(member, size);
}

size_t nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_SendGoal_Event__response(
  const void * untyped_member)
{
  const nail_msgs__action__FollowContour_SendGoal_Response__Sequence * member =
    (const nail_msgs__action__FollowContour_SendGoal_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_SendGoal_Event__response(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__FollowContour_SendGoal_Response__Sequence * member =
    (const nail_msgs__action__FollowContour_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_SendGoal_Event__response(
  void * untyped_member, size_t index)
{
  nail_msgs__action__FollowContour_SendGoal_Response__Sequence * member =
    (nail_msgs__action__FollowContour_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_SendGoal_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__FollowContour_SendGoal_Response * item =
    ((const nail_msgs__action__FollowContour_SendGoal_Response *)
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_SendGoal_Event__response(untyped_member, index));
  nail_msgs__action__FollowContour_SendGoal_Response * value =
    (nail_msgs__action__FollowContour_SendGoal_Response *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_SendGoal_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__FollowContour_SendGoal_Response * item =
    ((nail_msgs__action__FollowContour_SendGoal_Response *)
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_SendGoal_Event__response(untyped_member, index));
  const nail_msgs__action__FollowContour_SendGoal_Response * value =
    (const nail_msgs__action__FollowContour_SendGoal_Response *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_SendGoal_Event__response(
  void * untyped_member, size_t size)
{
  nail_msgs__action__FollowContour_SendGoal_Response__Sequence * member =
    (nail_msgs__action__FollowContour_SendGoal_Response__Sequence *)(untyped_member);
  nail_msgs__action__FollowContour_SendGoal_Response__Sequence__fini(member);
  return nail_msgs__action__FollowContour_SendGoal_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_SendGoal_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_SendGoal_Event, request),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_SendGoal_Event__request,  // size() function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_SendGoal_Event__request,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_SendGoal_Event__request,  // get(index) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_SendGoal_Event__request,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_SendGoal_Event__request,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_SendGoal_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_SendGoal_Event, response),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_SendGoal_Event__response,  // size() function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_SendGoal_Event__response,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_SendGoal_Event__response,  // get(index) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_SendGoal_Event__response,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_SendGoal_Event__response,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_SendGoal_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_SendGoal_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs__action__FollowContour_SendGoal_Event),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_member_array,  // message members
  nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_SendGoal_Event__get_type_hash,
  &nail_msgs__action__FollowContour_SendGoal_Event__get_type_description,
  &nail_msgs__action__FollowContour_SendGoal_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Event)() {
  nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Request)();
  nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Response)();
  if (!nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_service_members = {
  "nail_msgs__action",  // service namespace
  "FollowContour_SendGoal",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_type_support_handle,
  NULL,  // response message
  // nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_type_support_handle
  NULL  // event_message
  // nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_type_support_handle
};


static rosidl_service_type_support_t nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_service_type_support_handle = {
  0,
  &nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_service_members,
  get_service_typesupport_handle_function,
  &nail_msgs__action__FollowContour_SendGoal_Request__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Request_message_type_support_handle,
  &nail_msgs__action__FollowContour_SendGoal_Response__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Response_message_type_support_handle,
  &nail_msgs__action__FollowContour_SendGoal_Event__rosidl_typesupport_introspection_c__FollowContour_SendGoal_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    FollowContour_SendGoal
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    FollowContour_SendGoal
  ),
  &nail_msgs__action__FollowContour_SendGoal__get_type_hash,
  &nail_msgs__action__FollowContour_SendGoal__get_type_description,
  &nail_msgs__action__FollowContour_SendGoal__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal)(void) {
  if (!nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_service_type_support_handle.typesupport_identifier) {
    nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_SendGoal_Event)()->data;
  }

  return &nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_SendGoal_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_GetResult_Request__init(message_memory);
}

void nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_GetResult_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_member_array[1] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_GetResult_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_GetResult_Request",  // message name
  1,  // number of fields
  sizeof(nail_msgs__action__FollowContour_GetResult_Request),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_member_array,  // message members
  nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_GetResult_Request__get_type_hash,
  &nail_msgs__action__FollowContour_GetResult_Request__get_type_description,
  &nail_msgs__action__FollowContour_GetResult_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Request)() {
  nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  if (!nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `result`
// already included above
// #include "nail_msgs/action/follow_contour.h"
// Member `result`
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_GetResult_Response__init(message_memory);
}

void nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_GetResult_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_member_array[2] = {
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_GetResult_Response, status),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_GetResult_Response, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_GetResult_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__FollowContour_GetResult_Response),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_member_array,  // message members
  nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_GetResult_Response__get_type_hash,
  &nail_msgs__action__FollowContour_GetResult_Response__get_type_description,
  &nail_msgs__action__FollowContour_GetResult_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Response)() {
  nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_Result)();
  if (!nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `info`
// already included above
// #include "service_msgs/msg/service_event_info.h"
// Member `info`
// already included above
// #include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/follow_contour.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_GetResult_Event__init(message_memory);
}

void nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_GetResult_Event__fini(message_memory);
}

size_t nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_GetResult_Event__request(
  const void * untyped_member)
{
  const nail_msgs__action__FollowContour_GetResult_Request__Sequence * member =
    (const nail_msgs__action__FollowContour_GetResult_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_GetResult_Event__request(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__FollowContour_GetResult_Request__Sequence * member =
    (const nail_msgs__action__FollowContour_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_GetResult_Event__request(
  void * untyped_member, size_t index)
{
  nail_msgs__action__FollowContour_GetResult_Request__Sequence * member =
    (nail_msgs__action__FollowContour_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_GetResult_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__FollowContour_GetResult_Request * item =
    ((const nail_msgs__action__FollowContour_GetResult_Request *)
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_GetResult_Event__request(untyped_member, index));
  nail_msgs__action__FollowContour_GetResult_Request * value =
    (nail_msgs__action__FollowContour_GetResult_Request *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_GetResult_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__FollowContour_GetResult_Request * item =
    ((nail_msgs__action__FollowContour_GetResult_Request *)
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_GetResult_Event__request(untyped_member, index));
  const nail_msgs__action__FollowContour_GetResult_Request * value =
    (const nail_msgs__action__FollowContour_GetResult_Request *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_GetResult_Event__request(
  void * untyped_member, size_t size)
{
  nail_msgs__action__FollowContour_GetResult_Request__Sequence * member =
    (nail_msgs__action__FollowContour_GetResult_Request__Sequence *)(untyped_member);
  nail_msgs__action__FollowContour_GetResult_Request__Sequence__fini(member);
  return nail_msgs__action__FollowContour_GetResult_Request__Sequence__init(member, size);
}

size_t nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_GetResult_Event__response(
  const void * untyped_member)
{
  const nail_msgs__action__FollowContour_GetResult_Response__Sequence * member =
    (const nail_msgs__action__FollowContour_GetResult_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_GetResult_Event__response(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__FollowContour_GetResult_Response__Sequence * member =
    (const nail_msgs__action__FollowContour_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_GetResult_Event__response(
  void * untyped_member, size_t index)
{
  nail_msgs__action__FollowContour_GetResult_Response__Sequence * member =
    (nail_msgs__action__FollowContour_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_GetResult_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__FollowContour_GetResult_Response * item =
    ((const nail_msgs__action__FollowContour_GetResult_Response *)
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_GetResult_Event__response(untyped_member, index));
  nail_msgs__action__FollowContour_GetResult_Response * value =
    (nail_msgs__action__FollowContour_GetResult_Response *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_GetResult_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__FollowContour_GetResult_Response * item =
    ((nail_msgs__action__FollowContour_GetResult_Response *)
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_GetResult_Event__response(untyped_member, index));
  const nail_msgs__action__FollowContour_GetResult_Response * value =
    (const nail_msgs__action__FollowContour_GetResult_Response *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_GetResult_Event__response(
  void * untyped_member, size_t size)
{
  nail_msgs__action__FollowContour_GetResult_Response__Sequence * member =
    (nail_msgs__action__FollowContour_GetResult_Response__Sequence *)(untyped_member);
  nail_msgs__action__FollowContour_GetResult_Response__Sequence__fini(member);
  return nail_msgs__action__FollowContour_GetResult_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_GetResult_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_GetResult_Event, request),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_GetResult_Event__request,  // size() function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_GetResult_Event__request,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_GetResult_Event__request,  // get(index) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_GetResult_Event__request,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_GetResult_Event__request,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_GetResult_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_GetResult_Event, response),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__size_function__FollowContour_GetResult_Event__response,  // size() function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__FollowContour_GetResult_Event__response,  // get_const(index) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__get_function__FollowContour_GetResult_Event__response,  // get(index) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__FollowContour_GetResult_Event__response,  // fetch(index, &value) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__FollowContour_GetResult_Event__response,  // assign(index, value) function pointer
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__FollowContour_GetResult_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_GetResult_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs__action__FollowContour_GetResult_Event),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_member_array,  // message members
  nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_GetResult_Event__get_type_hash,
  &nail_msgs__action__FollowContour_GetResult_Event__get_type_description,
  &nail_msgs__action__FollowContour_GetResult_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Event)() {
  nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Request)();
  nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Response)();
  if (!nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_service_members = {
  "nail_msgs__action",  // service namespace
  "FollowContour_GetResult",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_type_support_handle,
  NULL,  // response message
  // nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_type_support_handle
  NULL  // event_message
  // nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_type_support_handle
};


static rosidl_service_type_support_t nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_service_type_support_handle = {
  0,
  &nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_service_members,
  get_service_typesupport_handle_function,
  &nail_msgs__action__FollowContour_GetResult_Request__rosidl_typesupport_introspection_c__FollowContour_GetResult_Request_message_type_support_handle,
  &nail_msgs__action__FollowContour_GetResult_Response__rosidl_typesupport_introspection_c__FollowContour_GetResult_Response_message_type_support_handle,
  &nail_msgs__action__FollowContour_GetResult_Event__rosidl_typesupport_introspection_c__FollowContour_GetResult_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    FollowContour_GetResult
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    FollowContour_GetResult
  ),
  &nail_msgs__action__FollowContour_GetResult__get_type_hash,
  &nail_msgs__action__FollowContour_GetResult__get_type_description,
  &nail_msgs__action__FollowContour_GetResult__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult)(void) {
  if (!nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_service_type_support_handle.typesupport_identifier) {
    nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_GetResult_Event)()->data;
  }

  return &nail_msgs__action__detail__follow_contour__rosidl_typesupport_introspection_c__FollowContour_GetResult_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__functions.h"
// already included above
// #include "nail_msgs/action/detail/follow_contour__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `feedback`
// already included above
// #include "nail_msgs/action/follow_contour.h"
// Member `feedback`
// already included above
// #include "nail_msgs/action/detail/follow_contour__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__FollowContour_FeedbackMessage__init(message_memory);
}

void nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_fini_function(void * message_memory)
{
  nail_msgs__action__FollowContour_FeedbackMessage__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_FeedbackMessage, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "feedback",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__FollowContour_FeedbackMessage, feedback),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_members = {
  "nail_msgs__action",  // message namespace
  "FollowContour_FeedbackMessage",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__FollowContour_FeedbackMessage),
  false,  // has_any_key_member_
  nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_member_array,  // message members
  nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_type_support_handle = {
  0,
  &nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__FollowContour_FeedbackMessage__get_type_hash,
  &nail_msgs__action__FollowContour_FeedbackMessage__get_type_description,
  &nail_msgs__action__FollowContour_FeedbackMessage__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_FeedbackMessage)() {
  nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, FollowContour_Feedback)();
  if (!nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__FollowContour_FeedbackMessage__rosidl_typesupport_introspection_c__FollowContour_FeedbackMessage_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
