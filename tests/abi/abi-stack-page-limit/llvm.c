#include "types.h"

#define LLVM_SECTION(name) __attribute__((noinline, section(name)))
#define LLVM_BOUNDARY_SECTION(name) \
  __attribute__((noinline, optnone, section(name)))

#define ABI_STACK_PAGE_FUNCTION llvm_stack_page_limit
#define ABI_STACK_PAGE_ATTRIBUTE LLVM_BOUNDARY_SECTION(".llvm_stack_page_limit")
#include "stack-page-kernel.inc"

LLVM_SECTION(".llvm_call_tasking_stack_page_limit")
abi_u16 llvm_call_tasking_stack_page_limit(void) {
  return tasking_stack_page_limit();
}

LLVM_SECTION(".llvm_stack_edge_apply")
abi_u16 llvm_stack_edge_apply(volatile abi_u16 *edge, abi_u16 value) {
  *edge = value;
  return *edge;
}

extern volatile abi_u16 *volatile llvm_escaped_stack_edge;

LLVM_BOUNDARY_SECTION(".llvm_stack_escape_store")
abi_u16 llvm_stack_escape_store(void) {
  volatile struct abi_stack_page_frame frame;

  frame.edge = 0x3456U;
  llvm_escaped_stack_edge = &frame.edge;
  return frame.edge;
}
