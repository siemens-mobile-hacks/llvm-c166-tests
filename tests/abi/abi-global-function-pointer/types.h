#ifndef ABI_GLOBAL_FUNCTION_POINTER_TYPES_H
#define ABI_GLOBAL_FUNCTION_POINTER_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;
typedef abi_u16 (*abi_fn)(abi_u16, abi_u16);

abi_u16 tasking_global_roundtrip(abi_fn target, abi_u16 a, abi_u16 b);
abi_fn tasking_global_exchange(abi_fn target);

#endif
