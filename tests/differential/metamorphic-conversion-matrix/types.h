#ifndef C166_METAMORPHIC_CONVERSION_TYPES_H
#define C166_METAMORPHIC_CONVERSION_TYPES_H

typedef signed char abi_s8;
typedef unsigned char abi_u8;
typedef signed int abi_s16;
typedef unsigned int abi_u16;
typedef signed long abi_s32;
typedef unsigned long abi_u32;

enum metamorphic_operation {
  META_ADD_SUB = 0,
  META_XOR_ROUNDTRIP = 1,
  META_ROTATE_ROUNDTRIP = 2,
  META_DIVISION_DECOMPOSITION = 3,
  META_DE_MORGAN = 4,
  META_COMPLEMENTARY_COMPARE = 5,
  META_BRANCH_SELECT = 6,
  META_VOLATILE_ROUNDTRIP = 7,
  META_DISTRIBUTION = 8,
  META_SHIFT_RECONSTRUCTION = 9,
  META_S8_TO_S32 = 10,
  META_U8_TO_U32 = 11,
  META_S8_S32_S8 = 12,
  META_U8_U32_U8 = 13,
  META_S32_S8_S32 = 14,
  META_U32_U8_U32 = 15,
  META_S16_TO_S32 = 16
};

#endif
