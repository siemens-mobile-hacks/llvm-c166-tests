#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_mov_extend(test_u16 value, test_u16 flags,
                                       test_u16 form);

static test_u32 run_form(test_u16 form) {
  test_u16 value;
  test_u16 initial;

  for (value = 0; value != 256U; ++value) {
    for (initial = 0; initial != 32U; ++initial) {
      test_u16 result = value;
      test_u16 flags = initial & 6U;
      test_u32 expected;
      test_u32 actual;

      if (value == 0U)
        flags |= 8U;
      if (form < 5U && value >= 128U) {
        result |= 0xff00U;
        flags |= 1U;
      }
      expected = ((test_u32)flags << 16) | result;
      actual = test_mov_extend(value, initial, form);
      if (actual != expected)
        return ((test_u32)(value + 1U) << 8) | (initial + 1U);
    }
  }
  return 0;
}

void main(void) {
  test_u16 form;

  tap_plan(10U);
  for (form = 0; form != 10U; ++form)
    tap_is_u32(run_form(form), 0UL, "move extend form");
}
