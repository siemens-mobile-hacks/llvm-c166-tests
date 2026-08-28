#include "types.h"

#define NOINLINE __attribute__((noinline))
#define NEAR_CALLER \
  __attribute__((noinline, section(".c166.near.callers")))

NOINLINE abi_u16 ABI_NEAR_FUNCTION llvm_near_code_base(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a + (abi_u16)(b << 1) + 0x1101U);
}

NOINLINE abi_u16 ABI_NEAR_FUNCTION
llvm_near_calls_near(abi_near_fn2 target, abi_u16 a, abi_u16 b) {
  return (abi_u16)(target((abi_u16)(a + 1U),
                          (abi_u16)(b ^ 0x00ffU)) +
                   0x0101U);
}

NOINLINE abi_u16 ABI_NEAR_FUNCTION
llvm_near_calls_far(abi_far_fn2 target, abi_u16 a, abi_u16 b) {
  return (abi_u16)(target((abi_u16)(a ^ 0x1111U),
                          (abi_u16)(b + 3U)) ^
                   0x00f0U);
}

NEAR_CALLER abi_u32 llvm_entry(abi_near_fn2 near_target,
                                abi_far_fn2 far_target,
                                abi_u16 a, abi_u16 b) {
  union abi_u32_words result;
  abi_u16 direct = llvm_near_code_base(a, b);
  abi_u16 near_result = llvm_near_calls_near(near_target, a, b);
  abi_u16 far_result = llvm_near_calls_far(far_target, a, b);

  result.words.low =
      (abi_u16)(near_result ^ far_result ^ sizeof(abi_near_fn2));
  result.words.high = direct;
  return result.value;
}
