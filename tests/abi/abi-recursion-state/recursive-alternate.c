#include "types.h"

#define ABI_RECURSION_FUNCTION recursion_alternate
#define ABI_RECURSION_LEAF recursion_alternate_leaf
#define ABI_RECURSION_ATTRIBUTE                                                \
  C166_NOINLINE C166_SECTION(".abi.recursion.alternate")
#include "recursion-kernel.inc"

#define ABI_RECURSION_FUNCTION recursion_alternate_primary_leaf
#define ABI_RECURSION_LEAF recursion_primary_leaf
#define ABI_RECURSION_ATTRIBUTE                                                \
  C166_NOINLINE C166_SECTION(".abi.recursion.alternate-primary-leaf")
#include "recursion-kernel.inc"
