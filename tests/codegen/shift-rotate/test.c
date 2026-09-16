#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_shift_rotate(test_u16 value, test_u16 count,
                                         test_u16 form, test_u16 flags);

static const test_u16 words[] = {
    0x0000U, 0x0001U, 0x0002U, 0x7fffU,
    0x8000U, 0xffffU, 0xa55aU, 0x5aa5U,
};
static const test_u16 upper_counts[] = {0x0000U, 0x0010U, 0x8000U, 0xfff0U};

enum operation { OP_ROL, OP_ROR, OP_SHL, OP_SHR, OP_ASHR, OP_COUNT };

static test_u32 expected(test_u16 word, test_u16 count,
                         enum operation operation) {
  test_u16 carry = 0;
  test_u16 overflow = 0;
  test_u16 flags;

  count &= 15U;
  while (count != 0U) {
    --count;
    if (operation == OP_ROR || operation == OP_SHR ||
        operation == OP_ASHR) {
      overflow |= carry;
      carry = word & 1U;
      if (operation == OP_ROR)
        word = (word >> 1) | (carry << 15);
      else if (operation == OP_ASHR)
        word = (word >> 1) | (word & 0x8000U);
      else
        word >>= 1;
    } else {
      carry = word >> 15;
      word = (word << 1) | (operation == OP_ROL ? carry : 0U);
    }
  }
  flags = (word >> 15) | (carry << 1) | (overflow << 2);
  if (word == 0U)
    flags |= 8U;
  return ((test_u32)flags << 16) | word;
}

static test_u32 run_form(test_u16 form) {
  test_u16 value_index;
  test_u16 index;
  test_u16 initial;
  test_u16 limit = form < OP_COUNT ? 64U :
                   (form < 2U * OP_COUNT ? 16U : 1U);

  for (value_index = 0; value_index != 8U; ++value_index) {
    for (index = 0; index != limit; ++index) {
      test_u16 count = form < OP_COUNT
                           ? upper_counts[index / 16U] | (index & 15U)
                           : index;
      test_u32 wanted;

      if (form >= 2U * OP_COUNT)
        count = words[value_index];
      wanted = expected(words[value_index], count,
                        (enum operation)(form % OP_COUNT));
      for (initial = 0; initial != 32U; ++initial) {
        test_u32 actual = test_shift_rotate(words[value_index], count, form,
                                            initial);

        if (actual != wanted)
          return ((test_u32)(value_index + 1U) << 24) |
                 ((test_u32)(index + 1U) << 8) | (initial + 1U);
      }
    }
  }
  return 0;
}

void main(void) {
  test_u16 form;

  tap_plan(3U * OP_COUNT);
  for (form = 0; form != 3U * OP_COUNT; ++form)
    tap_is_u32(run_form(form), 0UL, "shift rotate form");
}
