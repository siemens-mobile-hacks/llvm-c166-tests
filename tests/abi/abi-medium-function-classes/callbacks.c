#include "types.h"

C166_NOINLINE C166_SECTION(".medium.near.callback")
abi_u16 c166_medium_near_callback(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x1357U) ^
                   (abi_u16)(b + (abi_u16)(a << 1)));
}

C166_NOINLINE C166_HUGE C166_SECTION(".medium.huge.callback")
abi_u16 c166_medium_huge_callback(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0xa55aU) +
                   (abi_u16)(b ^ (abi_u16)(a >> 1)));
}
