#ifndef ABI_NESTED_CALL_CHAINS_TYPES_H
#define ABI_NESTED_CALL_CHAINS_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

typedef abi_u32 (*abi_chain_fn)(abi_u16 a, abi_u32 b, abi_u16 c,
                                abi_u16 d);

enum abi_nested_operation {
  ABI_NESTED_DIRECT_INDIRECT = 0,
  ABI_NESTED_INDIRECT_DIRECT = 1,
  ABI_NESTED_LEAF = 2,
  ABI_NESTED_DIRECT_INDIRECT_TWICE = 3
};

extern volatile abi_u16 c166_nested_call_count;

abi_u32 c166_nested_leaf_a(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);
abi_u32 c166_nested_leaf_b(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);
abi_u32 c166_nested_reentrant(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);
abi_u32 c166_nested_leaf(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);
abi_u32 c166_direct_indirect_middle(abi_chain_fn callback, abi_u16 a,
                                    abi_u32 b, abi_u16 c, abi_u16 d);
abi_u32 c166_nested_dispatch(abi_u16 operation, abi_chain_fn callback,
                             abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);

#endif
