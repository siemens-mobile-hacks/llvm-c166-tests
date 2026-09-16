#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

C166_NOINLINE C166_SECTION(".llvm_clz32_eval")
abi_u16 llvm_clz32_eval(abi_u32 value) {
#if defined(C166_TEST_LLVM)
  if (value == 0UL)
    return 32U;
  return (abi_u16)__builtin_clzl(value);
#else
  abi_u16 count = 0;

  if (value == 0UL)
    return 32U;
  while ((value & 0x80000000UL) == 0) {
    value <<= 1;
    ++count;
  }
  return count;
#endif
}

#define CHECK_CLZ(id, value, expected)                                        \
  tap_is_u32(llvm_clz32_eval(value), expected, "clz32 vector " #id);

void main(void) {
  tap_plan(18);
  ABI_CLZ32_VECTORS(CHECK_CLZ)
}
