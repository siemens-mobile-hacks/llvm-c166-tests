#include "c166_test.h"
#include "vectors.inc"

C166_TEST_NOINLINE
unsigned long c166_test_entry(unsigned int a, unsigned int b,
                              unsigned int c, unsigned long pair,
                              unsigned int tail) {
  unsigned long value = pair;
  value += (unsigned long)a << 16;
  value += ((unsigned long)b << 1) + c;
  value ^= ((unsigned long)tail << 16) | b;
  return value;
}

#define RUN_STOP(id, a, b, c, pair, tail, golden)                         \
  tap_is_u32(c166_test_entry(a, b, c, pair, tail), golden,                \
             "stack argument vector");

void main(void) {
  tap_plan(8U);
  ABI_STOP_VECTORS(RUN_STOP)
}
