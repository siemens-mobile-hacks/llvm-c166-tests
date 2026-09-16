#include "c166_test.h"
#include "vectors.inc"

C166_TEST_NOINLINE
volatile unsigned int *c166_test_entry(
    unsigned int tag0, unsigned int tag1, volatile unsigned int *address,
    unsigned int value) {
  *address = (value ^ tag0) + tag1;
  return address;
}

static volatile unsigned int cells[4];

static void run_vector(unsigned int vector_id, unsigned int tag0,
                       unsigned int tag1, unsigned int value,
                       unsigned long expected) {
  unsigned int index = vector_id - 1U;
  volatile unsigned int *result;

  cells[index] = 0xa55aU;
  result = c166_test_entry(tag0, tag1, &cells[index], value);
  tap_is_u32(cells[index], expected, "far pointer store");
  tap_ok(result == &cells[index], "far pointer return");
}

#define RUN_FAR_POINTER(id, tag0, tag1, value, expected)                    \
  run_vector(id, tag0, tag1, value, expected);

void main(void) {
  tap_plan(8);
  ABI_FAR_POINTER_VECTORS(RUN_FAR_POINTER)
}
