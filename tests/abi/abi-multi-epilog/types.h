#ifndef ABI_MULTI_EPILOG_TYPES_H
#define ABI_MULTI_EPILOG_TYPES_H

#include "c166_test.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

abi_u16 multi_epilog_selector(abi_u16 selector);
abi_u32 multi_epilog_pressure(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                              abi_u32 a3, abi_u32 a4, abi_u32 a5,
                              abi_u16 mode, abi_u16 rounds);
abi_u32 multi_epilog_pressure_probe(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                                    abi_u32 a3, abi_u32 a4, abi_u32 a5,
                                    abi_u16 mode, abi_u16 rounds);

#endif
