#include "types.h"

extern float tasking_float_slot3(abi_u16, abi_u16, abi_u16, float, abi_u16);
extern float llvm_float32_slot3(abi_u16, abi_u16, abi_u16, float, abi_u16);

__attribute__((noinline, section(".llvm_float32_call_tasking")))
float llvm_float32_call_tasking(abi_u16 a, abi_u16 b, abi_u16 c, float value,
                                abi_u16 tail) {
  return tasking_float_slot3(a, b, c, value, tail);
}

__attribute__((noinline, section(".llvm_float32_icall_tasking")))
float llvm_float32_icall_tasking(abi_float_slot3_fn callee, abi_u16 a,
                                 abi_u16 b, abi_u16 c, float value,
                                 abi_u16 tail) {
  return callee(a, b, c, value, tail);
}

// This call crosses an actual LLVM translation-unit boundary.  It prevents
// same-TU knowledge or inlining from hiding a public Classic ABI mismatch.
__attribute__((noinline, section(".llvm_float32_cross_tu")))
float llvm_float32_cross_tu(abi_u16 a, abi_u16 b, abi_u16 c, float value,
                            abi_u16 tail) {
  return llvm_float32_slot3(a, b, c, value, tail);
}
