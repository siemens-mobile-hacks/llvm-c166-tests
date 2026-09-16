#ifndef ABI_STACK_PAGE_LIMIT_TYPES_H
#define ABI_STACK_PAGE_LIMIT_TYPES_H

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_stack_page_frame {
  abi_u8 bytes[16382];
  abi_u16 edge;
};

struct abi_stack_escape_frame {
  abi_u8 bytes[16000];
  abi_u16 edge;
};

abi_u16 stack_page_limit(void);
abi_u16 stack_edge_apply(volatile abi_u16 *edge, abi_u16 value);
abi_u16 stack_escape_store(void);

#endif
