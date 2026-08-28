#ifndef C166_FAR_POINTER_ALIAS_TYPES_H
#define C166_FAR_POINTER_ALIAS_TYPES_H

typedef unsigned char abi_u8;
typedef signed int abi_s16;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#ifdef __clang__
typedef abi_u8 __attribute__((c166_far)) abi_far_u8;
typedef abi_u16 __attribute__((c166_far)) abi_far_u16;
typedef abi_u32 __attribute__((c166_far)) abi_far_u32;
#else
typedef abi_u8 _far abi_far_u8;
typedef abi_u16 _far abi_far_u16;
typedef abi_u32 _far abi_far_u32;
#endif

enum far_alias_operation {
  FAR_ALIAS_BYTE_RELATIONS = 0,
  FAR_ALIAS_BYTE_WRITE_ORDER = 1,
  FAR_ALIAS_WORD_BYTE_OVERLAP = 2,
  FAR_ALIAS_LONG_BYTE_OVERLAP = 3,
  FAR_ALIAS_OBSERVED_ORDER = 4,
  FAR_ALIAS_WORD_RELATIONS = 5,
  FAR_ALIAS_FORWARD_COPY = 6,
  FAR_ALIAS_REVERSE_COPY = 7,
  FAR_ALIAS_POINTER_SELECT = 8
};

#define FAR_ALIAS_ARENA_SIZE 96U

#endif
