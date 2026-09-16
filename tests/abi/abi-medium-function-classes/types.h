#ifndef ABI_MEDIUM_FUNCTION_CLASSES_TYPES_H
#define ABI_MEDIUM_FUNCTION_CLASSES_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#include "c166_test.h"

typedef abi_u16 (*abi_near_fn2)(abi_u16, abi_u16);
typedef abi_u16 C166_HUGE abi_huge_fn2_type(abi_u16, abi_u16);
typedef abi_huge_fn2_type *abi_huge_fn2;

abi_u16 c166_medium_near_callback(abi_u16 a, abi_u16 b);
abi_u16 C166_HUGE c166_medium_huge_callback(abi_u16 a, abi_u16 b);
abi_u16 c166_medium_near_target(abi_u16 a, abi_u16 b);
abi_u16 C166_HUGE c166_medium_huge_target(abi_u16 a, abi_u16 b);
abi_u32 c166_medium_dispatch(abi_near_fn2 near_target,
                             abi_huge_fn2 huge_target, abi_u16 a,
                             abi_u16 b);

#endif
