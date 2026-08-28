#include "types.h"

__attribute__((noinline, section(".llvm_clz32_eval")))
abi_u16 llvm_clz32_eval(abi_u32 value) {
  if (value == 0UL)
    return 32U;
  return (abi_u16)__builtin_clzl(value);
}
