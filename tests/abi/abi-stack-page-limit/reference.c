#include "types.h"

#pragma fragment

#define ABI_STACK_PAGE_FUNCTION tasking_stack_page_limit
#include "stack-page-kernel.inc"

abi_u16 tasking_calls_llvm_stack_page_limit(void) {
  return llvm_stack_page_limit_bridge();
}

abi_u16 tasking_stack_edge_apply(volatile abi_u16 *edge, abi_u16 value) {
  *edge = value;
  return *edge;
}
