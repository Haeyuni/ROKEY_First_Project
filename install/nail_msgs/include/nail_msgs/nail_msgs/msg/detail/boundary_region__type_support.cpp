// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from nail_msgs:msg/BoundaryRegion.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "nail_msgs/msg/detail/boundary_region__functions.h"
#include "nail_msgs/msg/detail/boundary_region__struct.hpp"
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

void BoundaryRegion_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) nail_msgs::msg::BoundaryRegion(_init);
}

void BoundaryRegion_fini_function(void * message_memory)
{
  auto typed_message = static_cast<nail_msgs::msg::BoundaryRegion *>(message_memory);
  typed_message->~BoundaryRegion();
}

size_t size_function__BoundaryRegion__allowed_polygon(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return member->size();
}

const void * get_const_function__BoundaryRegion__allowed_polygon(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void * get_function__BoundaryRegion__allowed_polygon(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void fetch_function__BoundaryRegion__allowed_polygon(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const geometry_msgs::msg::Point *>(
    get_const_function__BoundaryRegion__allowed_polygon(untyped_member, index));
  auto & value = *reinterpret_cast<geometry_msgs::msg::Point *>(untyped_value);
  value = item;
}

void assign_function__BoundaryRegion__allowed_polygon(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<geometry_msgs::msg::Point *>(
    get_function__BoundaryRegion__allowed_polygon(untyped_member, index));
  const auto & value = *reinterpret_cast<const geometry_msgs::msg::Point *>(untyped_value);
  item = value;
}

void resize_function__BoundaryRegion__allowed_polygon(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  member->resize(size);
}

size_t size_function__BoundaryRegion__forbidden_polygon(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return member->size();
}

const void * get_const_function__BoundaryRegion__forbidden_polygon(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void * get_function__BoundaryRegion__forbidden_polygon(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void fetch_function__BoundaryRegion__forbidden_polygon(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const geometry_msgs::msg::Point *>(
    get_const_function__BoundaryRegion__forbidden_polygon(untyped_member, index));
  auto & value = *reinterpret_cast<geometry_msgs::msg::Point *>(untyped_value);
  value = item;
}

void assign_function__BoundaryRegion__forbidden_polygon(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<geometry_msgs::msg::Point *>(
    get_function__BoundaryRegion__forbidden_polygon(untyped_member, index));
  const auto & value = *reinterpret_cast<const geometry_msgs::msg::Point *>(untyped_value);
  item = value;
}

void resize_function__BoundaryRegion__forbidden_polygon(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  member->resize(size);
}

size_t size_function__BoundaryRegion__coat_polygon(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return member->size();
}

const void * get_const_function__BoundaryRegion__coat_polygon(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void * get_function__BoundaryRegion__coat_polygon(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  return &member[index];
}

void fetch_function__BoundaryRegion__coat_polygon(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const geometry_msgs::msg::Point *>(
    get_const_function__BoundaryRegion__coat_polygon(untyped_member, index));
  auto & value = *reinterpret_cast<geometry_msgs::msg::Point *>(untyped_value);
  value = item;
}

void assign_function__BoundaryRegion__coat_polygon(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<geometry_msgs::msg::Point *>(
    get_function__BoundaryRegion__coat_polygon(untyped_member, index));
  const auto & value = *reinterpret_cast<const geometry_msgs::msg::Point *>(untyped_value);
  item = value;
}

void resize_function__BoundaryRegion__coat_polygon(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<geometry_msgs::msg::Point> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember BoundaryRegion_message_member_array[9] = {
  {
    "session_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::BoundaryRegion, session_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
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
    offsetof(nail_msgs::msg::BoundaryRegion, frame_id),  // bytes offset in struct
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
    offsetof(nail_msgs::msg::BoundaryRegion, target_index),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "allowed_polygon",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Point>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::BoundaryRegion, allowed_polygon),  // bytes offset in struct
    nullptr,  // default value
    size_function__BoundaryRegion__allowed_polygon,  // size() function pointer
    get_const_function__BoundaryRegion__allowed_polygon,  // get_const(index) function pointer
    get_function__BoundaryRegion__allowed_polygon,  // get(index) function pointer
    fetch_function__BoundaryRegion__allowed_polygon,  // fetch(index, &value) function pointer
    assign_function__BoundaryRegion__allowed_polygon,  // assign(index, value) function pointer
    resize_function__BoundaryRegion__allowed_polygon  // resize(index) function pointer
  },
  {
    "forbidden_polygon",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Point>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::BoundaryRegion, forbidden_polygon),  // bytes offset in struct
    nullptr,  // default value
    size_function__BoundaryRegion__forbidden_polygon,  // size() function pointer
    get_const_function__BoundaryRegion__forbidden_polygon,  // get_const(index) function pointer
    get_function__BoundaryRegion__forbidden_polygon,  // get(index) function pointer
    fetch_function__BoundaryRegion__forbidden_polygon,  // fetch(index, &value) function pointer
    assign_function__BoundaryRegion__forbidden_polygon,  // assign(index, value) function pointer
    resize_function__BoundaryRegion__forbidden_polygon  // resize(index) function pointer
  },
  {
    "coat_polygon",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Point>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::BoundaryRegion, coat_polygon),  // bytes offset in struct
    nullptr,  // default value
    size_function__BoundaryRegion__coat_polygon,  // size() function pointer
    get_const_function__BoundaryRegion__coat_polygon,  // get_const(index) function pointer
    get_function__BoundaryRegion__coat_polygon,  // get(index) function pointer
    fetch_function__BoundaryRegion__coat_polygon,  // fetch(index, &value) function pointer
    assign_function__BoundaryRegion__coat_polygon,  // assign(index, value) function pointer
    resize_function__BoundaryRegion__coat_polygon  // resize(index) function pointer
  },
  {
    "boundary_offset_mm",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::BoundaryRegion, boundary_offset_mm),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "repeat_deviation_mm",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::BoundaryRegion, repeat_deviation_mm),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "reliable",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs::msg::BoundaryRegion, reliable),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers BoundaryRegion_message_members = {
  "nail_msgs::msg",  // message namespace
  "BoundaryRegion",  // message name
  9,  // number of fields
  sizeof(nail_msgs::msg::BoundaryRegion),
  false,  // has_any_key_member_
  BoundaryRegion_message_member_array,  // message members
  BoundaryRegion_init_function,  // function to initialize message memory (memory has to be allocated)
  BoundaryRegion_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t BoundaryRegion_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &BoundaryRegion_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__msg__BoundaryRegion__get_type_hash,
  &nail_msgs__msg__BoundaryRegion__get_type_description,
  &nail_msgs__msg__BoundaryRegion__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace nail_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<nail_msgs::msg::BoundaryRegion>()
{
  return &::nail_msgs::msg::rosidl_typesupport_introspection_cpp::BoundaryRegion_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, nail_msgs, msg, BoundaryRegion)() {
  return &::nail_msgs::msg::rosidl_typesupport_introspection_cpp::BoundaryRegion_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
