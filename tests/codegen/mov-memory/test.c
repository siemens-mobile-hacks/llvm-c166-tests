#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_mov_memory(test_u16 value, test_u16 flags,
                                       test_u16 form);

static const test_u16 words[] = {
    0x0000U, 0x0001U, 0x0002U, 0x7fffU, 0x8000U, 0xfffeU,
    0xffffU, 0xa55aU, 0x0100U, 0x0200U, 0xfe00U, 0x5a00U,
};

static test_u32 run_form(test_u16 form) {
  test_u16 index;
  test_u16 initial;

  for (index = 0; index != 12U; ++index) {
    test_u16 value = words[index];
    test_u16 source = form < 6U ? value : value >> 8;
    test_u16 result =
        form < 6U ? value : (value & 0xff00U) | (~value & 0x00ffU);

    if (form == 9U)
      result = (~value & 0xff00U) | (value >> 8);
    if (form == 10U || form == 12U || form == 13U)
      result = value >> 8;
    for (initial = 0; initial != 32U; ++initial) {
      test_u16 flags = (initial & 6U) |
                       (source >> (form < 6U ? 15U : 7U));
      test_u32 expected;
      test_u32 actual;

      if (source == 0U)
        flags |= 8U;
      if (source == (form < 6U ? 0x8000U : 0x0080U))
        flags |= 16U;
      expected = ((test_u32)flags << 16) | result;
      actual = test_mov_memory(value, initial, form);
      if (actual != expected)
        return ((test_u32)(index + 1U) << 8) | (initial + 1U);
    }
  }
  return 0;
}

void main(void) {
  test_u16 form;

  tap_plan(15U);
  for (form = 0; form != 15U; ++form)
    tap_is_u32(run_form(form), 0UL, "move memory form");
}
