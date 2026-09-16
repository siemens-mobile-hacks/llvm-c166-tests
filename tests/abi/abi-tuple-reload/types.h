#ifndef ABI_TUPLE_RELOAD_TYPES_H
#define ABI_TUPLE_RELOAD_TYPES_H

#include "c166_test.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

abi_u16 tuple_reload_selector(abi_u16 selector);
abi_u16 tuple_reload_low(abi_u32 a0, abi_u32 a1, abi_u32 a2, abi_u32 a3,
                         abi_u32 a4, abi_u32 a5, abi_u16 selector);
abi_u16 tuple_reload_high(abi_u32 a0, abi_u32 a1, abi_u32 a2, abi_u32 a3,
                          abi_u32 a4, abi_u32 a5, abi_u16 selector);
abi_u16 tuple_reload_low_probe(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                               abi_u32 a3, abi_u32 a4, abi_u32 a5,
                               abi_u16 selector);
abi_u16 tuple_reload_high_probe(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                                abi_u32 a3, abi_u32 a4, abi_u32 a5,
                                abi_u16 selector);

#endif
