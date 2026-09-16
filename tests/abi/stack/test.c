#include "c166_test.h"

C166_TEST_NOINLINE
unsigned int entry(unsigned int a, unsigned int b, unsigned int c,
                   unsigned long d, unsigned int e) {
  return a + 3U * b + 5U * c + 7U * (unsigned int)d +
         11U * (unsigned int)(d >> 16) + 13U * e;
}

void main(void) {
  tap_plan(1);
  tap_is_u32(entry(1U, 2U, 3U, 0x00040005UL, 6U), 179U,
             "stack arguments");
}
