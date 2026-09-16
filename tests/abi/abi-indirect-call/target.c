#include "c166_test.h"
#include "types.h"

C166_NOINLINE
abi_u32 c166_indirect_target(abi_u16 a, abi_u16 b, abi_u16 c, abi_u32 pair,
                             abi_u16 tail) {
  abi_u32 value = pair;

  value += (abi_u32)a << 16;
  value += ((abi_u32)b << 1) + c;
  value ^= ((abi_u32)tail << 16) | b;
  return value;
}
