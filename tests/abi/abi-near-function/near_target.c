#include "types.h"

extern abi_u16 ABI_NEAR_FUNCTION llvm_near_code_base(abi_u16 a, abi_u16 b);

abi_u16 ABI_NEAR_FUNCTION tasking_near_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a ^ b ^ 0x5a5aU);
}

abi_u16 tasking_far_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a + (abi_u16)(b << 1) + 0x2468U);
}

abi_u16 tasking_calls_llvm_near(abi_u16 a, abi_u16 b) {
  return (abi_u16)(llvm_near_code_base(a, b) + 7U);
}
