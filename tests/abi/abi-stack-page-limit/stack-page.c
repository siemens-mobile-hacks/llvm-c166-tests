#include "types.h"

#if defined(C166_TEST_LLVM)
#define STACK_PAGE_BOUNDARY                                                \
  __attribute__((noinline, optnone, section(".stack_page_limit")))
#else
#define STACK_PAGE_BOUNDARY
#endif

#define ABI_STACK_PAGE_FUNCTION stack_page_limit
#define ABI_STACK_PAGE_ATTRIBUTE STACK_PAGE_BOUNDARY
#include "stack-page-kernel.inc"
