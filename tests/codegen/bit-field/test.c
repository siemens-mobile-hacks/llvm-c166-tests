#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_bit_field(test_u16 value, test_u16 vector,
                                      test_u16 flags, test_u16 form);

static const test_u16 words[] = {0x0000U, 0xffffU, 0x8000U, 0x007fU};
static const test_u16 bytes[] = {
    0U, 255U, 85U, 170U, 1U, 128U, 127U, 254U,
};

static test_u32 run_form(test_u16 form) {
  test_u16 mask_index;
  test_u16 data_index;
  test_u16 word_index;
  test_u16 initial;
  test_u16 shift = (form & 1U) ? 8U : 0U;

  for (mask_index = 0; mask_index != 8U; ++mask_index) {
    test_u16 mask = bytes[mask_index] << shift;

    for (data_index = 0; data_index != 8U; ++data_index) {
      test_u16 data = bytes[data_index] << shift;
      test_u16 vector = mask_index * 8U + data_index;

      for (word_index = 0; word_index != 4U; ++word_index) {
        test_u16 result = (words[word_index] & ~mask) | data;
        test_u16 flags = (result & 0x8000U) ? 1U : 0U;

        if (result == 0U)
          flags |= 8U;
        for (initial = 0; initial != 32U; ++initial) {
          test_u32 expected = ((test_u32)flags << 16) | result;
          test_u32 actual =
              test_bit_field(words[word_index], vector, initial, form);

          if (actual != expected)
            return ((test_u32)(vector + 1U) << 16) |
                   ((test_u32)(word_index + 1U) << 8) | (initial + 1U);
        }
      }
    }
  }
  return 0;
}

void main(void) {
  test_u16 form;

  tap_plan(6U);
  for (form = 0; form != 6U; ++form)
    tap_is_u32(run_form(form), 0UL, "bit field form");
}
