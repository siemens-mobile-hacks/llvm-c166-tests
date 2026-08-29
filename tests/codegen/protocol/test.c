#include "c166-test-compat.h"

extern unsigned long c166_mix_helper(unsigned int a, unsigned long b,
                                     signed char c);

volatile unsigned long c166_protocol_state;

C166_TEST_NOINLINE
unsigned long c166_test_entry(unsigned int a, unsigned long b, signed char c) {
  c166_protocol_state = c166_mix_helper(a, b, c) ^ 0x5aa5a55aUL;
  return c166_protocol_state ^ 0x5aa5a55aUL;
}
