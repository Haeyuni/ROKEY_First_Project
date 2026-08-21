// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from nail_msgs:action/VerifyCure.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "nail_msgs/action/detail/verify_cure__functions.h"
#include "nail_msgs/action/detail/verify_cure__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_Goal_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_Goal(_init);
}

void VerifyCure_Goal_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_Goal *>(message_memory);
  typed_message->~VerifyCure_Goal();
}

size_t size_function__VerifyCure_Goal__probe_points(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return member->size();
}

const void * get_const_function__VerifyCure_Goal__probe_points(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void * get_function__VerifyCure_Goal__probe_points(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void fetch_function__VerifyCure_Goal__probe_points(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const geometry_msgs::msg::Point *>(
    get_const_function__VerifyCure_Goal__probe_points(untyped_member, index));
  auto & value = *reinterpret_cast<geometry_msgs::msg::Point *>(untyped_value);
  value = item;
}

void assign_function__VerifyCure_Goal__probe_points(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<geometry_msgs::msg::Point *>(
    get_function__VerifyCure_Goal__probe_points(untyped_member, index));
  const auto & value = *reinterpret_cast<const geometry_msgs::msg::Point *>(untyped_value);
  item = value;
}

void resize_function__VerifyCure_Goal__probe_points(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_Goal_message_member_array[8] = {
  {
    "probe_points",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Point>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, probe_points),  // bytes offset in struct
    nullptr,  // default value
    size_function__VerifyCure_Goal__probe_points,  // size() function pointer
    get_const_function__VerifyCure_Goal__probe_points,  // get_const(index) function pointer
    get_function__VerifyCure_Goal__probe_points,  // get(index) function pointer
    fetch_function__VerifyCure_Goal__probe_points,  // fetch(index, &value) function pointer
    assign_function__VerifyCure_Goal__probe_points,  // assign(index, value) function pointer
    resize_function__VerifyCure_Goal__probe_points  // resize(index) function pointer
  },
  {
    "frame_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, frame_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "session_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, session_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "target_index",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, target_index),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "layer_index",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, layer_index),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "tensile_fail_n",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, tensile_fail_n),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "tensile_pass_n",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, tensile_pass_n),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "probe_force_limit_n",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Goal, probe_force_limit_n),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_Goal_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_Goal",  // message name
  8,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_Goal),
  false,  // has_any_key_member_
  VerifyCure_Goal_message_member_array,  // message members
  VerifyCure_Goal_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_Goal_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_Goal_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_Goal_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_Goal__get_type_hash,
  &nail_msgs__action__VerifyCure_Goal__get_type_description,
  &nail_msgs__action__VerifyCure_Goal__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_Goal>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_Goal_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_Goal)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_Goal_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_Result_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_Result(_init);
}

void VerifyCure_Result_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_Result *>(message_memory);
  typed_message->~VerifyCure_Result();
}

size_t size_function__VerifyCure_Result__verdicts(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::msg::Verdict> *>(untyped_member);
  return member->size();
}

const void * get_const_function__VerifyCure_Result__verdicts(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::msg::Verdict> *>(untyped_member);
  return &member[index];
}

void * get_function__VerifyCure_Result__verdicts(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::msg::Verdict> *>(untyped_member);
  return &member[index];
}

void fetch_function__VerifyCure_Result__verdicts(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::msg::Verdict *>(
    get_const_function__VerifyCure_Result__verdicts(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::msg::Verdict *>(untyped_value);
  value = item;
}

void assign_function__VerifyCure_Result__verdicts(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::msg::Verdict *>(
    get_function__VerifyCure_Result__verdicts(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::msg::Verdict *>(untyped_value);
  item = value;
}

void resize_function__VerifyCure_Result__verdicts(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::msg::Verdict> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_Result_message_member_array[4] = {
  {
    "base",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::msg::ActionResultBase>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Result, base),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "verdicts",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::msg::Verdict>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Result, verdicts),  // bytes offset in struct
    nullptr,  // default value
    size_function__VerifyCure_Result__verdicts,  // size() function pointer
    get_const_function__VerifyCure_Result__verdicts,  // get_const(index) function pointer
    get_function__VerifyCure_Result__verdicts,  // get(index) function pointer
    fetch_function__VerifyCure_Result__verdicts,  // fetch(index, &value) function pointer
    assign_function__VerifyCure_Result__verdicts,  // assign(index, value) function pointer
    resize_function__VerifyCure_Result__verdicts  // resize(index) function pointer
  },
  {
    "overall_result",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Result, overall_result),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "contamination_suspected",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Result, contamination_suspected),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_Result_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_Result",  // message name
  4,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_Result),
  false,  // has_any_key_member_
  VerifyCure_Result_message_member_array,  // message members
  VerifyCure_Result_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_Result_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_Result_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_Result_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_Result__get_type_hash,
  &nail_msgs__action__VerifyCure_Result__get_type_description,
  &nail_msgs__action__VerifyCure_Result__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_Result>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_Result_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_Result)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_Result_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_Feedback_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_Feedback(_init);
}

void VerifyCure_Feedback_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_Feedback *>(message_memory);
  typed_message->~VerifyCure_Feedback();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_Feedback_message_member_array[3] = {
  {
    "percent",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Feedback, percent),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "points_done",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Feedback, points_done),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "last_verdict",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::msg::Verdict>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_Feedback, last_verdict),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_Feedback_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_Feedback",  // message name
  3,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_Feedback),
  false,  // has_any_key_member_
  VerifyCure_Feedback_message_member_array,  // message members
  VerifyCure_Feedback_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_Feedback_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_Feedback_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_Feedback_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_Feedback__get_type_hash,
  &nail_msgs__action__VerifyCure_Feedback__get_type_description,
  &nail_msgs__action__VerifyCure_Feedback__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_Feedback>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_Feedback_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_Feedback)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_Feedback_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_SendGoal_Request_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_SendGoal_Request(_init);
}

void VerifyCure_SendGoal_Request_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_SendGoal_Request *>(message_memory);
  typed_message->~VerifyCure_SendGoal_Request();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_SendGoal_Request_message_member_array[2] = {
  {
    "goal_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<unique_identifier_msgs::msg::UUID>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_SendGoal_Request, goal_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "goal",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_Goal>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_SendGoal_Request, goal),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_SendGoal_Request_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_SendGoal_Request",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_SendGoal_Request),
  false,  // has_any_key_member_
  VerifyCure_SendGoal_Request_message_member_array,  // message members
  VerifyCure_SendGoal_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_SendGoal_Request_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_SendGoal_Request_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_SendGoal_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_SendGoal_Request__get_type_hash,
  &nail_msgs__action__VerifyCure_SendGoal_Request__get_type_description,
  &nail_msgs__action__VerifyCure_SendGoal_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Request>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_SendGoal_Request_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_SendGoal_Request)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_SendGoal_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_SendGoal_Response_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_SendGoal_Response(_init);
}

void VerifyCure_SendGoal_Response_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_SendGoal_Response *>(message_memory);
  typed_message->~VerifyCure_SendGoal_Response();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_SendGoal_Response_message_member_array[2] = {
  {
    "accepted",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_SendGoal_Response, accepted),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "stamp",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<builtin_interfaces::msg::Time>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_SendGoal_Response, stamp),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_SendGoal_Response_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_SendGoal_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_SendGoal_Response),
  false,  // has_any_key_member_
  VerifyCure_SendGoal_Response_message_member_array,  // message members
  VerifyCure_SendGoal_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_SendGoal_Response_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_SendGoal_Response_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_SendGoal_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_SendGoal_Response__get_type_hash,
  &nail_msgs__action__VerifyCure_SendGoal_Response__get_type_description,
  &nail_msgs__action__VerifyCure_SendGoal_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Response>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_SendGoal_Response_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_SendGoal_Response)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_SendGoal_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_SendGoal_Event_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_SendGoal_Event(_init);
}

void VerifyCure_SendGoal_Event_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_SendGoal_Event *>(message_memory);
  typed_message->~VerifyCure_SendGoal_Event();
}

size_t size_function__VerifyCure_SendGoal_Event__request(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_SendGoal_Request> *>(untyped_member);
  return member->size();
}

const void * get_const_function__VerifyCure_SendGoal_Event__request(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_SendGoal_Request> *>(untyped_member);
  return &member[index];
}

void * get_function__VerifyCure_SendGoal_Event__request(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_SendGoal_Request> *>(untyped_member);
  return &member[index];
}

void fetch_function__VerifyCure_SendGoal_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::VerifyCure_SendGoal_Request *>(
    get_const_function__VerifyCure_SendGoal_Event__request(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::VerifyCure_SendGoal_Request *>(untyped_value);
  value = item;
}

void assign_function__VerifyCure_SendGoal_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::VerifyCure_SendGoal_Request *>(
    get_function__VerifyCure_SendGoal_Event__request(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::VerifyCure_SendGoal_Request *>(untyped_value);
  item = value;
}

void resize_function__VerifyCure_SendGoal_Event__request(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_SendGoal_Request> *>(untyped_member);
  member->resize(size);
}

size_t size_function__VerifyCure_SendGoal_Event__response(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_SendGoal_Response> *>(untyped_member);
  return member->size();
}

const void * get_const_function__VerifyCure_SendGoal_Event__response(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_SendGoal_Response> *>(untyped_member);
  return &member[index];
}

void * get_function__VerifyCure_SendGoal_Event__response(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_SendGoal_Response> *>(untyped_member);
  return &member[index];
}

void fetch_function__VerifyCure_SendGoal_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::VerifyCure_SendGoal_Response *>(
    get_const_function__VerifyCure_SendGoal_Event__response(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::VerifyCure_SendGoal_Response *>(untyped_value);
  value = item;
}

void assign_function__VerifyCure_SendGoal_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::VerifyCure_SendGoal_Response *>(
    get_function__VerifyCure_SendGoal_Event__response(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::VerifyCure_SendGoal_Response *>(untyped_value);
  item = value;
}

void resize_function__VerifyCure_SendGoal_Event__response(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_SendGoal_Response> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_SendGoal_Event_message_member_array[3] = {
  {
    "info",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<service_msgs::msg::ServiceEventInfo>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_SendGoal_Event, info),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "request",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Request>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_SendGoal_Event, request),  // bytes offset in struct
    nullptr,  // default value
    size_function__VerifyCure_SendGoal_Event__request,  // size() function pointer
    get_const_function__VerifyCure_SendGoal_Event__request,  // get_const(index) function pointer
    get_function__VerifyCure_SendGoal_Event__request,  // get(index) function pointer
    fetch_function__VerifyCure_SendGoal_Event__request,  // fetch(index, &value) function pointer
    assign_function__VerifyCure_SendGoal_Event__request,  // assign(index, value) function pointer
    resize_function__VerifyCure_SendGoal_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Response>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_SendGoal_Event, response),  // bytes offset in struct
    nullptr,  // default value
    size_function__VerifyCure_SendGoal_Event__response,  // size() function pointer
    get_const_function__VerifyCure_SendGoal_Event__response,  // get_const(index) function pointer
    get_function__VerifyCure_SendGoal_Event__response,  // get(index) function pointer
    fetch_function__VerifyCure_SendGoal_Event__response,  // fetch(index, &value) function pointer
    assign_function__VerifyCure_SendGoal_Event__response,  // assign(index, value) function pointer
    resize_function__VerifyCure_SendGoal_Event__response  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_SendGoal_Event_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_SendGoal_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_SendGoal_Event),
  false,  // has_any_key_member_
  VerifyCure_SendGoal_Event_message_member_array,  // message members
  VerifyCure_SendGoal_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_SendGoal_Event_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_SendGoal_Event_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_SendGoal_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_SendGoal_Event__get_type_hash,
  &nail_msgs__action__VerifyCure_SendGoal_Event__get_type_description,
  &nail_msgs__action__VerifyCure_SendGoal_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Event>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_SendGoal_Event_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_SendGoal_Event)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_SendGoal_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/service_type_support_decl.hpp"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

// this is intentionally not const to allow initialization later to prevent an initialization race
static ::rosidl_typesupport_introspection_cpp::ServiceMembers VerifyCure_SendGoal_service_members = {
  "nail_msgs::action",  // service namespace
  "VerifyCure_SendGoal",  // service name
  // the following fields are initialized below on first access
  // see get_service_type_support_handle<nail_msgs::action::VerifyCure_SendGoal>()
  nullptr,  // request message
  nullptr,  // response message
  nullptr,  // event message
};

static const rosidl_service_type_support_t VerifyCure_SendGoal_service_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_SendGoal_service_members,
  get_service_typesupport_handle_function,
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Request>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Response>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_SendGoal_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<nail_msgs::action::VerifyCure_SendGoal>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<nail_msgs::action::VerifyCure_SendGoal>,
  &nail_msgs__action__VerifyCure_SendGoal__get_type_hash,
  &nail_msgs__action__VerifyCure_SendGoal__get_type_description,
  &nail_msgs__action__VerifyCure_SendGoal__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<nail_msgs::action::VerifyCure_SendGoal>()
{
  // get a handle to the value to be returned
  auto service_type_support =
    &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_SendGoal_service_type_support_handle;
  // get a non-const and properly typed version of the data void *
  auto service_members = const_cast<::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
    static_cast<const ::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
      service_type_support->data));
  // make sure all of the service_members are initialized
  // if they are not, initialize them
  if (
    service_members->request_members_ == nullptr ||
    service_members->response_members_ == nullptr ||
    service_members->event_members_ == nullptr)
  {
    // initialize the request_members_ with the static function from the external library
    service_members->request_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::VerifyCure_SendGoal_Request
      >()->data
      );
    // initialize the response_members_ with the static function from the external library
    service_members->response_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::VerifyCure_SendGoal_Response
      >()->data
      );
    // initialize the event_members_ with the static function from the external library
    service_members->event_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::VerifyCure_SendGoal_Event
      >()->data
      );
  }
  // finally return the properly initialized service_type_support handle
  return service_type_support;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_SendGoal)() {
  return ::rosidl_typesupport_introspection_cpp::get_service_type_support_handle<nail_msgs::action::VerifyCure_SendGoal>();
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_GetResult_Request_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_GetResult_Request(_init);
}

void VerifyCure_GetResult_Request_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_GetResult_Request *>(message_memory);
  typed_message->~VerifyCure_GetResult_Request();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_GetResult_Request_message_member_array[1] = {
  {
    "goal_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<unique_identifier_msgs::msg::UUID>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_GetResult_Request, goal_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_GetResult_Request_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_GetResult_Request",  // message name
  1,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_GetResult_Request),
  false,  // has_any_key_member_
  VerifyCure_GetResult_Request_message_member_array,  // message members
  VerifyCure_GetResult_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_GetResult_Request_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_GetResult_Request_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_GetResult_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_GetResult_Request__get_type_hash,
  &nail_msgs__action__VerifyCure_GetResult_Request__get_type_description,
  &nail_msgs__action__VerifyCure_GetResult_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Request>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_GetResult_Request_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_GetResult_Request)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_GetResult_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_GetResult_Response_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_GetResult_Response(_init);
}

void VerifyCure_GetResult_Response_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_GetResult_Response *>(message_memory);
  typed_message->~VerifyCure_GetResult_Response();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_GetResult_Response_message_member_array[2] = {
  {
    "status",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_GetResult_Response, status),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "result",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_Result>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_GetResult_Response, result),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_GetResult_Response_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_GetResult_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_GetResult_Response),
  false,  // has_any_key_member_
  VerifyCure_GetResult_Response_message_member_array,  // message members
  VerifyCure_GetResult_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_GetResult_Response_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_GetResult_Response_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_GetResult_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_GetResult_Response__get_type_hash,
  &nail_msgs__action__VerifyCure_GetResult_Response__get_type_description,
  &nail_msgs__action__VerifyCure_GetResult_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Response>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_GetResult_Response_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_GetResult_Response)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_GetResult_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_GetResult_Event_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_GetResult_Event(_init);
}

void VerifyCure_GetResult_Event_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_GetResult_Event *>(message_memory);
  typed_message->~VerifyCure_GetResult_Event();
}

size_t size_function__VerifyCure_GetResult_Event__request(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_GetResult_Request> *>(untyped_member);
  return member->size();
}

const void * get_const_function__VerifyCure_GetResult_Event__request(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_GetResult_Request> *>(untyped_member);
  return &member[index];
}

void * get_function__VerifyCure_GetResult_Event__request(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_GetResult_Request> *>(untyped_member);
  return &member[index];
}

void fetch_function__VerifyCure_GetResult_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::VerifyCure_GetResult_Request *>(
    get_const_function__VerifyCure_GetResult_Event__request(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::VerifyCure_GetResult_Request *>(untyped_value);
  value = item;
}

void assign_function__VerifyCure_GetResult_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::VerifyCure_GetResult_Request *>(
    get_function__VerifyCure_GetResult_Event__request(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::VerifyCure_GetResult_Request *>(untyped_value);
  item = value;
}

void resize_function__VerifyCure_GetResult_Event__request(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_GetResult_Request> *>(untyped_member);
  member->resize(size);
}

size_t size_function__VerifyCure_GetResult_Event__response(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_GetResult_Response> *>(untyped_member);
  return member->size();
}

const void * get_const_function__VerifyCure_GetResult_Event__response(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::VerifyCure_GetResult_Response> *>(untyped_member);
  return &member[index];
}

void * get_function__VerifyCure_GetResult_Event__response(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_GetResult_Response> *>(untyped_member);
  return &member[index];
}

void fetch_function__VerifyCure_GetResult_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::VerifyCure_GetResult_Response *>(
    get_const_function__VerifyCure_GetResult_Event__response(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::VerifyCure_GetResult_Response *>(untyped_value);
  value = item;
}

void assign_function__VerifyCure_GetResult_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::VerifyCure_GetResult_Response *>(
    get_function__VerifyCure_GetResult_Event__response(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::VerifyCure_GetResult_Response *>(untyped_value);
  item = value;
}

void resize_function__VerifyCure_GetResult_Event__response(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::VerifyCure_GetResult_Response> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_GetResult_Event_message_member_array[3] = {
  {
    "info",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<service_msgs::msg::ServiceEventInfo>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_GetResult_Event, info),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "request",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Request>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_GetResult_Event, request),  // bytes offset in struct
    nullptr,  // default value
    size_function__VerifyCure_GetResult_Event__request,  // size() function pointer
    get_const_function__VerifyCure_GetResult_Event__request,  // get_const(index) function pointer
    get_function__VerifyCure_GetResult_Event__request,  // get(index) function pointer
    fetch_function__VerifyCure_GetResult_Event__request,  // fetch(index, &value) function pointer
    assign_function__VerifyCure_GetResult_Event__request,  // assign(index, value) function pointer
    resize_function__VerifyCure_GetResult_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Response>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_GetResult_Event, response),  // bytes offset in struct
    nullptr,  // default value
    size_function__VerifyCure_GetResult_Event__response,  // size() function pointer
    get_const_function__VerifyCure_GetResult_Event__response,  // get_const(index) function pointer
    get_function__VerifyCure_GetResult_Event__response,  // get(index) function pointer
    fetch_function__VerifyCure_GetResult_Event__response,  // fetch(index, &value) function pointer
    assign_function__VerifyCure_GetResult_Event__response,  // assign(index, value) function pointer
    resize_function__VerifyCure_GetResult_Event__response  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_GetResult_Event_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_GetResult_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_GetResult_Event),
  false,  // has_any_key_member_
  VerifyCure_GetResult_Event_message_member_array,  // message members
  VerifyCure_GetResult_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_GetResult_Event_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_GetResult_Event_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_GetResult_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_GetResult_Event__get_type_hash,
  &nail_msgs__action__VerifyCure_GetResult_Event__get_type_description,
  &nail_msgs__action__VerifyCure_GetResult_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Event>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_GetResult_Event_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_GetResult_Event)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_GetResult_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/service_type_support_decl.hpp"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

// this is intentionally not const to allow initialization later to prevent an initialization race
static ::rosidl_typesupport_introspection_cpp::ServiceMembers VerifyCure_GetResult_service_members = {
  "nail_msgs::action",  // service namespace
  "VerifyCure_GetResult",  // service name
  // the following fields are initialized below on first access
  // see get_service_type_support_handle<nail_msgs::action::VerifyCure_GetResult>()
  nullptr,  // request message
  nullptr,  // response message
  nullptr,  // event message
};

static const rosidl_service_type_support_t VerifyCure_GetResult_service_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_GetResult_service_members,
  get_service_typesupport_handle_function,
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Request>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Response>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_GetResult_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<nail_msgs::action::VerifyCure_GetResult>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<nail_msgs::action::VerifyCure_GetResult>,
  &nail_msgs__action__VerifyCure_GetResult__get_type_hash,
  &nail_msgs__action__VerifyCure_GetResult__get_type_description,
  &nail_msgs__action__VerifyCure_GetResult__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<nail_msgs::action::VerifyCure_GetResult>()
{
  // get a handle to the value to be returned
  auto service_type_support =
    &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_GetResult_service_type_support_handle;
  // get a non-const and properly typed version of the data void *
  auto service_members = const_cast<::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
    static_cast<const ::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
      service_type_support->data));
  // make sure all of the service_members are initialized
  // if they are not, initialize them
  if (
    service_members->request_members_ == nullptr ||
    service_members->response_members_ == nullptr ||
    service_members->event_members_ == nullptr)
  {
    // initialize the request_members_ with the static function from the external library
    service_members->request_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::VerifyCure_GetResult_Request
      >()->data
      );
    // initialize the response_members_ with the static function from the external library
    service_members->response_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::VerifyCure_GetResult_Response
      >()->data
      );
    // initialize the event_members_ with the static function from the external library
    service_members->event_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::VerifyCure_GetResult_Event
      >()->data
      );
  }
  // finally return the properly initialized service_type_support handle
  return service_type_support;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_GetResult)() {
  return ::rosidl_typesupport_introspection_cpp::get_service_type_support_handle<nail_msgs::action::VerifyCure_GetResult>();
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__functions.h"
// already included above
// #include "nail_msgs/action/detail/verify_cure__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace nail_msgs
{

namespace action
{

namespace rosidl_typesupport_introspection_cpp
{

void VerifyCure_FeedbackMessage_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::VerifyCure_FeedbackMessage(_init);
}

void VerifyCure_FeedbackMessage_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::VerifyCure_FeedbackMessage *>(message_memory);
  typed_message->~VerifyCure_FeedbackMessage();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VerifyCure_FeedbackMessage_message_member_array[2] = {
  {
    "goal_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<unique_identifier_msgs::msg::UUID>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_FeedbackMessage, goal_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "feedback",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::VerifyCure_Feedback>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::VerifyCure_FeedbackMessage, feedback),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VerifyCure_FeedbackMessage_message_members = {
  "nail_msgs::action",  // message namespace
  "VerifyCure_FeedbackMessage",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::VerifyCure_FeedbackMessage),
  false,  // has_any_key_member_
  VerifyCure_FeedbackMessage_message_member_array,  // message members
  VerifyCure_FeedbackMessage_init_function,  // function to initialize message memory (memory has to be allocated)
  VerifyCure_FeedbackMessage_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VerifyCure_FeedbackMessage_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VerifyCure_FeedbackMessage_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__VerifyCure_FeedbackMessage__get_type_hash,
  &nail_msgs__action__VerifyCure_FeedbackMessage__get_type_description,
  &nail_msgs__action__VerifyCure_FeedbackMessage__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::VerifyCure_FeedbackMessage>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_FeedbackMessage_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, VerifyCure_FeedbackMessage)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::VerifyCure_FeedbackMessage_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
