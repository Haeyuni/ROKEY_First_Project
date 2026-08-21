// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:action/CureRegion.idl
// generated code does not contain a copyright notice
#include "nail_msgs/action/detail/cure_region__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `trajectory`
#include "geometry_msgs/msg/detail/pose__functions.h"
// Member `frame_id`
// Member `session_id`
#include "rosidl_runtime_c/string_functions.h"
// Member `regions`
#include "geometry_msgs/msg/detail/point__functions.h"

bool
nail_msgs__action__CureRegion_Goal__init(nail_msgs__action__CureRegion_Goal * msg)
{
  if (!msg) {
    return false;
  }
  // trajectory
  if (!geometry_msgs__msg__Pose__Sequence__init(&msg->trajectory, 0)) {
    nail_msgs__action__CureRegion_Goal__fini(msg);
    return false;
  }
  // frame_id
  if (!rosidl_runtime_c__String__init(&msg->frame_id)) {
    nail_msgs__action__CureRegion_Goal__fini(msg);
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__init(&msg->session_id)) {
    nail_msgs__action__CureRegion_Goal__fini(msg);
    return false;
  }
  // target_index
  // layer_index
  // regions
  if (!geometry_msgs__msg__Point__Sequence__init(&msg->regions, 0)) {
    nail_msgs__action__CureRegion_Goal__fini(msg);
    return false;
  }
  // exposure_s_per_region
  // max_exposure_s
  // standoff_target_mm
  // standoff_tolerance_mm
  // exposure_scale
  return true;
}

void
nail_msgs__action__CureRegion_Goal__fini(nail_msgs__action__CureRegion_Goal * msg)
{
  if (!msg) {
    return;
  }
  // trajectory
  geometry_msgs__msg__Pose__Sequence__fini(&msg->trajectory);
  // frame_id
  rosidl_runtime_c__String__fini(&msg->frame_id);
  // session_id
  rosidl_runtime_c__String__fini(&msg->session_id);
  // target_index
  // layer_index
  // regions
  geometry_msgs__msg__Point__Sequence__fini(&msg->regions);
  // exposure_s_per_region
  // max_exposure_s
  // standoff_target_mm
  // standoff_tolerance_mm
  // exposure_scale
}

bool
nail_msgs__action__CureRegion_Goal__are_equal(const nail_msgs__action__CureRegion_Goal * lhs, const nail_msgs__action__CureRegion_Goal * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // trajectory
  if (!geometry_msgs__msg__Pose__Sequence__are_equal(
      &(lhs->trajectory), &(rhs->trajectory)))
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
  // layer_index
  if (lhs->layer_index != rhs->layer_index) {
    return false;
  }
  // regions
  if (!geometry_msgs__msg__Point__Sequence__are_equal(
      &(lhs->regions), &(rhs->regions)))
  {
    return false;
  }
  // exposure_s_per_region
  if (lhs->exposure_s_per_region != rhs->exposure_s_per_region) {
    return false;
  }
  // max_exposure_s
  if (lhs->max_exposure_s != rhs->max_exposure_s) {
    return false;
  }
  // standoff_target_mm
  if (lhs->standoff_target_mm != rhs->standoff_target_mm) {
    return false;
  }
  // standoff_tolerance_mm
  if (lhs->standoff_tolerance_mm != rhs->standoff_tolerance_mm) {
    return false;
  }
  // exposure_scale
  if (lhs->exposure_scale != rhs->exposure_scale) {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_Goal__copy(
  const nail_msgs__action__CureRegion_Goal * input,
  nail_msgs__action__CureRegion_Goal * output)
{
  if (!input || !output) {
    return false;
  }
  // trajectory
  if (!geometry_msgs__msg__Pose__Sequence__copy(
      &(input->trajectory), &(output->trajectory)))
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
  // layer_index
  output->layer_index = input->layer_index;
  // regions
  if (!geometry_msgs__msg__Point__Sequence__copy(
      &(input->regions), &(output->regions)))
  {
    return false;
  }
  // exposure_s_per_region
  output->exposure_s_per_region = input->exposure_s_per_region;
  // max_exposure_s
  output->max_exposure_s = input->max_exposure_s;
  // standoff_target_mm
  output->standoff_target_mm = input->standoff_target_mm;
  // standoff_tolerance_mm
  output->standoff_tolerance_mm = input->standoff_tolerance_mm;
  // exposure_scale
  output->exposure_scale = input->exposure_scale;
  return true;
}

nail_msgs__action__CureRegion_Goal *
nail_msgs__action__CureRegion_Goal__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Goal * msg = (nail_msgs__action__CureRegion_Goal *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_Goal), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_Goal));
  bool success = nail_msgs__action__CureRegion_Goal__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_Goal__destroy(nail_msgs__action__CureRegion_Goal * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_Goal__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_Goal__Sequence__init(nail_msgs__action__CureRegion_Goal__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Goal * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_Goal)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_Goal *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_Goal), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_Goal__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_Goal__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_Goal__Sequence__fini(nail_msgs__action__CureRegion_Goal__Sequence * array)
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
      nail_msgs__action__CureRegion_Goal__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_Goal__Sequence *
nail_msgs__action__CureRegion_Goal__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Goal__Sequence * array = (nail_msgs__action__CureRegion_Goal__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_Goal__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_Goal__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_Goal__Sequence__destroy(nail_msgs__action__CureRegion_Goal__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_Goal__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_Goal__Sequence__are_equal(const nail_msgs__action__CureRegion_Goal__Sequence * lhs, const nail_msgs__action__CureRegion_Goal__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_Goal__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_Goal__Sequence__copy(
  const nail_msgs__action__CureRegion_Goal__Sequence * input,
  nail_msgs__action__CureRegion_Goal__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_Goal)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_Goal);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_Goal * data =
      (nail_msgs__action__CureRegion_Goal *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_Goal__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_Goal__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_Goal__copy(
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
// Member `missed_region_indices`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
nail_msgs__action__CureRegion_Result__init(nail_msgs__action__CureRegion_Result * msg)
{
  if (!msg) {
    return false;
  }
  // base
  if (!nail_msgs__msg__ActionResultBase__init(&msg->base)) {
    nail_msgs__action__CureRegion_Result__fini(msg);
    return false;
  }
  // total_exposure_s
  // missed_region_indices
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->missed_region_indices, 0)) {
    nail_msgs__action__CureRegion_Result__fini(msg);
    return false;
  }
  // lamp_off_confirmed
  return true;
}

void
nail_msgs__action__CureRegion_Result__fini(nail_msgs__action__CureRegion_Result * msg)
{
  if (!msg) {
    return;
  }
  // base
  nail_msgs__msg__ActionResultBase__fini(&msg->base);
  // total_exposure_s
  // missed_region_indices
  rosidl_runtime_c__int32__Sequence__fini(&msg->missed_region_indices);
  // lamp_off_confirmed
}

bool
nail_msgs__action__CureRegion_Result__are_equal(const nail_msgs__action__CureRegion_Result * lhs, const nail_msgs__action__CureRegion_Result * rhs)
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
  // total_exposure_s
  if (lhs->total_exposure_s != rhs->total_exposure_s) {
    return false;
  }
  // missed_region_indices
  if (!rosidl_runtime_c__int32__Sequence__are_equal(
      &(lhs->missed_region_indices), &(rhs->missed_region_indices)))
  {
    return false;
  }
  // lamp_off_confirmed
  if (lhs->lamp_off_confirmed != rhs->lamp_off_confirmed) {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_Result__copy(
  const nail_msgs__action__CureRegion_Result * input,
  nail_msgs__action__CureRegion_Result * output)
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
  // total_exposure_s
  output->total_exposure_s = input->total_exposure_s;
  // missed_region_indices
  if (!rosidl_runtime_c__int32__Sequence__copy(
      &(input->missed_region_indices), &(output->missed_region_indices)))
  {
    return false;
  }
  // lamp_off_confirmed
  output->lamp_off_confirmed = input->lamp_off_confirmed;
  return true;
}

nail_msgs__action__CureRegion_Result *
nail_msgs__action__CureRegion_Result__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Result * msg = (nail_msgs__action__CureRegion_Result *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_Result), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_Result));
  bool success = nail_msgs__action__CureRegion_Result__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_Result__destroy(nail_msgs__action__CureRegion_Result * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_Result__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_Result__Sequence__init(nail_msgs__action__CureRegion_Result__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Result * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_Result)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_Result *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_Result), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_Result__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_Result__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_Result__Sequence__fini(nail_msgs__action__CureRegion_Result__Sequence * array)
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
      nail_msgs__action__CureRegion_Result__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_Result__Sequence *
nail_msgs__action__CureRegion_Result__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Result__Sequence * array = (nail_msgs__action__CureRegion_Result__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_Result__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_Result__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_Result__Sequence__destroy(nail_msgs__action__CureRegion_Result__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_Result__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_Result__Sequence__are_equal(const nail_msgs__action__CureRegion_Result__Sequence * lhs, const nail_msgs__action__CureRegion_Result__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_Result__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_Result__Sequence__copy(
  const nail_msgs__action__CureRegion_Result__Sequence * input,
  nail_msgs__action__CureRegion_Result__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_Result)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_Result);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_Result * data =
      (nail_msgs__action__CureRegion_Result *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_Result__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_Result__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_Result__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
nail_msgs__action__CureRegion_Feedback__init(nail_msgs__action__CureRegion_Feedback * msg)
{
  if (!msg) {
    return false;
  }
  // percent
  // current_region
  // elapsed_exposure_s
  // lamp_on
  return true;
}

void
nail_msgs__action__CureRegion_Feedback__fini(nail_msgs__action__CureRegion_Feedback * msg)
{
  if (!msg) {
    return;
  }
  // percent
  // current_region
  // elapsed_exposure_s
  // lamp_on
}

bool
nail_msgs__action__CureRegion_Feedback__are_equal(const nail_msgs__action__CureRegion_Feedback * lhs, const nail_msgs__action__CureRegion_Feedback * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // percent
  if (lhs->percent != rhs->percent) {
    return false;
  }
  // current_region
  if (lhs->current_region != rhs->current_region) {
    return false;
  }
  // elapsed_exposure_s
  if (lhs->elapsed_exposure_s != rhs->elapsed_exposure_s) {
    return false;
  }
  // lamp_on
  if (lhs->lamp_on != rhs->lamp_on) {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_Feedback__copy(
  const nail_msgs__action__CureRegion_Feedback * input,
  nail_msgs__action__CureRegion_Feedback * output)
{
  if (!input || !output) {
    return false;
  }
  // percent
  output->percent = input->percent;
  // current_region
  output->current_region = input->current_region;
  // elapsed_exposure_s
  output->elapsed_exposure_s = input->elapsed_exposure_s;
  // lamp_on
  output->lamp_on = input->lamp_on;
  return true;
}

nail_msgs__action__CureRegion_Feedback *
nail_msgs__action__CureRegion_Feedback__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Feedback * msg = (nail_msgs__action__CureRegion_Feedback *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_Feedback), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_Feedback));
  bool success = nail_msgs__action__CureRegion_Feedback__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_Feedback__destroy(nail_msgs__action__CureRegion_Feedback * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_Feedback__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_Feedback__Sequence__init(nail_msgs__action__CureRegion_Feedback__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Feedback * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_Feedback)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_Feedback *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_Feedback), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_Feedback__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_Feedback__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_Feedback__Sequence__fini(nail_msgs__action__CureRegion_Feedback__Sequence * array)
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
      nail_msgs__action__CureRegion_Feedback__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_Feedback__Sequence *
nail_msgs__action__CureRegion_Feedback__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_Feedback__Sequence * array = (nail_msgs__action__CureRegion_Feedback__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_Feedback__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_Feedback__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_Feedback__Sequence__destroy(nail_msgs__action__CureRegion_Feedback__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_Feedback__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_Feedback__Sequence__are_equal(const nail_msgs__action__CureRegion_Feedback__Sequence * lhs, const nail_msgs__action__CureRegion_Feedback__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_Feedback__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_Feedback__Sequence__copy(
  const nail_msgs__action__CureRegion_Feedback__Sequence * input,
  nail_msgs__action__CureRegion_Feedback__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_Feedback)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_Feedback);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_Feedback * data =
      (nail_msgs__action__CureRegion_Feedback *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_Feedback__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_Feedback__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_Feedback__copy(
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
// #include "nail_msgs/action/detail/cure_region__functions.h"

bool
nail_msgs__action__CureRegion_SendGoal_Request__init(nail_msgs__action__CureRegion_SendGoal_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    nail_msgs__action__CureRegion_SendGoal_Request__fini(msg);
    return false;
  }
  // goal
  if (!nail_msgs__action__CureRegion_Goal__init(&msg->goal)) {
    nail_msgs__action__CureRegion_SendGoal_Request__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__CureRegion_SendGoal_Request__fini(nail_msgs__action__CureRegion_SendGoal_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // goal
  nail_msgs__action__CureRegion_Goal__fini(&msg->goal);
}

bool
nail_msgs__action__CureRegion_SendGoal_Request__are_equal(const nail_msgs__action__CureRegion_SendGoal_Request * lhs, const nail_msgs__action__CureRegion_SendGoal_Request * rhs)
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
  if (!nail_msgs__action__CureRegion_Goal__are_equal(
      &(lhs->goal), &(rhs->goal)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_SendGoal_Request__copy(
  const nail_msgs__action__CureRegion_SendGoal_Request * input,
  nail_msgs__action__CureRegion_SendGoal_Request * output)
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
  if (!nail_msgs__action__CureRegion_Goal__copy(
      &(input->goal), &(output->goal)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__CureRegion_SendGoal_Request *
nail_msgs__action__CureRegion_SendGoal_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Request * msg = (nail_msgs__action__CureRegion_SendGoal_Request *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_SendGoal_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_SendGoal_Request));
  bool success = nail_msgs__action__CureRegion_SendGoal_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_SendGoal_Request__destroy(nail_msgs__action__CureRegion_SendGoal_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_SendGoal_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_SendGoal_Request__Sequence__init(nail_msgs__action__CureRegion_SendGoal_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_SendGoal_Request)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_SendGoal_Request *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_SendGoal_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_SendGoal_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_SendGoal_Request__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_SendGoal_Request__Sequence__fini(nail_msgs__action__CureRegion_SendGoal_Request__Sequence * array)
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
      nail_msgs__action__CureRegion_SendGoal_Request__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_SendGoal_Request__Sequence *
nail_msgs__action__CureRegion_SendGoal_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Request__Sequence * array = (nail_msgs__action__CureRegion_SendGoal_Request__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_SendGoal_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_SendGoal_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_SendGoal_Request__Sequence__destroy(nail_msgs__action__CureRegion_SendGoal_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_SendGoal_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_SendGoal_Request__Sequence__are_equal(const nail_msgs__action__CureRegion_SendGoal_Request__Sequence * lhs, const nail_msgs__action__CureRegion_SendGoal_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_SendGoal_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_SendGoal_Request__Sequence__copy(
  const nail_msgs__action__CureRegion_SendGoal_Request__Sequence * input,
  nail_msgs__action__CureRegion_SendGoal_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_SendGoal_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_SendGoal_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_SendGoal_Request * data =
      (nail_msgs__action__CureRegion_SendGoal_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_SendGoal_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_SendGoal_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_SendGoal_Request__copy(
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
nail_msgs__action__CureRegion_SendGoal_Response__init(nail_msgs__action__CureRegion_SendGoal_Response * msg)
{
  if (!msg) {
    return false;
  }
  // accepted
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    nail_msgs__action__CureRegion_SendGoal_Response__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__CureRegion_SendGoal_Response__fini(nail_msgs__action__CureRegion_SendGoal_Response * msg)
{
  if (!msg) {
    return;
  }
  // accepted
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
}

bool
nail_msgs__action__CureRegion_SendGoal_Response__are_equal(const nail_msgs__action__CureRegion_SendGoal_Response * lhs, const nail_msgs__action__CureRegion_SendGoal_Response * rhs)
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
nail_msgs__action__CureRegion_SendGoal_Response__copy(
  const nail_msgs__action__CureRegion_SendGoal_Response * input,
  nail_msgs__action__CureRegion_SendGoal_Response * output)
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

nail_msgs__action__CureRegion_SendGoal_Response *
nail_msgs__action__CureRegion_SendGoal_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Response * msg = (nail_msgs__action__CureRegion_SendGoal_Response *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_SendGoal_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_SendGoal_Response));
  bool success = nail_msgs__action__CureRegion_SendGoal_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_SendGoal_Response__destroy(nail_msgs__action__CureRegion_SendGoal_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_SendGoal_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_SendGoal_Response__Sequence__init(nail_msgs__action__CureRegion_SendGoal_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_SendGoal_Response)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_SendGoal_Response *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_SendGoal_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_SendGoal_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_SendGoal_Response__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_SendGoal_Response__Sequence__fini(nail_msgs__action__CureRegion_SendGoal_Response__Sequence * array)
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
      nail_msgs__action__CureRegion_SendGoal_Response__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_SendGoal_Response__Sequence *
nail_msgs__action__CureRegion_SendGoal_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Response__Sequence * array = (nail_msgs__action__CureRegion_SendGoal_Response__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_SendGoal_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_SendGoal_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_SendGoal_Response__Sequence__destroy(nail_msgs__action__CureRegion_SendGoal_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_SendGoal_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_SendGoal_Response__Sequence__are_equal(const nail_msgs__action__CureRegion_SendGoal_Response__Sequence * lhs, const nail_msgs__action__CureRegion_SendGoal_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_SendGoal_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_SendGoal_Response__Sequence__copy(
  const nail_msgs__action__CureRegion_SendGoal_Response__Sequence * input,
  nail_msgs__action__CureRegion_SendGoal_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_SendGoal_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_SendGoal_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_SendGoal_Response * data =
      (nail_msgs__action__CureRegion_SendGoal_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_SendGoal_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_SendGoal_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_SendGoal_Response__copy(
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
// #include "nail_msgs/action/detail/cure_region__functions.h"

bool
nail_msgs__action__CureRegion_SendGoal_Event__init(nail_msgs__action__CureRegion_SendGoal_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    nail_msgs__action__CureRegion_SendGoal_Event__fini(msg);
    return false;
  }
  // request
  if (!nail_msgs__action__CureRegion_SendGoal_Request__Sequence__init(&msg->request, 0)) {
    nail_msgs__action__CureRegion_SendGoal_Event__fini(msg);
    return false;
  }
  // response
  if (!nail_msgs__action__CureRegion_SendGoal_Response__Sequence__init(&msg->response, 0)) {
    nail_msgs__action__CureRegion_SendGoal_Event__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__CureRegion_SendGoal_Event__fini(nail_msgs__action__CureRegion_SendGoal_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  nail_msgs__action__CureRegion_SendGoal_Request__Sequence__fini(&msg->request);
  // response
  nail_msgs__action__CureRegion_SendGoal_Response__Sequence__fini(&msg->response);
}

bool
nail_msgs__action__CureRegion_SendGoal_Event__are_equal(const nail_msgs__action__CureRegion_SendGoal_Event * lhs, const nail_msgs__action__CureRegion_SendGoal_Event * rhs)
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
  if (!nail_msgs__action__CureRegion_SendGoal_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__CureRegion_SendGoal_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_SendGoal_Event__copy(
  const nail_msgs__action__CureRegion_SendGoal_Event * input,
  nail_msgs__action__CureRegion_SendGoal_Event * output)
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
  if (!nail_msgs__action__CureRegion_SendGoal_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__CureRegion_SendGoal_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__CureRegion_SendGoal_Event *
nail_msgs__action__CureRegion_SendGoal_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Event * msg = (nail_msgs__action__CureRegion_SendGoal_Event *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_SendGoal_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_SendGoal_Event));
  bool success = nail_msgs__action__CureRegion_SendGoal_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_SendGoal_Event__destroy(nail_msgs__action__CureRegion_SendGoal_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_SendGoal_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_SendGoal_Event__Sequence__init(nail_msgs__action__CureRegion_SendGoal_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_SendGoal_Event)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_SendGoal_Event *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_SendGoal_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_SendGoal_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_SendGoal_Event__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_SendGoal_Event__Sequence__fini(nail_msgs__action__CureRegion_SendGoal_Event__Sequence * array)
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
      nail_msgs__action__CureRegion_SendGoal_Event__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_SendGoal_Event__Sequence *
nail_msgs__action__CureRegion_SendGoal_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_SendGoal_Event__Sequence * array = (nail_msgs__action__CureRegion_SendGoal_Event__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_SendGoal_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_SendGoal_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_SendGoal_Event__Sequence__destroy(nail_msgs__action__CureRegion_SendGoal_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_SendGoal_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_SendGoal_Event__Sequence__are_equal(const nail_msgs__action__CureRegion_SendGoal_Event__Sequence * lhs, const nail_msgs__action__CureRegion_SendGoal_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_SendGoal_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_SendGoal_Event__Sequence__copy(
  const nail_msgs__action__CureRegion_SendGoal_Event__Sequence * input,
  nail_msgs__action__CureRegion_SendGoal_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_SendGoal_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_SendGoal_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_SendGoal_Event * data =
      (nail_msgs__action__CureRegion_SendGoal_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_SendGoal_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_SendGoal_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_SendGoal_Event__copy(
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
nail_msgs__action__CureRegion_GetResult_Request__init(nail_msgs__action__CureRegion_GetResult_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    nail_msgs__action__CureRegion_GetResult_Request__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__CureRegion_GetResult_Request__fini(nail_msgs__action__CureRegion_GetResult_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
}

bool
nail_msgs__action__CureRegion_GetResult_Request__are_equal(const nail_msgs__action__CureRegion_GetResult_Request * lhs, const nail_msgs__action__CureRegion_GetResult_Request * rhs)
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
nail_msgs__action__CureRegion_GetResult_Request__copy(
  const nail_msgs__action__CureRegion_GetResult_Request * input,
  nail_msgs__action__CureRegion_GetResult_Request * output)
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

nail_msgs__action__CureRegion_GetResult_Request *
nail_msgs__action__CureRegion_GetResult_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Request * msg = (nail_msgs__action__CureRegion_GetResult_Request *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_GetResult_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_GetResult_Request));
  bool success = nail_msgs__action__CureRegion_GetResult_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_GetResult_Request__destroy(nail_msgs__action__CureRegion_GetResult_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_GetResult_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_GetResult_Request__Sequence__init(nail_msgs__action__CureRegion_GetResult_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_GetResult_Request)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_GetResult_Request *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_GetResult_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_GetResult_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_GetResult_Request__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_GetResult_Request__Sequence__fini(nail_msgs__action__CureRegion_GetResult_Request__Sequence * array)
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
      nail_msgs__action__CureRegion_GetResult_Request__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_GetResult_Request__Sequence *
nail_msgs__action__CureRegion_GetResult_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Request__Sequence * array = (nail_msgs__action__CureRegion_GetResult_Request__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_GetResult_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_GetResult_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_GetResult_Request__Sequence__destroy(nail_msgs__action__CureRegion_GetResult_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_GetResult_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_GetResult_Request__Sequence__are_equal(const nail_msgs__action__CureRegion_GetResult_Request__Sequence * lhs, const nail_msgs__action__CureRegion_GetResult_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_GetResult_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_GetResult_Request__Sequence__copy(
  const nail_msgs__action__CureRegion_GetResult_Request__Sequence * input,
  nail_msgs__action__CureRegion_GetResult_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_GetResult_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_GetResult_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_GetResult_Request * data =
      (nail_msgs__action__CureRegion_GetResult_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_GetResult_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_GetResult_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_GetResult_Request__copy(
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
// #include "nail_msgs/action/detail/cure_region__functions.h"

bool
nail_msgs__action__CureRegion_GetResult_Response__init(nail_msgs__action__CureRegion_GetResult_Response * msg)
{
  if (!msg) {
    return false;
  }
  // status
  // result
  if (!nail_msgs__action__CureRegion_Result__init(&msg->result)) {
    nail_msgs__action__CureRegion_GetResult_Response__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__CureRegion_GetResult_Response__fini(nail_msgs__action__CureRegion_GetResult_Response * msg)
{
  if (!msg) {
    return;
  }
  // status
  // result
  nail_msgs__action__CureRegion_Result__fini(&msg->result);
}

bool
nail_msgs__action__CureRegion_GetResult_Response__are_equal(const nail_msgs__action__CureRegion_GetResult_Response * lhs, const nail_msgs__action__CureRegion_GetResult_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // status
  if (lhs->status != rhs->status) {
    return false;
  }
  // result
  if (!nail_msgs__action__CureRegion_Result__are_equal(
      &(lhs->result), &(rhs->result)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_GetResult_Response__copy(
  const nail_msgs__action__CureRegion_GetResult_Response * input,
  nail_msgs__action__CureRegion_GetResult_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // status
  output->status = input->status;
  // result
  if (!nail_msgs__action__CureRegion_Result__copy(
      &(input->result), &(output->result)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__CureRegion_GetResult_Response *
nail_msgs__action__CureRegion_GetResult_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Response * msg = (nail_msgs__action__CureRegion_GetResult_Response *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_GetResult_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_GetResult_Response));
  bool success = nail_msgs__action__CureRegion_GetResult_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_GetResult_Response__destroy(nail_msgs__action__CureRegion_GetResult_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_GetResult_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_GetResult_Response__Sequence__init(nail_msgs__action__CureRegion_GetResult_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_GetResult_Response)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_GetResult_Response *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_GetResult_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_GetResult_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_GetResult_Response__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_GetResult_Response__Sequence__fini(nail_msgs__action__CureRegion_GetResult_Response__Sequence * array)
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
      nail_msgs__action__CureRegion_GetResult_Response__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_GetResult_Response__Sequence *
nail_msgs__action__CureRegion_GetResult_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Response__Sequence * array = (nail_msgs__action__CureRegion_GetResult_Response__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_GetResult_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_GetResult_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_GetResult_Response__Sequence__destroy(nail_msgs__action__CureRegion_GetResult_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_GetResult_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_GetResult_Response__Sequence__are_equal(const nail_msgs__action__CureRegion_GetResult_Response__Sequence * lhs, const nail_msgs__action__CureRegion_GetResult_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_GetResult_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_GetResult_Response__Sequence__copy(
  const nail_msgs__action__CureRegion_GetResult_Response__Sequence * input,
  nail_msgs__action__CureRegion_GetResult_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_GetResult_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_GetResult_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_GetResult_Response * data =
      (nail_msgs__action__CureRegion_GetResult_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_GetResult_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_GetResult_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_GetResult_Response__copy(
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
// #include "nail_msgs/action/detail/cure_region__functions.h"

bool
nail_msgs__action__CureRegion_GetResult_Event__init(nail_msgs__action__CureRegion_GetResult_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    nail_msgs__action__CureRegion_GetResult_Event__fini(msg);
    return false;
  }
  // request
  if (!nail_msgs__action__CureRegion_GetResult_Request__Sequence__init(&msg->request, 0)) {
    nail_msgs__action__CureRegion_GetResult_Event__fini(msg);
    return false;
  }
  // response
  if (!nail_msgs__action__CureRegion_GetResult_Response__Sequence__init(&msg->response, 0)) {
    nail_msgs__action__CureRegion_GetResult_Event__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__CureRegion_GetResult_Event__fini(nail_msgs__action__CureRegion_GetResult_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  nail_msgs__action__CureRegion_GetResult_Request__Sequence__fini(&msg->request);
  // response
  nail_msgs__action__CureRegion_GetResult_Response__Sequence__fini(&msg->response);
}

bool
nail_msgs__action__CureRegion_GetResult_Event__are_equal(const nail_msgs__action__CureRegion_GetResult_Event * lhs, const nail_msgs__action__CureRegion_GetResult_Event * rhs)
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
  if (!nail_msgs__action__CureRegion_GetResult_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__CureRegion_GetResult_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_GetResult_Event__copy(
  const nail_msgs__action__CureRegion_GetResult_Event * input,
  nail_msgs__action__CureRegion_GetResult_Event * output)
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
  if (!nail_msgs__action__CureRegion_GetResult_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__action__CureRegion_GetResult_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__CureRegion_GetResult_Event *
nail_msgs__action__CureRegion_GetResult_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Event * msg = (nail_msgs__action__CureRegion_GetResult_Event *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_GetResult_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_GetResult_Event));
  bool success = nail_msgs__action__CureRegion_GetResult_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_GetResult_Event__destroy(nail_msgs__action__CureRegion_GetResult_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_GetResult_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_GetResult_Event__Sequence__init(nail_msgs__action__CureRegion_GetResult_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_GetResult_Event)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_GetResult_Event *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_GetResult_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_GetResult_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_GetResult_Event__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_GetResult_Event__Sequence__fini(nail_msgs__action__CureRegion_GetResult_Event__Sequence * array)
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
      nail_msgs__action__CureRegion_GetResult_Event__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_GetResult_Event__Sequence *
nail_msgs__action__CureRegion_GetResult_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_GetResult_Event__Sequence * array = (nail_msgs__action__CureRegion_GetResult_Event__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_GetResult_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_GetResult_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_GetResult_Event__Sequence__destroy(nail_msgs__action__CureRegion_GetResult_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_GetResult_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_GetResult_Event__Sequence__are_equal(const nail_msgs__action__CureRegion_GetResult_Event__Sequence * lhs, const nail_msgs__action__CureRegion_GetResult_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_GetResult_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_GetResult_Event__Sequence__copy(
  const nail_msgs__action__CureRegion_GetResult_Event__Sequence * input,
  nail_msgs__action__CureRegion_GetResult_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_GetResult_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_GetResult_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_GetResult_Event * data =
      (nail_msgs__action__CureRegion_GetResult_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_GetResult_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_GetResult_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_GetResult_Event__copy(
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
// #include "nail_msgs/action/detail/cure_region__functions.h"

bool
nail_msgs__action__CureRegion_FeedbackMessage__init(nail_msgs__action__CureRegion_FeedbackMessage * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    nail_msgs__action__CureRegion_FeedbackMessage__fini(msg);
    return false;
  }
  // feedback
  if (!nail_msgs__action__CureRegion_Feedback__init(&msg->feedback)) {
    nail_msgs__action__CureRegion_FeedbackMessage__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__action__CureRegion_FeedbackMessage__fini(nail_msgs__action__CureRegion_FeedbackMessage * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // feedback
  nail_msgs__action__CureRegion_Feedback__fini(&msg->feedback);
}

bool
nail_msgs__action__CureRegion_FeedbackMessage__are_equal(const nail_msgs__action__CureRegion_FeedbackMessage * lhs, const nail_msgs__action__CureRegion_FeedbackMessage * rhs)
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
  if (!nail_msgs__action__CureRegion_Feedback__are_equal(
      &(lhs->feedback), &(rhs->feedback)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__action__CureRegion_FeedbackMessage__copy(
  const nail_msgs__action__CureRegion_FeedbackMessage * input,
  nail_msgs__action__CureRegion_FeedbackMessage * output)
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
  if (!nail_msgs__action__CureRegion_Feedback__copy(
      &(input->feedback), &(output->feedback)))
  {
    return false;
  }
  return true;
}

nail_msgs__action__CureRegion_FeedbackMessage *
nail_msgs__action__CureRegion_FeedbackMessage__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_FeedbackMessage * msg = (nail_msgs__action__CureRegion_FeedbackMessage *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_FeedbackMessage), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__action__CureRegion_FeedbackMessage));
  bool success = nail_msgs__action__CureRegion_FeedbackMessage__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__action__CureRegion_FeedbackMessage__destroy(nail_msgs__action__CureRegion_FeedbackMessage * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__action__CureRegion_FeedbackMessage__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__action__CureRegion_FeedbackMessage__Sequence__init(nail_msgs__action__CureRegion_FeedbackMessage__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_FeedbackMessage * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_FeedbackMessage)) {
      return false;
    }
    data = (nail_msgs__action__CureRegion_FeedbackMessage *)allocator.zero_allocate(size, sizeof(nail_msgs__action__CureRegion_FeedbackMessage), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__action__CureRegion_FeedbackMessage__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__action__CureRegion_FeedbackMessage__fini(&data[i - 1]);
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
nail_msgs__action__CureRegion_FeedbackMessage__Sequence__fini(nail_msgs__action__CureRegion_FeedbackMessage__Sequence * array)
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
      nail_msgs__action__CureRegion_FeedbackMessage__fini(&array->data[i]);
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

nail_msgs__action__CureRegion_FeedbackMessage__Sequence *
nail_msgs__action__CureRegion_FeedbackMessage__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__action__CureRegion_FeedbackMessage__Sequence * array = (nail_msgs__action__CureRegion_FeedbackMessage__Sequence *)allocator.allocate(sizeof(nail_msgs__action__CureRegion_FeedbackMessage__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__action__CureRegion_FeedbackMessage__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__action__CureRegion_FeedbackMessage__Sequence__destroy(nail_msgs__action__CureRegion_FeedbackMessage__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__action__CureRegion_FeedbackMessage__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__action__CureRegion_FeedbackMessage__Sequence__are_equal(const nail_msgs__action__CureRegion_FeedbackMessage__Sequence * lhs, const nail_msgs__action__CureRegion_FeedbackMessage__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__action__CureRegion_FeedbackMessage__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__action__CureRegion_FeedbackMessage__Sequence__copy(
  const nail_msgs__action__CureRegion_FeedbackMessage__Sequence * input,
  nail_msgs__action__CureRegion_FeedbackMessage__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__action__CureRegion_FeedbackMessage)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__action__CureRegion_FeedbackMessage);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__action__CureRegion_FeedbackMessage * data =
      (nail_msgs__action__CureRegion_FeedbackMessage *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__action__CureRegion_FeedbackMessage__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__action__CureRegion_FeedbackMessage__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__action__CureRegion_FeedbackMessage__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
