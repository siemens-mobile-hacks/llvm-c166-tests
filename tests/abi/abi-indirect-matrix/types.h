#ifndef ABI_INDIRECT_MATRIX_TYPES_H
#define ABI_INDIRECT_MATRIX_TYPES_H

#include "c166_test.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

typedef abi_u32 (*abi_fn0)(void);
typedef abi_u32 (*abi_fn2)(abi_u16 a, abi_u16 b);
typedef abi_u32 (*abi_fn5)(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                           abi_u16 e);

abi_u32 indirect_target0(void);
abi_u32 indirect_target2(abi_u16 a, abi_u16 b);
abi_u32 indirect_target5(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                         abi_u16 e);

abi_u32 indirect_call0(abi_fn0 volatile *slot, abi_fn0 target);
abi_u32 indirect_call2(abi_fn2 volatile *slot, abi_fn2 target, abi_u16 a,
                       abi_u16 b);
abi_u32 indirect_call5(abi_fn5 volatile *slot, abi_fn5 target, abi_u16 a,
                       abi_u16 b, abi_u16 c, abi_u16 d, abi_u16 e);

abi_fn0 indirect_get0(void);
abi_fn2 indirect_get2(void);
abi_fn5 indirect_get5(void);

#endif
