#include "c166-test-compat.h"

C166_TEST_NOINLINE
unsigned long c166_mix_helper(unsigned int a, unsigned long b,
                              signed char c) {
  return (b ^ ((unsigned long)a << 16)) + (long)c;
}
