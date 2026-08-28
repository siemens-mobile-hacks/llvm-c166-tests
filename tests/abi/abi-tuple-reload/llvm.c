#include "types.h"

#define LLVM_FUNCTION(name) __attribute__((noinline, section(name)))

LLVM_FUNCTION(".llvm_tuple_reload_low")
abi_u16 llvm_tuple_reload_low(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                              abi_u32 a3, abi_u32 a4, abi_u32 a5,
                              abi_u16 selector) {
  abi_u32 v0 = a0 + 0x00010001UL;
  abi_u32 v1 = a1 + 0x01020304UL;
  abi_u32 v2 = a2 + 0x10203040UL;
  abi_u32 v3 = a3 + 0x7fff0001UL;
  abi_u32 v4 = a4 + 0x8000ffffUL;
  abi_u32 v5 = a5 + 0xfedcba98UL;

  switch (tuple_reload_selector(selector)) {
  case 0: return (abi_u16)v0;
  case 1: return (abi_u16)v1;
  case 2: return (abi_u16)v2;
  case 3: return (abi_u16)v3;
  case 4: return (abi_u16)v4;
  default: return (abi_u16)v5;
  }
}

LLVM_FUNCTION(".llvm_tuple_reload_high")
abi_u16 llvm_tuple_reload_high(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                               abi_u32 a3, abi_u32 a4, abi_u32 a5,
                               abi_u16 selector) {
  abi_u32 v0 = a0 + 0x00010001UL;
  abi_u32 v1 = a1 + 0x01020304UL;
  abi_u32 v2 = a2 + 0x10203040UL;
  abi_u32 v3 = a3 + 0x7fff0001UL;
  abi_u32 v4 = a4 + 0x8000ffffUL;
  abi_u32 v5 = a5 + 0xfedcba98UL;

  switch (tuple_reload_selector(selector)) {
  case 0: return (abi_u16)(v0 >> 16);
  case 1: return (abi_u16)(v1 >> 16);
  case 2: return (abi_u16)(v2 >> 16);
  case 3: return (abi_u16)(v3 >> 16);
  case 4: return (abi_u16)(v4 >> 16);
  default: return (abi_u16)(v5 >> 16);
  }
}
