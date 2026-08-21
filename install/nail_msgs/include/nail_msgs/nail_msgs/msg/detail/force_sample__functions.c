// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/ForceSample.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/force_sample__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
nail_msgs__msg__ForceSample__init(nail_msgs__msg__ForceSample * msg)
{
  if (!msg) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    nail_msgs__msg__ForceSample__fini(msg);
    return false;
  }
  // fx
  // fy
  // fz
  // tx
  // ty
  // tz
  return true;
}

void
nail_msgs__msg__ForceSample__fini(nail_msgs__msg__ForceSample * msg)
{
  if (!msg) {
    return;
  }
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
  // fx
  // fy
  // fz
  // tx
  // ty
  // tz
}

bool
nail_msgs__msg__ForceSample__are_equal(const nail_msgs__msg__ForceSample * lhs, const nail_msgs__msg__ForceSample * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->stamp), &(rhs->stamp)))
  {
    return false;
  }
  // fx
  if (lhs->fx != rhs->fx) {
    return false;
  }
  // fy
  if (lhs->fy != rhs->fy) {
    return false;
  }
  // fz
  if (lhs->fz != rhs->fz) {
    return false;
  }
  // tx
  if (lhs->tx != rhs->tx) {
    return false;
  }
  // ty
  if (lhs->ty != rhs->ty) {
    return false;
  }
  // tz
  if (lhs->tz != rhs->tz) {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__ForceSample__copy(
  const nail_msgs__msg__ForceSample * input,
  nail_msgs__msg__ForceSample * output)
{
  if (!input || !output) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->stamp), &(output->stamp)))
  {
    return false;
  }
  // fx
  output->fx = input->fx;
  // fy
  output->fy = input->fy;
  // fz
  output->fz = input->fz;
  // tx
  output->tx = input->tx;
  // ty
  output->ty = input->ty;
  // tz
  output->tz = input->tz;
  return true;
}

nail_msgs__msg__ForceSample *
nail_msgs__msg__ForceSample__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ForceSample * msg = (nail_msgs__msg__ForceSample *)allocator.allocate(sizeof(nail_msgs__msg__ForceSample), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__ForceSample));
  bool success = nail_msgs__msg__ForceSample__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__ForceSample__destroy(nail_msgs__msg__ForceSample * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__ForceSample__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__ForceSample__Sequence__init(nail_msgs__msg__ForceSample__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ForceSample * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__ForceSample)) {
      return false;
    }
    data = (nail_msgs__msg__ForceSample *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__ForceSample), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__ForceSample__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__ForceSample__fini(&data[i - 1]);
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
nail_msgs__msg__ForceSample__Sequence__fini(nail_msgs__msg__ForceSample__Sequence * array)
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
      nail_msgs__msg__ForceSample__fini(&array->data[i]);
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

nail_msgs__msg__ForceSample__Sequence *
nail_msgs__msg__ForceSample__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__ForceSample__Sequence * array = (nail_msgs__msg__ForceSample__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__ForceSample__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__ForceSample__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__ForceSample__Sequence__destroy(nail_msgs__msg__ForceSample__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__ForceSample__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__ForceSample__Sequence__are_equal(const nail_msgs__msg__ForceSample__Sequence * lhs, const nail_msgs__msg__ForceSample__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__ForceSample__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__ForceSample__Sequence__copy(
  const nail_msgs__msg__ForceSample__Sequence * input,
  nail_msgs__msg__ForceSample__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__ForceSample)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__ForceSample);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__ForceSample * data =
      (nail_msgs__msg__ForceSample *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__ForceSample__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__ForceSample__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__ForceSample__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
