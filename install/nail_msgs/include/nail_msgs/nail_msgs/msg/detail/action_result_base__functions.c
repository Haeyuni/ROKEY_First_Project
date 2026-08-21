// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/ActionResultBase.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/action_result_base__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `error`
#include "nail_msgs/msg/detail/error_code__functions.h"
// Member `final_pose`
#include "geometry_msgs/msg/detail/pose__functions.h"
// Member `completed_at`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
nail_msgs__msg__ActionResultBase__init(nail_msgs__msg__ActionResultBase * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // error
  if (!nail_msgs__msg__ErrorCode__init(&msg->error)) {
    nail_msgs__msg__ActionResultBase__fini(msg);
    return false;
  }
  // final_fz
  // final_pose
  if (!geometry_msgs__msg__Pose__init(&msg->final_pose)) {
    nail_msgs__msg__ActionResultBase__fini(msg);
    return false;
  }
  // completed_at
  if (!builtin_interfaces__msg__Time__init(&msg->completed_at)) {
    nail_msgs__msg__ActionResultBase__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__msg__ActionResultBase__fini(nail_msgs__msg__ActionResultBase * msg)
{
  if (!msg) {
    return;
  }
  // success
  // error
  nail_msgs__msg__ErrorCode__fini(&msg->error);
  // final_fz
  // final_pose
  geometry_msgs__msg__Pose__fini(&msg->final_pose);
  // completed_at
  builtin_interfaces__msg__Time__fini(&msg->completed_at);
}

bool
nail_msgs__msg__ActionResultBase__are_equal(const nail_msgs__msg__ActionResultBase * lhs, const nail_msgs__msg__ActionResultBase * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // error
  if (!nail_msgs__msg__ErrorCode__are_equal(
      &(lhs->error), &(rhs->error)))
  {
    return false;
  }
  // final_fz
  if (lhs->final_fz != rhs->final_fz) {
    return false;
  }
  // final_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->final_pose), &(rhs->final_pose)))
  {
    return false;
  }
  // completed_at
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->completed_at), &(rhs->completed_at)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__ActionResultBase__copy(
  const nail_msgs__msg__ActionResultBase * input,
  nail_msgs__msg__ActionResultBase * output)
{
  if (!input || !output) {
    return false;
  }
  // success
  output->success = input->success;
  // error
  if (!nail_msgs__msg__ErrorCode__copy(
      &(input->error), &(output->error)))
  {
    return false;
  }
  // final_fz
  output->final_fz = input->final_fz;
  // final_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->final_pose), &(output->final_pose)))
  {
    return false;
  }
  // completed_at
  if (!builtin_interfaces__msg__Time__copy(
      &(input->completed_at), &(output->completed_at)))
  {
    return false;
  }
  return true;
}

nail_msgs__msg__ActionResultBase *
nail_msgs__msg__ActionResultBase__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ActionResultBase * msg = (nail_msgs__msg__ActionResultBase *)allocator.allocate(sizeof(nail_msgs__msg__ActionResultBase), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__ActionResultBase));
  bool success = nail_msgs__msg__ActionResultBase__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__ActionResultBase__destroy(nail_msgs__msg__ActionResultBase * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__ActionResultBase__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__ActionResultBase__Sequence__init(nail_msgs__msg__ActionResultBase__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ActionResultBase * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__ActionResultBase)) {
      return false;
    }
    data = (nail_msgs__msg__ActionResultBase *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__ActionResultBase), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__ActionResultBase__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__ActionResultBase__fini(&data[i - 1]);
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
nail_msgs__msg__ActionResultBase__Sequence__fini(nail_msgs__msg__ActionResultBase__Sequence * array)
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
      nail_msgs__msg__ActionResultBase__fini(&array->data[i]);
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

nail_msgs__msg__ActionResultBase__Sequence *
nail_msgs__msg__ActionResultBase__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ActionResultBase__Sequence * array = (nail_msgs__msg__ActionResultBase__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__ActionResultBase__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__ActionResultBase__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__ActionResultBase__Sequence__destroy(nail_msgs__msg__ActionResultBase__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__ActionResultBase__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__ActionResultBase__Sequence__are_equal(const nail_msgs__msg__ActionResultBase__Sequence * lhs, const nail_msgs__msg__ActionResultBase__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__ActionResultBase__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__ActionResultBase__Sequence__copy(
  const nail_msgs__msg__ActionResultBase__Sequence * input,
  nail_msgs__msg__ActionResultBase__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__ActionResultBase)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__ActionResultBase);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__ActionResultBase * data =
      (nail_msgs__msg__ActionResultBase *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__ActionResultBase__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__ActionResultBase__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__ActionResultBase__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
