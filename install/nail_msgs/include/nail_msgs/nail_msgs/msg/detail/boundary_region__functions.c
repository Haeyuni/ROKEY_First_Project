// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/BoundaryRegion.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/boundary_region__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `session_id`
// Member `frame_id`
#include "rosidl_runtime_c/string_functions.h"
// Member `allowed_polygon`
// Member `forbidden_polygon`
// Member `coat_polygon`
#include "geometry_msgs/msg/detail/point__functions.h"

bool
nail_msgs__msg__BoundaryRegion__init(nail_msgs__msg__BoundaryRegion * msg)
{
  if (!msg) {
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__init(&msg->session_id)) {
    nail_msgs__msg__BoundaryRegion__fini(msg);
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__init(&msg->frame_id)) {
    nail_msgs__msg__BoundaryRegion__fini(msg);
    return false;
  }
  // target_index
  // allowed_polygon
  if (!geometry_msgs__msg__Point__Sequence__init(&msg->allowed_polygon, 0)) {
    nail_msgs__msg__BoundaryRegion__fini(msg);
    return false;
  }
  // forbidden_polygon
  if (!geometry_msgs__msg__Point__Sequence__init(&msg->forbidden_polygon, 0)) {
    nail_msgs__msg__BoundaryRegion__fini(msg);
    return false;
  }
  // coat_polygon
  if (!geometry_msgs__msg__Point__Sequence__init(&msg->coat_polygon, 0)) {
    nail_msgs__msg__BoundaryRegion__fini(msg);
    return false;
  }
  // boundary_offset_mm
  // repeat_deviation_mm
  // reliable
  return true;
}

void
nail_msgs__msg__BoundaryRegion__fini(nail_msgs__msg__BoundaryRegion * msg)
{
  if (!msg) {
    return;
  }
  // session_id
  rosidl_runtime_c__String__fini(&msg->session_id);
  // frame_id
  rosidl_runtime_c__String__fini(&msg->frame_id);
  // target_index
  // allowed_polygon
  geometry_msgs__msg__Point__Sequence__fini(&msg->allowed_polygon);
  // forbidden_polygon
  geometry_msgs__msg__Point__Sequence__fini(&msg->forbidden_polygon);
  // coat_polygon
  geometry_msgs__msg__Point__Sequence__fini(&msg->coat_polygon);
  // boundary_offset_mm
  // repeat_deviation_mm
  // reliable
}

bool
nail_msgs__msg__BoundaryRegion__are_equal(const nail_msgs__msg__BoundaryRegion * lhs, const nail_msgs__msg__BoundaryRegion * rhs)
{
  if (!lhs || !rhs) {
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
  // allowed_polygon
  if (!geometry_msgs__msg__Point__Sequence__are_equal(
      &(lhs->allowed_polygon), &(rhs->allowed_polygon)))
  {
    return false;
  }
  // forbidden_polygon
  if (!geometry_msgs__msg__Point__Sequence__are_equal(
      &(lhs->forbidden_polygon), &(rhs->forbidden_polygon)))
  {
    return false;
  }
  // coat_polygon
  if (!geometry_msgs__msg__Point__Sequence__are_equal(
      &(lhs->coat_polygon), &(rhs->coat_polygon)))
  {
    return false;
  }
  // boundary_offset_mm
  if (lhs->boundary_offset_mm != rhs->boundary_offset_mm) {
    return false;
  }
  // repeat_deviation_mm
  if (lhs->repeat_deviation_mm != rhs->repeat_deviation_mm) {
    return false;
  }
  // reliable
  if (lhs->reliable != rhs->reliable) {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__BoundaryRegion__copy(
  const nail_msgs__msg__BoundaryRegion * input,
  nail_msgs__msg__BoundaryRegion * output)
{
  if (!input || !output) {
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
  // allowed_polygon
  if (!geometry_msgs__msg__Point__Sequence__copy(
      &(input->allowed_polygon), &(output->allowed_polygon)))
  {
    return false;
  }
  // forbidden_polygon
  if (!geometry_msgs__msg__Point__Sequence__copy(
      &(input->forbidden_polygon), &(output->forbidden_polygon)))
  {
    return false;
  }
  // coat_polygon
  if (!geometry_msgs__msg__Point__Sequence__copy(
      &(input->coat_polygon), &(output->coat_polygon)))
  {
    return false;
  }
  // boundary_offset_mm
  output->boundary_offset_mm = input->boundary_offset_mm;
  // repeat_deviation_mm
  output->repeat_deviation_mm = input->repeat_deviation_mm;
  // reliable
  output->reliable = input->reliable;
  return true;
}

nail_msgs__msg__BoundaryRegion *
nail_msgs__msg__BoundaryRegion__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__BoundaryRegion * msg = (nail_msgs__msg__BoundaryRegion *)allocator.allocate(sizeof(nail_msgs__msg__BoundaryRegion), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__BoundaryRegion));
  bool success = nail_msgs__msg__BoundaryRegion__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__BoundaryRegion__destroy(nail_msgs__msg__BoundaryRegion * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__BoundaryRegion__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__BoundaryRegion__Sequence__init(nail_msgs__msg__BoundaryRegion__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__BoundaryRegion * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__BoundaryRegion)) {
      return false;
    }
    data = (nail_msgs__msg__BoundaryRegion *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__BoundaryRegion), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__BoundaryRegion__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__BoundaryRegion__fini(&data[i - 1]);
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
nail_msgs__msg__BoundaryRegion__Sequence__fini(nail_msgs__msg__BoundaryRegion__Sequence * array)
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
      nail_msgs__msg__BoundaryRegion__fini(&array->data[i]);
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

nail_msgs__msg__BoundaryRegion__Sequence *
nail_msgs__msg__BoundaryRegion__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__BoundaryRegion__Sequence * array = (nail_msgs__msg__BoundaryRegion__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__BoundaryRegion__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__BoundaryRegion__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__BoundaryRegion__Sequence__destroy(nail_msgs__msg__BoundaryRegion__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__BoundaryRegion__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__BoundaryRegion__Sequence__are_equal(const nail_msgs__msg__BoundaryRegion__Sequence * lhs, const nail_msgs__msg__BoundaryRegion__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__BoundaryRegion__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__BoundaryRegion__Sequence__copy(
  const nail_msgs__msg__BoundaryRegion__Sequence * input,
  nail_msgs__msg__BoundaryRegion__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__BoundaryRegion)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__BoundaryRegion);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__BoundaryRegion * data =
      (nail_msgs__msg__BoundaryRegion *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__BoundaryRegion__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__BoundaryRegion__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__BoundaryRegion__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
