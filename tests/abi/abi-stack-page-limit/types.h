#ifndef ABI_STACK_PAGE_LIMIT_TYPES_H
#define ABI_STACK_PAGE_LIMIT_TYPES_H

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_stack_page_frame {
  abi_u8 bytes[16382];
  abi_u16 edge;
};

abi_u16 tasking_stack_page_limit(void);
abi_u16 tasking_calls_llvm_stack_page_limit(void);
abi_u16 llvm_stack_page_limit_bridge(void);
abi_u16 tasking_stack_edge_apply(volatile abi_u16 *edge, abi_u16 value);
abi_u16 llvm_stack_edge_apply_bridge(volatile abi_u16 *edge, abi_u16 value);

#define ABI_STACK_EDGE_OFFSET 0x3ffeU
#define ABI_STACK_EDGE_PAGE 0x0001U

#endif
