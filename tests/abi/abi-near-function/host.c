#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_entry_proxy(abi_near_fn2 near_target,
                                abi_far_fn2 far_target,
                                abi_u16 a, abi_u16 b);

static abi_u16 expected_llvm_near(abi_u16 a, abi_u16 b) {
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

static abi_u32 expected_llvm_entry(abi_u16 a, abi_u16 b) {
  union abi_u32_words result;
  result.words.low =
      (abi_u16)(expected_near_call(a, b) ^ expected_far_call(a, b) ^ 2U);
  result.words.high = expected_llvm_near(a, b);
  return result.value;
}

void main(void) {
  abi_u16 case_id = 3;

  c166_test_begin(305, 0x16600131UL);
  c166_test_check_u32(1, 2UL, sizeof(abi_near_fn2));
  c166_test_check_u32(2, 4UL, sizeof(abi_far_fn2));

#define RUN_NEAR_FUNCTION_VECTOR(a, b)                                      \
  c166_test_check_u32(case_id++,                                            \
                      (abi_u16)(expected_llvm_near(a, b) + 7U),             \
                      tasking_calls_llvm_near(a, b));                       \
  c166_test_check_u32(case_id++, expected_llvm_entry(a, b),                 \
                      llvm_entry_proxy(tasking_near_target,                 \
                                       tasking_far_target, a, b));

  ABI_NEAR_FUNCTION_VECTORS(RUN_NEAR_FUNCTION_VECTOR)
#undef RUN_NEAR_FUNCTION_VECTOR

  c166_test_finish();
  simulator_stop();
}

