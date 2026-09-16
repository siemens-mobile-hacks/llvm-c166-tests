#ifndef ABI_GLOBAL_FUNCTION_POINTER_TYPES_H
#define ABI_GLOBAL_FUNCTION_POINTER_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;
typedef abi_u16 (*abi_fn)(abi_u16, abi_u16);

extern abi_fn volatile global_slot;

abi_u16 global_target(abi_u16 a, abi_u16 b);
abi_u16 alternate_target(abi_u16 a, abi_u16 b);
abi_fn global_exchange(abi_fn target);
abi_u16 call_global(abi_u16 a, abi_u16 b);
abi_u16 global_roundtrip(abi_fn target, abi_u16 a, abi_u16 b);
abi_fn get_global_target(void);
abi_u16 alternate_roundtrip(abi_fn target, abi_u16 a, abi_u16 b);
abi_fn alternate_exchange(abi_fn target);
abi_u16 forward_roundtrip(abi_fn target, abi_u16 a, abi_u16 b);

#endif
