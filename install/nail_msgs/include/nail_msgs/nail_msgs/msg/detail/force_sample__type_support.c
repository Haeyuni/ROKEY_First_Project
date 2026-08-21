// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:msg/ForceSample.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/msg/detail/force_sample__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/msg/detail/force_sample__functions.h"
#include "nail_msgs/msg/detail/force_sample__struct.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/time.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__msg__ForceSample__init(message_memory);
}

void nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_fini_function(void * message_memory)
{
  nail_msgs__msg__ForceSample__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_member_array[7] = {
  {
    "stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ForceSample, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "fx",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ForceSample, fx),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "fy",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ForceSample, fy),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "fz",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ForceSample, fz),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "tx",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ForceSample, tx),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "ty",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ForceSample, ty),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "tz",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__ForceSample, tz),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_members = {
  "nail_msgs__msg",  // message namespace
  "ForceSample",  // message name
  7,  // number of fields
  sizeof(nail_msgs__msg__ForceSample),
  false,  // has_any_key_member_
  nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_member_array,  // message members
  nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_type_support_handle = {
  0,
  &nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__msg__ForceSample__get_type_hash,
  &nail_msgs__msg__ForceSample__get_type_description,
  &nail_msgs__msg__ForceSample__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ForceSample)() {
  nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_type_support_handle.typesupport_identifier) {
    nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__msg__ForceSample__rosidl_typesupport_introspection_c__ForceSample_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
