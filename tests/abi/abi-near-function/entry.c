#include "types.h"

C166_NOINLINE C166_NEAR C166_SECTION(".c166.near.entry")
abi_u32 c166_near_entry(abi_near_fn2 near_target, abi_far_fn2 far_target,
                        abi_u16 a, abi_u16 b) {
  union abi_u32_words result;
  abi_u16 direct = c166_near_code_base(a, b);
  abi_u16 near_result = c166_near_calls_near(near_target, a, b);
  abi_u16 far_result = c166_near_calls_far(far_target, a, b);

  result.words.low =
      (abi_u16)(near_result ^ far_result ^ sizeof(abi_near_fn2));
  result.words.high = direct;
  return result.value;
}
