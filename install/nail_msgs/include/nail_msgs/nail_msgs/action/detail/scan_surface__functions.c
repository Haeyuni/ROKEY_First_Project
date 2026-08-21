// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:action/ScanSurface.idl
// generated code does not contain a copyright notice
#include "nail_msgs/action/detail/scan_surface__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `scan_area`
#include "geometry_msgs/msg/detail/point__functions.h"
// Member `frame_id`
// Member `session_id`
#include "rosidl_runtime_c/string_functions.h"

bool
nail_msgs__action__ScanSurface_Goal__init(nail_msgs__action__ScanSurface_Goal * msg)
{
  if (!msg) {
    return false;
  }
  // scan_area
  if (!geometry_msgs__msg__Point__Sequence__init(&msg->scan_area, 0)) {
    nail_msgs__action__ScanSurface_Goal__fini(msg);
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__init(&msg->frame_id)) {
    nail_msgs__action__ScanSurface_Goal__fini(msg);
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__init(&msg->session_id)) {
    nail_msgs__action__ScanSurface_Goal__fini(msg);
    return false;
  }
  // target_index
  // grid_pitch_mm
  // verify_reference_point
  return true;
}

void
nail_msgs__action__ScanSurface_Goal__fini(nail_msgs__action__ScanSurface_Goal * msg)
{
  if (!msg) {
    return;
  }
  // scan_area
  geometry_msgs__msg__Point__Sequence__fini(&msg->scan_area);
  // frame_id
  rosidl_runtime_c__String__fini(&msg->frame_id);
  // session_id
  rosidl_runtime_c__String__fini(&msg->session_id);
  // target_index
  // grid_pitch_mm
  // verify_reference_point
}

bool
nail_msgs__action__ScanSurface_Goal__are_equal(const nail_msgs__action__ScanSurface_Goal * lhs, const nail_msgs__action__ScanSurface_Goal * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // scan_area
  if (!geometry_msgs__msg__Point__Sequence__are_equal(
      &(lhs->scan_area), &(rhs->scan_area)))
  {
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->frame_id), &(rhs->frame_id)))
  {
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
  // grid_pitch_mm
  if (lhs->grid_pitch_mm != rhs->grid_pitch_mm) {
    return false;
  }
  // verify_reference_point
  if (lhs->verify_reference_point != rhs->verify_reference_point) {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_Goal__copy(
  const nail_msgs__action__ScanSurface_Goal * input,
  nail_msgs__action__ScanSurface_Goal * output)
{
  if (!input || !output) {
    return false;
  }
  // scan_area
  if (!geometry_msgs__msg__Point__Sequence__copy(
      &(input->scan_area), &(output->scan_area)))
  {
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__copy(
      &(input->frame_id), &(output->frame_id)))
  {
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
  // grid_pitch_mm
  output->grid_pitch_mm = input->grid_pitch_mm;
  // verify_reference_point
  output->verify_reference_point = input->verify_reference_point;
  return true;
}

nail_msgs__action__ScanSurface_Goal *
nail_msgs__action__ScanSurface_Goal__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Goal * msg = (nail_msgs__action__ScanSurface_Goal *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_Goal), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_Goal));
  bool success = nail_msgs__action__ScanSurface_Goal__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_Goal__destroy(nail_msgs__action__ScanSurface_Goal * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_Goal__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_Goal__Sequence__init(nail_msgs__action__ScanSurface_Goal__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Goal * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_Goal)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_Goal *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_Goal), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_Goal__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_Goal__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_Goal__Sequence__fini(nail_msgs__action__ScanSurface_Goal__Sequence * array)
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
      nail_msgs__action__ScanSurface_Goal__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_Goal__Sequence *
nail_msgs__action__ScanSurface_Goal__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Goal__Sequence * array = (nail_msgs__action__ScanSurface_Goal__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_Goal__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_Goal__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_Goal__Sequence__destroy(nail_msgs__action__ScanSurface_Goal__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_Goal__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_Goal__Sequence__are_equal(const nail_msgs__action__ScanSurface_Goal__Sequence * lhs, const nail_msgs__action__ScanSurface_Goal__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_Goal__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_Goal__Sequence__copy(
  const nail_msgs__action__ScanSurface_Goal__Sequence * input,
  nail_msgs__action__ScanSurface_Goal__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_Goal)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_Goal);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_Goal * data =
      (nail_msgs__action__ScanSurface_Goal *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_Goal__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_Goal__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_Goal__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `base`
#include "nail_msgs/msg/detail/action_result_base__functions.h"
// Member `map`
#include "nail_msgs/msg/detail/stiffness_map__functions.h"

bool
nail_msgs__action__ScanSurface_Result__init(nail_msgs__action__ScanSurface_Result * msg)
{
  if (!msg) {
    return false;
  }
  // base
  if (!nail_msgs__msg__ActionResultBase__init(&msg->base)) {
    nail_msgs__action__ScanSurface_Result__fini(msg);
    return false;
  }
  // map
  if (!nail_msgs__msg__StiffnessMap__init(&msg->map)) {
    nail_msgs__action__ScanSurface_Result__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_Result__fini(nail_msgs__action__ScanSurface_Result * msg)
{
  if (!msg) {
    return;
  }
  // base
  nail_msgs__msg__ActionResultBase__fini(&msg->base);
  // map
  nail_msgs__msg__StiffnessMap__fini(&msg->map);
}

bool
nail_msgs__action__ScanSurface_Result__are_equal(const nail_msgs__action__ScanSurface_Result * lhs, const nail_msgs__action__ScanSurface_Result * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // base
  if (!nail_msgs__msg__ActionResultBase__are_equal(
      &(lhs->base), &(rhs->base)))
  {
    return false;
  }
  // map
  if (!nail_msgs__msg__StiffnessMap__are_equal(
      &(lhs->map), &(rhs->map)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_Result__copy(
  const nail_msgs__action__ScanSurface_Result * input,
  nail_msgs__action__ScanSurface_Result * output)
{
  if (!input || !output) {
    return false;
  }
  // base
  if (!nail_msgs__msg__ActionResultBase__copy(
      &(input->base), &(output->base)))
  {
    return false;
  }
  // map
  if (!nail_msgs__msg__StiffnessMap__copy(
      &(input->map), &(output->map)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_Result *
nail_msgs__action__ScanSurface_Result__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Result * msg = (nail_msgs__action__ScanSurface_Result *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_Result), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_Result));
  bool success = nail_msgs__action__ScanSurface_Result__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_Result__destroy(nail_msgs__action__ScanSurface_Result * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_Result__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_Result__Sequence__init(nail_msgs__action__ScanSurface_Result__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Result * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_Result)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_Result *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_Result), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_Result__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_Result__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_Result__Sequence__fini(nail_msgs__action__ScanSurface_Result__Sequence * array)
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
      nail_msgs__action__ScanSurface_Result__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_Result__Sequence *
nail_msgs__action__ScanSurface_Result__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Result__Sequence * array = (nail_msgs__action__ScanSurface_Result__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_Result__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_Result__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_Result__Sequence__destroy(nail_msgs__action__ScanSurface_Result__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_Result__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_Result__Sequence__are_equal(const nail_msgs__action__ScanSurface_Result__Sequence * lhs, const nail_msgs__action__ScanSurface_Result__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_Result__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_Result__Sequence__copy(
  const nail_msgs__action__ScanSurface_Result__Sequence * input,
  nail_msgs__action__ScanSurface_Result__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_Result)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_Result);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_Result * data =
      (nail_msgs__action__ScanSurface_Result *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_Result__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_Result__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_Result__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `last_point`
#include "nail_msgs/msg/detail/stiffness_point__functions.h"

bool
nail_msgs__action__ScanSurface_Feedback__init(nail_msgs__action__ScanSurface_Feedback * msg)
{
  if (!msg) {
    return false;
  }
  // percent
  // last_point
  if (!nail_msgs__msg__StiffnessPoint__init(&msg->last_point)) {
    nail_msgs__action__ScanSurface_Feedback__fini(msg);
    return false;
  }
  // points_done
  // points_total
  return true;
}

void
nail_msgs__action__ScanSurface_Feedback__fini(nail_msgs__action__ScanSurface_Feedback * msg)
{
  if (!msg) {
    return;
  }
  // percent
  // last_point
  nail_msgs__msg__StiffnessPoint__fini(&msg->last_point);
  // points_done
  // points_total
}

bool
nail_msgs__action__ScanSurface_Feedback__are_equal(const nail_msgs__action__ScanSurface_Feedback * lhs, const nail_msgs__action__ScanSurface_Feedback * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // percent
  if (lhs->percent != rhs->percent) {
    return false;
  }
  // last_point
  if (!nail_msgs__msg__StiffnessPoint__are_equal(
      &(lhs->last_point), &(rhs->last_point)))
  {
    return false;
  }
  // points_done
  if (lhs->points_done != rhs->points_done) {
    return false;
  }
  // points_total
  if (lhs->points_total != rhs->points_total) {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_Feedback__copy(
  const nail_msgs__action__ScanSurface_Feedback * input,
  nail_msgs__action__ScanSurface_Feedback * output)
{
  if (!input || !output) {
    return false;
  }
  // percent
  output->percent = input->percent;
  // last_point
  if (!nail_msgs__msg__StiffnessPoint__copy(
      &(input->last_point), &(output->last_point)))
  {
    return false;
  }
  // points_done
  output->points_done = input->points_done;
  // points_total
  output->points_total = input->points_total;
  return true;
}

nail_msgs__action__ScanSurface_Feedback *
nail_msgs__action__ScanSurface_Feedback__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Feedback * msg = (nail_msgs__action__ScanSurface_Feedback *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_Feedback), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_Feedback));
  bool success = nail_msgs__action__ScanSurface_Feedback__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_Feedback__destroy(nail_msgs__action__ScanSurface_Feedback * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_Feedback__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_Feedback__Sequence__init(nail_msgs__action__ScanSurface_Feedback__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Feedback * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_Feedback)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_Feedback *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_Feedback), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_Feedback__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_Feedback__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_Feedback__Sequence__fini(nail_msgs__action__ScanSurface_Feedback__Sequence * array)
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
      nail_msgs__action__ScanSurface_Feedback__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_Feedback__Sequence *
nail_msgs__action__ScanSurface_Feedback__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_Feedback__Sequence * array = (nail_msgs__action__ScanSurface_Feedback__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_Feedback__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_Feedback__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_Feedback__Sequence__destroy(nail_msgs__action__ScanSurface_Feedback__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_Feedback__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_Feedback__Sequence__are_equal(const nail_msgs__action__ScanSurface_Feedback__Sequence * lhs, const nail_msgs__action__ScanSurface_Feedback__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_Feedback__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_Feedback__Sequence__copy(
  const nail_msgs__action__ScanSurface_Feedback__Sequence * input,
  nail_msgs__action__ScanSurface_Feedback__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_Feedback)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_Feedback);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_Feedback * data =
      (nail_msgs__action__ScanSurface_Feedback *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_Feedback__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_Feedback__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_Feedback__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__functions.h"
// Member `goal`
// already included above
// #include "nail_msgs/action/detail/scan_surface__functions.h"

bool
nail_msgs__action__ScanSurface_SendGoal_Request__init(nail_msgs__action__ScanSurface_SendGoal_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    nail_msgs__action__ScanSurface_SendGoal_Request__fini(msg);
    return false;
  }
  // goal
  if (!nail_msgs__action__ScanSurface_Goal__init(&msg->goal)) {
    nail_msgs__action__ScanSurface_SendGoal_Request__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_SendGoal_Request__fini(nail_msgs__action__ScanSurface_SendGoal_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // goal
  nail_msgs__action__ScanSurface_Goal__fini(&msg->goal);
}

bool
nail_msgs__action__ScanSurface_SendGoal_Request__are_equal(const nail_msgs__action__ScanSurface_SendGoal_Request * lhs, const nail_msgs__action__ScanSurface_SendGoal_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  // goal
  if (!nail_msgs__action__ScanSurface_Goal__are_equal(
      &(lhs->goal), &(rhs->goal)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_SendGoal_Request__copy(
  const nail_msgs__action__ScanSurface_SendGoal_Request * input,
  nail_msgs__action__ScanSurface_SendGoal_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  // goal
  if (!nail_msgs__action__ScanSurface_Goal__copy(
      &(input->goal), &(output->goal)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_SendGoal_Request *
nail_msgs__action__ScanSurface_SendGoal_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Request * msg = (nail_msgs__action__ScanSurface_SendGoal_Request *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_SendGoal_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_SendGoal_Request));
  bool success = nail_msgs__action__ScanSurface_SendGoal_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_SendGoal_Request__destroy(nail_msgs__action__ScanSurface_SendGoal_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_SendGoal_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__init(nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_SendGoal_Request)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_SendGoal_Request *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_SendGoal_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_SendGoal_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_SendGoal_Request__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__fini(nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * array)
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
      nail_msgs__action__ScanSurface_SendGoal_Request__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_SendGoal_Request__Sequence *
nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * array = (nail_msgs__action__ScanSurface_SendGoal_Request__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_SendGoal_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__destroy(nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__are_equal(const nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * lhs, const nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_SendGoal_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__copy(
  const nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * input,
  nail_msgs__action__ScanSurface_SendGoal_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_SendGoal_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_SendGoal_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_SendGoal_Request * data =
      (nail_msgs__action__ScanSurface_SendGoal_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_SendGoal_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_SendGoal_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_SendGoal_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
nail_msgs__action__ScanSurface_SendGoal_Response__init(nail_msgs__action__ScanSurface_SendGoal_Response * msg)
{
  if (!msg) {
    return false;
  }
  // accepted
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    nail_msgs__action__ScanSurface_SendGoal_Response__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_SendGoal_Response__fini(nail_msgs__action__ScanSurface_SendGoal_Response * msg)
{
  if (!msg) {
    return;
  }
  // accepted
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
}

bool
nail_msgs__action__ScanSurface_SendGoal_Response__are_equal(const nail_msgs__action__ScanSurface_SendGoal_Response * lhs, const nail_msgs__action__ScanSurface_SendGoal_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // accepted
  if (lhs->accepted != rhs->accepted) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->stamp), &(rhs->stamp)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_SendGoal_Response__copy(
  const nail_msgs__action__ScanSurface_SendGoal_Response * input,
  nail_msgs__action__ScanSurface_SendGoal_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // accepted
  output->accepted = input->accepted;
  // stamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->stamp), &(output->stamp)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_SendGoal_Response *
nail_msgs__action__ScanSurface_SendGoal_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Response * msg = (nail_msgs__action__ScanSurface_SendGoal_Response *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_SendGoal_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_SendGoal_Response));
  bool success = nail_msgs__action__ScanSurface_SendGoal_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_SendGoal_Response__destroy(nail_msgs__action__ScanSurface_SendGoal_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_SendGoal_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__init(nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_SendGoal_Response)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_SendGoal_Response *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_SendGoal_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_SendGoal_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_SendGoal_Response__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__fini(nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * array)
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
      nail_msgs__action__ScanSurface_SendGoal_Response__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_SendGoal_Response__Sequence *
nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * array = (nail_msgs__action__ScanSurface_SendGoal_Response__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_SendGoal_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__destroy(nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__are_equal(const nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * lhs, const nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_SendGoal_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__copy(
  const nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * input,
  nail_msgs__action__ScanSurface_SendGoal_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_SendGoal_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_SendGoal_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_SendGoal_Response * data =
      (nail_msgs__action__ScanSurface_SendGoal_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_SendGoal_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_SendGoal_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_SendGoal_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
#include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/detail/scan_surface__functions.h"

bool
nail_msgs__action__ScanSurface_SendGoal_Event__init(nail_msgs__action__ScanSurface_SendGoal_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    nail_msgs__action__ScanSurface_SendGoal_Event__fini(msg);
    return false;
  }
  // request
  if (!nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__init(&msg->request, 0)) {
    nail_msgs__action__ScanSurface_SendGoal_Event__fini(msg);
    return false;
  }
  // response
  if (!nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__init(&msg->response, 0)) {
    nail_msgs__action__ScanSurface_SendGoal_Event__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_SendGoal_Event__fini(nail_msgs__action__ScanSurface_SendGoal_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__fini(&msg->request);
  // response
  nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__fini(&msg->response);
}

bool
nail_msgs__action__ScanSurface_SendGoal_Event__are_equal(const nail_msgs__action__ScanSurface_SendGoal_Event * lhs, const nail_msgs__action__ScanSurface_SendGoal_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_SendGoal_Event__copy(
  const nail_msgs__action__ScanSurface_SendGoal_Event * input,
  nail_msgs__action__ScanSurface_SendGoal_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!nail_msgs__action__ScanSurface_SendGoal_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__ScanSurface_SendGoal_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_SendGoal_Event *
nail_msgs__action__ScanSurface_SendGoal_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Event * msg = (nail_msgs__action__ScanSurface_SendGoal_Event *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_SendGoal_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_SendGoal_Event));
  bool success = nail_msgs__action__ScanSurface_SendGoal_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_SendGoal_Event__destroy(nail_msgs__action__ScanSurface_SendGoal_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_SendGoal_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__init(nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_SendGoal_Event)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_SendGoal_Event *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_SendGoal_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_SendGoal_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_SendGoal_Event__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__fini(nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * array)
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
      nail_msgs__action__ScanSurface_SendGoal_Event__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_SendGoal_Event__Sequence *
nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * array = (nail_msgs__action__ScanSurface_SendGoal_Event__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_SendGoal_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__destroy(nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__are_equal(const nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * lhs, const nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_SendGoal_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_SendGoal_Event__Sequence__copy(
  const nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * input,
  nail_msgs__action__ScanSurface_SendGoal_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_SendGoal_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_SendGoal_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_SendGoal_Event * data =
      (nail_msgs__action__ScanSurface_SendGoal_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_SendGoal_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_SendGoal_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_SendGoal_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__functions.h"

bool
nail_msgs__action__ScanSurface_GetResult_Request__init(nail_msgs__action__ScanSurface_GetResult_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    nail_msgs__action__ScanSurface_GetResult_Request__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_GetResult_Request__fini(nail_msgs__action__ScanSurface_GetResult_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
}

bool
nail_msgs__action__ScanSurface_GetResult_Request__are_equal(const nail_msgs__action__ScanSurface_GetResult_Request * lhs, const nail_msgs__action__ScanSurface_GetResult_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_GetResult_Request__copy(
  const nail_msgs__action__ScanSurface_GetResult_Request * input,
  nail_msgs__action__ScanSurface_GetResult_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_GetResult_Request *
nail_msgs__action__ScanSurface_GetResult_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Request * msg = (nail_msgs__action__ScanSurface_GetResult_Request *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_GetResult_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_GetResult_Request));
  bool success = nail_msgs__action__ScanSurface_GetResult_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_GetResult_Request__destroy(nail_msgs__action__ScanSurface_GetResult_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_GetResult_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_GetResult_Request__Sequence__init(nail_msgs__action__ScanSurface_GetResult_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_GetResult_Request)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_GetResult_Request *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_GetResult_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_GetResult_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_GetResult_Request__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_GetResult_Request__Sequence__fini(nail_msgs__action__ScanSurface_GetResult_Request__Sequence * array)
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
      nail_msgs__action__ScanSurface_GetResult_Request__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_GetResult_Request__Sequence *
nail_msgs__action__ScanSurface_GetResult_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Request__Sequence * array = (nail_msgs__action__ScanSurface_GetResult_Request__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_GetResult_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_GetResult_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_GetResult_Request__Sequence__destroy(nail_msgs__action__ScanSurface_GetResult_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_GetResult_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_GetResult_Request__Sequence__are_equal(const nail_msgs__action__ScanSurface_GetResult_Request__Sequence * lhs, const nail_msgs__action__ScanSurface_GetResult_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_GetResult_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_GetResult_Request__Sequence__copy(
  const nail_msgs__action__ScanSurface_GetResult_Request__Sequence * input,
  nail_msgs__action__ScanSurface_GetResult_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_GetResult_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_GetResult_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_GetResult_Request * data =
      (nail_msgs__action__ScanSurface_GetResult_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_GetResult_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_GetResult_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_GetResult_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `result`
// already included above
// #include "nail_msgs/action/detail/scan_surface__functions.h"

bool
nail_msgs__action__ScanSurface_GetResult_Response__init(nail_msgs__action__ScanSurface_GetResult_Response * msg)
{
  if (!msg) {
    return false;
  }
  // status
  // result
  if (!nail_msgs__action__ScanSurface_Result__init(&msg->result)) {
    nail_msgs__action__ScanSurface_GetResult_Response__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_GetResult_Response__fini(nail_msgs__action__ScanSurface_GetResult_Response * msg)
{
  if (!msg) {
    return;
  }
  // status
  // result
  nail_msgs__action__ScanSurface_Result__fini(&msg->result);
}

bool
nail_msgs__action__ScanSurface_GetResult_Response__are_equal(const nail_msgs__action__ScanSurface_GetResult_Response * lhs, const nail_msgs__action__ScanSurface_GetResult_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // status
  if (lhs->status != rhs->status) {
    return false;
  }
  // result
  if (!nail_msgs__action__ScanSurface_Result__are_equal(
      &(lhs->result), &(rhs->result)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_GetResult_Response__copy(
  const nail_msgs__action__ScanSurface_GetResult_Response * input,
  nail_msgs__action__ScanSurface_GetResult_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // status
  output->status = input->status;
  // result
  if (!nail_msgs__action__ScanSurface_Result__copy(
      &(input->result), &(output->result)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_GetResult_Response *
nail_msgs__action__ScanSurface_GetResult_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Response * msg = (nail_msgs__action__ScanSurface_GetResult_Response *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_GetResult_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_GetResult_Response));
  bool success = nail_msgs__action__ScanSurface_GetResult_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_GetResult_Response__destroy(nail_msgs__action__ScanSurface_GetResult_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_GetResult_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_GetResult_Response__Sequence__init(nail_msgs__action__ScanSurface_GetResult_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_GetResult_Response)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_GetResult_Response *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_GetResult_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_GetResult_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_GetResult_Response__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_GetResult_Response__Sequence__fini(nail_msgs__action__ScanSurface_GetResult_Response__Sequence * array)
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
      nail_msgs__action__ScanSurface_GetResult_Response__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_GetResult_Response__Sequence *
nail_msgs__action__ScanSurface_GetResult_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Response__Sequence * array = (nail_msgs__action__ScanSurface_GetResult_Response__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_GetResult_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_GetResult_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_GetResult_Response__Sequence__destroy(nail_msgs__action__ScanSurface_GetResult_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_GetResult_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_GetResult_Response__Sequence__are_equal(const nail_msgs__action__ScanSurface_GetResult_Response__Sequence * lhs, const nail_msgs__action__ScanSurface_GetResult_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_GetResult_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_GetResult_Response__Sequence__copy(
  const nail_msgs__action__ScanSurface_GetResult_Response__Sequence * input,
  nail_msgs__action__ScanSurface_GetResult_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_GetResult_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_GetResult_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_GetResult_Response * data =
      (nail_msgs__action__ScanSurface_GetResult_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_GetResult_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_GetResult_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_GetResult_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
// already included above
// #include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "nail_msgs/action/detail/scan_surface__functions.h"

bool
nail_msgs__action__ScanSurface_GetResult_Event__init(nail_msgs__action__ScanSurface_GetResult_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    nail_msgs__action__ScanSurface_GetResult_Event__fini(msg);
    return false;
  }
  // request
  if (!nail_msgs__action__ScanSurface_GetResult_Request__Sequence__init(&msg->request, 0)) {
    nail_msgs__action__ScanSurface_GetResult_Event__fini(msg);
    return false;
  }
  // response
  if (!nail_msgs__action__ScanSurface_GetResult_Response__Sequence__init(&msg->response, 0)) {
    nail_msgs__action__ScanSurface_GetResult_Event__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_GetResult_Event__fini(nail_msgs__action__ScanSurface_GetResult_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  nail_msgs__action__ScanSurface_GetResult_Request__Sequence__fini(&msg->request);
  // response
  nail_msgs__action__ScanSurface_GetResult_Response__Sequence__fini(&msg->response);
}

bool
nail_msgs__action__ScanSurface_GetResult_Event__are_equal(const nail_msgs__action__ScanSurface_GetResult_Event * lhs, const nail_msgs__action__ScanSurface_GetResult_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!nail_msgs__action__ScanSurface_GetResult_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__ScanSurface_GetResult_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_GetResult_Event__copy(
  const nail_msgs__action__ScanSurface_GetResult_Event * input,
  nail_msgs__action__ScanSurface_GetResult_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!nail_msgs__action__ScanSurface_GetResult_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__ScanSurface_GetResult_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_GetResult_Event *
nail_msgs__action__ScanSurface_GetResult_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Event * msg = (nail_msgs__action__ScanSurface_GetResult_Event *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_GetResult_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_GetResult_Event));
  bool success = nail_msgs__action__ScanSurface_GetResult_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_GetResult_Event__destroy(nail_msgs__action__ScanSurface_GetResult_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_GetResult_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_GetResult_Event__Sequence__init(nail_msgs__action__ScanSurface_GetResult_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_GetResult_Event)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_GetResult_Event *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_GetResult_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_GetResult_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_GetResult_Event__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_GetResult_Event__Sequence__fini(nail_msgs__action__ScanSurface_GetResult_Event__Sequence * array)
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
      nail_msgs__action__ScanSurface_GetResult_Event__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_GetResult_Event__Sequence *
nail_msgs__action__ScanSurface_GetResult_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_GetResult_Event__Sequence * array = (nail_msgs__action__ScanSurface_GetResult_Event__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_GetResult_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_GetResult_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_GetResult_Event__Sequence__destroy(nail_msgs__action__ScanSurface_GetResult_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_GetResult_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_GetResult_Event__Sequence__are_equal(const nail_msgs__action__ScanSurface_GetResult_Event__Sequence * lhs, const nail_msgs__action__ScanSurface_GetResult_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_GetResult_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_GetResult_Event__Sequence__copy(
  const nail_msgs__action__ScanSurface_GetResult_Event__Sequence * input,
  nail_msgs__action__ScanSurface_GetResult_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_GetResult_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_GetResult_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_GetResult_Event * data =
      (nail_msgs__action__ScanSurface_GetResult_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_GetResult_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_GetResult_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_GetResult_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__functions.h"
// Member `feedback`
// already included above
// #include "nail_msgs/action/detail/scan_surface__functions.h"

bool
nail_msgs__action__ScanSurface_FeedbackMessage__init(nail_msgs__action__ScanSurface_FeedbackMessage * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    nail_msgs__action__ScanSurface_FeedbackMessage__fini(msg);
    return false;
  }
  // feedback
  if (!nail_msgs__action__ScanSurface_Feedback__init(&msg->feedback)) {
    nail_msgs__action__ScanSurface_FeedbackMessage__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__ScanSurface_FeedbackMessage__fini(nail_msgs__action__ScanSurface_FeedbackMessage * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // feedback
  nail_msgs__action__ScanSurface_Feedback__fini(&msg->feedback);
}

bool
nail_msgs__action__ScanSurface_FeedbackMessage__are_equal(const nail_msgs__action__ScanSurface_FeedbackMessage * lhs, const nail_msgs__action__ScanSurface_FeedbackMessage * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  // feedback
  if (!nail_msgs__action__ScanSurface_Feedback__are_equal(
      &(lhs->feedback), &(rhs->feedback)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_FeedbackMessage__copy(
  const nail_msgs__action__ScanSurface_FeedbackMessage * input,
  nail_msgs__action__ScanSurface_FeedbackMessage * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  // feedback
  if (!nail_msgs__action__ScanSurface_Feedback__copy(
      &(input->feedback), &(output->feedback)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__ScanSurface_FeedbackMessage *
nail_msgs__action__ScanSurface_FeedbackMessage__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_FeedbackMessage * msg = (nail_msgs__action__ScanSurface_FeedbackMessage *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_FeedbackMessage), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__ScanSurface_FeedbackMessage));
  bool success = nail_msgs__action__ScanSurface_FeedbackMessage__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__ScanSurface_FeedbackMessage__destroy(nail_msgs__action__ScanSurface_FeedbackMessage * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__ScanSurface_FeedbackMessage__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__init(nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_FeedbackMessage * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_FeedbackMessage)) {
      return false;
    }
    data = (nail_msgs__action__ScanSurface_FeedbackMessage *)allocator.zero_allocate(size, sizeof(nail_msgs__action__ScanSurface_FeedbackMessage), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__ScanSurface_FeedbackMessage__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__ScanSurface_FeedbackMessage__fini(&data[i - 1]);
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
nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__fini(nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * array)
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
      nail_msgs__action__ScanSurface_FeedbackMessage__fini(&array->data[i]);
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

nail_msgs__action__ScanSurface_FeedbackMessage__Sequence *
nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * array = (nail_msgs__action__ScanSurface_FeedbackMessage__Sequence *)allocator.allocate(sizeof(nail_msgs__action__ScanSurface_FeedbackMessage__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__destroy(nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__are_equal(const nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * lhs, const nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__ScanSurface_FeedbackMessage__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__ScanSurface_FeedbackMessage__Sequence__copy(
  const nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * input,
  nail_msgs__action__ScanSurface_FeedbackMessage__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__ScanSurface_FeedbackMessage)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__ScanSurface_FeedbackMessage);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__ScanSurface_FeedbackMessage * data =
      (nail_msgs__action__ScanSurface_FeedbackMessage *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__ScanSurface_FeedbackMessage__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__ScanSurface_FeedbackMessage__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__ScanSurface_FeedbackMessage__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
