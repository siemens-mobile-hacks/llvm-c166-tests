#include "c166_test.h"

struct pair {
  unsigned int low;
  unsigned int high;
};

unsigned int llvm_entry(unsigned int head, struct pair value,
                        unsigned int tail) {
  return head + 2 * value.low + 3 * value.high + 4 * tail;
}

void main(void) {
  struct pair value;

  value.low = 11U;
  value.high = 13U;
  tap_plan(1);
  tap_is_u32(llvm_entry(7U, value, 17U), 136U, "aggregate argument");
}
