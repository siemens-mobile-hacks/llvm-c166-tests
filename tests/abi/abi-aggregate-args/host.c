#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern unsigned int llvm_entry_proxy(ABI_AGGREGATE_PARAMETERS);
extern unsigned int c166_test_entry(ABI_AGGREGATE_PARAMETERS);

#define INIT_SHAPE(value, size) \
  do { \
    for (index = 0; index < size; ++index) \
      value.bytes[index] = \
          (unsigned char)(seed + size * 0x11U + index * 0x23U); \
  } while (0)

static void run_aggregate_vector(unsigned int vector_id, unsigned int seed,
                                 unsigned long golden) {
  struct shape1 value1;
  struct shape2 value2;
  struct shape3 value3;
  struct shape4 value4;
  struct shape5 value5;
  struct shape6 value6;
  struct shape7 value7;
  struct shape8 value8;
  unsigned int index;
  unsigned int reference;
  unsigned int actual;

  INIT_SHAPE(value1, 1);
  INIT_SHAPE(value2, 2);
  INIT_SHAPE(value3, 3);
  INIT_SHAPE(value4, 4);
  INIT_SHAPE(value5, 5);
  INIT_SHAPE(value6, 6);
  INIT_SHAPE(value7, 7);
  INIT_SHAPE(value8, 8);

  reference = c166_test_entry(ABI_AGGREGATE_ARGUMENTS);
  actual = llvm_entry_proxy(ABI_AGGREGATE_ARGUMENTS);
  c166_test_check_u32(vector_id * 2 - 1, golden, reference);
  c166_test_check_u32(vector_id * 2, golden, actual);
}

#define RUN_AGGREGATE(id, seed, golden) \
  run_aggregate_vector(id, seed, golden);

void main(void) {
  c166_test_begin(265, 0x16600109UL);
  ABI_AGGREGATE_ARG_VECTORS(RUN_AGGREGATE)
  c166_test_finish();
  simulator_stop();
}
