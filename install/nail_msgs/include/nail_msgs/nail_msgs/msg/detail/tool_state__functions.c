// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/ToolState.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/tool_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `current_tool`
// Member `active_tcp`
#include "rosidl_runtime_c/string_functions.h"

bool
nail_msgs__msg__ToolState__init(nail_msgs__msg__ToolState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    nail_msgs__msg__ToolState__fini(msg);
    return false;
  }
  // current_tool
  if (!rosidl_runtime_c__String__init(&msg->current_tool)) {
    nail_msgs__msg__ToolState__fini(msg);
    return false;
  }
  // active_tcp
  if (!rosidl_runtime_c__String__init(&msg->active_tcp)) {
    nail_msgs__msg__ToolState__fini(msg);
    return false;
  }
  // grip_width_mm
  // expected_width_mm
  // grip_verified
  return true;
}

void
nail_msgs__msg__ToolState__fini(nail_msgs__msg__ToolState * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // current_tool
  rosidl_runtime_c__String__fini(&msg->current_tool);
  // active_tcp
  rosidl_runtime_c__String__fini(&msg->active_tcp);
  // grip_width_mm
  // expected_width_mm
  // grip_verified
}

bool
nail_msgs__msg__ToolState__are_equal(const nail_msgs__msg__ToolState * lhs, const nail_msgs__msg__ToolState * rhs)
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
  // current_tool
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->current_tool), &(rhs->current_tool)))
  {
    return false;
  }
  // active_tcp
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->active_tcp), &(rhs->active_tcp)))
  {
    return false;
  }
  // grip_width_mm
  if (lhs->grip_width_mm != rhs->grip_width_mm) {
    return false;
  }
  // expected_width_mm
  if (lhs->expected_width_mm != rhs->expected_width_mm) {
    return false;
  }
  // grip_verified
  if (lhs->grip_verified != rhs->grip_verified) {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__ToolState__copy(
  const nail_msgs__msg__ToolState * input,
  nail_msgs__msg__ToolState * output)
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
  // current_tool
  if (!rosidl_runtime_c__String__copy(
      &(input->current_tool), &(output->current_tool)))
  {
    return false;
  }
  // active_tcp
  if (!rosidl_runtime_c__String__copy(
      &(input->active_tcp), &(output->active_tcp)))
  {
    return false;
  }
  // grip_width_mm
  output->grip_width_mm = input->grip_width_mm;
  // expected_width_mm
  output->expected_width_mm = input->expected_width_mm;
  // grip_verified
  output->grip_verified = input->grip_verified;
  return true;
}

nail_msgs__msg__ToolState *
nail_msgs__msg__ToolState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ToolState * msg = (nail_msgs__msg__ToolState *)allocator.allocate(sizeof(nail_msgs__msg__ToolState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__ToolState));
  bool success = nail_msgs__msg__ToolState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__ToolState__destroy(nail_msgs__msg__ToolState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__ToolState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__ToolState__Sequence__init(nail_msgs__msg__ToolState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ToolState * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__ToolState)) {
      return false;
    }
    data = (nail_msgs__msg__ToolState *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__ToolState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__ToolState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__ToolState__fini(&data[i - 1]);
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
nail_msgs__msg__ToolState__Sequence__fini(nail_msgs__msg__ToolState__Sequence * array)
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
      nail_msgs__msg__ToolState__fini(&array->data[i]);
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

nail_msgs__msg__ToolState__Sequence *
nail_msgs__msg__ToolState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ToolState__Sequence * array = (nail_msgs__msg__ToolState__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__ToolState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__ToolState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__ToolState__Sequence__destroy(nail_msgs__msg__ToolState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__ToolState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__ToolState__Sequence__are_equal(const nail_msgs__msg__ToolState__Sequence * lhs, const nail_msgs__msg__ToolState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__ToolState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__ToolState__Sequence__copy(
  const nail_msgs__msg__ToolState__Sequence * input,
  nail_msgs__msg__ToolState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__ToolState)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__ToolState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__ToolState * data =
      (nail_msgs__msg__ToolState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__ToolState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__ToolState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__ToolState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
