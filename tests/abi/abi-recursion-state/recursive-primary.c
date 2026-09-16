#include "types.h"

#define ABI_RECURSION_FUNCTION recursion_primary
#define ABI_RECURSION_LEAF recursion_primary_leaf
#define ABI_RECURSION_ATTRIBUTE                                                \
  C166_NOINLINE C166_SECTION(".abi.recursion.primary")
#include "recursion-kernel.inc"

#define ABI_RECURSION_FUNCTION recursion_primary_alternate_leaf
#define ABI_RECURSION_LEAF recursion_alternate_leaf
#define ABI_RECURSION_ATTRIBUTE                                                \
  C166_NOINLINE C166_SECTION(".abi.recursion.primary-alternate-leaf")
#include "recursion-kernel.inc"
