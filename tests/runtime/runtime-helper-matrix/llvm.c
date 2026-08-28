#include "types.h"

__attribute__((noinline, section(".llvm_runtime_helper_eval")))
abi_u32 llvm_runtime_helper_eval(abi_u16 operation, abi_u32 a, abi_u32 b,
                                 abi_u16 shift) {
  abi_s32 sa = (abi_s32)a;
  abi_s32 sb = (abi_s32)b;

  switch (operation) {
  case 0:
    return a * b;
  case 1:
    return a / b;
  case 2:
    return a % b;
  case 3:
    return (abi_u32)(sa / sb);
  case 4:
    return (abi_u32)(sa % sb);
  case 5:
    return a << shift;
  case 6:
    return a >> shift;
  case 7:
    return (abi_u32)(sa >> shift);
  case 8:
    return a == 0UL ? 32UL : (abi_u32)__builtin_clzl(a);
  default:
    return 0UL;
  }
}
