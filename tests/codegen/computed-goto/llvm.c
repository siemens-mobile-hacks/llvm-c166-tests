#include "c166_test.h"

C166_NOINLINE static unsigned int select_label(unsigned int selector) {
  static void *const labels[] = {&&first, &&second};
  goto *labels[selector & 1U];

first:
  return 11U;
second:
  return 22U;
}

unsigned int llvm_entry(unsigned int selector) {
  unsigned int first = select_label(selector);
  unsigned int second = select_label(selector + 1U);
  return (unsigned int)((first << 8) | second);
}

void main(void) {
  tap_plan(1);
  tap_is_u32(llvm_entry(0U), 2838U, "computed goto");
}
