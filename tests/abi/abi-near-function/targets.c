#include "types.h"

C166_NOINLINE C166_NEAR
abi_u16 c166_near_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a ^ b ^ 0x5a5aU);
}

C166_NOINLINE
abi_u16 c166_far_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a + (abi_u16)(b << 1) + 0x2468U);
}

C166_NOINLINE C166_SECTION(".c166.near.driver")
abi_u16 c166_calls_near(abi_u16 a, abi_u16 b) {
  return (abi_u16)(c166_near_code_base(a, b) + 7U);
}
