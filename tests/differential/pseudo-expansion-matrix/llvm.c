#define PSEUDO_NAME llvm_pseudo
#include "kernel.inc"

abi_u32 llvm_entry(abi_u16 seed, volatile abi_s8 _far *far_value,
                   volatile abi_s8 _huge *segmented_value, abi_u32 wide) {
  return llvm_pseudo_eval(seed, far_value, segmented_value, wide);
}
