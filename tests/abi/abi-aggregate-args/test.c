#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

static unsigned int rotate5(unsigned int value) {
  return (value << 5) | (value >> 11);
}

#define MIX_SHAPE(value, size) \
  do { \
    for (index = 0; index < size; ++index) { \
      result = rotate5(result); \
      result ^= value.bytes[index]; \
      result ^= ((unsigned int)size << 8) | index; \
    } \
  } while (0)

C166_TEST_NOINLINE
unsigned int c166_test_entry(ABI_AGGREGATE_PARAMETERS) {
  unsigned int result = 0x4a39U;
  unsigned int index;
  MIX_SHAPE(value1, 1);
  MIX_SHAPE(value2, 2);
  MIX_SHAPE(value3, 3);
  MIX_SHAPE(value4, 4);
  MIX_SHAPE(value5, 5);
  MIX_SHAPE(value6, 6);
  MIX_SHAPE(value7, 7);
  MIX_SHAPE(value8, 8);
  return result;
}

#define AGGREGATE_SEED(id, seed, expected) seed,
static const unsigned int seeds[] = {
    ABI_AGGREGATE_ARG_VECTORS(AGGREGATE_SEED)
};

#define INIT_SHAPE(value, size)                                             \
  do {                                                                      \
    for (index = 0; index < size; ++index)                                  \
      value.bytes[index] =                                                  \
          (unsigned char)(seed + size * 0x11U + index * 0x23U);             \
  } while (0)

unsigned long c166_test_case(unsigned int case_id) {
  struct shape1 value1;
  struct shape2 value2;
  struct shape3 value3;
  struct shape4 value4;
  struct shape5 value5;
  struct shape6 value6;
  struct shape7 value7;
  struct shape8 value8;
  unsigned int index;
  unsigned int seed = seeds[case_id];

  INIT_SHAPE(value1, 1);
  INIT_SHAPE(value2, 2);
  INIT_SHAPE(value3, 3);
  INIT_SHAPE(value4, 4);
  INIT_SHAPE(value5, 5);
  INIT_SHAPE(value6, 6);
  INIT_SHAPE(value7, 7);
  INIT_SHAPE(value8, 8);
  return c166_test_entry(ABI_AGGREGATE_ARGUMENTS);
}
