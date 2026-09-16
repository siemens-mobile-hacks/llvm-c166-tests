#ifndef GLOBAL_DATA_TYPES_H
#define GLOBAL_DATA_TYPES_H

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

extern abi_u8 global_data_bytes[8];
extern abi_u16 global_data_words[8];
extern abi_u32 global_data_longs[8];

extern abi_u8 global_bss_bytes[8];
extern abi_u16 global_bss_words[8];
extern abi_u32 global_bss_longs[8];
extern abi_u32 global_digest;

extern const abi_u8 global_ro_bytes[8];
extern const abi_u16 global_ro_words[8];
extern const abi_u32 global_ro_longs[8];
extern const abi_u8 global_string[17];

extern abi_u8 *global_data_pointer;
extern const abi_u8 *global_string_pointer;

abi_u8 *mutate_globals(abi_u16 index, abi_u32 seed);
const abi_u32 *update_global_digest(void);

#endif
