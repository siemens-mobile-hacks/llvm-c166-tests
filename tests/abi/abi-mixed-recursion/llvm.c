#include "types.h"

#define ABI_MIXED_FUNCTION llvm_mixed_recursion
#define ABI_MIXED_OTHER tasking_mixed_recursion
#define ABI_MIXED_ATTRIBUTE \
  __attribute__((noinline, section(".llvm_mixed_recursion")))
#include "mixed-kernel.inc"
