#ifndef ABI_AGGREGATE_TYPES_H
#define ABI_AGGREGATE_TYPES_H

struct shape1 { unsigned char bytes[1]; };
struct shape2 { unsigned char bytes[2]; };
struct shape3 { unsigned char bytes[3]; };
struct shape4 { unsigned char bytes[4]; };
struct shape5 { unsigned char bytes[5]; };
struct shape6 { unsigned char bytes[6]; };
struct shape7 { unsigned char bytes[7]; };
struct shape8 { unsigned char bytes[8]; };

typedef char assert_shape1_size[sizeof(struct shape1) == 2 ? 1 : -1];
typedef char assert_shape2_size[sizeof(struct shape2) == 2 ? 1 : -1];
typedef char assert_shape3_size[sizeof(struct shape3) == 4 ? 1 : -1];
typedef char assert_shape4_size[sizeof(struct shape4) == 4 ? 1 : -1];
typedef char assert_shape5_size[sizeof(struct shape5) == 6 ? 1 : -1];
typedef char assert_shape6_size[sizeof(struct shape6) == 6 ? 1 : -1];
typedef char assert_shape7_size[sizeof(struct shape7) == 8 ? 1 : -1];
typedef char assert_shape8_size[sizeof(struct shape8) == 8 ? 1 : -1];

#define ABI_AGGREGATE_PARAMETERS \
  struct shape1 value1, struct shape2 value2, struct shape3 value3, \
  struct shape4 value4, struct shape5 value5, struct shape6 value6, \
  struct shape7 value7, struct shape8 value8

#define ABI_AGGREGATE_ARGUMENTS \
  value1, value2, value3, value4, value5, value6, value7, value8

#endif
