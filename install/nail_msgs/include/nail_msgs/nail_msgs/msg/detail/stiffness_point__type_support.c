// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:msg/StiffnessPoint.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/msg/detail/stiffness_point__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/msg/detail/stiffness_point__functions.h"
#include "nail_msgs/msg/detail/stiffness_point__struct.h"


// Include directives for member types
// Member `position`
#include "geometry_msgs/msg/point.h"
// Member `position`
#include "geometry_msgs/msg/detail/point__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__msg__StiffnessPoint__init(message_memory);
}

void nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_fini_function(void * message_memory)
{
  nail_msgs__msg__StiffnessPoint__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_member_array[6] = {
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__StiffnessPoint, position),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "stiffness_n_per_mm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__StiffnessPoint, stiffness_n_per_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "peak_tensile_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__StiffnessPoint, peak_tensile_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "hysteresis_ratio",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__StiffnessPoint, hysteresis_ratio),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "lateral_force_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__StiffnessPoint, lateral_force_n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "valid",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__StiffnessPoint, valid),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_members = {
  "nail_msgs__msg",  // message namespace
  "StiffnessPoint",  // message name
  6,  // number of fields
  sizeof(nail_msgs__msg__StiffnessPoint),
  false,  // has_any_key_member_
  nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_member_array,  // message members
  nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_type_support_handle = {
  0,
  &nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__msg__StiffnessPoint__get_type_hash,
  &nail_msgs__msg__StiffnessPoint__get_type_description,
  &nail_msgs__msg__StiffnessPoint__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, StiffnessPoint)() {
  nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  if (!nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_type_support_handle.typesupport_identifier) {
    nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__msg__StiffnessPoint__rosidl_typesupport_introspection_c__StiffnessPoint_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
