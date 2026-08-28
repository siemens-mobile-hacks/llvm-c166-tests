#include "types.h"

#define LLVM_SECTION(name) __attribute__((noinline, section(name)))

LLVM_SECTION(".llvm_recursion_leaf")
abi_u16 llvm_recursion_leaf(abi_u16 depth, abi_u16 seed) {
  return (seed ^ ((depth << 12) + (depth << 8) + (depth << 4) + depth)) +
         0x2468U;
}

#define ABI_RECURSION_FUNCTION llvm_recursive
#define ABI_RECURSION_LEAF llvm_recursion_leaf
#define ABI_RECURSION_ATTRIBUTE LLVM_SECTION(".llvm_recursive")
#include "recursion-kernel.inc"

#define ABI_RECURSION_FUNCTION llvm_recursive_tasking_leaf
#define ABI_RECURSION_LEAF tasking_recursion_leaf
#define ABI_RECURSION_ATTRIBUTE LLVM_SECTION(".llvm_recursive_tasking_leaf")
#include "recursion-kernel.inc"

LLVM_SECTION(".llvm_call_tasking_recursive")
abi_u32 llvm_call_tasking_recursive(abi_u16 depth, abi_u16 seed) {
  return tasking_recursive(depth, seed);
}
