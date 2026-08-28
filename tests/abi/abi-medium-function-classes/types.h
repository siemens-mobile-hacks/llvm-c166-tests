#ifndef ABI_MEDIUM_FUNCTION_CLASSES_TYPES_H
#define ABI_MEDIUM_FUNCTION_CLASSES_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#ifdef __clang__
#define ABI_HUGE_FUNCTION __attribute__((c166_huge))
#else
#define ABI_HUGE_FUNCTION _huge
#endif

typedef abi_u16 (*abi_near_fn2)(abi_u16, abi_u16);
typedef abi_u16 ABI_HUGE_FUNCTION abi_huge_fn2_type(abi_u16, abi_u16);
typedef abi_huge_fn2_type *abi_huge_fn2;

abi_u16 tasking_medium_near_target(abi_u16 a, abi_u16 b);
abi_u16 ABI_HUGE_FUNCTION tasking_medium_huge_target(abi_u16 a, abi_u16 b);

#endif
