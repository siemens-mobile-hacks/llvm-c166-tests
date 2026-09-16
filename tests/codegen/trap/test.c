#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_trap(test_u16 psw, test_u16 vector);

static const test_u16 vectors[] = {1U, 63U, 127U};

static test_u32 run_vector(test_u16 vector) {
  test_u16 priority;
  test_u16 flags;

  for (priority = 0; priority != 16U; ++priority) {
    for (flags = 0; flags != 32U; ++flags) {
      test_u16 psw = (priority << 12) | flags;
      test_u32 expected = (test_u32)psw << 16;
      test_u32 actual = test_trap(psw, vector);

      if (actual != expected)
        return ((test_u32)(priority + 1U) << 16) | (flags + 1U);
    }
  }
  return 0;
}

void main(void) {
  test_u16 index;

  tap_plan(3U);
  for (index = 0; index != 3U; ++index)
    tap_is_u32(run_vector(vectors[index]), 0UL, "trap vector");
}
