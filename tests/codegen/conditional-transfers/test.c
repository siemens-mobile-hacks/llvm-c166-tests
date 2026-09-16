#include "c166_test.h"

typedef unsigned int test_u16;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u16 TEST_FUNCTION test_transfer(test_u16 condition, test_u16 flags,
                                     test_u16 form);

static test_u16 condition_true(test_u16 condition, test_u16 flags) {
  test_u16 negative = flags & 1U;
  test_u16 carry = (flags >> 1) & 1U;
  test_u16 overflow = (flags >> 2) & 1U;
  test_u16 zero = (flags >> 3) & 1U;
  test_u16 end_of_table = (flags >> 4) & 1U;

  switch (condition) {
  case 0: return 1U;
  case 1: return !(zero || end_of_table);
  case 2: return zero;
  case 3: return !zero;
  case 4: return overflow;
  case 5: return !overflow;
  case 6: return negative;
  case 7: return !negative;
  case 8: return carry;
  case 9: return !carry;
  case 10: return !(zero || (negative ^ overflow));
  case 11: return zero || (negative ^ overflow);
  case 12: return negative ^ overflow;
  case 13: return !(negative ^ overflow);
  case 14: return !(zero || carry);
  default: return zero || carry;
  }
}

static test_u16 run_condition(test_u16 form, test_u16 condition) {
  test_u16 flags;

  for (flags = 0; flags != 32U; ++flags) {
    test_u16 expected = flags | (condition_true(condition, flags) << 8);
    test_u16 actual = test_transfer(condition, flags, form);

    if (actual != expected)
      return flags + 1U;
  }
  return 0;
}

void main(void) {
  test_u16 form;
  test_u16 condition;

  tap_plan(6U * 16U);
  for (form = 0; form != 6U; ++form)
    for (condition = 0; condition != 16U; ++condition)
      tap_is_u32(run_condition(form, condition), 0UL,
                 "conditional transfer");
}
