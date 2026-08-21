// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from nail_msgs:msg/StiffnessPoint.idl
// generated code does not contain a copyright notice

#include "nail_msgs/msg/detail/stiffness_point__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__msg__StiffnessPoint__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x9c, 0x4c, 0xcb, 0xa1, 0x9e, 0xb0, 0x73, 0x38,
      0xaf, 0x1a, 0x63, 0x96, 0x84, 0x31, 0x7b, 0x50,
      0xb7, 0xb6, 0xeb, 0x58, 0xea, 0x9c, 0x0b, 0xea,
      0xac, 0x3e, 0xef, 0xd8, 0x14, 0x7d, 0xdf, 0xd8,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "geometry_msgs/msg/detail/point__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t geometry_msgs__msg__Point__EXPECTED_HASH = {1, {
    0x69, 0x63, 0x08, 0x48, 0x42, 0xa9, 0xb0, 0x44,
    0x94, 0xd6, 0xb2, 0x94, 0x1d, 0x11, 0x44, 0x47,
    0x08, 0xd8, 0x92, 0xda, 0x2f, 0x4b, 0x09, 0x84,
    0x3b, 0x9c, 0x43, 0xf4, 0x2a, 0x7f, 0x68, 0x81,
  }};
#endif

static char nail_msgs__msg__StiffnessPoint__TYPE_NAME[] = "nail_msgs/msg/StiffnessPoint";
static char geometry_msgs__msg__Point__TYPE_NAME[] = "geometry_msgs/msg/Point";

// Define type names, field names, and default values
static char nail_msgs__msg__StiffnessPoint__FIELD_NAME__position[] = "position";
static char nail_msgs__msg__StiffnessPoint__FIELD_NAME__stiffness_n_per_mm[] = "stiffness_n_per_mm";
static char nail_msgs__msg__StiffnessPoint__FIELD_NAME__peak_tensile_n[] = "peak_tensile_n";
static char nail_msgs__msg__StiffnessPoint__FIELD_NAME__hysteresis_ratio[] = "hysteresis_ratio";
static char nail_msgs__msg__StiffnessPoint__FIELD_NAME__lateral_force_n[] = "lateral_force_n";
static char nail_msgs__msg__StiffnessPoint__FIELD_NAME__valid[] = "valid";

static rosidl_runtime_c__type_description__Field nail_msgs__msg__StiffnessPoint__FIELDS[] = {
  {
    {nail_msgs__msg__StiffnessPoint__FIELD_NAME__position, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__FIELD_NAME__stiffness_n_per_mm, 18, 18},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__FIELD_NAME__peak_tensile_n, 14, 14},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__FIELD_NAME__hysteresis_ratio, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__FIELD_NAME__lateral_force_n, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__StiffnessPoint__FIELD_NAME__valid, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__msg__StiffnessPoint__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__msg__StiffnessPoint__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__msg__StiffnessPoint__TYPE_NAME, 28, 28},
      {nail_msgs__msg__StiffnessPoint__FIELDS, 6, 6},
    },
    {nail_msgs__msg__StiffnessPoint__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "geometry_msgs/Point position          # target_N \\xed\\x94\\x84\\xeb\\xa0\\x88\\xec\\x9e\\x84 \\xea\\xb8\\xb0\\xec\\xa4\\x80\n"
  "float64 stiffness_n_per_mm\n"
  "float64 peak_tensile_n                # \\xec\\x9d\\xb4\\xed\\x83\\x88 \\xec\\x8b\\x9c \\xec\\x9d\\xb8\\xec\\x9e\\xa5\\xeb\\xa0\\xa5 (\\xea\\xb2\\x80\\xec\\x82\\xac\\xec\\x97\\x90\\xec\\x84\\x9c \\xec\\x82\\xac\\xec\\x9a\\xa9)\n"
  "float64 hysteresis_ratio              # \\xec\\xa0\\x90\\xed\\x83\\x84\\xec\\x84\\xb1 \\xec\\xa7\\x80\\xed\\x91\\x9c, \\xea\\xb0\\x95\\xec\\x84\\xb1 \\xeb\\xb3\\xb4\\xec\\xa1\\xb0 \\xed\\x8c\\x90\\xeb\\xb3\\x84\\xec\\x9a\\xa9\n"
  "float64 lateral_force_n               # E1006 \\xed\\x8c\\x90\\xec\\xa0\\x95\\xec\\x9a\\xa9\n"
  "bool    valid";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__msg__StiffnessPoint__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__msg__StiffnessPoint__TYPE_NAME, 28, 28},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 264, 264},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__msg__StiffnessPoint__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__msg__StiffnessPoint__get_individual_type_description_source(NULL),
    sources[1] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
