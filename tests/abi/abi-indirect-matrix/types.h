#ifndef ABI_INDIRECT_MATRIX_TYPES_H
#define ABI_INDIRECT_MATRIX_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

typedef abi_u32 (*abi_fn0)(void);
typedef abi_u32 (*abi_fn2)(abi_u16 a, abi_u16 b);
typedef abi_u32 (*abi_fn5)(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                           abi_u16 e);

abi_u32 tasking_indirect0(void);
abi_u32 tasking_indirect2(abi_u16 a, abi_u16 b);
abi_u32 tasking_indirect5(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                          abi_u16 e);

#endif
