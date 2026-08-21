// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from nail_msgs:msg/ErrorCode.idl
// generated code does not contain a copyright notice

#include "nail_msgs/msg/detail/error_code__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_nail_msgs
const rosidl_type_hash_t *
nail_msgs__msg__ErrorCode__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xd7, 0xf9, 0xd3, 0xa8, 0xbc, 0x9c, 0x58, 0x91,
      0xe6, 0x60, 0x07, 0x64, 0x8f, 0x24, 0x97, 0x2f,
      0x79, 0xc1, 0xcd, 0xf3, 0xb8, 0x5b, 0x82, 0x4a,
      0xf4, 0xb3, 0xe9, 0xf6, 0x37, 0x0d, 0x50, 0x1e,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char nail_msgs__msg__ErrorCode__TYPE_NAME[] = "nail_msgs/msg/ErrorCode";

// Define type names, field names, and default values
static char nail_msgs__msg__ErrorCode__FIELD_NAME__code[] = "code";
static char nail_msgs__msg__ErrorCode__FIELD_NAME__severity[] = "severity";
static char nail_msgs__msg__ErrorCode__FIELD_NAME__detail[] = "detail";

static rosidl_runtime_c__type_description__Field nail_msgs__msg__ErrorCode__FIELDS[] = {
  {
    {nail_msgs__msg__ErrorCode__FIELD_NAME__code, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ErrorCode__FIELD_NAME__severity, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {nail_msgs__msg__ErrorCode__FIELD_NAME__detail, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
nail_msgs__msg__ErrorCode__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
      {nail_msgs__msg__ErrorCode__FIELDS, 3, 3},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# \\xec\\x97\\x90\\xeb\\x9f\\xac \\xec\\xbd\\x94\\xeb\\x93\\x9c \\xec\\x83\\x81\\xec\\x88\\x98 \\xec\\xa0\\x95\\xec\\x9d\\x98. \\xea\\xb0\\x9c\\xeb\\xb0\\x9c \\xeb\\xaa\\x85\\xec\\x84\\xb8\\xec\\x84\\x9c \\xc2\\xa75 \\xec\\x99\\x80 1:1 \\xeb\\x8c\\x80\\xec\\x9d\\x91.\n"
  "\n"
  "uint16 OK = 0\n"
  "\n"
  "# --- E1 \\xec\\x9c\\x84\\xec\\xb9\\x98 \\xed\\x83\\x90\\xec\\x83\\x89 ---\n"
  "uint16 E1001_TARGET_MOVED        = 1001\n"
  "uint16 E1002_NO_CONTACT          = 1002\n"
  "uint16 E1003_EARLY_CONTACT       = 1003\n"
  "uint16 E1004_STIFFNESS_AMBIGUOUS = 1004\n"
  "uint16 E1005_BOUNDARY_UNSTABLE   = 1005\n"
  "uint16 E1006_PROBE_SKEWED        = 1006\n"
  "\n"
  "# --- E2 \\xed\\x91\\x9c\\xeb\\xa9\\xb4 \\xec\\x97\\xb0\\xeb\\xa7\\x88 ---\n"
  "uint16 E2001_FORCE_OVER          = 2001\n"
  "uint16 E2002_FORCE_UNDER         = 2002\n"
  "uint16 E2003_FORBIDDEN_ZONE      = 2003\n"
  "uint16 E2004_OVER_SANDING        = 2004\n"
  "uint16 E2005_MISSED_SEGMENT      = 2005\n"
  "uint16 E2006_EDGE_CATCH          = 2006\n"
  "\n"
  "# --- E3 \\xeb\\x8d\\x94\\xec\\x8a\\xa4\\xed\\x8a\\xb8 \\xec\\xa0\\x9c\\xea\\xb1\\xb0 ---\n"
  "uint16 E3001_NO_CONTACT          = 3001\n"
  "uint16 E3002_FORCE_OVER          = 3002\n"
  "uint16 E3003_ZONE_EXIT           = 3003\n"
  "\n"
  "# --- E4 \\xec\\xa0\\xa4 \\xeb\\x8f\\x84\\xed\\x8f\\xac ---\n"
  "uint16 E4001_ZONE_EXIT           = 4001\n"
  "uint16 E4002_BOUNDARY_TOO_CLOSE  = 4002\n"
  "uint16 E4003_FORCE_OVER          = 4003\n"
  "uint16 E4004_FORCE_UNDER         = 4004\n"
  "uint16 E4005_MISSED_SEGMENT      = 4005\n"
  "uint16 E4006_BRUSH_DRAG          = 4006\n"
  "uint16 E4007_CURED_RESIDUE       = 4007\n"
  "\n"
  "# --- E5 UV \\xea\\xb2\\xbd\\xed\\x99\\x94 ---\n"
  "uint16 E5001_OVER_EXPOSURE       = 5001\n"
  "uint16 E5002_MISSED_REGION       = 5002\n"
  "uint16 E5003_STANDOFF_ERROR      = 5003\n"
  "uint16 E5004_UV_ON_WHILE_STOPPED = 5004\n"
  "\n"
  "# --- E6 \\xed\\x92\\x88\\xec\\xa7\\x88\\xea\\xb2\\x80\\xec\\x82\\xac ---\n"
  "uint16 E6001_UNCURED             = 6001\n"
  "uint16 E6002_VERDICT_UNCERTAIN   = 6002\n"
  "uint16 E6003_PROBE_CONTAMINATED  = 6003\n"
  "uint16 E6004_PROBE_FORCE_OVER    = 6004\n"
  "uint16 E6005_PROBE_POSITION      = 6005\n"
  "uint16 E6006_PARTIAL_UNCURED     = 6006\n"
  "uint16 E6007_RECURE_EXHAUSTED    = 6007\n"
  "\n"
  "# --- E7 \\xed\\x88\\xb4/\\xea\\xb7\\xb8\\xeb\\xa6\\xac\\xed\\x8d\\xbc ---\n"
  "uint16 E7001_GRIP_FAILED         = 7001\n"
  "uint16 E7002_GRIP_INCOMPLETE     = 7002\n"
  "uint16 E7003_TOOL_SLIPPED        = 7003\n"
  "uint16 E7004_RETURN_FAILED       = 7004\n"
  "uint16 E7005_TCP_MISMATCH        = 7005\n"
  "\n"
  "# --- E9 \\xec\\x8b\\x9c\\xec\\x8a\\xa4\\xed\\x85\\x9c ---\n"
  "uint16 E9001_SEQUENCE_VIOLATION  = 9001\n"
  "uint16 E9002_COMM_LOST           = 9002\n"
  "uint16 E9003_USER_CANCELLED      = 9003\n"
  "uint16 E9004_DB_WRITE_FAILED     = 9004\n"
  "uint16 E9005_ROBOT_ALARM         = 9005\n"
  "uint16 E9006_RETRY_EXHAUSTED     = 9006\n"
  "uint16 E9007_MAP_SESSION_MISMATCH= 9007\n"
  "uint16 E9008_SAFETY_INTERLOCK    = 9008\n"
  "\n"
  "# --- \\xec\\x8b\\xac\\xea\\xb0\\x81\\xeb\\x8f\\x84 ---\n"
  "uint8 SEV_NONE   = 0\n"
  "uint8 SEV_WARN   = 3    # S3\n"
  "uint8 SEV_RETRY  = 2    # S2\n"
  "uint8 SEV_ABORT  = 1    # S1\n"
  "uint8 SEV_SAFETY = 0    # S0\n"
  "\n"
  "uint16 code\n"
  "uint8  severity\n"
  "string detail";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
nail_msgs__msg__ErrorCode__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {nail_msgs__msg__ErrorCode__TYPE_NAME, 23, 23},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 2242, 2242},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
nail_msgs__msg__ErrorCode__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *nail_msgs__msg__ErrorCode__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
