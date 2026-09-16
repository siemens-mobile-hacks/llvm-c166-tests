#include "c166_test.h"

typedef unsigned int test_u16;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u16 TEST_FUNCTION test_near_calls(test_u16 word, test_u16 flags,
                                       test_u16 expected_flags);

static const test_u16 words[] = {
    0x0000U, 0x0001U, 0x7fffU, 0x8000U,
    0xffffU, 0xa55aU, 0x5555U, 0xfffeU,
};

static test_u16 run_word(test_u16 word) {
  test_u16 flags;

  for (flags = 0; flags != 32U; ++flags) {
    test_u16 expected_flags = (flags & 6U) | (word >> 15);
    test_u16 actual;

    if (word == 0U)
      expected_flags |= 8U;
    if (word == 0x8000U)
      expected_flags |= 16U;
    actual = test_near_calls(word, flags, expected_flags);
    if (actual != 0U)
      return (flags + 1U) | (actual << 8);
  }
  return 0;
}

void main(void) {
  test_u16 index;

  tap_plan(8U);
  for (index = 0; index != 8U; ++index)
    tap_is_u32(run_word(words[index]), 0UL, "near call stack word");
}
