#include "types.h"

C166_NOINLINE C166_SECTION(".abi.recursion.forward-alternate")
abi_u32 recursion_forward_alternate(abi_u16 depth, abi_u16 seed) {
  return recursion_alternate(depth, seed);
}
