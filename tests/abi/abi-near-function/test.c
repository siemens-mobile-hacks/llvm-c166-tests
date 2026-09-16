#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

static abi_u16 expected_base(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a + (abi_u16)(b << 1) + 0x1101U);
}

static abi_u16 expected_near_call(abi_u16 a, abi_u16 b) {
  abi_u16 target =
      (abi_u16)((abi_u16)(a + 1U) ^ (abi_u16)(b ^ 0x00ffU) ^ 0x5a5aU);
  return (abi_u16)(target + 0x0101U);
}

static abi_u16 expected_far_call(abi_u16 a, abi_u16 b) {
  abi_u16 target =
      (abi_u16)((abi_u16)(a ^ 0x1111U) +
                (abi_u16)((abi_u16)(b + 3U) << 1) + 0x2468U);
  return (abi_u16)(target ^ 0x00f0U);
}

static abi_u32 expected_entry(abi_u16 a, abi_u16 b) {
  union abi_u32_words result;

  result.words.low =
      (abi_u16)(expected_near_call(a, b) ^ expected_far_call(a, b) ^ 2U);
  result.words.high = expected_base(a, b);
  return result.value;
}

#define RUN_VECTOR(a, b)                                                     \
  tap_is_u32(c166_calls_near(a, b), (abi_u16)(expected_base(a, b) + 7U),    \
             "direct call to near function");                              \
  tap_is_u32(c166_near_entry(c166_near_target, c166_far_target, a, b),      \
             expected_entry(a, b), "near and far function pointers");

C166_SECTION(".c166.near.driver")
void main(void) {
  tap_plan(14U);
  tap_is_u32(sizeof(abi_near_fn2), 2UL, "near function-pointer size");
  tap_is_u32(sizeof(abi_far_fn2), 4UL, "far function-pointer size");
  ABI_NEAR_FUNCTION_VECTORS(RUN_VECTOR)
}
