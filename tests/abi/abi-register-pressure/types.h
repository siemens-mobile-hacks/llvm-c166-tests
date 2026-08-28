#ifndef ABI_REGISTER_PRESSURE_TYPES_H
#define ABI_REGISTER_PRESSURE_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

abi_u16 tasking_pressure_selector(abi_u16 selector);
abi_u16 llvm_pressure_selector(abi_u16 selector);
abi_u16 llvm_pressure_selector_bridge(abi_u16 selector);

abi_u16 tasking_pressure_words(abi_u16 a0, abi_u16 a1, abi_u16 a2,
                               abi_u16 a3, abi_u16 a4, abi_u16 a5,
                               abi_u16 a6, abi_u16 a7, abi_u16 selector);
abi_u32 tasking_pressure_longs(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                              abi_u32 a3, abi_u32 a4, abi_u32 a5,
                              abi_u16 selector);

#endif
