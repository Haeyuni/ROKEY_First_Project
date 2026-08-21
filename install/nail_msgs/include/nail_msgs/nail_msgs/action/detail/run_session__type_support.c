// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:action/RunSession.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/action/detail/run_session__functions.h"
#include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `session_id`
// Member `recipe_id`
// Member `shape_profile_id`
// Member `target_material`
#include "rosidl_runtime_c/string_functions.h"
// Member `target_indices`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_Goal__init(message_memory);
}

void nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_Goal__fini(message_memory);
}

size_t nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__size_function__RunSession_Goal__target_indices(
  const void * untyped_member)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__get_const_function__RunSession_Goal__target_indices(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__get_function__RunSession_Goal__target_indices(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__fetch_function__RunSession_Goal__target_indices(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int32_t * item =
    ((const int32_t *)
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__get_const_function__RunSession_Goal__target_indices(untyped_member, index));
  int32_t * value =
    (int32_t *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__assign_function__RunSession_Goal__target_indices(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int32_t * item =
    ((int32_t *)
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__get_function__RunSession_Goal__target_indices(untyped_member, index));
  const int32_t * value =
    (const int32_t *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__resize_function__RunSession_Goal__target_indices(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  rosidl_runtime_c__int32__Sequence__fini(member);
  return rosidl_runtime_c__int32__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_member_array[6] = {
  {
    "session_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Goal, session_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "recipe_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Goal, recipe_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "shape_profile_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Goal, shape_profile_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "length_mm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Goal, length_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "target_indices",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Goal, target_indices),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__size_function__RunSession_Goal__target_indices,  // size() function pointer
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__get_const_function__RunSession_Goal__target_indices,  // get_const(index) function pointer
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__get_function__RunSession_Goal__target_indices,  // get(index) function pointer
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__fetch_function__RunSession_Goal__target_indices,  // fetch(index, &value) function pointer
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__assign_function__RunSession_Goal__target_indices,  // assign(index, value) function pointer
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__resize_function__RunSession_Goal__target_indices  // resize(index) function pointer
  },
  {
    "target_material",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Goal, target_material),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_Goal",  // message name
  6,  // number of fields
  sizeof(nail_msgs__action__RunSession_Goal),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_member_array,  // message members
  nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_Goal__get_type_hash,
  &nail_msgs__action__RunSession_Goal__get_type_description,
  &nail_msgs__action__RunSession_Goal__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_Goal)() {
  if (!nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_Goal__rosidl_typesupport_introspection_c__RunSession_Goal_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `result_code`
// already included above
// #include "rosidl_runtime_c/string_functions.h"
// Member `all_verdicts`
#include "nail_msgs/msg/verdict.h"
// Member `all_verdicts`
#include "nail_msgs/msg/detail/verdict__rosidl_typesupport_introspection_c.h"
// Member `final_error`
#include "nail_msgs/msg/error_code.h"
// Member `final_error`
#include "nail_msgs/msg/detail/error_code__rosidl_typesupport_introspection_c.h"
// Member `started_at`
// Member `finished_at`
#include "builtin_interfaces/msg/time.h"
// Member `started_at`
// Member `finished_at`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_Result__init(message_memory);
}

void nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_Result__fini(message_memory);
}

size_t nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__size_function__RunSession_Result__all_verdicts(
  const void * untyped_member)
{
  const nail_msgs__msg__Verdict__Sequence * member =
    (const nail_msgs__msg__Verdict__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__get_const_function__RunSession_Result__all_verdicts(
  const void * untyped_member, size_t index)
{
  const nail_msgs__msg__Verdict__Sequence * member =
    (const nail_msgs__msg__Verdict__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__get_function__RunSession_Result__all_verdicts(
  void * untyped_member, size_t index)
{
  nail_msgs__msg__Verdict__Sequence * member =
    (nail_msgs__msg__Verdict__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__fetch_function__RunSession_Result__all_verdicts(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__msg__Verdict * item =
    ((const nail_msgs__msg__Verdict *)
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__get_const_function__RunSession_Result__all_verdicts(untyped_member, index));
  nail_msgs__msg__Verdict * value =
    (nail_msgs__msg__Verdict *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__assign_function__RunSession_Result__all_verdicts(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__msg__Verdict * item =
    ((nail_msgs__msg__Verdict *)
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__get_function__RunSession_Result__all_verdicts(untyped_member, index));
  const nail_msgs__msg__Verdict * value =
    (const nail_msgs__msg__Verdict *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__resize_function__RunSession_Result__all_verdicts(
  void * untyped_member, size_t size)
{
  nail_msgs__msg__Verdict__Sequence * member =
    (nail_msgs__msg__Verdict__Sequence *)(untyped_member);
  nail_msgs__msg__Verdict__Sequence__fini(member);
  return nail_msgs__msg__Verdict__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_member_array[8] = {
  {
    "success",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, success),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "result_code",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, result_code),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "all_verdicts",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, all_verdicts),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__size_function__RunSession_Result__all_verdicts,  // size() function pointer
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__get_const_function__RunSession_Result__all_verdicts,  // get_const(index) function pointer
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__get_function__RunSession_Result__all_verdicts,  // get(index) function pointer
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__fetch_function__RunSession_Result__all_verdicts,  // fetch(index, &value) function pointer
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__assign_function__RunSession_Result__all_verdicts,  // assign(index, value) function pointer
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__resize_function__RunSession_Result__all_verdicts  // resize(index) function pointer
  },
  {
    "final_error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, final_error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "warn_count",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, warn_count),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "total_retries",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, total_retries),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "started_at",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, started_at),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "finished_at",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Result, finished_at),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_Result",  // message name
  8,  // number of fields
  sizeof(nail_msgs__action__RunSession_Result),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_member_array,  // message members
  nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_Result__get_type_hash,
  &nail_msgs__action__RunSession_Result__get_type_description,
  &nail_msgs__action__RunSession_Result__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_Result)() {
  nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, Verdict)();
  nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_member_array[3].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ErrorCode)();
  nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_member_array[6].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_member_array[7].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_Result__rosidl_typesupport_introspection_c__RunSession_Result_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `state`
// already included above
// #include "rosidl_runtime_c/string_functions.h"
// Member `last_error`
// already included above
// #include "nail_msgs/msg/error_code.h"
// Member `last_error`
// already included above
// #include "nail_msgs/msg/detail/error_code__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_Feedback__init(message_memory);
}

void nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_Feedback__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_member_array[7] = {
  {
    "state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Feedback, state),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_target",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Feedback, current_target),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_layer",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Feedback, current_layer),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "total_layers",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Feedback, total_layers),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "stage_percent",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Feedback, stage_percent),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "session_percent",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Feedback, session_percent),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "last_error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_Feedback, last_error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_Feedback",  // message name
  7,  // number of fields
  sizeof(nail_msgs__action__RunSession_Feedback),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_member_array,  // message members
  nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_Feedback__get_type_hash,
  &nail_msgs__action__RunSession_Feedback__get_type_description,
  &nail_msgs__action__RunSession_Feedback__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_Feedback)() {
  nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_member_array[6].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ErrorCode)();
  if (!nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_Feedback__rosidl_typesupport_introspection_c__RunSession_Feedback_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `goal`
#include "nail_msgs/action/run_session.h"
// Member `goal`
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_SendGoal_Request__init(message_memory);
}

void nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_SendGoal_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_SendGoal_Request, goal_id),  // bytes offset in struct
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
    offsetof(nail_msgs__action__RunSession_SendGoal_Request, goal),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_SendGoal_Request",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__RunSession_SendGoal_Request),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_member_array,  // message members
  nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_SendGoal_Request__get_type_hash,
  &nail_msgs__action__RunSession_SendGoal_Request__get_type_description,
  &nail_msgs__action__RunSession_SendGoal_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Request)() {
  nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_Goal)();
  if (!nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `stamp`
// already included above
// #include "builtin_interfaces/msg/time.h"
// Member `stamp`
// already included above
// #include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_SendGoal_Response__init(message_memory);
}

void nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_SendGoal_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_member_array[2] = {
  {
    "accepted",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_SendGoal_Response, accepted),  // bytes offset in struct
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
    offsetof(nail_msgs__action__RunSession_SendGoal_Response, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_SendGoal_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__RunSession_SendGoal_Response),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_member_array,  // message members
  nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_SendGoal_Response__get_type_hash,
  &nail_msgs__action__RunSession_SendGoal_Response__get_type_description,
  &nail_msgs__action__RunSession_SendGoal_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Response)() {
  nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/run_session.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_SendGoal_Event__init(message_memory);
}

void nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_SendGoal_Event__fini(message_memory);
}

size_t nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__RunSession_SendGoal_Event__request(
  const void * untyped_member)
{
  const nail_msgs__action__RunSession_SendGoal_Request__Sequence * member =
    (const nail_msgs__action__RunSession_SendGoal_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_SendGoal_Event__request(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__RunSession_SendGoal_Request__Sequence * member =
    (const nail_msgs__action__RunSession_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__RunSession_SendGoal_Event__request(
  void * untyped_member, size_t index)
{
  nail_msgs__action__RunSession_SendGoal_Request__Sequence * member =
    (nail_msgs__action__RunSession_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_SendGoal_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__RunSession_SendGoal_Request * item =
    ((const nail_msgs__action__RunSession_SendGoal_Request *)
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_SendGoal_Event__request(untyped_member, index));
  nail_msgs__action__RunSession_SendGoal_Request * value =
    (nail_msgs__action__RunSession_SendGoal_Request *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_SendGoal_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__RunSession_SendGoal_Request * item =
    ((nail_msgs__action__RunSession_SendGoal_Request *)
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__RunSession_SendGoal_Event__request(untyped_member, index));
  const nail_msgs__action__RunSession_SendGoal_Request * value =
    (const nail_msgs__action__RunSession_SendGoal_Request *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_SendGoal_Event__request(
  void * untyped_member, size_t size)
{
  nail_msgs__action__RunSession_SendGoal_Request__Sequence * member =
    (nail_msgs__action__RunSession_SendGoal_Request__Sequence *)(untyped_member);
  nail_msgs__action__RunSession_SendGoal_Request__Sequence__fini(member);
  return nail_msgs__action__RunSession_SendGoal_Request__Sequence__init(member, size);
}

size_t nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__RunSession_SendGoal_Event__response(
  const void * untyped_member)
{
  const nail_msgs__action__RunSession_SendGoal_Response__Sequence * member =
    (const nail_msgs__action__RunSession_SendGoal_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_SendGoal_Event__response(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__RunSession_SendGoal_Response__Sequence * member =
    (const nail_msgs__action__RunSession_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__RunSession_SendGoal_Event__response(
  void * untyped_member, size_t index)
{
  nail_msgs__action__RunSession_SendGoal_Response__Sequence * member =
    (nail_msgs__action__RunSession_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_SendGoal_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__RunSession_SendGoal_Response * item =
    ((const nail_msgs__action__RunSession_SendGoal_Response *)
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_SendGoal_Event__response(untyped_member, index));
  nail_msgs__action__RunSession_SendGoal_Response * value =
    (nail_msgs__action__RunSession_SendGoal_Response *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_SendGoal_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__RunSession_SendGoal_Response * item =
    ((nail_msgs__action__RunSession_SendGoal_Response *)
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__RunSession_SendGoal_Event__response(untyped_member, index));
  const nail_msgs__action__RunSession_SendGoal_Response * value =
    (const nail_msgs__action__RunSession_SendGoal_Response *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_SendGoal_Event__response(
  void * untyped_member, size_t size)
{
  nail_msgs__action__RunSession_SendGoal_Response__Sequence * member =
    (nail_msgs__action__RunSession_SendGoal_Response__Sequence *)(untyped_member);
  nail_msgs__action__RunSession_SendGoal_Response__Sequence__fini(member);
  return nail_msgs__action__RunSession_SendGoal_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_SendGoal_Event, info),  // bytes offset in struct
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
    offsetof(nail_msgs__action__RunSession_SendGoal_Event, request),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__RunSession_SendGoal_Event__request,  // size() function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_SendGoal_Event__request,  // get_const(index) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__RunSession_SendGoal_Event__request,  // get(index) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_SendGoal_Event__request,  // fetch(index, &value) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_SendGoal_Event__request,  // assign(index, value) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_SendGoal_Event__request  // resize(index) function pointer
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
    offsetof(nail_msgs__action__RunSession_SendGoal_Event, response),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__RunSession_SendGoal_Event__response,  // size() function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_SendGoal_Event__response,  // get_const(index) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__RunSession_SendGoal_Event__response,  // get(index) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_SendGoal_Event__response,  // fetch(index, &value) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_SendGoal_Event__response,  // assign(index, value) function pointer
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_SendGoal_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_SendGoal_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs__action__RunSession_SendGoal_Event),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_member_array,  // message members
  nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_SendGoal_Event__get_type_hash,
  &nail_msgs__action__RunSession_SendGoal_Event__get_type_description,
  &nail_msgs__action__RunSession_SendGoal_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Event)() {
  nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Request)();
  nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Response)();
  if (!nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_service_members = {
  "nail_msgs__action",  // service namespace
  "RunSession_SendGoal",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_type_support_handle,
  NULL,  // response message
  // nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_type_support_handle
  NULL  // event_message
  // nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_type_support_handle
};


static rosidl_service_type_support_t nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_service_type_support_handle = {
  0,
  &nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_service_members,
  get_service_typesupport_handle_function,
  &nail_msgs__action__RunSession_SendGoal_Request__rosidl_typesupport_introspection_c__RunSession_SendGoal_Request_message_type_support_handle,
  &nail_msgs__action__RunSession_SendGoal_Response__rosidl_typesupport_introspection_c__RunSession_SendGoal_Response_message_type_support_handle,
  &nail_msgs__action__RunSession_SendGoal_Event__rosidl_typesupport_introspection_c__RunSession_SendGoal_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    RunSession_SendGoal
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    RunSession_SendGoal
  ),
  &nail_msgs__action__RunSession_SendGoal__get_type_hash,
  &nail_msgs__action__RunSession_SendGoal__get_type_description,
  &nail_msgs__action__RunSession_SendGoal__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal)(void) {
  if (!nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_service_type_support_handle.typesupport_identifier) {
    nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_SendGoal_Event)()->data;
  }

  return &nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_SendGoal_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


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

void nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_GetResult_Request__init(message_memory);
}

void nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_GetResult_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_member_array[1] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_GetResult_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_GetResult_Request",  // message name
  1,  // number of fields
  sizeof(nail_msgs__action__RunSession_GetResult_Request),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_member_array,  // message members
  nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_GetResult_Request__get_type_hash,
  &nail_msgs__action__RunSession_GetResult_Request__get_type_description,
  &nail_msgs__action__RunSession_GetResult_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Request)() {
  nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  if (!nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `result`
// already included above
// #include "nail_msgs/action/run_session.h"
// Member `result`
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_GetResult_Response__init(message_memory);
}

void nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_GetResult_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_member_array[2] = {
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_GetResult_Response, status),  // bytes offset in struct
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
    offsetof(nail_msgs__action__RunSession_GetResult_Response, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_GetResult_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__RunSession_GetResult_Response),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_member_array,  // message members
  nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_GetResult_Response__get_type_hash,
  &nail_msgs__action__RunSession_GetResult_Response__get_type_description,
  &nail_msgs__action__RunSession_GetResult_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Response)() {
  nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_Result)();
  if (!nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


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
// #include "nail_msgs/action/run_session.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_GetResult_Event__init(message_memory);
}

void nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_GetResult_Event__fini(message_memory);
}

size_t nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__size_function__RunSession_GetResult_Event__request(
  const void * untyped_member)
{
  const nail_msgs__action__RunSession_GetResult_Request__Sequence * member =
    (const nail_msgs__action__RunSession_GetResult_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_GetResult_Event__request(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__RunSession_GetResult_Request__Sequence * member =
    (const nail_msgs__action__RunSession_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_function__RunSession_GetResult_Event__request(
  void * untyped_member, size_t index)
{
  nail_msgs__action__RunSession_GetResult_Request__Sequence * member =
    (nail_msgs__action__RunSession_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_GetResult_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__RunSession_GetResult_Request * item =
    ((const nail_msgs__action__RunSession_GetResult_Request *)
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_GetResult_Event__request(untyped_member, index));
  nail_msgs__action__RunSession_GetResult_Request * value =
    (nail_msgs__action__RunSession_GetResult_Request *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_GetResult_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__RunSession_GetResult_Request * item =
    ((nail_msgs__action__RunSession_GetResult_Request *)
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_function__RunSession_GetResult_Event__request(untyped_member, index));
  const nail_msgs__action__RunSession_GetResult_Request * value =
    (const nail_msgs__action__RunSession_GetResult_Request *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_GetResult_Event__request(
  void * untyped_member, size_t size)
{
  nail_msgs__action__RunSession_GetResult_Request__Sequence * member =
    (nail_msgs__action__RunSession_GetResult_Request__Sequence *)(untyped_member);
  nail_msgs__action__RunSession_GetResult_Request__Sequence__fini(member);
  return nail_msgs__action__RunSession_GetResult_Request__Sequence__init(member, size);
}

size_t nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__size_function__RunSession_GetResult_Event__response(
  const void * untyped_member)
{
  const nail_msgs__action__RunSession_GetResult_Response__Sequence * member =
    (const nail_msgs__action__RunSession_GetResult_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_GetResult_Event__response(
  const void * untyped_member, size_t index)
{
  const nail_msgs__action__RunSession_GetResult_Response__Sequence * member =
    (const nail_msgs__action__RunSession_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_function__RunSession_GetResult_Event__response(
  void * untyped_member, size_t index)
{
  nail_msgs__action__RunSession_GetResult_Response__Sequence * member =
    (nail_msgs__action__RunSession_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_GetResult_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__action__RunSession_GetResult_Response * item =
    ((const nail_msgs__action__RunSession_GetResult_Response *)
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_GetResult_Event__response(untyped_member, index));
  nail_msgs__action__RunSession_GetResult_Response * value =
    (nail_msgs__action__RunSession_GetResult_Response *)(untyped_value);
  *value = *item;
}

void nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_GetResult_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__action__RunSession_GetResult_Response * item =
    ((nail_msgs__action__RunSession_GetResult_Response *)
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_function__RunSession_GetResult_Event__response(untyped_member, index));
  const nail_msgs__action__RunSession_GetResult_Response * value =
    (const nail_msgs__action__RunSession_GetResult_Response *)(untyped_value);
  *item = *value;
}

bool nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_GetResult_Event__response(
  void * untyped_member, size_t size)
{
  nail_msgs__action__RunSession_GetResult_Response__Sequence * member =
    (nail_msgs__action__RunSession_GetResult_Response__Sequence *)(untyped_member);
  nail_msgs__action__RunSession_GetResult_Response__Sequence__fini(member);
  return nail_msgs__action__RunSession_GetResult_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_GetResult_Event, info),  // bytes offset in struct
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
    offsetof(nail_msgs__action__RunSession_GetResult_Event, request),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__size_function__RunSession_GetResult_Event__request,  // size() function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_GetResult_Event__request,  // get_const(index) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_function__RunSession_GetResult_Event__request,  // get(index) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_GetResult_Event__request,  // fetch(index, &value) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_GetResult_Event__request,  // assign(index, value) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_GetResult_Event__request  // resize(index) function pointer
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
    offsetof(nail_msgs__action__RunSession_GetResult_Event, response),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__size_function__RunSession_GetResult_Event__response,  // size() function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__RunSession_GetResult_Event__response,  // get_const(index) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__get_function__RunSession_GetResult_Event__response,  // get(index) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__RunSession_GetResult_Event__response,  // fetch(index, &value) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__RunSession_GetResult_Event__response,  // assign(index, value) function pointer
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__RunSession_GetResult_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_GetResult_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs__action__RunSession_GetResult_Event),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_member_array,  // message members
  nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_GetResult_Event__get_type_hash,
  &nail_msgs__action__RunSession_GetResult_Event__get_type_description,
  &nail_msgs__action__RunSession_GetResult_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Event)() {
  nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Request)();
  nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Response)();
  if (!nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_service_members = {
  "nail_msgs__action",  // service namespace
  "RunSession_GetResult",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_type_support_handle,
  NULL,  // response message
  // nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_type_support_handle
  NULL  // event_message
  // nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_type_support_handle
};


static rosidl_service_type_support_t nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_service_type_support_handle = {
  0,
  &nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_service_members,
  get_service_typesupport_handle_function,
  &nail_msgs__action__RunSession_GetResult_Request__rosidl_typesupport_introspection_c__RunSession_GetResult_Request_message_type_support_handle,
  &nail_msgs__action__RunSession_GetResult_Response__rosidl_typesupport_introspection_c__RunSession_GetResult_Response_message_type_support_handle,
  &nail_msgs__action__RunSession_GetResult_Event__rosidl_typesupport_introspection_c__RunSession_GetResult_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    RunSession_GetResult
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    action,
    RunSession_GetResult
  ),
  &nail_msgs__action__RunSession_GetResult__get_type_hash,
  &nail_msgs__action__RunSession_GetResult__get_type_description,
  &nail_msgs__action__RunSession_GetResult__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult)(void) {
  if (!nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_service_type_support_handle.typesupport_identifier) {
    nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_GetResult_Event)()->data;
  }

  return &nail_msgs__action__detail__run_session__rosidl_typesupport_introspection_c__RunSession_GetResult_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/action/detail/run_session__functions.h"
// already included above
// #include "nail_msgs/action/detail/run_session__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `feedback`
// already included above
// #include "nail_msgs/action/run_session.h"
// Member `feedback`
// already included above
// #include "nail_msgs/action/detail/run_session__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__action__RunSession_FeedbackMessage__init(message_memory);
}

void nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_fini_function(void * message_memory)
{
  nail_msgs__action__RunSession_FeedbackMessage__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__action__RunSession_FeedbackMessage, goal_id),  // bytes offset in struct
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
    offsetof(nail_msgs__action__RunSession_FeedbackMessage, feedback),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_members = {
  "nail_msgs__action",  // message namespace
  "RunSession_FeedbackMessage",  // message name
  2,  // number of fields
  sizeof(nail_msgs__action__RunSession_FeedbackMessage),
  false,  // has_any_key_member_
  nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_member_array,  // message members
  nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_type_support_handle = {
  0,
  &nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__RunSession_FeedbackMessage__get_type_hash,
  &nail_msgs__action__RunSession_FeedbackMessage__get_type_description,
  &nail_msgs__action__RunSession_FeedbackMessage__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_FeedbackMessage)() {
  nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, action, RunSession_Feedback)();
  if (!nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_type_support_handle.typesupport_identifier) {
    nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__action__RunSession_FeedbackMessage__rosidl_typesupport_introspection_c__RunSession_FeedbackMessage_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
