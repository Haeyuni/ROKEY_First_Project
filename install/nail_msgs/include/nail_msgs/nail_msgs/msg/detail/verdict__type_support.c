// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:msg/Verdict.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/msg/detail/verdict__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/msg/detail/verdict__functions.h"
#include "nail_msgs/msg/detail/verdict__struct.h"


// Include directives for member types
// Member `session_id`
// Member `result`
#include "rosidl_runtime_c/string_functions.h"
// Member `waveform`
#include "nail_msgs/msg/force_sample.h"
// Member `waveform`
#include "nail_msgs/msg/detail/force_sample__rosidl_typesupport_introspection_c.h"
// Member `measured_at`
#include "builtin_interfaces/msg/time.h"
// Member `measured_at`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__msg__Verdict__init(message_memory);
}

void nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_fini_function(void * message_memory)
{
  nail_msgs__msg__Verdict__fini(message_memory);
}

size_t nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__size_function__Verdict__waveform(
  const void * untyped_member)
{
  const nail_msgs__msg__ForceSample__Sequence * member =
    (const nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__get_const_function__Verdict__waveform(
  const void * untyped_member, size_t index)
{
  const nail_msgs__msg__ForceSample__Sequence * member =
    (const nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__get_function__Verdict__waveform(
  void * untyped_member, size_t index)
{
  nail_msgs__msg__ForceSample__Sequence * member =
    (nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__fetch_function__Verdict__waveform(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__msg__ForceSample * item =
    ((const nail_msgs__msg__ForceSample *)
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__get_const_function__Verdict__waveform(untyped_member, index));
  nail_msgs__msg__ForceSample * value =
    (nail_msgs__msg__ForceSample *)(untyped_value);
  *value = *item;
}

void nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__assign_function__Verdict__waveform(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__msg__ForceSample * item =
    ((nail_msgs__msg__ForceSample *)
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__get_function__Verdict__waveform(untyped_member, index));
  const nail_msgs__msg__ForceSample * value =
    (const nail_msgs__msg__ForceSample *)(untyped_value);
  *item = *value;
}

bool nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__resize_function__Verdict__waveform(
  void * untyped_member, size_t size)
{
  nail_msgs__msg__ForceSample__Sequence * member =
    (nail_msgs__msg__ForceSample__Sequence *)(untyped_member);
  nail_msgs__msg__ForceSample__Sequence__fini(member);
  return nail_msgs__msg__ForceSample__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_member_array[10] = {
  {
    "session_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, session_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "target_index",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, target_index),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "layer_index",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, layer_index),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "probe_index",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, probe_index),  // bytes offset in struct
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
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "tensile_n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, tensile_n),  // bytes offset in struct
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
    offsetof(nail_msgs__msg__Verdict, stiffness_n_per_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "error_code",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, error_code),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "waveform",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, waveform),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__size_function__Verdict__waveform,  // size() function pointer
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__get_const_function__Verdict__waveform,  // get_const(index) function pointer
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__get_function__Verdict__waveform,  // get(index) function pointer
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__fetch_function__Verdict__waveform,  // fetch(index, &value) function pointer
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__assign_function__Verdict__waveform,  // assign(index, value) function pointer
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__resize_function__Verdict__waveform  // resize(index) function pointer
  },
  {
    "measured_at",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__Verdict, measured_at),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_members = {
  "nail_msgs__msg",  // message namespace
  "Verdict",  // message name
  10,  // number of fields
  sizeof(nail_msgs__msg__Verdict),
  false,  // has_any_key_member_
  nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_member_array,  // message members
  nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_type_support_handle = {
  0,
  &nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__msg__Verdict__get_type_hash,
  &nail_msgs__msg__Verdict__get_type_description,
  &nail_msgs__msg__Verdict__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, Verdict)() {
  nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_member_array[8].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ForceSample)();
  nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_member_array[9].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_type_support_handle.typesupport_identifier) {
    nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__msg__Verdict__rosidl_typesupport_introspection_c__Verdict_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
