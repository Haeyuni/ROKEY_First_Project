// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from nail_msgs:msg/BoundaryRegion.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "nail_msgs/msg/detail/boundary_region__rosidl_typesupport_introspection_c.h"
#include "nail_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "nail_msgs/msg/detail/boundary_region__functions.h"
#include "nail_msgs/msg/detail/boundary_region__struct.h"


// Include directives for member types
// Member `session_id`
// Member `frame_id`
#include "rosidl_runtime_c/string_functions.h"
// Member `allowed_polygon`
// Member `forbidden_polygon`
// Member `coat_polygon`
#include "geometry_msgs/msg/point.h"
// Member `allowed_polygon`
// Member `forbidden_polygon`
// Member `coat_polygon`
#include "geometry_msgs/msg/detail/point__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  nail_msgs__msg__BoundaryRegion__init(message_memory);
}

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_fini_function(void * message_memory)
{
  nail_msgs__msg__BoundaryRegion__fini(message_memory);
}

size_t nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__size_function__BoundaryRegion__allowed_polygon(
  const void * untyped_member)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__allowed_polygon(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__allowed_polygon(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__fetch_function__BoundaryRegion__allowed_polygon(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Point * item =
    ((const geometry_msgs__msg__Point *)
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__allowed_polygon(untyped_member, index));
  geometry_msgs__msg__Point * value =
    (geometry_msgs__msg__Point *)(untyped_value);
  *value = *item;
}

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__assign_function__BoundaryRegion__allowed_polygon(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Point * item =
    ((geometry_msgs__msg__Point *)
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__allowed_polygon(untyped_member, index));
  const geometry_msgs__msg__Point * value =
    (const geometry_msgs__msg__Point *)(untyped_value);
  *item = *value;
}

bool nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__resize_function__BoundaryRegion__allowed_polygon(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  geometry_msgs__msg__Point__Sequence__fini(member);
  return geometry_msgs__msg__Point__Sequence__init(member, size);
}

size_t nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__size_function__BoundaryRegion__forbidden_polygon(
  const void * untyped_member)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__forbidden_polygon(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__forbidden_polygon(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__fetch_function__BoundaryRegion__forbidden_polygon(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Point * item =
    ((const geometry_msgs__msg__Point *)
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__forbidden_polygon(untyped_member, index));
  geometry_msgs__msg__Point * value =
    (geometry_msgs__msg__Point *)(untyped_value);
  *value = *item;
}

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__assign_function__BoundaryRegion__forbidden_polygon(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Point * item =
    ((geometry_msgs__msg__Point *)
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__forbidden_polygon(untyped_member, index));
  const geometry_msgs__msg__Point * value =
    (const geometry_msgs__msg__Point *)(untyped_value);
  *item = *value;
}

bool nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__resize_function__BoundaryRegion__forbidden_polygon(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  geometry_msgs__msg__Point__Sequence__fini(member);
  return geometry_msgs__msg__Point__Sequence__init(member, size);
}

size_t nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__size_function__BoundaryRegion__coat_polygon(
  const void * untyped_member)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return member->size;
}

const void * nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__coat_polygon(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Point__Sequence * member =
    (const geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void * nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__coat_polygon(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  return &member->data[index];
}

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__fetch_function__BoundaryRegion__coat_polygon(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Point * item =
    ((const geometry_msgs__msg__Point *)
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__coat_polygon(untyped_member, index));
  geometry_msgs__msg__Point * value =
    (geometry_msgs__msg__Point *)(untyped_value);
  *value = *item;
}

void nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__assign_function__BoundaryRegion__coat_polygon(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Point * item =
    ((geometry_msgs__msg__Point *)
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__coat_polygon(untyped_member, index));
  const geometry_msgs__msg__Point * value =
    (const geometry_msgs__msg__Point *)(untyped_value);
  *item = *value;
}

bool nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__resize_function__BoundaryRegion__coat_polygon(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Point__Sequence * member =
    (geometry_msgs__msg__Point__Sequence *)(untyped_member);
  geometry_msgs__msg__Point__Sequence__fini(member);
  return geometry_msgs__msg__Point__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_member_array[9] = {
  {
    "session_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, session_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "frame_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, frame_id),  // bytes offset in struct
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
    offsetof(nail_msgs__msg__BoundaryRegion, target_index),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "allowed_polygon",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, allowed_polygon),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__size_function__BoundaryRegion__allowed_polygon,  // size() function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__allowed_polygon,  // get_const(index) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__allowed_polygon,  // get(index) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__fetch_function__BoundaryRegion__allowed_polygon,  // fetch(index, &value) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__assign_function__BoundaryRegion__allowed_polygon,  // assign(index, value) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__resize_function__BoundaryRegion__allowed_polygon  // resize(index) function pointer
  },
  {
    "forbidden_polygon",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, forbidden_polygon),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__size_function__BoundaryRegion__forbidden_polygon,  // size() function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__forbidden_polygon,  // get_const(index) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__forbidden_polygon,  // get(index) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__fetch_function__BoundaryRegion__forbidden_polygon,  // fetch(index, &value) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__assign_function__BoundaryRegion__forbidden_polygon,  // assign(index, value) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__resize_function__BoundaryRegion__forbidden_polygon  // resize(index) function pointer
  },
  {
    "coat_polygon",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, coat_polygon),  // bytes offset in struct
    NULL,  // default value
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__size_function__BoundaryRegion__coat_polygon,  // size() function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_const_function__BoundaryRegion__coat_polygon,  // get_const(index) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__get_function__BoundaryRegion__coat_polygon,  // get(index) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__fetch_function__BoundaryRegion__coat_polygon,  // fetch(index, &value) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__assign_function__BoundaryRegion__coat_polygon,  // assign(index, value) function pointer
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__resize_function__BoundaryRegion__coat_polygon  // resize(index) function pointer
  },
  {
    "boundary_offset_mm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, boundary_offset_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "repeat_deviation_mm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, repeat_deviation_mm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "reliable",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(nail_msgs__msg__BoundaryRegion, reliable),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_members = {
  "nail_msgs__msg",  // message namespace
  "BoundaryRegion",  // message name
  9,  // number of fields
  sizeof(nail_msgs__msg__BoundaryRegion),
  false,  // has_any_key_member_
  nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_member_array,  // message members
  nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_init_function,  // function to initialize message memory (memory has to be allocated)
  nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_type_support_handle = {
  0,
  &nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_members,
  get_message_typesupport_handle_function,
  &nail_msgs__msg__BoundaryRegion__get_type_hash,
  &nail_msgs__msg__BoundaryRegion__get_type_description,
  &nail_msgs__msg__BoundaryRegion__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_nail_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, nail_msgs, msg, BoundaryRegion)() {
  nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_member_array[3].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_member_array[4].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_member_array[5].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  if (!nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_type_support_handle.typesupport_identifier) {
    nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &nail_msgs__msg__BoundaryRegion__rosidl_typesupport_introspection_c__BoundaryRegion_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
