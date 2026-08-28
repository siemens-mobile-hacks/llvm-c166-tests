#ifndef SEEDED_FAR_POINTER_TYPES_H
#define SEEDED_FAR_POINTER_TYPES_H

typedef unsigned char abi_u8;
typedef signed int abi_s16;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#define ABI_SEEDED_FAR_BYTE_COUNT 4032U
#define ABI_SEEDED_FAR_WORD_COUNT 2016U
#define ABI_SEEDED_FAR_LONG_COUNT 1008U
#define ABI_SEEDED_FAR_RECORD_COUNT 504U

struct abi_seeded_far_record {
  abi_u8 first;
  abi_u8 second;
  abi_u16 word;
  abi_u32 wide;
};

struct abi_seeded_far_arena {
  abi_u8 bytes[ABI_SEEDED_FAR_BYTE_COUNT];
  abi_u16 words[ABI_SEEDED_FAR_WORD_COUNT];
  abi_u32 longs[ABI_SEEDED_FAR_LONG_COUNT];
  struct abi_seeded_far_record records[ABI_SEEDED_FAR_RECORD_COUNT];
};

typedef char abi_seeded_far_record_size_must_be_8[
    sizeof(struct abi_seeded_far_record) == 8 ? 1 : -1];
typedef char abi_seeded_far_arena_size_must_be_0x3f00[
    sizeof(struct abi_seeded_far_arena) == 0x3f00 ? 1 : -1];

#endif
