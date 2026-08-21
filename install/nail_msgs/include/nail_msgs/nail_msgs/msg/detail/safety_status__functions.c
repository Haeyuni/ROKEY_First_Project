// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/SafetyStatus.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/safety_status__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `reason`
#include "rosidl_runtime_c/string_functions.h"

bool
nail_msgs__msg__SafetyStatus__init(nail_msgs__msg__SafetyStatus * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    nail_msgs__msg__SafetyStatus__fini(msg);
    return false;
  }
  // status
  // estop_released
  // target_seated
  // tool_grip_ok
  // map_valid
  // uv_interlock_ok
  // dust_extraction_on
  // robot_alarm_clear
  // reason
  if (!rosidl_runtime_c__String__init(&msg->reason)) {
    nail_msgs__msg__SafetyStatus__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__msg__SafetyStatus__fini(nail_msgs__msg__SafetyStatus * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // status
  // estop_released
  // target_seated
  // tool_grip_ok
  // map_valid
  // uv_interlock_ok
  // dust_extraction_on
  // robot_alarm_clear
  // reason
  rosidl_runtime_c__String__fini(&msg->reason);
}

bool
nail_msgs__msg__SafetyStatus__are_equal(const nail_msgs__msg__SafetyStatus * lhs, const nail_msgs__msg__SafetyStatus * rhs)
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
  // status
  if (lhs->status != rhs->status) {
    return false;
  }
  // estop_released
  if (lhs->estop_released != rhs->estop_released) {
    return false;
  }
  // target_seated
  if (lhs->target_seated != rhs->target_seated) {
    return false;
  }
  // tool_grip_ok
  if (lhs->tool_grip_ok != rhs->tool_grip_ok) {
    return false;
  }
  // map_valid
  if (lhs->map_valid != rhs->map_valid) {
    return false;
  }
  // uv_interlock_ok
  if (lhs->uv_interlock_ok != rhs->uv_interlock_ok) {
    return false;
  }
  // dust_extraction_on
  if (lhs->dust_extraction_on != rhs->dust_extraction_on) {
    return false;
  }
  // robot_alarm_clear
  if (lhs->robot_alarm_clear != rhs->robot_alarm_clear) {
    return false;
  }
  // reason
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->reason), &(rhs->reason)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__SafetyStatus__copy(
  const nail_msgs__msg__SafetyStatus * input,
  nail_msgs__msg__SafetyStatus * output)
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
  // status
  output->status = input->status;
  // estop_released
  output->estop_released = input->estop_released;
  // target_seated
  output->target_seated = input->target_seated;
  // tool_grip_ok
  output->tool_grip_ok = input->tool_grip_ok;
  // map_valid
  output->map_valid = input->map_valid;
  // uv_interlock_ok
  output->uv_interlock_ok = input->uv_interlock_ok;
  // dust_extraction_on
  output->dust_extraction_on = input->dust_extraction_on;
  // robot_alarm_clear
  output->robot_alarm_clear = input->robot_alarm_clear;
  // reason
  if (!rosidl_runtime_c__String__copy(
      &(input->reason), &(output->reason)))
  {
    return false;
  }
  return true;
}

nail_msgs__msg__SafetyStatus *
nail_msgs__msg__SafetyStatus__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__SafetyStatus * msg = (nail_msgs__msg__SafetyStatus *)allocator.allocate(sizeof(nail_msgs__msg__SafetyStatus), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__SafetyStatus));
  bool success = nail_msgs__msg__SafetyStatus__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__SafetyStatus__destroy(nail_msgs__msg__SafetyStatus * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__SafetyStatus__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__SafetyStatus__Sequence__init(nail_msgs__msg__SafetyStatus__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__SafetyStatus * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__SafetyStatus)) {
      return false;
    }
    data = (nail_msgs__msg__SafetyStatus *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__SafetyStatus), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__SafetyStatus__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__SafetyStatus__fini(&data[i - 1]);
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
nail_msgs__msg__SafetyStatus__Sequence__fini(nail_msgs__msg__SafetyStatus__Sequence * array)
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
      nail_msgs__msg__SafetyStatus__fini(&array->data[i]);
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

nail_msgs__msg__SafetyStatus__Sequence *
nail_msgs__msg__SafetyStatus__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__SafetyStatus__Sequence * array = (nail_msgs__msg__SafetyStatus__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__SafetyStatus__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__SafetyStatus__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__SafetyStatus__Sequence__destroy(nail_msgs__msg__SafetyStatus__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__SafetyStatus__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__SafetyStatus__Sequence__are_equal(const nail_msgs__msg__SafetyStatus__Sequence * lhs, const nail_msgs__msg__SafetyStatus__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__SafetyStatus__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__SafetyStatus__Sequence__copy(
  const nail_msgs__msg__SafetyStatus__Sequence * input,
  nail_msgs__msg__SafetyStatus__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__SafetyStatus)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__SafetyStatus);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__SafetyStatus * data =
      (nail_msgs__msg__SafetyStatus *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__SafetyStatus__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__SafetyStatus__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__SafetyStatus__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
