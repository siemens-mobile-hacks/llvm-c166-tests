#include "c166_test.h"

unsigned long c166_test_entry(unsigned int seed);

void main(void) {
  tap_plan(4U);
  tap_is_u32(c166_test_entry(0U), 0x00010013UL,
             "assembly call with zero seed");
  tap_is_u32(c166_test_entry(1U), 0x00020019UL,
             "assembly call with unit seed");
  tap_is_u32(c166_test_entry(0x1234U), 0x00036d4bUL,
             "assembly call with ordinary seed");
  tap_is_u32(c166_test_entry(0xffffU), 0x0004000dUL,
             "assembly call with wrapping seed");
}
