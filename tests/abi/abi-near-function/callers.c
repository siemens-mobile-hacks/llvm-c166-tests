#include "types.h"

C166_NOINLINE C166_NEAR C166_SECTION(".c166.near.base")
abi_u16 c166_near_code_base(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a + (abi_u16)(b << 1) + 0x1101U);
}

C166_NOINLINE C166_NEAR
abi_u16 c166_near_calls_near(abi_near_fn2 target, abi_u16 a, abi_u16 b) {
  return (abi_u16)(target((abi_u16)(a + 1U), (abi_u16)(b ^ 0x00ffU)) +
                   0x0101U);
}

C166_NOINLINE C166_NEAR
abi_u16 c166_near_calls_far(abi_far_fn2 target, abi_u16 a, abi_u16 b) {
  return (abi_u16)(target((abi_u16)(a ^ 0x1111U), (abi_u16)(b + 3U)) ^
                   0x00f0U);
}
