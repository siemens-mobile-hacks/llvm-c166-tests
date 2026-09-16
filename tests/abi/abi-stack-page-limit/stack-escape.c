#include "c166_test.h"
#include "types.h"

C166_NOINLINE C166_SECTION(".stack_edge_apply")
abi_u16 stack_edge_apply(volatile abi_u16 *edge, abi_u16 value) {
  *edge = value;
  return *edge;
}

C166_NOINLINE C166_SECTION(".stack_escape_store")
abi_u16 stack_escape_store(void) {
  volatile struct abi_stack_escape_frame frame;

  frame.edge = 0x3456U;
  return stack_edge_apply(&frame.edge, frame.edge);
}
