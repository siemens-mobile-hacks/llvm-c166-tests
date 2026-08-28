#include "types.h"

#pragma fragment

abi_u16 tasking_recursion_leaf(abi_u16 depth, abi_u16 seed) {
  return (seed ^ ((depth << 12) + (depth << 8) + (depth << 4) + depth)) +
         0x2468U;
}

#define ABI_RECURSION_FUNCTION tasking_recursive
#define ABI_RECURSION_LEAF tasking_recursion_leaf
#include "recursion-kernel.inc"

#define ABI_RECURSION_FUNCTION tasking_recursive_llvm_leaf
#define ABI_RECURSION_LEAF llvm_recursion_leaf_bridge
#include "recursion-kernel.inc"
