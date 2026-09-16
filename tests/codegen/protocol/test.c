#include "c166_test.h"

extern unsigned long c166_mix_helper(unsigned int a, unsigned long b,
                                     signed char c);

volatile unsigned long c166_protocol_state;
static volatile unsigned int protocol_word;
static volatile unsigned long protocol_long;
static volatile signed char protocol_byte;

C166_TEST_NOINLINE
unsigned long c166_test_entry(unsigned int a, unsigned long b, signed char c) {
  c166_protocol_state = c166_mix_helper(a, b, c) ^ 0x5aa5a55aUL;
  return c166_protocol_state ^ 0x5aa5a55aUL;
}

void main(void) {
  protocol_word = 0x1357U;
  protocol_long = 0x2468ace0UL;
  protocol_byte = -5;

  tap_plan(1U);
  tap_is_u32(c166_test_entry(protocol_word, protocol_long, protocol_byte),
             0x373facdbUL, "cross-translation-unit call");
}
