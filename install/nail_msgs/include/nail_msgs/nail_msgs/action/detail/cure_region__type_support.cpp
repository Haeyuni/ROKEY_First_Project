// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from nail_msgs:action/CureRegion.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "nail_msgs/action/detail/cure_region__functions.h"
#include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_Goal_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_Goal(_init);
}

void CureRegion_Goal_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_Goal *>(message_memory);
  typed_message->~CureRegion_Goal();
}

size_t size_function__CureRegion_Goal__trajectory(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CureRegion_Goal__trajectory(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  return &member[index];
}

void * get_function__CureRegion_Goal__trajectory(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  return &member[index];
}

void fetch_function__CureRegion_Goal__trajectory(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const geometry_msgs::msg::Pose *>(
    get_const_function__CureRegion_Goal__trajectory(untyped_member, index));
  auto & value = *reinterpret_cast<geometry_msgs::msg::Pose *>(untyped_value);
  value = item;
}

void assign_function__CureRegion_Goal__trajectory(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<geometry_msgs::msg::Pose *>(
    get_function__CureRegion_Goal__trajectory(untyped_member, index));
  const auto & value = *reinterpret_cast<const geometry_msgs::msg::Pose *>(untyped_value);
  item = value;
}

void resize_function__CureRegion_Goal__trajectory(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<geometry_msgs::msg::Pose> *>(untyped_member);
  member->resize(size);
}

size_t size_function__CureRegion_Goal__regions(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CureRegion_Goal__regions(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void * get_function__CureRegion_Goal__regions(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void fetch_function__CureRegion_Goal__regions(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const geometry_msgs::msg::Point *>(
    get_const_function__CureRegion_Goal__regions(untyped_member, index));
  auto & value = *reinterpret_cast<geometry_msgs::msg::Point *>(untyped_value);
  value = item;
}

void assign_function__CureRegion_Goal__regions(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<geometry_msgs::msg::Point *>(
    get_function__CureRegion_Goal__regions(untyped_member, index));
  const auto & value = *reinterpret_cast<const geometry_msgs::msg::Point *>(untyped_value);
  item = value;
}

void resize_function__CureRegion_Goal__regions(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_Goal_message_member_array[11] = {
  {
    "trajectory",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Pose>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Goal, trajectory),  // bytes offset in struct
    nullptr,  // default value
    size_function__CureRegion_Goal__trajectory,  // size() function pointer
    get_const_function__CureRegion_Goal__trajectory,  // get_const(index) function pointer
    get_function__CureRegion_Goal__trajectory,  // get(index) function pointer
    fetch_function__CureRegion_Goal__trajectory,  // fetch(index, &value) function pointer
    assign_function__CureRegion_Goal__trajectory,  // assign(index, value) function pointer
    resize_function__CureRegion_Goal__trajectory  // resize(index) function pointer
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
    offsetof(nail_msgs::action::CureRegion_Goal, frame_id),  // bytes offset in struct
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
    offsetof(nail_msgs::action::CureRegion_Goal, session_id),  // bytes offset in struct
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
    offsetof(nail_msgs::action::CureRegion_Goal, target_index),  // bytes offset in struct
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
    offsetof(nail_msgs::action::CureRegion_Goal, layer_index),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "regions",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Point>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Goal, regions),  // bytes offset in struct
    nullptr,  // default value
    size_function__CureRegion_Goal__regions,  // size() function pointer
    get_const_function__CureRegion_Goal__regions,  // get_const(index) function pointer
    get_function__CureRegion_Goal__regions,  // get(index) function pointer
    fetch_function__CureRegion_Goal__regions,  // fetch(index, &value) function pointer
    assign_function__CureRegion_Goal__regions,  // assign(index, value) function pointer
    resize_function__CureRegion_Goal__regions  // resize(index) function pointer
  },
  {
    "exposure_s_per_region",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Goal, exposure_s_per_region),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "max_exposure_s",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Goal, max_exposure_s),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "standoff_target_mm",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Goal, standoff_target_mm),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "standoff_tolerance_mm",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Goal, standoff_tolerance_mm),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "exposure_scale",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Goal, exposure_scale),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_Goal_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_Goal",  // message name
  11,  // number of fields
  sizeof(nail_msgs::action::CureRegion_Goal),
  false,  // has_any_key_member_
  CureRegion_Goal_message_member_array,  // message members
  CureRegion_Goal_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_Goal_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_Goal_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_Goal_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_Goal__get_type_hash,
  &nail_msgs__action__CureRegion_Goal__get_type_description,
  &nail_msgs__action__CureRegion_Goal__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_Goal>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_Goal_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_Goal)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_Goal_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_Result_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_Result(_init);
}

void CureRegion_Result_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_Result *>(message_memory);
  typed_message->~CureRegion_Result();
}

size_t size_function__CureRegion_Result__missed_region_indices(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<int32_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CureRegion_Result__missed_region_indices(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<int32_t> *>(untyped_member);
  return &member[index];
}

void * get_function__CureRegion_Result__missed_region_indices(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<int32_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__CureRegion_Result__missed_region_indices(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const int32_t *>(
    get_const_function__CureRegion_Result__missed_region_indices(untyped_member, index));
  auto & value = *reinterpret_cast<int32_t *>(untyped_value);
  value = item;
}

void assign_function__CureRegion_Result__missed_region_indices(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<int32_t *>(
    get_function__CureRegion_Result__missed_region_indices(untyped_member, index));
  const auto & value = *reinterpret_cast<const int32_t *>(untyped_value);
  item = value;
}

void resize_function__CureRegion_Result__missed_region_indices(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<int32_t> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_Result_message_member_array[4] = {
  {
    "base",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::msg::ActionResultBase>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Result, base),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "total_exposure_s",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Result, total_exposure_s),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "missed_region_indices",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Result, missed_region_indices),  // bytes offset in struct
    nullptr,  // default value
    size_function__CureRegion_Result__missed_region_indices,  // size() function pointer
    get_const_function__CureRegion_Result__missed_region_indices,  // get_const(index) function pointer
    get_function__CureRegion_Result__missed_region_indices,  // get(index) function pointer
    fetch_function__CureRegion_Result__missed_region_indices,  // fetch(index, &value) function pointer
    assign_function__CureRegion_Result__missed_region_indices,  // assign(index, value) function pointer
    resize_function__CureRegion_Result__missed_region_indices  // resize(index) function pointer
  },
  {
    "lamp_off_confirmed",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Result, lamp_off_confirmed),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_Result_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_Result",  // message name
  4,  // number of fields
  sizeof(nail_msgs::action::CureRegion_Result),
  false,  // has_any_key_member_
  CureRegion_Result_message_member_array,  // message members
  CureRegion_Result_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_Result_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_Result_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_Result_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_Result__get_type_hash,
  &nail_msgs__action__CureRegion_Result__get_type_description,
  &nail_msgs__action__CureRegion_Result__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_Result>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_Result_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_Result)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_Result_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_Feedback_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_Feedback(_init);
}

void CureRegion_Feedback_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_Feedback *>(message_memory);
  typed_message->~CureRegion_Feedback();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_Feedback_message_member_array[4] = {
  {
    "percent",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Feedback, percent),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "current_region",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Feedback, current_region),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "elapsed_exposure_s",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Feedback, elapsed_exposure_s),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "lamp_on",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_Feedback, lamp_on),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_Feedback_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_Feedback",  // message name
  4,  // number of fields
  sizeof(nail_msgs::action::CureRegion_Feedback),
  false,  // has_any_key_member_
  CureRegion_Feedback_message_member_array,  // message members
  CureRegion_Feedback_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_Feedback_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_Feedback_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_Feedback_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_Feedback__get_type_hash,
  &nail_msgs__action__CureRegion_Feedback__get_type_description,
  &nail_msgs__action__CureRegion_Feedback__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_Feedback>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_Feedback_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_Feedback)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_Feedback_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_SendGoal_Request_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_SendGoal_Request(_init);
}

void CureRegion_SendGoal_Request_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_SendGoal_Request *>(message_memory);
  typed_message->~CureRegion_SendGoal_Request();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_SendGoal_Request_message_member_array[2] = {
  {
    "goal_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<unique_identifier_msgs::msg::UUID>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_SendGoal_Request, goal_id),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_Goal>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_SendGoal_Request, goal),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_SendGoal_Request_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_SendGoal_Request",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::CureRegion_SendGoal_Request),
  false,  // has_any_key_member_
  CureRegion_SendGoal_Request_message_member_array,  // message members
  CureRegion_SendGoal_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_SendGoal_Request_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_SendGoal_Request_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_SendGoal_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_SendGoal_Request__get_type_hash,
  &nail_msgs__action__CureRegion_SendGoal_Request__get_type_description,
  &nail_msgs__action__CureRegion_SendGoal_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Request>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_SendGoal_Request_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_SendGoal_Request)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_SendGoal_Request_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_SendGoal_Response_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_SendGoal_Response(_init);
}

void CureRegion_SendGoal_Response_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_SendGoal_Response *>(message_memory);
  typed_message->~CureRegion_SendGoal_Response();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_SendGoal_Response_message_member_array[2] = {
  {
    "accepted",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_SendGoal_Response, accepted),  // bytes offset in struct
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
    offsetof(nail_msgs::action::CureRegion_SendGoal_Response, stamp),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_SendGoal_Response_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_SendGoal_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::CureRegion_SendGoal_Response),
  false,  // has_any_key_member_
  CureRegion_SendGoal_Response_message_member_array,  // message members
  CureRegion_SendGoal_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_SendGoal_Response_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_SendGoal_Response_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_SendGoal_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_SendGoal_Response__get_type_hash,
  &nail_msgs__action__CureRegion_SendGoal_Response__get_type_description,
  &nail_msgs__action__CureRegion_SendGoal_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Response>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_SendGoal_Response_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_SendGoal_Response)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_SendGoal_Response_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_SendGoal_Event_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_SendGoal_Event(_init);
}

void CureRegion_SendGoal_Event_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_SendGoal_Event *>(message_memory);
  typed_message->~CureRegion_SendGoal_Event();
}

size_t size_function__CureRegion_SendGoal_Event__request(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_SendGoal_Request> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CureRegion_SendGoal_Event__request(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_SendGoal_Request> *>(untyped_member);
  return &member[index];
}

void * get_function__CureRegion_SendGoal_Event__request(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::CureRegion_SendGoal_Request> *>(untyped_member);
  return &member[index];
}

void fetch_function__CureRegion_SendGoal_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::CureRegion_SendGoal_Request *>(
    get_const_function__CureRegion_SendGoal_Event__request(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::CureRegion_SendGoal_Request *>(untyped_value);
  value = item;
}

void assign_function__CureRegion_SendGoal_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::CureRegion_SendGoal_Request *>(
    get_function__CureRegion_SendGoal_Event__request(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::CureRegion_SendGoal_Request *>(untyped_value);
  item = value;
}

void resize_function__CureRegion_SendGoal_Event__request(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::CureRegion_SendGoal_Request> *>(untyped_member);
  member->resize(size);
}

size_t size_function__CureRegion_SendGoal_Event__response(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_SendGoal_Response> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CureRegion_SendGoal_Event__response(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_SendGoal_Response> *>(untyped_member);
  return &member[index];
}

void * get_function__CureRegion_SendGoal_Event__response(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::CureRegion_SendGoal_Response> *>(untyped_member);
  return &member[index];
}

void fetch_function__CureRegion_SendGoal_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::CureRegion_SendGoal_Response *>(
    get_const_function__CureRegion_SendGoal_Event__response(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::CureRegion_SendGoal_Response *>(untyped_value);
  value = item;
}

void assign_function__CureRegion_SendGoal_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::CureRegion_SendGoal_Response *>(
    get_function__CureRegion_SendGoal_Event__response(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::CureRegion_SendGoal_Response *>(untyped_value);
  item = value;
}

void resize_function__CureRegion_SendGoal_Event__response(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::CureRegion_SendGoal_Response> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_SendGoal_Event_message_member_array[3] = {
  {
    "info",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<service_msgs::msg::ServiceEventInfo>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_SendGoal_Event, info),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Request>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_SendGoal_Event, request),  // bytes offset in struct
    nullptr,  // default value
    size_function__CureRegion_SendGoal_Event__request,  // size() function pointer
    get_const_function__CureRegion_SendGoal_Event__request,  // get_const(index) function pointer
    get_function__CureRegion_SendGoal_Event__request,  // get(index) function pointer
    fetch_function__CureRegion_SendGoal_Event__request,  // fetch(index, &value) function pointer
    assign_function__CureRegion_SendGoal_Event__request,  // assign(index, value) function pointer
    resize_function__CureRegion_SendGoal_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Response>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_SendGoal_Event, response),  // bytes offset in struct
    nullptr,  // default value
    size_function__CureRegion_SendGoal_Event__response,  // size() function pointer
    get_const_function__CureRegion_SendGoal_Event__response,  // get_const(index) function pointer
    get_function__CureRegion_SendGoal_Event__response,  // get(index) function pointer
    fetch_function__CureRegion_SendGoal_Event__response,  // fetch(index, &value) function pointer
    assign_function__CureRegion_SendGoal_Event__response,  // assign(index, value) function pointer
    resize_function__CureRegion_SendGoal_Event__response  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_SendGoal_Event_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_SendGoal_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs::action::CureRegion_SendGoal_Event),
  false,  // has_any_key_member_
  CureRegion_SendGoal_Event_message_member_array,  // message members
  CureRegion_SendGoal_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_SendGoal_Event_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_SendGoal_Event_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_SendGoal_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_SendGoal_Event__get_type_hash,
  &nail_msgs__action__CureRegion_SendGoal_Event__get_type_description,
  &nail_msgs__action__CureRegion_SendGoal_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Event>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_SendGoal_Event_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_SendGoal_Event)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_SendGoal_Event_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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
static ::rosidl_typesupport_introspection_cpp::ServiceMembers CureRegion_SendGoal_service_members = {
  "nail_msgs::action",  // service namespace
  "CureRegion_SendGoal",  // service name
  // the following fields are initialized below on first access
  // see get_service_type_support_handle<nail_msgs::action::CureRegion_SendGoal>()
  nullptr,  // request message
  nullptr,  // response message
  nullptr,  // event message
};

static const rosidl_service_type_support_t CureRegion_SendGoal_service_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_SendGoal_service_members,
  get_service_typesupport_handle_function,
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Request>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Response>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_SendGoal_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<nail_msgs::action::CureRegion_SendGoal>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<nail_msgs::action::CureRegion_SendGoal>,
  &nail_msgs__action__CureRegion_SendGoal__get_type_hash,
  &nail_msgs__action__CureRegion_SendGoal__get_type_description,
  &nail_msgs__action__CureRegion_SendGoal__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<nail_msgs::action::CureRegion_SendGoal>()
{
  // get a handle to the value to be returned
  auto service_type_support =
    &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_SendGoal_service_type_support_handle;
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
        ::nail_msgs::action::CureRegion_SendGoal_Request
      >()->data
      );
    // initialize the response_members_ with the static function from the external library
    service_members->response_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::CureRegion_SendGoal_Response
      >()->data
      );
    // initialize the event_members_ with the static function from the external library
    service_members->event_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::CureRegion_SendGoal_Event
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
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_SendGoal)() {
  return ::rosidl_typesupport_introspection_cpp::get_service_type_support_handle<nail_msgs::action::CureRegion_SendGoal>();
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_GetResult_Request_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_GetResult_Request(_init);
}

void CureRegion_GetResult_Request_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_GetResult_Request *>(message_memory);
  typed_message->~CureRegion_GetResult_Request();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_GetResult_Request_message_member_array[1] = {
  {
    "goal_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<unique_identifier_msgs::msg::UUID>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_GetResult_Request, goal_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_GetResult_Request_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_GetResult_Request",  // message name
  1,  // number of fields
  sizeof(nail_msgs::action::CureRegion_GetResult_Request),
  false,  // has_any_key_member_
  CureRegion_GetResult_Request_message_member_array,  // message members
  CureRegion_GetResult_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_GetResult_Request_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_GetResult_Request_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_GetResult_Request_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_GetResult_Request__get_type_hash,
  &nail_msgs__action__CureRegion_GetResult_Request__get_type_description,
  &nail_msgs__action__CureRegion_GetResult_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Request>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_GetResult_Request_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_GetResult_Request)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_GetResult_Request_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_GetResult_Response_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_GetResult_Response(_init);
}

void CureRegion_GetResult_Response_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_GetResult_Response *>(message_memory);
  typed_message->~CureRegion_GetResult_Response();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_GetResult_Response_message_member_array[2] = {
  {
    "status",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_GetResult_Response, status),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_Result>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_GetResult_Response, result),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_GetResult_Response_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_GetResult_Response",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::CureRegion_GetResult_Response),
  false,  // has_any_key_member_
  CureRegion_GetResult_Response_message_member_array,  // message members
  CureRegion_GetResult_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_GetResult_Response_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_GetResult_Response_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_GetResult_Response_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_GetResult_Response__get_type_hash,
  &nail_msgs__action__CureRegion_GetResult_Response__get_type_description,
  &nail_msgs__action__CureRegion_GetResult_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Response>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_GetResult_Response_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_GetResult_Response)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_GetResult_Response_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_GetResult_Event_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_GetResult_Event(_init);
}

void CureRegion_GetResult_Event_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_GetResult_Event *>(message_memory);
  typed_message->~CureRegion_GetResult_Event();
}

size_t size_function__CureRegion_GetResult_Event__request(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_GetResult_Request> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CureRegion_GetResult_Event__request(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_GetResult_Request> *>(untyped_member);
  return &member[index];
}

void * get_function__CureRegion_GetResult_Event__request(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::CureRegion_GetResult_Request> *>(untyped_member);
  return &member[index];
}

void fetch_function__CureRegion_GetResult_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::CureRegion_GetResult_Request *>(
    get_const_function__CureRegion_GetResult_Event__request(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::CureRegion_GetResult_Request *>(untyped_value);
  value = item;
}

void assign_function__CureRegion_GetResult_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::CureRegion_GetResult_Request *>(
    get_function__CureRegion_GetResult_Event__request(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::CureRegion_GetResult_Request *>(untyped_value);
  item = value;
}

void resize_function__CureRegion_GetResult_Event__request(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::CureRegion_GetResult_Request> *>(untyped_member);
  member->resize(size);
}

size_t size_function__CureRegion_GetResult_Event__response(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_GetResult_Response> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CureRegion_GetResult_Event__response(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<nail_msgs::action::CureRegion_GetResult_Response> *>(untyped_member);
  return &member[index];
}

void * get_function__CureRegion_GetResult_Event__response(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<nail_msgs::action::CureRegion_GetResult_Response> *>(untyped_member);
  return &member[index];
}

void fetch_function__CureRegion_GetResult_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const nail_msgs::action::CureRegion_GetResult_Response *>(
    get_const_function__CureRegion_GetResult_Event__response(untyped_member, index));
  auto & value = *reinterpret_cast<nail_msgs::action::CureRegion_GetResult_Response *>(untyped_value);
  value = item;
}

void assign_function__CureRegion_GetResult_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<nail_msgs::action::CureRegion_GetResult_Response *>(
    get_function__CureRegion_GetResult_Event__response(untyped_member, index));
  const auto & value = *reinterpret_cast<const nail_msgs::action::CureRegion_GetResult_Response *>(untyped_value);
  item = value;
}

void resize_function__CureRegion_GetResult_Event__response(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<nail_msgs::action::CureRegion_GetResult_Response> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_GetResult_Event_message_member_array[3] = {
  {
    "info",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<service_msgs::msg::ServiceEventInfo>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_GetResult_Event, info),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Request>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_GetResult_Event, request),  // bytes offset in struct
    nullptr,  // default value
    size_function__CureRegion_GetResult_Event__request,  // size() function pointer
    get_const_function__CureRegion_GetResult_Event__request,  // get_const(index) function pointer
    get_function__CureRegion_GetResult_Event__request,  // get(index) function pointer
    fetch_function__CureRegion_GetResult_Event__request,  // fetch(index, &value) function pointer
    assign_function__CureRegion_GetResult_Event__request,  // assign(index, value) function pointer
    resize_function__CureRegion_GetResult_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Response>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_GetResult_Event, response),  // bytes offset in struct
    nullptr,  // default value
    size_function__CureRegion_GetResult_Event__response,  // size() function pointer
    get_const_function__CureRegion_GetResult_Event__response,  // get_const(index) function pointer
    get_function__CureRegion_GetResult_Event__response,  // get(index) function pointer
    fetch_function__CureRegion_GetResult_Event__response,  // fetch(index, &value) function pointer
    assign_function__CureRegion_GetResult_Event__response,  // assign(index, value) function pointer
    resize_function__CureRegion_GetResult_Event__response  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_GetResult_Event_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_GetResult_Event",  // message name
  3,  // number of fields
  sizeof(nail_msgs::action::CureRegion_GetResult_Event),
  false,  // has_any_key_member_
  CureRegion_GetResult_Event_message_member_array,  // message members
  CureRegion_GetResult_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_GetResult_Event_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_GetResult_Event_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_GetResult_Event_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_GetResult_Event__get_type_hash,
  &nail_msgs__action__CureRegion_GetResult_Event__get_type_description,
  &nail_msgs__action__CureRegion_GetResult_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Event>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_GetResult_Event_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_GetResult_Event)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_GetResult_Event_message_type_support_handle;
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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
static ::rosidl_typesupport_introspection_cpp::ServiceMembers CureRegion_GetResult_service_members = {
  "nail_msgs::action",  // service namespace
  "CureRegion_GetResult",  // service name
  // the following fields are initialized below on first access
  // see get_service_type_support_handle<nail_msgs::action::CureRegion_GetResult>()
  nullptr,  // request message
  nullptr,  // response message
  nullptr,  // event message
};

static const rosidl_service_type_support_t CureRegion_GetResult_service_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_GetResult_service_members,
  get_service_typesupport_handle_function,
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Request>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Response>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_GetResult_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<nail_msgs::action::CureRegion_GetResult>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<nail_msgs::action::CureRegion_GetResult>,
  &nail_msgs__action__CureRegion_GetResult__get_type_hash,
  &nail_msgs__action__CureRegion_GetResult__get_type_description,
  &nail_msgs__action__CureRegion_GetResult__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<nail_msgs::action::CureRegion_GetResult>()
{
  // get a handle to the value to be returned
  auto service_type_support =
    &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_GetResult_service_type_support_handle;
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
        ::nail_msgs::action::CureRegion_GetResult_Request
      >()->data
      );
    // initialize the response_members_ with the static function from the external library
    service_members->response_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::CureRegion_GetResult_Response
      >()->data
      );
    // initialize the event_members_ with the static function from the external library
    service_members->event_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::nail_msgs::action::CureRegion_GetResult_Event
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
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_GetResult)() {
  return ::rosidl_typesupport_introspection_cpp::get_service_type_support_handle<nail_msgs::action::CureRegion_GetResult>();
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
// #include "nail_msgs/action/detail/cure_region__functions.h"
// already included above
// #include "nail_msgs/action/detail/cure_region__struct.hpp"
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

void CureRegion_FeedbackMessage_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::action::CureRegion_FeedbackMessage(_init);
}

void CureRegion_FeedbackMessage_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::action::CureRegion_FeedbackMessage *>(message_memory);
  typed_message->~CureRegion_FeedbackMessage();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CureRegion_FeedbackMessage_message_member_array[2] = {
  {
    "goal_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<unique_identifier_msgs::msg::UUID>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_FeedbackMessage, goal_id),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<nail_msgs::action::CureRegion_Feedback>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::action::CureRegion_FeedbackMessage, feedback),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CureRegion_FeedbackMessage_message_members = {
  "nail_msgs::action",  // message namespace
  "CureRegion_FeedbackMessage",  // message name
  2,  // number of fields
  sizeof(nail_msgs::action::CureRegion_FeedbackMessage),
  false,  // has_any_key_member_
  CureRegion_FeedbackMessage_message_member_array,  // message members
  CureRegion_FeedbackMessage_init_function,  // function to initialize message memory (memory has to be allocated)
  CureRegion_FeedbackMessage_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CureRegion_FeedbackMessage_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CureRegion_FeedbackMessage_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__action__CureRegion_FeedbackMessage__get_type_hash,
  &nail_msgs__action__CureRegion_FeedbackMessage__get_type_description,
  &nail_msgs__action__CureRegion_FeedbackMessage__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace action

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::action::CureRegion_FeedbackMessage>()
{
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_FeedbackMessage_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, action, CureRegion_FeedbackMessage)() {
  return &::nail_msgs::action::rosidl_typesupport_introspection_cpp::CureRegion_FeedbackMessage_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
