#include "types.h"

C166_NOINLINE C166_SECTION(".abi.indirect.target0")
abi_u32 indirect_target0(void) {
  return 0x13572468UL;
}

C166_NOINLINE C166_SECTION(".abi.indirect.target2")
abi_u32 indirect_target2(abi_u16 a, abi_u16 b) {
  return 0x24680000UL ^ ((abi_u32)a << 16) ^ b;
}

C166_NOINLINE C166_SECTION(".abi.indirect.target5")
abi_u32 indirect_target5(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                         abi_u16 e) {
  abi_u32 value = 0x5a5a0000UL;
  value += a;
  value ^= (abi_u32)b << 16;
  value += (abi_u32)c << 1;
  value ^= d;
  value += (abi_u32)e << 16;
  return value;
}
