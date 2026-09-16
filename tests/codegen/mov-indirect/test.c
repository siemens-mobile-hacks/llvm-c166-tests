#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_mov_indirect(test_u16 value, test_u16 flags,
                                         test_u16 form);

static const test_u16 words[] = {
    0x0000U, 0x0001U, 0x0002U, 0x7fffU,
    0x8000U, 0xffffU, 0xa55aU, 0x5aa5U,
};
static const test_u16 register_kinds[] = {0U, 1U, 2U, 3U, 7U, 8U};

static test_u32 run_form(test_u16 form) {
  test_u16 kind = form < 27U ? form % 9U : register_kinds[(form - 27U) % 6U];
  test_u16 high_result;
  test_u16 count = form < 9U ? 8U : 256U;
  test_u16 sign = form < 9U ? 0x8000U : 0x0080U;
  test_u16 index;
  test_u16 initial;

  if (kind == 0U || kind == 1U || kind == 7U)
    high_result = form >= 27U;
  else
    high_result = (form >= 18U && form < 27U) || form >= 33U;

  for (index = 0; index != count; ++index) {
    test_u16 value = form < 9U ? words[index] : index;
    test_u16 result = value;

    if (form >= 9U) {
      if (high_result)
        result = (value << 8) | 0x00c3U;
      else
        result |= 0x3c00U;
    }
    for (initial = 0; initial != 32U; ++initial) {
      test_u16 flags = initial & 6U;
      test_u32 expected;
      test_u32 actual;

      if (value & sign)
        flags |= 1U;
      if (value == 0U)
        flags |= 8U;
      if (value == sign)
        flags |= 16U;
      expected = ((test_u32)flags << 16) | result;
      actual = test_mov_indirect(value, initial, form);
      if (actual != expected)
        return ((test_u32)(value + 1U) << 8) | (initial + 1U);
    }
  }
  return 0;
}

void main(void) {
  test_u16 form;

  tap_plan(39U);
  for (form = 0; form != 39U; ++form)
    tap_is_u32(run_form(form), 0UL, "move indirect form");
}
