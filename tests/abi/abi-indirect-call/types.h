#ifndef C166_ABI_INDIRECT_CALL_TYPES_H
#define C166_ABI_INDIRECT_CALL_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

typedef abi_u32 (*abi_indirect_fn)(abi_u16 a, abi_u16 b, abi_u16 c,
                                   abi_u32 pair, abi_u16 tail);

abi_u32 c166_indirect_entry(abi_u16 prefix, abi_indirect_fn target,
                            abi_u16 a, abi_u16 b, abi_u16 c, abi_u32 pair,
                            abi_u16 tail);
abi_u32 c166_indirect_target(abi_u16 a, abi_u16 b, abi_u16 c, abi_u32 pair,
                             abi_u16 tail);

#endif
