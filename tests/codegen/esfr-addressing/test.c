#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_esfr(test_u16 case_id, test_u16 flags);

static test_u32 expected_result(test_u16 index, test_u16 initial) {
  test_u16 count = (index & 3U) + 1U;

  if (index >= 68U)
    return initial;
  if (index >= 48U)
    return 0x002a0001UL;
  if (index < 20U || index >= 28U)
    return 0x002a0000UL | (1UL << (count - 1U));
  if (index < 24U)
    return 0x00200000UL | ((1UL << count) - 1UL);
  return 1UL;
}

static test_u16 run_index(test_u16 index) {
  test_u16 initial;
  test_u16 count = index >= 68U ? 32U : 1U;

  for (initial = 0; initial != count; ++initial) {
    if (test_esfr(index, initial) != expected_result(index, initial))
      return initial + 1U;
  }
  return 0;
}

void main(void) {
  test_u16 index;

  tap_plan(108U);
  for (index = 0; index != 108U; ++index)
    tap_is_u32(run_index(index), 0UL, "ESFR addressing case");
}
