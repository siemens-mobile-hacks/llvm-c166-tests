#include "c166-test-compat.h"

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
