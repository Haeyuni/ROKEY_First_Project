// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from nail_msgs:srv/ExtractBoundary.idl
// generated code does not contain a copyright notice

#include "nail_msgs/srv/detail/extract_boundary__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ExtractBoundary__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xa9, 0xae, 0x46, 0x2c, 0xb6, 0xb4, 0x1e, 0xb8,
      0x9e, 0x7a, 0x82, 0xd1, 0x48, 0x31, 0x5b, 0x90,
      0x50, 0x71, 0x22, 0xa3, 0x1b, 0xf4, 0x68, 0x35,
      0x59, 0xf3, 0xce, 0x3c, 0x4a, 0x27, 0xde, 0x08,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ExtractBoundary_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x44, 0x09, 0x96, 0x83, 0xde, 0x5f, 0xcf, 0x99,
      0x90, 0x47, 0x7f, 0x3e, 0x78, 0x8f, 0xb1, 0x69,
      0x1a, 0xca, 0x6e, 0x5d, 0xf7, 0xfc, 0x32, 0x31,
      0xeb, 0x65, 0x8b, 0x5e, 0xd8, 0xce, 0xca, 0x5f,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ExtractBoundary_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xca, 0x79, 0x72, 0xd4, 0x0b, 0x99, 0x1b, 0x47,
      0xf4, 0xf3, 0x95, 0xa9, 0x54, 0x6d, 0x20, 0xe2,
      0xc3, 0x19, 0xae, 0xf8, 0x27, 0xcc, 0x66, 0x38,
      0x42, 0xbd, 0x6e, 0x96, 0x16, 0x7d, 0xdc, 0xa8,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ExtractBoundary_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x04, 0xa8, 0xd7, 0x95, 0xfc, 0x7c, 0xde, 0x44,
      0xfe, 0xac, 0x5d, 0x96, 0x73, 0xe8, 0xf2, 0xa7,
      0xbc, 0xed, 0x2b, 0xe0, 0x59, 0x08, 0xac, 0x45,
      0xbc, 0x18, 0x5a, 0x32, 0x75, 0x76, 0x1e, 0x70,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "nail_msgs/msg/detail/stiffness_point__functions.h"
#include "nail_msgs/msg/detail/stiffness_map__functions.h"
#include "nail_msgs/msg/detail/boundary_region__functions.h"
#include "service_msgs/msg/detail/service_event_info__functions.h"
#include "geometry_msgs/msg/detail/point__functions.h"
#include "nail_msgs/msg/detail/error_code__functions.h"
#include "std_msgs/msg/detail/header__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t geometry_msgs__msg__Point__EXPECTED_HASH = {1, {
    0x69, 0x63, 0x08, 0x48, 0x42, 0xa9, 0xb0, 0x44,
    0x94, 0xd6, 0xb2, 0x94, 0x1d, 0x11, 0x44, 0x47,
    0x08, 0xd8, 0x92, 0xda, 0x2f, 0x4b, 0x09, 0x84,
    0x3b, 0x9c, 0x43, 0xf4, 0x2a, 0x7f, 0x68, 0x81,
  }};
static const rosidl_type_hash_t nail_msgs__msg__BoundaryRegion__EXPECTED_HASH = {1, {
    0xa7, 0x2a, 0x3f, 0x12, 0xab, 0x90, 0x72, 0x55,
    0xf1, 0xf4, 0x2c, 0x99, 0xd1, 0xb5, 0x58, 0x34,
    0x93, 0x96, 0x9d, 0xf8, 0x30, 0xca, 0x49, 0x93,
    0x39, 0x02, 0xc8, 0x96, 0x1b, 0xa5, 0x3a, 0x85,
  }};
static const rosidl_type_hash_t nail_msgs__msg__ErrorCode__EXPECTED_HASH = {1, {
    0xd7, 0xf9, 0xd3, 0xa8, 0xbc, 0x9c, 0x58, 0x91,
    0xe6, 0x60, 0x07, 0x64, 0x8f, 0x24, 0x97, 0x2f,
    0x79, 0xc1, 0xcd, 0xf3, 0xb8, 0x5b, 0x82, 0x4a,
    0xf4, 0xb3, 0xe9, 0xf6, 0x37, 0x0d, 0x50, 0x1e,
  }};
static const rosidl_type_hash_t nail_msgs__msg__StiffnessMap__EXPECTED_HASH = {1, {
    0xc2, 0x67, 0x11, 0x28, 0x08, 0xda, 0x8b, 0xeb,
    0xe7, 0xa5, 0xa6, 0x13, 0x78, 0x54, 0x89, 0x20,
    0xdb, 0xe0, 0x25, 0x1d, 0x24, 0xd8, 0x4c, 0x00,
    0x2d, 0x85, 0xf6, 0x25, 0xf0, 0xb5, 0x30, 0xe1,
  }};
static const rosidl_type_hash_t nail_msgs__msg__StiffnessPoint__EXPECTED_HASH = {1, {
    0x9c, 0x4c, 0xcb, 0xa1, 0x9e, 0xb0, 0x73, 0x38,
    0xaf, 0x1a, 0x63, 0x96, 0x84, 0x31, 0x7b, 0x50,
    0xb7, 0xb6, 0xeb, 0x58, 0xea, 0x9c, 0x0b, 0xea,
    0xac, 0x3e, 0xef, 0xd8, 0x14, 0x7d, 0xdf, 0xd8,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
static const rosidl_type_hash_t std_msgs__msg__Header__EXPECTED_HASH = {1, {
    0xf4, 0x9f, 0xb3, 0xae, 0x2c, 0xf0, 0x70, 0xf7,
    0x93, 0x64, 0x5f, 0xf7, 0x49, 0x68, 0x3a, 0xc6,
    0xb0, 0x62, 0x03, 0xe4, 0x1c, 0x89, 0x1e, 0x17,
    0x70, 0x1b, 0x1c, 0xb5, 0x97, 0xce, 0x6a, 0x01,
  }};
#endif

static char nail_msgs__srv__ExtractBoundary__TYPE_NAME[] = "nail_msgs/srv/ExtractBoundary";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char geometry_msgs__msg__Point__TYPE_NAME[] = "geometry_msgs/msg/Point";
static char nail_msgs__msg__BoundaryRegion__TYPE_NAME[] = "nail_msgs/msg/BoundaryRegion";
static char nail_msgs__msg__ErrorCode__TYPE_NAME[] = "nail_msgs/msg/ErrorCode";
static char nail_msgs__msg__StiffnessMap__TYPE_NAME[] = "nail_msgs/msg/StiffnessMap";
static char nail_msgs__msg__StiffnessPoint__TYPE_NAME[] = "nail_msgs/msg/StiffnessPoint";
static char nail_msgs__srv__ExtractBoundary_Event__TYPE_NAME[] = "nail_msgs/srv/ExtractBoundary_Event";
static char nail_msgs__srv__ExtractBoundary_Request__TYPE_NAME[] = "nail_msgs/srv/ExtractBoundary_Request";
static char nail_msgs__srv__ExtractBoundary_Response__TYPE_NAME[] = "nail_msgs/srv/ExtractBoundary_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";
static char std_msgs__msg__Header__TYPE_NAME[] = "std_msgs/msg/Header";

// Define type names, field names, and default values
static char nail_msgs__srv__ExtractBoundary__FIELD_NAME__request_message[] = "request_message";
static char nail_msgs__srv__ExtractBoundary__FIELD_NAME__response_message[] = "response_message";
static char nail_msgs__srv__ExtractBoundary__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ExtractBoundary__FIELDS[] = {
  {
    {nail_msgs__srv__ExtractBoundary__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__srv__ExtractBoundary_Request__TYPE_NAME, 37, 37},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__srv__ExtractBoundary_Response__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__srv__ExtractBoundary_Event__TYPE_NAME, 35, 35},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__srv__ExtractBoundary__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__TYPE_NAME, 26, 26},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Event__TYPE_NAME, 35, 35},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Request__TYPE_NAME, 37, 37},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Response__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ExtractBoundary__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ExtractBoundary__TYPE_NAME, 29, 29},
      {nail_msgs__srv__ExtractBoundary__FIELDS, 3, 3},
    },
    {nail_msgs__srv__ExtractBoundary__REFERENCED_TYPE_DESCRIPTIONS, 11, 11},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__BoundaryRegion__EXPECTED_HASH, nail_msgs__msg__BoundaryRegion__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = nail_msgs__msg__BoundaryRegion__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__ErrorCode__EXPECTED_HASH, nail_msgs__msg__ErrorCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = nail_msgs__msg__ErrorCode__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__StiffnessMap__EXPECTED_HASH, nail_msgs__msg__StiffnessMap__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = nail_msgs__msg__StiffnessMap__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__StiffnessPoint__EXPECTED_HASH, nail_msgs__msg__StiffnessPoint__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[5].fields = nail_msgs__msg__StiffnessPoint__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[6].fields = nail_msgs__srv__ExtractBoundary_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[7].fields = nail_msgs__srv__ExtractBoundary_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[8].fields = nail_msgs__srv__ExtractBoundary_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[9].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[10].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char nail_msgs__srv__ExtractBoundary_Request__FIELD_NAME__map[] = "map";
static char nail_msgs__srv__ExtractBoundary_Request__FIELD_NAME__boundary_offset_mm[] = "boundary_offset_mm";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ExtractBoundary_Request__FIELDS[] = {
  {
    {nail_msgs__srv__ExtractBoundary_Request__FIELD_NAME__map, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__msg__StiffnessMap__TYPE_NAME, 26, 26},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Request__FIELD_NAME__boundary_offset_mm, 18, 18},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__srv__ExtractBoundary_Request__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__TYPE_NAME, 26, 26},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ExtractBoundary_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ExtractBoundary_Request__TYPE_NAME, 37, 37},
      {nail_msgs__srv__ExtractBoundary_Request__FIELDS, 2, 2},
    },
    {nail_msgs__srv__ExtractBoundary_Request__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__StiffnessMap__EXPECTED_HASH, nail_msgs__msg__StiffnessMap__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = nail_msgs__msg__StiffnessMap__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__StiffnessPoint__EXPECTED_HASH, nail_msgs__msg__StiffnessPoint__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = nail_msgs__msg__StiffnessPoint__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char nail_msgs__srv__ExtractBoundary_Response__FIELD_NAME__success[] = "success";
static char nail_msgs__srv__ExtractBoundary_Response__FIELD_NAME__region[] = "region";
static char nail_msgs__srv__ExtractBoundary_Response__FIELD_NAME__error[] = "error";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ExtractBoundary_Response__FIELDS[] = {
  {
    {nail_msgs__srv__ExtractBoundary_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Response__FIELD_NAME__region, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__msg__BoundaryRegion__TYPE_NAME, 28, 28},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Response__FIELD_NAME__error, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__srv__ExtractBoundary_Response__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ExtractBoundary_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ExtractBoundary_Response__TYPE_NAME, 38, 38},
      {nail_msgs__srv__ExtractBoundary_Response__FIELDS, 3, 3},
    },
    {nail_msgs__srv__ExtractBoundary_Response__REFERENCED_TYPE_DESCRIPTIONS, 3, 3},
  };
  if (!constructed) {
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__BoundaryRegion__EXPECTED_HASH, nail_msgs__msg__BoundaryRegion__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = nail_msgs__msg__BoundaryRegion__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__ErrorCode__EXPECTED_HASH, nail_msgs__msg__ErrorCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = nail_msgs__msg__ErrorCode__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char nail_msgs__srv__ExtractBoundary_Event__FIELD_NAME__info[] = "info";
static char nail_msgs__srv__ExtractBoundary_Event__FIELD_NAME__request[] = "request";
static char nail_msgs__srv__ExtractBoundary_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ExtractBoundary_Event__FIELDS[] = {
  {
    {nail_msgs__srv__ExtractBoundary_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {nail_msgs__srv__ExtractBoundary_Request__TYPE_NAME, 37, 37},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {nail_msgs__srv__ExtractBoundary_Response__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__srv__ExtractBoundary_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__TYPE_NAME, 26, 26},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Request__TYPE_NAME, 37, 37},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ExtractBoundary_Response__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ExtractBoundary_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ExtractBoundary_Event__TYPE_NAME, 35, 35},
      {nail_msgs__srv__ExtractBoundary_Event__FIELDS, 3, 3},
    },
    {nail_msgs__srv__ExtractBoundary_Event__REFERENCED_TYPE_DESCRIPTIONS, 10, 10},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__BoundaryRegion__EXPECTED_HASH, nail_msgs__msg__BoundaryRegion__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = nail_msgs__msg__BoundaryRegion__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__ErrorCode__EXPECTED_HASH, nail_msgs__msg__ErrorCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = nail_msgs__msg__ErrorCode__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__StiffnessMap__EXPECTED_HASH, nail_msgs__msg__StiffnessMap__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = nail_msgs__msg__StiffnessMap__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__StiffnessPoint__EXPECTED_HASH, nail_msgs__msg__StiffnessPoint__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[5].fields = nail_msgs__msg__StiffnessPoint__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[6].fields = nail_msgs__srv__ExtractBoundary_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[7].fields = nail_msgs__srv__ExtractBoundary_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[8].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[9].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# \\xea\\xb3\\x84\\xec\\x82\\xb0\\xeb\\xa7\\x8c \\xec\\x88\\x98\\xed\\x96\\x89. \\xeb\\xa1\\x9c\\xeb\\xb4\\x87\\xec\\x9d\\x84 \\xec\\x9b\\x80\\xec\\xa7\\x81\\xec\\x9d\\xb4\\xec\\xa7\\x80 \\xec\\x95\\x8a\\xec\\x9c\\xbc\\xeb\\xaf\\x80\\xeb\\xa1\\x9c \\xec\\x84\\x9c\\xeb\\xb9\\x84\\xec\\x8a\\xa4\\xeb\\xa1\\x9c \\xea\\xb5\\xac\\xed\\x98\\x84.\n"
  "\n"
  "nail_msgs/StiffnessMap map\n"
  "float64 boundary_offset_mm\n"
  "---\n"
  "bool                     success\n"
  "nail_msgs/BoundaryRegion region\n"
  "nail_msgs/ErrorCode      error";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ExtractBoundary__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ExtractBoundary__TYPE_NAME, 29, 29},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 188, 188},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ExtractBoundary_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ExtractBoundary_Request__TYPE_NAME, 37, 37},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ExtractBoundary_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ExtractBoundary_Response__TYPE_NAME, 38, 38},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ExtractBoundary_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ExtractBoundary_Event__TYPE_NAME, 35, 35},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ExtractBoundary__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[12];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 12, 12};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ExtractBoundary__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[3] = *nail_msgs__msg__BoundaryRegion__get_individual_type_description_source(NULL);
    sources[4] = *nail_msgs__msg__ErrorCode__get_individual_type_description_source(NULL);
    sources[5] = *nail_msgs__msg__StiffnessMap__get_individual_type_description_source(NULL);
    sources[6] = *nail_msgs__msg__StiffnessPoint__get_individual_type_description_source(NULL);
    sources[7] = *nail_msgs__srv__ExtractBoundary_Event__get_individual_type_description_source(NULL);
    sources[8] = *nail_msgs__srv__ExtractBoundary_Request__get_individual_type_description_source(NULL);
    sources[9] = *nail_msgs__srv__ExtractBoundary_Response__get_individual_type_description_source(NULL);
    sources[10] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    sources[11] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ExtractBoundary_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ExtractBoundary_Request__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[3] = *nail_msgs__msg__StiffnessMap__get_individual_type_description_source(NULL);
    sources[4] = *nail_msgs__msg__StiffnessPoint__get_individual_type_description_source(NULL);
    sources[5] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ExtractBoundary_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[4];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 4, 4};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ExtractBoundary_Response__get_individual_type_description_source(NULL),
    sources[1] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[2] = *nail_msgs__msg__BoundaryRegion__get_individual_type_description_source(NULL);
    sources[3] = *nail_msgs__msg__ErrorCode__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ExtractBoundary_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[11];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 11, 11};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ExtractBoundary_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[3] = *nail_msgs__msg__BoundaryRegion__get_individual_type_description_source(NULL);
    sources[4] = *nail_msgs__msg__ErrorCode__get_individual_type_description_source(NULL);
    sources[5] = *nail_msgs__msg__StiffnessMap__get_individual_type_description_source(NULL);
    sources[6] = *nail_msgs__msg__StiffnessPoint__get_individual_type_description_source(NULL);
    sources[7] = *nail_msgs__srv__ExtractBoundary_Request__get_individual_type_description_source(NULL);
    sources[8] = *nail_msgs__srv__ExtractBoundary_Response__get_individual_type_description_source(NULL);
    sources[9] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    sources[10] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
