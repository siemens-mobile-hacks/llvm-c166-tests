#include "c166_test.h"
#include "types.h"

#define ABI_MIXED_FUNCTION mixed_recursion_a
#define ABI_MIXED_OTHER mixed_recursion_b
#define ABI_MIXED_ATTRIBUTE \
  C166_NOINLINE C166_SECTION(".abi_mixed_recursion_a")
#include "mixed-kernel.inc"
