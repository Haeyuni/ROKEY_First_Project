// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from nail_msgs:msg/StiffnessMap.idl
// generated code does not contain a copyright notice

#include "nail_msgs/msg/detail/stiffness_map__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__msg__StiffnessMap__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xc2, 0x67, 0x11, 0x28, 0x08, 0xda, 0x8b, 0xeb,
      0xe7, 0xa5, 0xa6, 0x13, 0x78, 0x54, 0x89, 0x20,
      0xdb, 0xe0, 0x25, 0x1d, 0x24, 0xd8, 0x4c, 0x00,
      0x2d, 0x85, 0xf6, 0x25, 0xf0, 0xb5, 0x30, 0xe1,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "std_msgs/msg/detail/header__functions.h"
#include "geometry_msgs/msg/detail/point__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "nail_msgs/msg/detail/stiffness_point__functions.h"

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
static const rosidl_type_hash_t nail_msgs__msg__StiffnessPoint__EXPECTED_HASH = {1, {
    0x9c, 0x4c, 0xcb, 0xa1, 0x9e, 0xb0, 0x73, 0x38,
    0xaf, 0x1a, 0x63, 0x96, 0x84, 0x31, 0x7b, 0x50,
    0xb7, 0xb6, 0xeb, 0x58, 0xea, 0x9c, 0x0b, 0xea,
    0xac, 0x3e, 0xef, 0xd8, 0x14, 0x7d, 0xdf, 0xd8,
  }};
static const rosidl_type_hash_t std_msgs__msg__Header__EXPECTED_HASH = {1, {
    0xf4, 0x9f, 0xb3, 0xae, 0x2c, 0xf0, 0x70, 0xf7,
    0x93, 0x64, 0x5f, 0xf7, 0x49, 0x68, 0x3a, 0xc6,
    0xb0, 0x62, 0x03, 0xe4, 0x1c, 0x89, 0x1e, 0x17,
    0x70, 0x1b, 0x1c, 0xb5, 0x97, 0xce, 0x6a, 0x01,
  }};
#endif

static char nail_msgs__msg__StiffnessMap__TYPE_NAME[] = "nail_msgs/msg/StiffnessMap";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char geometry_msgs__msg__Point__TYPE_NAME[] = "geometry_msgs/msg/Point";
static char nail_msgs__msg__StiffnessPoint__TYPE_NAME[] = "nail_msgs/msg/StiffnessPoint";
static char std_msgs__msg__Header__TYPE_NAME[] = "std_msgs/msg/Header";

// Define type names, field names, and default values
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__header[] = "header";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__session_id[] = "session_id";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__frame_id[] = "frame_id";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__target_index[] = "target_index";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__points[] = "points";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__grid_pitch_mm[] = "grid_pitch_mm";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__hard_min_n_per_mm[] = "hard_min_n_per_mm";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__soft_max_n_per_mm[] = "soft_max_n_per_mm";
static char nail_msgs__msg__StiffnessMap__FIELD_NAME__created_at[] = "created_at";

static rosidl_runtime_c__type_description__Field nail_msgs__msg__StiffnessMap__FIELDS[] = {
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__session_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__frame_id, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__target_index, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__points, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {nail_msgs__msg__StiffnessPoint__TYPE_NAME, 28, 28},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__grid_pitch_mm, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__hard_min_n_per_mm, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__soft_max_n_per_mm, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessMap__FIELD_NAME__created_at, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__msg__StiffnessMap__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
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
nail_msgs__msg__StiffnessMap__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__msg__StiffnessMap__TYPE_NAME, 26, 26},
      {nail_msgs__msg__StiffnessMap__FIELDS, 9, 9},
    },
    {nail_msgs__msg__StiffnessMap__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__StiffnessPoint__EXPECTED_HASH, nail_msgs__msg__StiffnessPoint__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = nail_msgs__msg__StiffnessPoint__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "std_msgs/Header header\n"
  "string  session_id                    # E9007 \\xea\\xb2\\x80\\xec\\xa6\\x9d\\xec\\x9a\\xa9. \\xed\\x95\\x84\\xec\\x88\\x98.\n"
  "string  frame_id                      # target_1 \\xe2\\x80\\xa6\n"
  "int32   target_index\n"
  "\n"
  "nail_msgs/StiffnessPoint[] points\n"
  "float64 grid_pitch_mm\n"
  "\n"
  "# \\xed\\x8c\\x90\\xec\\xa0\\x95\\xec\\x97\\x90 \\xec\\x82\\xac\\xec\\x9a\\xa9\\xeb\\x90\\x9c \\xec\\x9e\\x84\\xea\\xb3\\x84\\xea\\xb0\\x92 (\\xec\\x82\\xac\\xed\\x9b\\x84 \\xec\\x9e\\xac\\xed\\x95\\xb4\\xec\\x84\\x9d \\xea\\xb0\\x80\\xeb\\x8a\\xa5\\xed\\x95\\x98\\xeb\\x8f\\x84\\xeb\\xa1\\x9d \\xed\\x95\\xa8\\xea\\xbb\\x98 \\xec\\xa0\\x80\\xec\\x9e\\xa5)\n"
  "float64 hard_min_n_per_mm\n"
  "float64 soft_max_n_per_mm\n"
  "\n"
  "builtin_interfaces/Time created_at";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__msg__StiffnessMap__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__msg__StiffnessMap__TYPE_NAME, 26, 26},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 331, 331},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__msg__StiffnessMap__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__msg__StiffnessMap__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[3] = *nail_msgs__msg__StiffnessPoint__get_individual_type_description_source(NULL);
    sources[4] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
