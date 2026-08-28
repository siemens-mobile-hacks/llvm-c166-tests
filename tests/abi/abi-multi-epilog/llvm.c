#include "types.h"

__attribute__((noinline, section(".llvm_multi_epilog_pressure")))
abi_u32 llvm_multi_epilog_pressure(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                                   abi_u32 a3, abi_u32 a4, abi_u32 a5,
                                   abi_u16 mode, abi_u16 rounds) {
  if (mode == 0xffffU)
    return a0 ^ a5;

  abi_u32 v0 = a0 + 0x00010001UL;
  abi_u32 v1 = a1 + 0x01020304UL;
  abi_u32 v2 = a2 + 0x10203040UL;
  abi_u32 v3 = a3 + 0x7fff0001UL;
  abi_u32 v4 = a4 + 0x8000ffffUL;
  abi_u32 v5 = a5 + 0xfedcba98UL;
  abi_u32 accumulator;
  abi_u16 gate = multi_epilog_selector(mode);
  abi_u16 index;

  if (gate == 0U)
    return v0 ^ v5;

  accumulator = v0 + (v1 ^ v5);
  for (index = 0; index < rounds; ++index) {
    abi_u16 step = multi_epilog_selector((gate + index) & 7U);
    abi_u32 selected;

    switch ((index + step) % 6U) {
    case 0: selected = v0; break;
    case 1: selected = v1; break;
    case 2: selected = v2; break;
    case 3: selected = v3; break;
    case 4: selected = v4; break;
    default: selected = v5; break;
    }
    accumulator = ((accumulator << 5) | (accumulator >> 27)) ^ selected;
    accumulator += (abi_u32)step * 0x10001UL + index;
    if (step == 7U)
      return accumulator ^ v4;
  }

  if (gate & 1U)
    return accumulator + v1 + v3;
  if (gate & 2U)
    return accumulator ^ v2 ^ v5;
  return accumulator + v4;
}
