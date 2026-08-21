// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/StiffnessMap.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/stiffness_map__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `session_id`
// Member `frame_id`
#include "rosidl_runtime_c/string_functions.h"
// Member `points`
#include "nail_msgs/msg/detail/stiffness_point__functions.h"
// Member `created_at`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
nail_msgs__msg__StiffnessMap__init(nail_msgs__msg__StiffnessMap * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    nail_msgs__msg__StiffnessMap__fini(msg);
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__init(&msg->session_id)) {
    nail_msgs__msg__StiffnessMap__fini(msg);
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__init(&msg->frame_id)) {
    nail_msgs__msg__StiffnessMap__fini(msg);
    return false;
  }
  // target_index
  // points
  if (!nail_msgs__msg__StiffnessPoint__Sequence__init(&msg->points, 0)) {
    nail_msgs__msg__StiffnessMap__fini(msg);
    return false;
  }
  // grid_pitch_mm
  // hard_min_n_per_mm
  // soft_max_n_per_mm
  // created_at
  if (!builtin_interfaces__msg__Time__init(&msg->created_at)) {
    nail_msgs__msg__StiffnessMap__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__msg__StiffnessMap__fini(nail_msgs__msg__StiffnessMap * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // session_id
  rosidl_runtime_c__String__fini(&msg->session_id);
  // frame_id
  rosidl_runtime_c__String__fini(&msg->frame_id);
  // target_index
  // points
  nail_msgs__msg__StiffnessPoint__Sequence__fini(&msg->points);
  // grid_pitch_mm
  // hard_min_n_per_mm
  // soft_max_n_per_mm
  // created_at
  builtin_interfaces__msg__Time__fini(&msg->created_at);
}

bool
nail_msgs__msg__StiffnessMap__are_equal(const nail_msgs__msg__StiffnessMap * lhs, const nail_msgs__msg__StiffnessMap * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->session_id), &(rhs->session_id)))
  {
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->frame_id), &(rhs->frame_id)))
  {
    return false;
  }
  // target_index
  if (lhs->target_index != rhs->target_index) {
    return false;
  }
  // points
  if (!nail_msgs__msg__StiffnessPoint__Sequence__are_equal(
      &(lhs->points), &(rhs->points)))
  {
    return false;
  }
  // grid_pitch_mm
  if (lhs->grid_pitch_mm != rhs->grid_pitch_mm) {
    return false;
  }
  // hard_min_n_per_mm
  if (lhs->hard_min_n_per_mm != rhs->hard_min_n_per_mm) {
    return false;
  }
  // soft_max_n_per_mm
  if (lhs->soft_max_n_per_mm != rhs->soft_max_n_per_mm) {
    return false;
  }
  // created_at
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->created_at), &(rhs->created_at)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__StiffnessMap__copy(
  const nail_msgs__msg__StiffnessMap * input,
  nail_msgs__msg__StiffnessMap * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__copy(
      &(input->session_id), &(output->session_id)))
  {
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__copy(
      &(input->frame_id), &(output->frame_id)))
  {
    return false;
  }
  // target_index
  output->target_index = input->target_index;
  // points
  if (!nail_msgs__msg__StiffnessPoint__Sequence__copy(
      &(input->points), &(output->points)))
  {
    return false;
  }
  // grid_pitch_mm
  output->grid_pitch_mm = input->grid_pitch_mm;
  // hard_min_n_per_mm
  output->hard_min_n_per_mm = input->hard_min_n_per_mm;
  // soft_max_n_per_mm
  output->soft_max_n_per_mm = input->soft_max_n_per_mm;
  // created_at
  if (!builtin_interfaces__msg__Time__copy(
      &(input->created_at), &(output->created_at)))
  {
    return false;
  }
  return true;
}

nail_msgs__msg__StiffnessMap *
nail_msgs__msg__StiffnessMap__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__StiffnessMap * msg = (nail_msgs__msg__StiffnessMap *)allocator.allocate(sizeof(nail_msgs__msg__StiffnessMap), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__StiffnessMap));
  bool success = nail_msgs__msg__StiffnessMap__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__StiffnessMap__destroy(nail_msgs__msg__StiffnessMap * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__StiffnessMap__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__StiffnessMap__Sequence__init(nail_msgs__msg__StiffnessMap__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__StiffnessMap * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__StiffnessMap)) {
      return false;
    }
    data = (nail_msgs__msg__StiffnessMap *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__StiffnessMap), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__StiffnessMap__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__StiffnessMap__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
nail_msgs__msg__StiffnessMap__Sequence__fini(nail_msgs__msg__StiffnessMap__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      nail_msgs__msg__StiffnessMap__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

nail_msgs__msg__StiffnessMap__Sequence *
nail_msgs__msg__StiffnessMap__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__StiffnessMap__Sequence * array = (nail_msgs__msg__StiffnessMap__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__StiffnessMap__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__StiffnessMap__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__StiffnessMap__Sequence__destroy(nail_msgs__msg__StiffnessMap__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__StiffnessMap__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__StiffnessMap__Sequence__are_equal(const nail_msgs__msg__StiffnessMap__Sequence * lhs, const nail_msgs__msg__StiffnessMap__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__StiffnessMap__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__StiffnessMap__Sequence__copy(
  const nail_msgs__msg__StiffnessMap__Sequence * input,
  nail_msgs__msg__StiffnessMap__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__StiffnessMap)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__StiffnessMap);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__StiffnessMap * data =
      (nail_msgs__msg__StiffnessMap *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__StiffnessMap__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__StiffnessMap__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__StiffnessMap__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
