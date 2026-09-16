#include "c166_test.h"

unsigned int llvm_entry(unsigned int value) {
  unsigned int accumulator = 0;
  unsigned int index;

  for (index = 0; index != value; ++index) {
    if (index & 1)
      accumulator += 3 * index;
    else
      accumulator ^= index + 7;
  }

  switch (value) {
  case 3:
    accumulator += 30;
    break;
  case 7:
    accumulator += 70;
    break;
  default:
    accumulator += 100;
    break;
  }
  return accumulator;
}

void main(void) {
  tap_plan(1);
  tap_is_u32(llvm_entry(9U), 163U, "branches");
}
