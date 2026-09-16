#include "types.h"

static abi_u16 recursion_leaf_value(abi_u16 depth, abi_u16 seed) {
  return (seed ^ ((depth << 12) + (depth << 8) + (depth << 4) + depth)) +
         0x2468U;
}

C166_NOINLINE C166_SECTION(".abi.recursion.primary-leaf")
abi_u16 recursion_primary_leaf(abi_u16 depth, abi_u16 seed) {
  return recursion_leaf_value(depth, seed);
}

C166_NOINLINE C166_SECTION(".abi.recursion.alternate-leaf")
abi_u16 recursion_alternate_leaf(abi_u16 depth, abi_u16 seed) {
  return recursion_leaf_value(depth, seed);
}
