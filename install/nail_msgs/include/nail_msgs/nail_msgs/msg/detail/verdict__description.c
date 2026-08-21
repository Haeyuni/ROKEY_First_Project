// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from nail_msgs:msg/Verdict.idl
// generated code does not contain a copyright notice

#include "nail_msgs/msg/detail/verdict__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__msg__Verdict__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xe6, 0x30, 0x90, 0x01, 0x71, 0x2d, 0xde, 0xda,
      0x23, 0xf8, 0x52, 0x50, 0xbb, 0x18, 0xa3, 0x4a,
      0x5a, 0x67, 0xfe, 0x68, 0xcf, 0x76, 0x54, 0x53,
      0x19, 0xae, 0xbf, 0x12, 0x5e, 0xc3, 0x56, 0x8b,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "nail_msgs/msg/detail/force_sample__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t nail_msgs__msg__ForceSample__EXPECTED_HASH = {1, {
    0xd4, 0xe2, 0xf5, 0x97, 0x31, 0x02, 0x97, 0x31,
    0x65, 0xa1, 0x80, 0x31, 0xe5, 0x96, 0xb0, 0x09,
    0xe6, 0x32, 0x54, 0x3d, 0x83, 0x53, 0xe8, 0xbf,
    0xdd, 0x4f, 0x8d, 0x92, 0xf9, 0x76, 0x43, 0x01,
  }};
#endif

static char nail_msgs__msg__Verdict__TYPE_NAME[] = "nail_msgs/msg/Verdict";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char nail_msgs__msg__ForceSample__TYPE_NAME[] = "nail_msgs/msg/ForceSample";

// Define type names, field names, and default values
static char nail_msgs__msg__Verdict__FIELD_NAME__session_id[] = "session_id";
static char nail_msgs__msg__Verdict__FIELD_NAME__target_index[] = "target_index";
static char nail_msgs__msg__Verdict__FIELD_NAME__layer_index[] = "layer_index";
static char nail_msgs__msg__Verdict__FIELD_NAME__probe_index[] = "probe_index";
static char nail_msgs__msg__Verdict__FIELD_NAME__result[] = "result";
static char nail_msgs__msg__Verdict__FIELD_NAME__tensile_n[] = "tensile_n";
static char nail_msgs__msg__Verdict__FIELD_NAME__stiffness_n_per_mm[] = "stiffness_n_per_mm";
static char nail_msgs__msg__Verdict__FIELD_NAME__error_code[] = "error_code";
static char nail_msgs__msg__Verdict__FIELD_NAME__waveform[] = "waveform";
static char nail_msgs__msg__Verdict__FIELD_NAME__measured_at[] = "measured_at";

static rosidl_runtime_c__type_description__Field nail_msgs__msg__Verdict__FIELDS[] = {
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__session_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__target_index, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__layer_index, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__probe_index, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__result, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__tensile_n, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__stiffness_n_per_mm, 18, 18},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__error_code, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__waveform, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {nail_msgs__msg__ForceSample__TYPE_NAME, 25, 25},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__Verdict__FIELD_NAME__measured_at, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription nail_msgs__msg__Verdict__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ForceSample__TYPE_NAME, 25, 25},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__msg__Verdict__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__msg__Verdict__TYPE_NAME, 21, 21},
      {nail_msgs__msg__Verdict__FIELDS, 10, 10},
    },
    {nail_msgs__msg__Verdict__REFERENCED_TYPE_DESCRIPTIONS, 2, 2},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&nail_msgs__msg__ForceSample__EXPECTED_HASH, nail_msgs__msg__ForceSample__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = nail_msgs__msg__ForceSample__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string  session_id\n"
  "int32   target_index\n"
  "int32   layer_index\n"
  "int32   probe_index\n"
  "\n"
  "string PASS      = \"PASS\"\n"
  "string FAIL      = \"FAIL\"\n"
  "string UNCERTAIN = \"UNCERTAIN\"    # \\xed\\x8c\\x90\\xec\\xa0\\x95 \\xea\\xb2\\xb0\\xea\\xb3\\xbc\\xeb\\xa1\\x9c\\xeb\\x8a\\x94 FAIL \\xeb\\xa1\\x9c \\xec\\xb2\\x98\\xeb\\xa6\\xac\\xeb\\x90\\xa8\n"
  "\n"
  "string  result\n"
  "float64 tensile_n\n"
  "float64 stiffness_n_per_mm\n"
  "uint16  error_code\n"
  "\n"
  "nail_msgs/ForceSample[] waveform   # FAIL \\xec\\x8b\\x9c \\xed\\x95\\x84\\xec\\x88\\x98 \\xec\\xa0\\x80\\xec\\x9e\\xa5. \\xec\\x9e\\x84\\xea\\xb3\\x84\\xea\\xb0\\x92 \\xec\\x9e\\xac\\xeb\\xb3\\xb4\\xec\\xa0\\x95 \\xea\\xb7\\xbc\\xea\\xb1\\xb0.\n"
  "builtin_interfaces/Time measured_at";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__msg__Verdict__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__msg__Verdict__TYPE_NAME, 21, 21},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 368, 368},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__msg__Verdict__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[3];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 3, 3};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__msg__Verdict__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *nail_msgs__msg__ForceSample__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
