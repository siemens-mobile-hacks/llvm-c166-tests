#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_bit_update(test_u16 value, test_u16 bit_index,
                                       test_u16 flags, test_u16 form);

static const test_u16 words[] = {
    0x0000U, 0xffffU, 0x5555U, 0xaaaaU,
    0x0001U, 0x8000U, 0x7fffU, 0xfffeU,
};

static test_u32 run_form(test_u16 form) {
  test_u16 bit_index;
  test_u16 index;
  test_u16 initial;

  for (bit_index = 0; bit_index != 16U; ++bit_index) {
    test_u16 mask = 1U << bit_index;

    for (index = 0; index != 8U; ++index) {
      test_u16 value = words[index];
      test_u16 result = (form & 1U) ? value | mask : value & ~mask;
      test_u16 flags = (value & mask) ? 1U : 8U;
      for (initial = 0; initial != 32U; ++initial) {
        test_u32 expected = ((test_u32)flags << 16) | result;
        test_u32 actual = test_bit_update(value, bit_index, initial, form);

        if (actual != expected)
          return ((test_u32)(bit_index + 1U) << 24) |
                 ((test_u32)(index + 1U) << 16) | (initial + 1U);
      }
    }
  }
  return 0;
}

void main(void) {
  test_u16 form;

  tap_plan(6U);
  for (form = 0; form != 6U; ++form)
    tap_is_u32(run_form(form), 0UL, "bit update form");
}
