#include "c166_test.h"

struct pair {
  unsigned int low;
  unsigned int high;
};

C166_TEST_NOINLINE
struct pair entry(unsigned int low, unsigned int high) {
  struct pair result;
  result.low = low;
  result.high = high;
  return result;
}

void main(void) {
  struct pair value = entry(21U, 34U);
  tap_plan(1);
  tap_is_u32(value.low + 2U * value.high, 89U, "aggregate return");
}
