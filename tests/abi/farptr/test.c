#include "c166_test.h"

static volatile unsigned int value;

C166_TEST_NOINLINE
unsigned int entry(volatile unsigned int *address, unsigned int new_value) {
  *address = new_value;
  return *address;
}

void main(void) {
  tap_plan(2);
  tap_is_u32(entry(&value, 0x1234U), 0x1234U, "pointer result");
  tap_is_u32(value, 0x1234U, "pointer store");
}
