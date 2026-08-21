// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:msg/Verdict.idl
// generated code does not contain a copyright notice
#include "nail_msgs/msg/detail/verdict__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `session_id`
// Member `result`
#include "rosidl_runtime_c/string_functions.h"
// Member `waveform`
#include "nail_msgs/msg/detail/force_sample__functions.h"
// Member `measured_at`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
nail_msgs__msg__Verdict__init(nail_msgs__msg__Verdict * msg)
{
  if (!msg) {
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__init(&msg->session_id)) {
    nail_msgs__msg__Verdict__fini(msg);
    return false;
  }
  // target_index
  // layer_index
  // probe_index
  // result
  if (!rosidl_runtime_c__String__init(&msg->result)) {
    nail_msgs__msg__Verdict__fini(msg);
    return false;
  }
  // tensile_n
  // stiffness_n_per_mm
  // error_code
  // waveform
  if (!nail_msgs__msg__ForceSample__Sequence__init(&msg->waveform, 0)) {
    nail_msgs__msg__Verdict__fini(msg);
    return false;
  }
  // measured_at
  if (!builtin_interfaces__msg__Time__init(&msg->measured_at)) {
    nail_msgs__msg__Verdict__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__msg__Verdict__fini(nail_msgs__msg__Verdict * msg)
{
  if (!msg) {
    return;
  }
  // session_id
  rosidl_runtime_c__String__fini(&msg->session_id);
  // target_index
  // layer_index
  // probe_index
  // result
  rosidl_runtime_c__String__fini(&msg->result);
  // tensile_n
  // stiffness_n_per_mm
  // error_code
  // waveform
  nail_msgs__msg__ForceSample__Sequence__fini(&msg->waveform);
  // measured_at
  builtin_interfaces__msg__Time__fini(&msg->measured_at);
}

bool
nail_msgs__msg__Verdict__are_equal(const nail_msgs__msg__Verdict * lhs, const nail_msgs__msg__Verdict * rhs)
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
  // target_index
  if (lhs->target_index != rhs->target_index) {
    return false;
  }
  // layer_index
  if (lhs->layer_index != rhs->layer_index) {
    return false;
  }
  // probe_index
  if (lhs->probe_index != rhs->probe_index) {
    return false;
  }
  // result
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->result), &(rhs->result)))
  {
    return false;
  }
  // tensile_n
  if (lhs->tensile_n != rhs->tensile_n) {
    return false;
  }
  // stiffness_n_per_mm
  if (lhs->stiffness_n_per_mm != rhs->stiffness_n_per_mm) {
    return false;
  }
  // error_code
  if (lhs->error_code != rhs->error_code) {
    return false;
  }
  // waveform
  if (!nail_msgs__msg__ForceSample__Sequence__are_equal(
      &(lhs->waveform), &(rhs->waveform)))
  {
    return false;
  }
  // measured_at
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->measured_at), &(rhs->measured_at)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__msg__Verdict__copy(
  const nail_msgs__msg__Verdict * input,
  nail_msgs__msg__Verdict * output)
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
  // target_index
  output->target_index = input->target_index;
  // layer_index
  output->layer_index = input->layer_index;
  // probe_index
  output->probe_index = input->probe_index;
  // result
  if (!rosidl_runtime_c__String__copy(
      &(input->result), &(output->result)))
  {
    return false;
  }
  // tensile_n
  output->tensile_n = input->tensile_n;
  // stiffness_n_per_mm
  output->stiffness_n_per_mm = input->stiffness_n_per_mm;
  // error_code
  output->error_code = input->error_code;
  // waveform
  if (!nail_msgs__msg__ForceSample__Sequence__copy(
      &(input->waveform), &(output->waveform)))
  {
    return false;
  }
  // measured_at
  if (!builtin_interfaces__msg__Time__copy(
      &(input->measured_at), &(output->measured_at)))
  {
    return false;
  }
  return true;
}

nail_msgs__msg__Verdict *
nail_msgs__msg__Verdict__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__Verdict * msg = (nail_msgs__msg__Verdict *)allocator.allocate(sizeof(nail_msgs__msg__Verdict), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__msg__Verdict));
  bool success = nail_msgs__msg__Verdict__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__msg__Verdict__destroy(nail_msgs__msg__Verdict * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__msg__Verdict__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__msg__Verdict__Sequence__init(nail_msgs__msg__Verdict__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__Verdict * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__msg__Verdict)) {
      return false;
    }
    data = (nail_msgs__msg__Verdict *)allocator.zero_allocate(size, sizeof(nail_msgs__msg__Verdict), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__msg__Verdict__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__msg__Verdict__fini(&data[i - 1]);
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
nail_msgs__msg__Verdict__Sequence__fini(nail_msgs__msg__Verdict__Sequence * array)
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
      nail_msgs__msg__Verdict__fini(&array->data[i]);
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

nail_msgs__msg__Verdict__Sequence *
nail_msgs__msg__Verdict__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__msg__Verdict__Sequence * array = (nail_msgs__msg__Verdict__Sequence *)allocator.allocate(sizeof(nail_msgs__msg__Verdict__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__msg__Verdict__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__msg__Verdict__Sequence__destroy(nail_msgs__msg__Verdict__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__msg__Verdict__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__msg__Verdict__Sequence__are_equal(const nail_msgs__msg__Verdict__Sequence * lhs, const nail_msgs__msg__Verdict__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__msg__Verdict__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__msg__Verdict__Sequence__copy(
  const nail_msgs__msg__Verdict__Sequence * input,
  nail_msgs__msg__Verdict__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__msg__Verdict)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__msg__Verdict);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__msg__Verdict * data =
      (nail_msgs__msg__Verdict *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__msg__Verdict__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__msg__Verdict__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__msg__Verdict__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
