#ifndef ABI_STACK_PAGE_FUNCTION
#error ABI_STACK_PAGE_FUNCTION must name the generated function
#endif
#ifndef ABI_STACK_PAGE_ATTRIBUTE
#define ABI_STACK_PAGE_ATTRIBUTE
#endif

ABI_STACK_PAGE_ATTRIBUTE
abi_u16 ABI_STACK_PAGE_FUNCTION(void) {
  volatile struct abi_stack_page_frame frame;

  frame.edge = 0x1234U;
  return frame.edge;
}

#undef ABI_STACK_PAGE_ATTRIBUTE
#undef ABI_STACK_PAGE_FUNCTION
