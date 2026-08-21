// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from nail_msgs:msg/ErrorCode.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "nail_msgs/msg/detail/error_code__functions.h"
#include "nail_msgs/msg/detail/error_code__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void ErrorCode_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::msg::ErrorCode(_init);
}

void ErrorCode_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::msg::ErrorCode *>(message_memory);
  typed_message->~ErrorCode();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ErrorCode_message_member_array[3] = {
  {
    "code",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::ErrorCode, code),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "severity",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::ErrorCode, severity),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "detail",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::ErrorCode, detail),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ErrorCode_message_members = {
  "nail_msgs::msg",  // message namespace
  "ErrorCode",  // message name
  3,  // number of fields
  sizeof(nail_msgs::msg::ErrorCode),
  false,  // has_any_key_member_
  ErrorCode_message_member_array,  // message members
  ErrorCode_init_function,  // function to initialize message memory (memory has to be allocated)
  ErrorCode_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ErrorCode_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ErrorCode_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__msg__ErrorCode__get_type_hash,
  &nail_msgs__msg__ErrorCode__get_type_description,
  &nail_msgs__msg__ErrorCode__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::msg::ErrorCode>()
{
  return &::nail_msgs::msg::rosidl_typesupport_introspection_cpp::ErrorCode_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, msg, ErrorCode)() {
  return &::nail_msgs::msg::rosidl_typesupport_introspection_cpp::ErrorCode_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
