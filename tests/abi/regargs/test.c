#include "c166_test.h"

C166_TEST_NOINLINE
int entry(int lhs, int rhs) {
  return lhs + rhs;
}

void main(void) {
  tap_plan(1);
  tap_is_u32((unsigned int)entry(19, 23), 42U, "register arguments");
}
