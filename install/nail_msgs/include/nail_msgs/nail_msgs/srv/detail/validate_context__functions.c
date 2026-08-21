// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from nail_msgs:srv/ValidateContext.idl
// generated code does not contain a copyright notice
#include "nail_msgs/srv/detail/validate_context__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `session_id`
// Member `required_tool`
#include "rosidl_runtime_c/string_functions.h"

bool
nail_msgs__srv__ValidateContext_Request__init(nail_msgs__srv__ValidateContext_Request * msg)
{
  if (!msg) {
    return false;
  }
  // session_id
  if (!rosidl_runtime_c__String__init(&msg->session_id)) {
    nail_msgs__srv__ValidateContext_Request__fini(msg);
    return false;
  }
  // required_tool
  if (!rosidl_runtime_c__String__init(&msg->required_tool)) {
    nail_msgs__srv__ValidateContext_Request__fini(msg);
    return false;
  }
  // require_map
  return true;
}

void
nail_msgs__srv__ValidateContext_Request__fini(nail_msgs__srv__ValidateContext_Request * msg)
{
  if (!msg) {
    return;
  }
  // session_id
  rosidl_runtime_c__String__fini(&msg->session_id);
  // required_tool
  rosidl_runtime_c__String__fini(&msg->required_tool);
  // require_map
}

bool
nail_msgs__srv__ValidateContext_Request__are_equal(const nail_msgs__srv__ValidateContext_Request * lhs, const nail_msgs__srv__ValidateContext_Request * rhs)
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
  // required_tool
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->required_tool), &(rhs->required_tool)))
  {
    return false;
  }
  // require_map
  if (lhs->require_map != rhs->require_map) {
    return false;
  }
  return true;
}

bool
nail_msgs__srv__ValidateContext_Request__copy(
  const nail_msgs__srv__ValidateContext_Request * input,
  nail_msgs__srv__ValidateContext_Request * output)
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
  // required_tool
  if (!rosidl_runtime_c__String__copy(
      &(input->required_tool), &(output->required_tool)))
  {
    return false;
  }
  // require_map
  output->require_map = input->require_map;
  return true;
}

nail_msgs__srv__ValidateContext_Request *
nail_msgs__srv__ValidateContext_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Request * msg = (nail_msgs__srv__ValidateContext_Request *)allocator.allocate(sizeof(nail_msgs__srv__ValidateContext_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__srv__ValidateContext_Request));
  bool success = nail_msgs__srv__ValidateContext_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__srv__ValidateContext_Request__destroy(nail_msgs__srv__ValidateContext_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__srv__ValidateContext_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__srv__ValidateContext_Request__Sequence__init(nail_msgs__srv__ValidateContext_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__srv__ValidateContext_Request)) {
      return false;
    }
    data = (nail_msgs__srv__ValidateContext_Request *)allocator.zero_allocate(size, sizeof(nail_msgs__srv__ValidateContext_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__srv__ValidateContext_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__srv__ValidateContext_Request__fini(&data[i - 1]);
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
nail_msgs__srv__ValidateContext_Request__Sequence__fini(nail_msgs__srv__ValidateContext_Request__Sequence * array)
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
      nail_msgs__srv__ValidateContext_Request__fini(&array->data[i]);
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

nail_msgs__srv__ValidateContext_Request__Sequence *
nail_msgs__srv__ValidateContext_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Request__Sequence * array = (nail_msgs__srv__ValidateContext_Request__Sequence *)allocator.allocate(sizeof(nail_msgs__srv__ValidateContext_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__srv__ValidateContext_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__srv__ValidateContext_Request__Sequence__destroy(nail_msgs__srv__ValidateContext_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__srv__ValidateContext_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__srv__ValidateContext_Request__Sequence__are_equal(const nail_msgs__srv__ValidateContext_Request__Sequence * lhs, const nail_msgs__srv__ValidateContext_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__srv__ValidateContext_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__srv__ValidateContext_Request__Sequence__copy(
  const nail_msgs__srv__ValidateContext_Request__Sequence * input,
  nail_msgs__srv__ValidateContext_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__srv__ValidateContext_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__srv__ValidateContext_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__srv__ValidateContext_Request * data =
      (nail_msgs__srv__ValidateContext_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__srv__ValidateContext_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__srv__ValidateContext_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__srv__ValidateContext_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `error`
#include "nail_msgs/msg/detail/error_code__functions.h"

bool
nail_msgs__srv__ValidateContext_Response__init(nail_msgs__srv__ValidateContext_Response * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // error
  if (!nail_msgs__msg__ErrorCode__init(&msg->error)) {
    nail_msgs__srv__ValidateContext_Response__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__srv__ValidateContext_Response__fini(nail_msgs__srv__ValidateContext_Response * msg)
{
  if (!msg) {
    return;
  }
  // success
  // error
  nail_msgs__msg__ErrorCode__fini(&msg->error);
}

bool
nail_msgs__srv__ValidateContext_Response__are_equal(const nail_msgs__srv__ValidateContext_Response * lhs, const nail_msgs__srv__ValidateContext_Response * rhs)
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
  return true;
}

bool
nail_msgs__srv__ValidateContext_Response__copy(
  const nail_msgs__srv__ValidateContext_Response * input,
  nail_msgs__srv__ValidateContext_Response * output)
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
  return true;
}

nail_msgs__srv__ValidateContext_Response *
nail_msgs__srv__ValidateContext_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Response * msg = (nail_msgs__srv__ValidateContext_Response *)allocator.allocate(sizeof(nail_msgs__srv__ValidateContext_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__srv__ValidateContext_Response));
  bool success = nail_msgs__srv__ValidateContext_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__srv__ValidateContext_Response__destroy(nail_msgs__srv__ValidateContext_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__srv__ValidateContext_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__srv__ValidateContext_Response__Sequence__init(nail_msgs__srv__ValidateContext_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__srv__ValidateContext_Response)) {
      return false;
    }
    data = (nail_msgs__srv__ValidateContext_Response *)allocator.zero_allocate(size, sizeof(nail_msgs__srv__ValidateContext_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__srv__ValidateContext_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__srv__ValidateContext_Response__fini(&data[i - 1]);
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
nail_msgs__srv__ValidateContext_Response__Sequence__fini(nail_msgs__srv__ValidateContext_Response__Sequence * array)
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
      nail_msgs__srv__ValidateContext_Response__fini(&array->data[i]);
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

nail_msgs__srv__ValidateContext_Response__Sequence *
nail_msgs__srv__ValidateContext_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Response__Sequence * array = (nail_msgs__srv__ValidateContext_Response__Sequence *)allocator.allocate(sizeof(nail_msgs__srv__ValidateContext_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__srv__ValidateContext_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__srv__ValidateContext_Response__Sequence__destroy(nail_msgs__srv__ValidateContext_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__srv__ValidateContext_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__srv__ValidateContext_Response__Sequence__are_equal(const nail_msgs__srv__ValidateContext_Response__Sequence * lhs, const nail_msgs__srv__ValidateContext_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__srv__ValidateContext_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__srv__ValidateContext_Response__Sequence__copy(
  const nail_msgs__srv__ValidateContext_Response__Sequence * input,
  nail_msgs__srv__ValidateContext_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__srv__ValidateContext_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__srv__ValidateContext_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__srv__ValidateContext_Response * data =
      (nail_msgs__srv__ValidateContext_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__srv__ValidateContext_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__srv__ValidateContext_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__srv__ValidateContext_Response__copy(
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
// #include "nail_msgs/srv/detail/validate_context__functions.h"

bool
nail_msgs__srv__ValidateContext_Event__init(nail_msgs__srv__ValidateContext_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    nail_msgs__srv__ValidateContext_Event__fini(msg);
    return false;
  }
  // request
  if (!nail_msgs__srv__ValidateContext_Request__Sequence__init(&msg->request, 0)) {
    nail_msgs__srv__ValidateContext_Event__fini(msg);
    return false;
  }
  // response
  if (!nail_msgs__srv__ValidateContext_Response__Sequence__init(&msg->response, 0)) {
    nail_msgs__srv__ValidateContext_Event__fini(msg);
    return false;
  }
  return true;
}

void
nail_msgs__srv__ValidateContext_Event__fini(nail_msgs__srv__ValidateContext_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  nail_msgs__srv__ValidateContext_Request__Sequence__fini(&msg->request);
  // response
  nail_msgs__srv__ValidateContext_Response__Sequence__fini(&msg->response);
}

bool
nail_msgs__srv__ValidateContext_Event__are_equal(const nail_msgs__srv__ValidateContext_Event * lhs, const nail_msgs__srv__ValidateContext_Event * rhs)
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
  if (!nail_msgs__srv__ValidateContext_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__srv__ValidateContext_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
nail_msgs__srv__ValidateContext_Event__copy(
  const nail_msgs__srv__ValidateContext_Event * input,
  nail_msgs__srv__ValidateContext_Event * output)
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
  if (!nail_msgs__srv__ValidateContext_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!nail_msgs__srv__ValidateContext_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

nail_msgs__srv__ValidateContext_Event *
nail_msgs__srv__ValidateContext_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Event * msg = (nail_msgs__srv__ValidateContext_Event *)allocator.allocate(sizeof(nail_msgs__srv__ValidateContext_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(nail_msgs__srv__ValidateContext_Event));
  bool success = nail_msgs__srv__ValidateContext_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
nail_msgs__srv__ValidateContext_Event__destroy(nail_msgs__srv__ValidateContext_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    nail_msgs__srv__ValidateContext_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
nail_msgs__srv__ValidateContext_Event__Sequence__init(nail_msgs__srv__ValidateContext_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(nail_msgs__srv__ValidateContext_Event)) {
      return false;
    }
    data = (nail_msgs__srv__ValidateContext_Event *)allocator.zero_allocate(size, sizeof(nail_msgs__srv__ValidateContext_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = nail_msgs__srv__ValidateContext_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        nail_msgs__srv__ValidateContext_Event__fini(&data[i - 1]);
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
nail_msgs__srv__ValidateContext_Event__Sequence__fini(nail_msgs__srv__ValidateContext_Event__Sequence * array)
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
      nail_msgs__srv__ValidateContext_Event__fini(&array->data[i]);
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

nail_msgs__srv__ValidateContext_Event__Sequence *
nail_msgs__srv__ValidateContext_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  nail_msgs__srv__ValidateContext_Event__Sequence * array = (nail_msgs__srv__ValidateContext_Event__Sequence *)allocator.allocate(sizeof(nail_msgs__srv__ValidateContext_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = nail_msgs__srv__ValidateContext_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
nail_msgs__srv__ValidateContext_Event__Sequence__destroy(nail_msgs__srv__ValidateContext_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    nail_msgs__srv__ValidateContext_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
nail_msgs__srv__ValidateContext_Event__Sequence__are_equal(const nail_msgs__srv__ValidateContext_Event__Sequence * lhs, const nail_msgs__srv__ValidateContext_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!nail_msgs__srv__ValidateContext_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
nail_msgs__srv__ValidateContext_Event__Sequence__copy(
  const nail_msgs__srv__ValidateContext_Event__Sequence * input,
  nail_msgs__srv__ValidateContext_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(nail_msgs__srv__ValidateContext_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(nail_msgs__srv__ValidateContext_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    nail_msgs__srv__ValidateContext_Event * data =
      (nail_msgs__srv__ValidateContext_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!nail_msgs__srv__ValidateContext_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          nail_msgs__srv__ValidateContext_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!nail_msgs__srv__ValidateContext_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
