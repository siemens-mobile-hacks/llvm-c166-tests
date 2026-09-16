#include "c166_test.h"

typedef unsigned int test_u16;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u16 TEST_FUNCTION test_boundaries(test_u16 case_id);

void main(void) {
  test_u16 case_id;

  tap_plan(11U);
  for (case_id = 0; case_id != 11U; ++case_id) {
    test_u16 expected = case_id == 8U || case_id == 10U ? 2U : 1U;

    tap_is_u32(test_boundaries(case_id), expected, "code segment boundary");
  }
}
