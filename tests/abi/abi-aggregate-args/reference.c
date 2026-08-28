#include "types.h"

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

unsigned int tasking_aggregate_reference(ABI_AGGREGATE_PARAMETERS) {
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
