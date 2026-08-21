// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:srv/ValidateContext.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/srv/detail/validate_context__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/srv/detail/validate_context__functions.h"
#include "nail_msgs/srv/detail/validate_context__struct.h"


// Include directives for member types
// Member `session_id`
// Member `required_tool`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__srv__ValidateContext_Request__init(message_memory);
}

void nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_fini_function(void * message_memory)
{
  nail_msgs__srv__ValidateContext_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_member_array[3] = {
  {
    "session_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__srv__ValidateContext_Request, session_id),  // bytes offset in struct
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
    offsetof(nail_msgs__srv__ValidateContext_Request, required_tool),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "require_map",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__srv__ValidateContext_Request, require_map),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_members = {
  "nail_msgs__srv",  // message namespace
  "ValidateContext_Request",  // message name
  3,  // number of fields
  sizeof(nail_msgs__srv__ValidateContext_Request),
  false,  // has_any_key_member_
  nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_member_array,  // message members
  nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_type_support_handle = {
  0,
  &nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__srv__ValidateContext_Request__get_type_hash,
  &nail_msgs__srv__ValidateContext_Request__get_type_description,
  &nail_msgs__srv__ValidateContext_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Request)() {
  if (!nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_type_support_handle.typesupport_identifier) {
    nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/srv/detail/validate_context__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/srv/detail/validate_context__functions.h"
// already included above
// #include "nail_msgs/srv/detail/validate_context__struct.h"


// Include directives for member types
// Member `error`
#include "nail_msgs/msg/error_code.h"
// Member `error`
#include "nail_msgs/msg/detail/error_code__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__srv__ValidateContext_Response__init(message_memory);
}

void nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_fini_function(void * message_memory)
{
  nail_msgs__srv__ValidateContext_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_member_array[2] = {
  {
    "success",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__srv__ValidateContext_Response, success),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__srv__ValidateContext_Response, error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_members = {
  "nail_msgs__srv",  // message namespace
  "ValidateContext_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs__srv__ValidateContext_Response),
  false,  // has_any_key_member_
  nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_member_array,  // message members
  nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_type_support_handle = {
  0,
  &nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__srv__ValidateContext_Response__get_type_hash,
  &nail_msgs__srv__ValidateContext_Response__get_type_description,
  &nail_msgs__srv__ValidateContext_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Response)() {
  nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, ErrorCode)();
  if (!nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_type_support_handle.typesupport_identifier) {
    nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "nail_msgs/srv/detail/validate_context__rosidl_typesupport_introspection_c.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "nail_msgs/srv/detail/validate_context__functions.h"
// already included above
// #include "nail_msgs/srv/detail/validate_context__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
#include "nail_msgs/srv/validate_context.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/srv/detail/validate_context__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__srv__ValidateContext_Event__init(message_memory);
}

void nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_fini_function(void * message_memory)
{
  nail_msgs__srv__ValidateContext_Event__fini(message_memory);
}

size_t nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__size_function__ValidateContext_Event__request(
  const void * untyped_member)
{
  const nail_msgs__srv__ValidateContext_Request__Sequence * member =
    (const nail_msgs__srv__ValidateContext_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_const_function__ValidateContext_Event__request(
  const void * untyped_member, size_t index)
{
  const nail_msgs__srv__ValidateContext_Request__Sequence * member =
    (const nail_msgs__srv__ValidateContext_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_function__ValidateContext_Event__request(
  void * untyped_member, size_t index)
{
  nail_msgs__srv__ValidateContext_Request__Sequence * member =
    (nail_msgs__srv__ValidateContext_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__fetch_function__ValidateContext_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__srv__ValidateContext_Request * item =
    ((const nail_msgs__srv__ValidateContext_Request *)
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_const_function__ValidateContext_Event__request(untyped_member, index));
  nail_msgs__srv__ValidateContext_Request * value =
    (nail_msgs__srv__ValidateContext_Request *)(untyped_value);
  *value = *item;
}

void nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__assign_function__ValidateContext_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__srv__ValidateContext_Request * item =
    ((nail_msgs__srv__ValidateContext_Request *)
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_function__ValidateContext_Event__request(untyped_member, index));
  const nail_msgs__srv__ValidateContext_Request * value =
    (const nail_msgs__srv__ValidateContext_Request *)(untyped_value);
  *item = *value;
}

bool nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__resize_function__ValidateContext_Event__request(
  void * untyped_member, size_t size)
{
  nail_msgs__srv__ValidateContext_Request__Sequence * member =
    (nail_msgs__srv__ValidateContext_Request__Sequence *)(untyped_member);
  nail_msgs__srv__ValidateContext_Request__Sequence__fini(member);
  return nail_msgs__srv__ValidateContext_Request__Sequence__init(member, size);
}

size_t nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__size_function__ValidateContext_Event__response(
  const void * untyped_member)
{
  const nail_msgs__srv__ValidateContext_Response__Sequence * member =
    (const nail_msgs__srv__ValidateContext_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_const_function__ValidateContext_Event__response(
  const void * untyped_member, size_t index)
{
  const nail_msgs__srv__ValidateContext_Response__Sequence * member =
    (const nail_msgs__srv__ValidateContext_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_function__ValidateContext_Event__response(
  void * untyped_member, size_t index)
{
  nail_msgs__srv__ValidateContext_Response__Sequence * member =
    (nail_msgs__srv__ValidateContext_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__fetch_function__ValidateContext_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const nail_msgs__srv__ValidateContext_Response * item =
    ((const nail_msgs__srv__ValidateContext_Response *)
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_const_function__ValidateContext_Event__response(untyped_member, index));
  nail_msgs__srv__ValidateContext_Response * value =
    (nail_msgs__srv__ValidateContext_Response *)(untyped_value);
  *value = *item;
}

void nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__assign_function__ValidateContext_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  nail_msgs__srv__ValidateContext_Response * item =
    ((nail_msgs__srv__ValidateContext_Response *)
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_function__ValidateContext_Event__response(untyped_member, index));
  const nail_msgs__srv__ValidateContext_Response * value =
    (const nail_msgs__srv__ValidateContext_Response *)(untyped_value);
  *item = *value;
}

bool nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__resize_function__ValidateContext_Event__response(
  void * untyped_member, size_t size)
{
  nail_msgs__srv__ValidateContext_Response__Sequence * member =
    (nail_msgs__srv__ValidateContext_Response__Sequence *)(untyped_member);
  nail_msgs__srv__ValidateContext_Response__Sequence__fini(member);
  return nail_msgs__srv__ValidateContext_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__srv__ValidateContext_Event, info),  // bytes offset in struct
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
    offsetof(nail_msgs__srv__ValidateContext_Event, request),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__size_function__ValidateContext_Event__request,  // size() function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_const_function__ValidateContext_Event__request,  // get_const(index) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_function__ValidateContext_Event__request,  // get(index) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__fetch_function__ValidateContext_Event__request,  // fetch(index, &value) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__assign_function__ValidateContext_Event__request,  // assign(index, value) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__resize_function__ValidateContext_Event__request  // resize(index) function pointer
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
    offsetof(nail_msgs__srv__ValidateContext_Event, response),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__size_function__ValidateContext_Event__response,  // size() function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_const_function__ValidateContext_Event__response,  // get_const(index) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__get_function__ValidateContext_Event__response,  // get(index) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__fetch_function__ValidateContext_Event__response,  // fetch(index, &value) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__assign_function__ValidateContext_Event__response,  // assign(index, value) function pointer
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__resize_function__ValidateContext_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_members = {
  "nail_msgs__srv",  // message namespace
  "ValidateContext_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs__srv__ValidateContext_Event),
  false,  // has_any_key_member_
  nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_member_array,  // message members
  nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_type_support_handle = {
  0,
  &nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__srv__ValidateContext_Event__get_type_hash,
  &nail_msgs__srv__ValidateContext_Event__get_type_description,
  &nail_msgs__srv__ValidateContext_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Event)() {
  nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Request)();
  nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Response)();
  if (!nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_type_support_handle.typesupport_identifier) {
    nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "nail_msgs/srv/detail/validate_context__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_service_members = {
  "nail_msgs__srv",  // service namespace
  "ValidateContext",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_Request_message_type_support_handle,
  NULL,  // response message
  // nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_Response_message_type_support_handle
  NULL  // event_message
  // nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_Response_message_type_support_handle
};


static rosidl_service_type_support_t nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_service_type_support_handle = {
  0,
  &nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_service_members,
  get_service_typesupport_handle_function,
  &nail_msgs__srv__ValidateContext_Request__rosidl_typesupport_introspection_c__ValidateContext_Request_message_type_support_handle,
  &nail_msgs__srv__ValidateContext_Response__rosidl_typesupport_introspection_c__ValidateContext_Response_message_type_support_handle,
  &nail_msgs__srv__ValidateContext_Event__rosidl_typesupport_introspection_c__ValidateContext_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    srv,
    ValidateContext
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    nail_msgs,
    srv,
    ValidateContext
  ),
  &nail_msgs__srv__ValidateContext__get_type_hash,
  &nail_msgs__srv__ValidateContext__get_type_description,
  &nail_msgs__srv__ValidateContext__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext)(void) {
  if (!nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_service_type_support_handle.typesupport_identifier) {
    nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, srv, ValidateContext_Event)()->data;
  }

  return &nail_msgs__srv__detail__validate_context__rosidl_typesupport_introspection_c__ValidateContext_service_type_support_handle;
}
