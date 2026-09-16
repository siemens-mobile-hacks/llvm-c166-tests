#include "types.h"

C166_NOINLINE C166_SECTION(".medium.near.target")
abi_u16 c166_medium_near_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x2468U) ^
                   (abi_u16)(b + (abi_u16)(a << 2)));
}

C166_NOINLINE C166_HUGE C166_SECTION(".medium.huge.target")
abi_u16 c166_medium_huge_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0x5aa5U) +
                   (abi_u16)(b ^ (abi_u16)(a >> 2)));
}
