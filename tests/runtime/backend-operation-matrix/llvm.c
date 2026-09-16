#include "c166_test.h"
#include "types.h"

#if __C166_MEMORY_MODEL__ == 4
#define TEST_FAR
#else
#define TEST_FAR C166_FAR
#endif

static volatile abi_s8 C166_NEAR near_values[4];
static volatile abi_s8 TEST_FAR test_values[4];

static abi_u16 fold32(abi_u32 value) {
  return (abi_u16)value ^ (abi_u16)(value >> 16);
}

C166_NOINLINE static abi_u16
carry16(abi_u16 left, abi_u16 right, abi_u16 carry_in, abi_u16 operation) {
  abi_u16 carry;
  abi_u16 value;

  if (operation & 1U)
    value = __builtin_subc(left, right, carry_in, &carry);
  else
    value = __builtin_addc(left, right, carry_in, &carry);
  return value ^ (abi_u16)(carry * 0x101U);
}

C166_NOINLINE static abi_u16
carry32(abi_u32 left, abi_u32 right, abi_u32 carry_in, abi_u16 operation) {
  abi_u32 carry;
  abi_u32 value;

  if (operation & 1U)
    value = __builtin_subcl(left, right, carry_in, &carry);
  else
    value = __builtin_addcl(left, right, carry_in, &carry);
  return fold32(value) ^ (abi_u16)(carry * 0x101U);
}

C166_NOINLINE static abi_u16
signed_postincrement(volatile abi_s8 TEST_FAR *far_values, abi_u16 seed) {
  volatile abi_s8 C166_NEAR *near_cursor;
  volatile abi_s8 TEST_FAR *far_cursor = far_values;
  abi_u16 count = (abi_u16)((seed >> 8) & 3U) + 1U;
  abi_s16 sum = 0;
  abi_u16 index;

  for (index = 0; index != count; ++index)
    near_values[index] = (abi_s8)(seed + index * 29U);
  near_cursor = near_values;
  for (index = 0; index != count; ++index) {
    sum += *near_cursor++;
    sum += *far_cursor++;
  }
  return (abi_u16)sum;
}

C166_NOINLINE C166_SECTION(".llvm_backend_operation_eval") abi_u16
llvm_backend_operation_eval(abi_u16 operation, abi_u32 left, abi_u32 right,
                            abi_u16 carry_in,
                            volatile abi_s8 TEST_FAR *far_values) {
  abi_u16 divisor = (abi_u16)right | 1U;

  switch (operation) {
  case 0:
  case 1:
    return carry16((abi_u16)left, (abi_u16)right, carry_in, operation);
  case 2:
  case 3:
    return carry32(left, right, carry_in, operation);
  case 4:
    return fold32((abi_u32)((abi_s32)left >> 1));
  case 5:
    return fold32((abi_u32)((abi_s32)left >> 1)) ^ (abi_u16)(left & 1U);
  case 6:
    return fold32((abi_u32)((abi_s16)left * (abi_s16)right +
                            (abi_s32)0x12345678L));
  case 7:
    return fold32(left / divisor);
  case 8: {
    abi_u32 quotient = left / divisor;
    abi_u16 remainder = (abi_u16)(left % divisor);
    return fold32(quotient) ^ remainder;
  }
  case 9:
    return signed_postincrement(far_values, (abi_u16)left);
  default:
    return 0;
  }
}

static abi_u16 expected_value(abi_u16 operation, abi_u32 left, abi_u32 right,
                              abi_u16 carry_in) {
  abi_u16 divisor = (abi_u16)right | 1U;
  abi_u32 value;
  abi_u32 carry;

  switch (operation) {
  case 0:
    value = (abi_u32)(abi_u16)left + (abi_u16)right + carry_in;
    carry = value >> 16;
    return (abi_u16)value ^ (abi_u16)(carry * 0x101U);
  case 1:
    value = (abi_u32)(abi_u16)left - (abi_u16)right - carry_in;
    carry = (abi_u32)(abi_u16)left <
            (abi_u32)(abi_u16)right + carry_in;
    return (abi_u16)value ^ (abi_u16)(carry * 0x101U);
  case 2: {
    abi_u32 result = left + right + carry_in;
    carry = result < left || (carry_in && result == left);
    return fold32(result) ^ (abi_u16)(carry * 0x101U);
  }
  case 3: {
    abi_u32 result = left - right - carry_in;
    carry = left < right || (carry_in && left == right);
    return fold32(result) ^ (abi_u16)(carry * 0x101U);
  }
  case 4:
    return fold32((abi_u32)((abi_s32)left >> 1));
  case 5:
    return fold32((abi_u32)((abi_s32)left >> 1)) ^ (abi_u16)(left & 1U);
  case 6:
    return fold32((abi_u32)((abi_s16)left * (abi_s16)right +
                            (abi_s32)0x12345678L));
  case 7:
    return fold32(left / divisor);
  case 8:
    return fold32(left / divisor) ^ (abi_u16)(left % divisor);
  case 9: {
    abi_u16 count = (abi_u16)(((abi_u16)left >> 8) & 3U) + 1U;
    abi_s16 sum = 0;
    abi_u16 index;
    for (index = 0; index != count; ++index) {
      sum += (abi_s8)((abi_u16)left + index * 29U);
      sum += test_values[index];
    }
    return (abi_u16)sum;
  }
  default:
    return 0;
  }
}

void main(void) {
  static const abi_u32 left[] = {
      0xffffUL,     0U,          0xffffffffUL, 0U,          0x87654321UL,
      0x87654321UL, 0x00008003UL, 0xfedcba98UL, 0xfedcba98UL, 0x12345678UL};
  static const abi_u32 right[] = {1UL, 1UL, 1UL, 1UL, 0U, 0U, 0x00007ffdUL,
                                  0x00001235UL, 0x00001235UL, 0U};
  static const abi_u16 carry[] = {1U, 1U, 1U, 1U, 0U,
                                  0U, 0U, 0U, 0U, 0U};
  abi_u16 operation;

  test_values[0] = -100;
  test_values[1] = -1;
  test_values[2] = 2;
  test_values[3] = 99;
  tap_plan(10);
  for (operation = 0; operation != 10U; ++operation) {
    abi_u16 actual = llvm_backend_operation_eval(
        operation, left[operation], right[operation], carry[operation],
        test_values);
    tap_is_u32(actual,
               expected_value(operation, left[operation], right[operation],
                              carry[operation]),
               "backend operation");
  }
}
