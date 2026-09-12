#include "c166-address-spaces.h"
#include "c166-test-result.h"
#include "c166-test-runtime.h"
#include "types.h"

#if C166_TEST_MODEL_IS_TINY
#define TEST_DATA_PTR
#else
#define TEST_DATA_PTR _far
#endif

static volatile abi_s8 _near far_values[4];

extern abi_u16 llvm_entry_proxy(abi_u16, abi_u32, abi_u32, abi_u16,
                                volatile abi_s8 TEST_DATA_PTR *);

static abi_u16 fold32(abi_u32 value) {
  return (abi_u16)value ^ (abi_u16)(value >> 16);
}

static abi_u16 expected(abi_u16 operation, abi_u32 left, abi_u32 right,
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
    carry = ((abi_u32)(abi_u16)left <
             (abi_u32)(abi_u16)right + carry_in);
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
      sum += far_values[index];
    }
    return (abi_u16)sum;
  }
  default:
    return 0;
  }
}

void main(void) {
  static const abi_u32 left[] = {
      0xffffUL, 0U, 0xffffffffUL, 0U, 0x87654321UL,
      0x87654321UL, 0x00008003UL, 0xfedcba98UL,
      0xfedcba98UL, 0x12345678UL};
  static const abi_u32 right[] = {
      1UL, 1UL, 1UL, 1UL, 0U, 0U, 0x00007ffdUL,
      0x00001235UL, 0x00001235UL, 0U};
  static const abi_u16 carry[] = {1U, 1U, 1U, 1U, 0U,
                                   0U, 0U, 0U, 0U, 0U};
  abi_u16 operation;

  far_values[0] = -100;
  far_values[1] = -1;
  far_values[2] = 2;
  far_values[3] = 99;
  c166_test_begin(324, 0);
  for (operation = 0; operation != 10U; ++operation)
    c166_test_check_u32(
        operation, expected(operation, left[operation], right[operation],
                            carry[operation]),
        llvm_entry_proxy(operation, left[operation], right[operation],
                         carry[operation], far_values));
  c166_test_finish();
  simulator_stop();
}
