// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/StiffnessPoint.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/stiffness_point__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `position`
#include "geometry_msgs/msg/detail/point__functions.h"

bool
nail_msgs__msg__StiffnessPoint__init(nail_msgs__msg__StiffnessPoint * msg)
{
  if (!msg) {
    return false;
  }
  // position
  if (!geometry_msgs__msg__Point__init(&msg->position)) {
    nail_msgs__msg__StiffnessPoint__fini(msg);
    return false;
  }
  // stiffness_n_per_mm
  // peak_tensile_n
  // hysteresis_ratio
  // lateral_force_n
  // valid
  return true;
}

void
nail_msgs__msg__StiffnessPoint__fini(nail_msgs__msg__StiffnessPoint * msg)
{
  if (!msg) {
    return;
  }
  // position
  geometry_msgs__msg__Point__fini(&msg->position);
  // stiffness_n_per_mm
  // peak_tensile_n
  // hysteresis_ratio
  // lateral_force_n
  // valid
}

bool
nail_msgs__msg__StiffnessPoint__are_equal(const nail_msgs__msg__StiffnessPoint * lhs, const nail_msgs__msg__StiffnessPoint * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // position
  if (!geometry_msgs__msg__Point__are_equal(
      &(lhs->position), &(rhs->position)))
  {
    return false;
  }
  // stiffness_n_per_mm
  if (lhs->stiffness_n_per_mm != rhs->stiffness_n_per_mm) {
    return false;
  }
  // peak_tensile_n
  if (lhs->peak_tensile_n != rhs->peak_tensile_n) {
    return false;
  }
  // hysteresis_ratio
  if (lhs->hysteresis_ratio != rhs->hysteresis_ratio) {
    return false;
  }
  // lateral_force_n
  if (lhs->lateral_force_n != rhs->lateral_force_n) {
    return false;
  }
  // valid
  if (lhs->valid != rhs->valid) {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__StiffnessPoint__copy(
  const nail_msgs__msg__StiffnessPoint * input,
  nail_msgs__msg__StiffnessPoint * output)
{
  if (!input || !output) {
    return false;
  }
  // position
  if (!geometry_msgs__msg__Point__copy(
      &(input->position), &(output->position)))
  {
    return false;
  }
  // stiffness_n_per_mm
  output->stiffness_n_per_mm = input->stiffness_n_per_mm;
  // peak_tensile_n
  output->peak_tensile_n = input->peak_tensile_n;
  // hysteresis_ratio
  output->hysteresis_ratio = input->hysteresis_ratio;
  // lateral_force_n
  output->lateral_force_n = input->lateral_force_n;
  // valid
  output->valid = input->valid;
  return true;
}

nail_msgs__msg__StiffnessPoint *
nail_msgs__msg__StiffnessPoint__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__StiffnessPoint * msg = (nail_msgs__msg__StiffnessPoint *)allocator.allocate(sizeof(nail_msgs__msg__StiffnessPoint), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__StiffnessPoint));
  bool success = nail_msgs__msg__StiffnessPoint__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__StiffnessPoint__destroy(nail_msgs__msg__StiffnessPoint * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__StiffnessPoint__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__StiffnessPoint__Sequence__init(nail_msgs__msg__StiffnessPoint__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__StiffnessPoint * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__StiffnessPoint)) {
      return false;
    }
    data = (nail_msgs__msg__StiffnessPoint *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__StiffnessPoint), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__StiffnessPoint__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__StiffnessPoint__fini(&data[i - 1]);
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
nail_msgs__msg__StiffnessPoint__Sequence__fini(nail_msgs__msg__StiffnessPoint__Sequence * array)
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
      nail_msgs__msg__StiffnessPoint__fini(&array->data[i]);
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

nail_msgs__msg__StiffnessPoint__Sequence *
nail_msgs__msg__StiffnessPoint__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__StiffnessPoint__Sequence * array = (nail_msgs__msg__StiffnessPoint__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__StiffnessPoint__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__StiffnessPoint__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__StiffnessPoint__Sequence__destroy(nail_msgs__msg__StiffnessPoint__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__StiffnessPoint__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__StiffnessPoint__Sequence__are_equal(const nail_msgs__msg__StiffnessPoint__Sequence * lhs, const nail_msgs__msg__StiffnessPoint__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__StiffnessPoint__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__StiffnessPoint__Sequence__copy(
  const nail_msgs__msg__StiffnessPoint__Sequence * input,
  nail_msgs__msg__StiffnessPoint__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__StiffnessPoint)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__StiffnessPoint);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__StiffnessPoint * data =
      (nail_msgs__msg__StiffnessPoint *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__StiffnessPoint__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__StiffnessPoint__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__StiffnessPoint__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
