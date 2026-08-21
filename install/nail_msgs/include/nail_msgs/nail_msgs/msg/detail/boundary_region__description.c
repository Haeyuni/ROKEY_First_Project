// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from nail_msgs:msg/BoundaryRegion.idl
// generated code does not contain a copyright notice

#include "nail_msgs/msg/detail/boundary_region__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__msg__BoundaryRegion__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xa7, 0x2a, 0x3f, 0x12, 0xab, 0x90, 0x72, 0x55,
      0xf1, 0xf4, 0x2c, 0x99, 0xd1, 0xb5, 0x58, 0x34,
      0x93, 0x96, 0x9d, 0xf8, 0x30, 0xca, 0x49, 0x93,
      0x39, 0x02, 0xc8, 0x96, 0x1b, 0xa5, 0x3a, 0x85,
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

static char nail_msgs__msg__BoundaryRegion__TYPE_NAME[] = "nail_msgs/msg/BoundaryRegion";
static char geometry_msgs__msg__Point__TYPE_NAME[] = "geometry_msgs/msg/Point";

// Define type names, field names, and default values
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__session_id[] = "session_id";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__frame_id[] = "frame_id";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__target_index[] = "target_index";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__allowed_polygon[] = "allowed_polygon";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__forbidden_polygon[] = "forbidden_polygon";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__coat_polygon[] = "coat_polygon";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__boundary_offset_mm[] = "boundary_offset_mm";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__repeat_deviation_mm[] = "repeat_deviation_mm";
static char nail_msgs__msg__BoundaryRegion__FIELD_NAME__reliable[] = "reliable";

static rosidl_runtime_c__type_description__Field nail_msgs__msg__BoundaryRegion__FIELDS[] = {
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__session_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__frame_id, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__target_index, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__allowed_polygon, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__forbidden_polygon, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__coat_polygon, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__boundary_offset_mm, 18, 18},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__repeat_deviation_mm, 19, 19},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__BoundaryRegion__FIELD_NAME__reliable, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__msg__BoundaryRegion__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__msg__BoundaryRegion__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__msg__BoundaryRegion__TYPE_NAME, 28, 28},
      {nail_msgs__msg__BoundaryRegion__FIELDS, 9, 9},
    },
    {nail_msgs__msg__BoundaryRegion__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string  session_id\n"
  "string  frame_id\n"
  "int32   target_index\n"
  "\n"
  "geometry_msgs/Point[] allowed_polygon    # \\xea\\xb0\\x80\\xea\\xb3\\xb5 \\xed\\x97\\x88\\xec\\x9a\\xa9\n"
  "geometry_msgs/Point[] forbidden_polygon  # \\xea\\xb0\\x80\\xea\\xb3\\xb5 \\xea\\xb8\\x88\\xec\\xa7\\x80\n"
  "geometry_msgs/Point[] coat_polygon       # \\xeb\\x8f\\x84\\xed\\x8f\\xac \\xec\\x98\\x81\\xec\\x97\\xad (\\xed\\x97\\x88\\xec\\x9a\\xa9\\xec\\x97\\x90\\xec\\x84\\x9c offset \\xec\\x95\\x88\\xec\\xaa\\xbd)\n"
  "\n"
  "float64 boundary_offset_mm\n"
  "float64 repeat_deviation_mm              # E1005 \\xed\\x8c\\x90\\xec\\xa0\\x95 \\xea\\xb7\\xbc\\xea\\xb1\\xb0\n"
  "bool    reliable";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__msg__BoundaryRegion__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__msg__BoundaryRegion__TYPE_NAME, 28, 28},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 322, 322},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__msg__BoundaryRegion__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__msg__BoundaryRegion__get_individual_type_description_source(NULL),
    sources[1] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
