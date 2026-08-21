// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:msg/ToolState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/msg/detail/tool_state__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/msg/detail/tool_state__functions.h"
#include "nail_msgs/msg/detail/tool_state__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `current_tool`
// Member `active_tcp`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__msg__ToolState__init(message_memory);
}

void nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_fini_function(void * message_memory)
{
  nail_msgs__msg__ToolState__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_member_array[6] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ToolState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_tool",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ToolState, current_tool),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "active_tcp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ToolState, active_tcp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "grip_width_mm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ToolState, grip_width_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "expected_width_mm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ToolState, expected_width_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "grip_verified",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ToolState, grip_verified),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_members = {
  "nail_msgs__msg",  // message namespace
  "ToolState",  // message name
  6,  // number of fields
  sizeof(nail_msgs__msg__ToolState),
  false,  // has_any_key_member_
  nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_member_array,  // message members
  nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_type_support_handle = {
  0,
  &nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__msg__ToolState__get_type_hash,
  &nail_msgs__msg__ToolState__get_type_description,
  &nail_msgs__msg__ToolState__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ToolState)() {
  nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_type_support_handle.typesupport_identifier) {
    nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__msg__ToolState__rosidl_typesupport_introspection_c__ToolState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
