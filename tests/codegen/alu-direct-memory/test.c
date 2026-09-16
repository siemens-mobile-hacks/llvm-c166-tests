#include "c166_test.h"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u32 TEST_FUNCTION test_alu_direct_memory(test_u16 operation,
                                              test_u16 lhs,
                                              test_u16 rhs,
                                              test_u16 flags);

static const test_u16 words[] = {
    0, 1, 2, 0x7fff, 0x8000, 0xfffe, 0xffff, 0xa55a};
static const test_u16 bytes[] = {0, 1, 2, 0x7f, 0x80, 0xfe, 0xff, 0x5a};
static const test_u16 immediates[] = {8, 16, 127, 128, 254, 255, 90, 64};
static const test_u16 sfr_immediates[] = {
    0, 1, 7, 0x7fff, 0x8000, 0xfffe, 0xffff, 0xa55a};
static const test_u16 loop_short[] = {0, 1, 2, 7, 8, 9, 14, 15};
static const test_u16 loop_long[] = {
    16, 17, 255, 256, 0x7fff, 0x8000, 0xfffe, 0xffff};

/* Evaluate the ISA rules at 32 bits to avoid 16-bit arithmetic overflow. */
static test_u32 expected(test_u16 operation, test_u16 lhs, test_u16 rhs,
                         test_u16 initial) {
  test_u16 result;
  test_u16 flags;
  test_u16 carry = 0;
  test_u16 overflow = 0;
  test_u16 chained;
  test_u16 input_carry;
  test_u16 byte =
      (operation >= 15 && operation < 40) ||
      (operation >= 71 && operation < 114);
  test_u16 updated = lhs;
  test_u16 mask = byte ? 255U : 65535U;
  test_u16 sign = byte ? 128U : 32768U;
  long signed_lhs;
  long signed_rhs;
  long signed_result;
  test_u32 wide;

  lhs &= mask;
  rhs &= mask;
  signed_lhs = lhs < sign ? (long)lhs : (long)lhs - ((long)mask + 1);
  signed_rhs = rhs < sign ? (long)rhs : (long)rhs - ((long)mask + 1);

  if (operation >= 126)
    operation = (operation - 126) % 8;
  else if (operation >= 114) {
    static const int changes[] = {-1, -2, 1, 2};

    updated = lhs + changes[(operation - 114) % 4];
    operation = 4;
  } else if (operation >= 106)
    operation = operation - 106 + 15;
  else if (operation >= 99)
    operation = operation - 99 + 23;
  else if (operation >= 91)
    operation = operation - 91 + 15;
  else if (operation >= 83)
    operation = operation - 83 + 15;
  else if (operation >= 71) {
    static const unsigned char operations[] = {15, 17, 19, 20, 21, 22};

    operation = operations[(operation - 71) % 6];
  } else if (operation >= 63)
    operation -= 63;
  else if (operation >= 48)
    operation -= 48;
  else if (operation >= 40)
    operation -= 40;
  else if (operation >= 30)
    operation = (operation & 1U) ? 18 : 16;

  if (byte)
    operation -= 15;
  if (operation >= 8) {
    operation -= 8;
    if (operation >= 4)
      ++operation;
  }

  chained = operation == 1 || operation == 3;
  input_carry = chained ? ((initial >> 1) & 1U) : 0;
  if (operation < 2) {
    wide = (test_u32)lhs + rhs + input_carry;
    result = (test_u16)wide;
    carry = wide > mask;
    signed_result = signed_lhs + signed_rhs + input_carry;
    overflow = signed_result < -(long)sign || signed_result >= (long)sign;
  } else if (operation < 5) {
    wide = (test_u32)rhs + input_carry;
    result = (test_u16)((test_u32)lhs - wide);
    carry = (test_u32)lhs < wide;
    signed_result = signed_lhs - signed_rhs - input_carry;
    overflow = signed_result < -(long)sign || signed_result >= (long)sign;
  } else if (operation == 5) {
    result = lhs ^ rhs;
  } else if (operation == 6) {
    result = lhs & rhs;
  } else {
    result = lhs | rhs;
  }

  result &= mask;
  flags = ((result & sign) != 0) | (carry << 1) | (overflow << 2);
  if (result == 0 && (!chained || (initial & 8U)))
    flags |= 8U;
  if (rhs == sign)
    flags |= 16U;
  return ((test_u32)flags << 16) |
         (operation == 4 ? (updated & mask) : result) |
         (byte ? 0xa500U : 0);
}

static test_u16 run_operation(test_u16 operation) {
  test_u16 lhs_index;
  test_u16 rhs_index;
  test_u16 initial;
  test_u16 case_id = 0;

  for (lhs_index = 0; lhs_index != 8; ++lhs_index) {
    for (rhs_index = 0; rhs_index != 8; ++rhs_index) {
      for (initial = 0; initial != 32; ++initial) {
        test_u16 lhs =
            (operation < 15 || (operation >= 40 && operation < 71) ||
             operation >= 114)
                ? words[lhs_index]
                : (0xa500U | bytes[lhs_index]);
        test_u16 rhs;

        if (operation >= 122)
          rhs = words[rhs_index];
        else if (operation >= 118)
          rhs = loop_long[rhs_index];
        else if (operation >= 114)
          rhs = loop_short[rhs_index];
        else if (operation >= 71)
          rhs = 0x5a00U | bytes[rhs_index];
        else if (operation >= 48)
          rhs = words[rhs_index];
        else if (operation >= 40)
          rhs = sfr_immediates[rhs_index];
        else if (operation >= 36)
          rhs = 0x5a00U |
                (operation < 38 ? rhs_index : immediates[rhs_index]);
        else
          rhs = operation < 15 ? words[rhs_index]
                               : (0x5a00U | bytes[rhs_index]);

        ++case_id;
        if (test_alu_direct_memory(operation, lhs, rhs, initial) !=
            expected(operation, lhs, rhs, initial))
          return case_id;
      }
    }
  }
  return 0;
}

void main(void) {
  test_u16 operation;

  tap_plan(142U);
  for (operation = 0; operation != 142U; ++operation)
    tap_is_u32(run_operation(operation), 0UL, "direct-memory ALU form");
}
