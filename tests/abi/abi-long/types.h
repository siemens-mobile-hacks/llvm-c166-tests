#ifndef C166_ABI_LONG_TYPES_H
#define C166_ABI_LONG_TYPES_H

typedef unsigned int abi_u16;
typedef signed long abi_s32;
typedef unsigned long abi_u32;

struct long_record {
  abi_u16 tag;
  abi_u32 value;
};

abi_u32 c166_long_entry(abi_u16 prefix, abi_u32 value, abi_u16 tail);
struct long_record c166_long_record_entry(struct long_record input,
                                          abi_u16 tail);
abi_u32 c166_long_mix(abi_u16 prefix, abi_u32 value, abi_u16 tail);
struct long_record c166_long_record_mix(struct long_record input, abi_u16 tail);

#endif
