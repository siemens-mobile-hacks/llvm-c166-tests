#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_extended_addressing(test_u16 case_id);

static test_u32 expected_result(test_u16 case_id) {
  if (case_id < 64U)
    return 0x5aa50000UL | ((case_id & 8U) ? 0x2468UL : 0x1357UL);
  if (case_id < 128U)
    return 0xa53c0000UL | (0x1111UL * ((case_id & 3U) + 1U));
  return 0x5aa50000UL | ((case_id & 16U) ? 0x2468UL : 0x1357UL);
}

void main(void) {
  test_u16 case_id;

  tap_plan(192U);
  for (case_id = 0; case_id != 192U; ++case_id)
    tap_is_u32(test_extended_addressing(case_id), expected_result(case_id),
               "extended addressing case");
}
