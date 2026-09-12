#include "c166-address-spaces.h"
#include "types.h"

#if C166_TEST_MODEL_IS_TINY
#define TEST_FAR
#else
#define TEST_FAR _far
#endif

static volatile abi_s8 _near near_values[4];

static abi_u16 fold32(abi_u32 value) {
  return (abi_u16)value ^ (abi_u16)(value >> 16);
}

__attribute__((noinline)) static abi_u16
carry16(abi_u16 left, abi_u16 right, abi_u16 carry_in, abi_u16 operation) {
  abi_u16 carry;
  abi_u16 value;

  if (operation & 1U)
    value = __builtin_subc(left, right, carry_in, &carry);
  else
    value = __builtin_addc(left, right, carry_in, &carry);
  return value ^ (abi_u16)(carry * 0x101U);
}

__attribute__((noinline)) static abi_u16
carry32(abi_u32 left, abi_u32 right, abi_u32 carry_in, abi_u16 operation) {
  abi_u32 carry;
  abi_u32 value;

  if (operation & 1U)
    value = __builtin_subcl(left, right, carry_in, &carry);
  else
    value = __builtin_addcl(left, right, carry_in, &carry);
  return fold32(value) ^ (abi_u16)(carry * 0x101U);
}

__attribute__((noinline)) static abi_u16
signed_postincrement(volatile abi_s8 TEST_FAR *far_values, abi_u16 seed) {
  volatile abi_s8 _near *near_cursor;
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

__attribute__((noinline, section(".llvm_backend_operation_eval"))) abi_u16
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
