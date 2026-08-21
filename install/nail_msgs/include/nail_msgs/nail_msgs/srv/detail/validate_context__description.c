// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from nail_msgs:srv/ValidateContext.idl
// generated code does not contain a copyright notice

#include "nail_msgs/srv/detail/validate_context__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ValidateContext__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x2f, 0x4c, 0x97, 0x67, 0x0b, 0xa9, 0xbb, 0xd1,
      0xac, 0xe3, 0xea, 0x35, 0xbd, 0x83, 0x07, 0xb8,
      0xd7, 0x24, 0x7e, 0xd9, 0xcf, 0x52, 0x4c, 0x88,
      0x64, 0x8f, 0x5b, 0x43, 0x28, 0x6a, 0xc6, 0xb6,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ValidateContext_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x7f, 0xcb, 0x98, 0x77, 0xba, 0x73, 0x3b, 0x73,
      0x8a, 0x4f, 0xda, 0x66, 0xcb, 0x7b, 0xbf, 0xdb,
      0x78, 0xe9, 0xfe, 0x0a, 0x9d, 0x9c, 0x52, 0x14,
      0xb2, 0xb3, 0x9b, 0x8b, 0xa9, 0x03, 0xa1, 0x31,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ValidateContext_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xd5, 0x54, 0x18, 0x54, 0x7c, 0x4e, 0x22, 0xad,
      0x2c, 0x40, 0xb9, 0x00, 0x75, 0xa4, 0x8f, 0xf3,
      0x5a, 0xee, 0x4f, 0x50, 0xcf, 0x65, 0x4b, 0x75,
      0xf1, 0x27, 0x52, 0x2a, 0x70, 0x4f, 0xc5, 0xba,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__srv__ValidateContext_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xbe, 0x5e, 0xfb, 0x53, 0xf0, 0x78, 0x20, 0xc9,
      0xd1, 0x04, 0x82, 0x8a, 0xb8, 0x51, 0x3a, 0xc7,
      0x14, 0xca, 0x1b, 0xa9, 0xc3, 0x4a, 0x55, 0xc7,
      0xed, 0x60, 0xd2, 0x80, 0x84, 0x43, 0x9a, 0xc8,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "nail_msgs/msg/detail/error_code__functions.h"
#include "service_msgs/msg/detail/service_event_info__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t nail_msgs__msg__ErrorCode__EXPECTED_HASH = {1, {
    0xd7, 0xf9, 0xd3, 0xa8, 0xbc, 0x9c, 0x58, 0x91,
    0xe6, 0x60, 0x07, 0x64, 0x8f, 0x24, 0x97, 0x2f,
    0x79, 0xc1, 0xcd, 0xf3, 0xb8, 0x5b, 0x82, 0x4a,
    0xf4, 0xb3, 0xe9, 0xf6, 0x37, 0x0d, 0x50, 0x1e,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char nail_msgs__srv__ValidateContext__TYPE_NAME[] = "nail_msgs/srv/ValidateContext";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char nail_msgs__msg__ErrorCode__TYPE_NAME[] = "nail_msgs/msg/ErrorCode";
static char nail_msgs__srv__ValidateContext_Event__TYPE_NAME[] = "nail_msgs/srv/ValidateContext_Event";
static char nail_msgs__srv__ValidateContext_Request__TYPE_NAME[] = "nail_msgs/srv/ValidateContext_Request";
static char nail_msgs__srv__ValidateContext_Response__TYPE_NAME[] = "nail_msgs/srv/ValidateContext_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char nail_msgs__srv__ValidateContext__FIELD_NAME__request_message[] = "request_message";
static char nail_msgs__srv__ValidateContext__FIELD_NAME__response_message[] = "response_message";
static char nail_msgs__srv__ValidateContext__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ValidateContext__FIELDS[] = {
  {
    {nail_msgs__srv__ValidateContext__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__srv__ValidateContext_Request__TYPE_NAME, 37, 37},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__srv__ValidateContext_Response__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__srv__ValidateContext_Event__TYPE_NAME, 35, 35},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__srv__ValidateContext__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Event__TYPE_NAME, 35, 35},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Request__TYPE_NAME, 37, 37},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Response__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ValidateContext__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ValidateContext__TYPE_NAME, 29, 29},
      {nail_msgs__srv__ValidateContext__FIELDS, 3, 3},
    },
    {nail_msgs__srv__ValidateContext__REFERENCED_TYPE_DESCRIPTIONS, 6, 6},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__ErrorCode__EXPECTED_HASH, nail_msgs__msg__ErrorCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = nail_msgs__msg__ErrorCode__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = nail_msgs__srv__ValidateContext_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = nail_msgs__srv__ValidateContext_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[4].fields = nail_msgs__srv__ValidateContext_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[5].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char nail_msgs__srv__ValidateContext_Request__FIELD_NAME__session_id[] = "session_id";
static char nail_msgs__srv__ValidateContext_Request__FIELD_NAME__required_tool[] = "required_tool";
static char nail_msgs__srv__ValidateContext_Request__FIELD_NAME__require_map[] = "require_map";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ValidateContext_Request__FIELDS[] = {
  {
    {nail_msgs__srv__ValidateContext_Request__FIELD_NAME__session_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Request__FIELD_NAME__required_tool, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Request__FIELD_NAME__require_map, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ValidateContext_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ValidateContext_Request__TYPE_NAME, 37, 37},
      {nail_msgs__srv__ValidateContext_Request__FIELDS, 3, 3},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char nail_msgs__srv__ValidateContext_Response__FIELD_NAME__success[] = "success";
static char nail_msgs__srv__ValidateContext_Response__FIELD_NAME__error[] = "error";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ValidateContext_Response__FIELDS[] = {
  {
    {nail_msgs__srv__ValidateContext_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Response__FIELD_NAME__error, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__srv__ValidateContext_Response__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ValidateContext_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ValidateContext_Response__TYPE_NAME, 38, 38},
      {nail_msgs__srv__ValidateContext_Response__FIELDS, 2, 2},
    },
    {nail_msgs__srv__ValidateContext_Response__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&nail_msgs__msg__ErrorCode__EXPECTED_HASH, nail_msgs__msg__ErrorCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = nail_msgs__msg__ErrorCode__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char nail_msgs__srv__ValidateContext_Event__FIELD_NAME__info[] = "info";
static char nail_msgs__srv__ValidateContext_Event__FIELD_NAME__request[] = "request";
static char nail_msgs__srv__ValidateContext_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field nail_msgs__srv__ValidateContext_Event__FIELDS[] = {
  {
    {nail_msgs__srv__ValidateContext_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {nail_msgs__srv__ValidateContext_Request__TYPE_NAME, 37, 37},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {nail_msgs__srv__ValidateContext_Response__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__srv__ValidateContext_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Request__TYPE_NAME, 37, 37},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__srv__ValidateContext_Response__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__srv__ValidateContext_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__srv__ValidateContext_Event__TYPE_NAME, 35, 35},
      {nail_msgs__srv__ValidateContext_Event__FIELDS, 3, 3},
    },
    {nail_msgs__srv__ValidateContext_Event__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__ErrorCode__EXPECTED_HASH, nail_msgs__msg__ErrorCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = nail_msgs__msg__ErrorCode__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = nail_msgs__srv__ValidateContext_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = nail_msgs__srv__ValidateContext_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# \\xeb\\xaa\\xa8\\xeb\\x93\\xa0 \\xea\\xb0\\x80\\xea\\xb3\\xb5 \\xeb\\x85\\xb8\\xeb\\x93\\x9c\\xea\\xb0\\x80 goal \\xec\\x88\\x98\\xeb\\x9d\\xbd \\xec\\xa0\\x84\\xec\\x97\\x90 \\xed\\x98\\xb8\\xec\\xb6\\x9c\\xed\\x95\\x98\\xeb\\x8a\\x94 \\xea\\xb3\\xb5\\xed\\x86\\xb5 \\xec\\x82\\xac\\xec\\xa0\\x84\\xea\\xb2\\x80\\xec\\xa6\\x9d.\n"
  "# \\xea\\xb0\\x9c\\xeb\\xb0\\x9c \\xeb\\xaa\\x85\\xec\\x84\\xb8\\xec\\x84\\x9c \\xc2\\xa73.3 \\xeb\\x8c\\x80\\xec\\x9d\\x91.\n"
  "\n"
  "string session_id\n"
  "string required_tool\n"
  "bool   require_map\n"
  "---\n"
  "bool                success\n"
  "nail_msgs/ErrorCode error      # E9007 / E7005 / E9008 \\xec\\xa4\\x91 \\xed\\x95\\x98\\xeb\\x82\\x98";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ValidateContext__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ValidateContext__TYPE_NAME, 29, 29},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 206, 206},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ValidateContext_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ValidateContext_Request__TYPE_NAME, 37, 37},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ValidateContext_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ValidateContext_Response__TYPE_NAME, 38, 38},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__srv__ValidateContext_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__srv__ValidateContext_Event__TYPE_NAME, 35, 35},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ValidateContext__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[7];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 7, 7};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ValidateContext__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *nail_msgs__msg__ErrorCode__get_individual_type_description_source(NULL);
    sources[3] = *nail_msgs__srv__ValidateContext_Event__get_individual_type_description_source(NULL);
    sources[4] = *nail_msgs__srv__ValidateContext_Request__get_individual_type_description_source(NULL);
    sources[5] = *nail_msgs__srv__ValidateContext_Response__get_individual_type_description_source(NULL);
    sources[6] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ValidateContext_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ValidateContext_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ValidateContext_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ValidateContext_Response__get_individual_type_description_source(NULL),
    sources[1] = *nail_msgs__msg__ErrorCode__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__srv__ValidateContext_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__srv__ValidateContext_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *nail_msgs__msg__ErrorCode__get_individual_type_description_source(NULL);
    sources[3] = *nail_msgs__srv__ValidateContext_Request__get_individual_type_description_source(NULL);
    sources[4] = *nail_msgs__srv__ValidateContext_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
