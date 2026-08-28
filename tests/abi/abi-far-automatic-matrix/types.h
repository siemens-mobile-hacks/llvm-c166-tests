#ifndef ABI_FAR_AUTOMATIC_TYPES_H
#define ABI_FAR_AUTOMATIC_TYPES_H

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_stack_arena {
  abi_u8 byte0;
  abi_u8 byte1;
  abi_u16 word2;
  abi_u8 gap4[10];
  abi_u32 long14;
  abi_u8 middle[238];
  abi_u16 word256;
  abi_u32 long258;
};

struct abi_stack_wrapper {
  abi_u32 before;
  struct abi_stack_arena arena;
  abi_u32 after;
};

typedef char abi_stack_arena_size_must_be_262[
    sizeof(struct abi_stack_arena) == 262 ? 1 : -1];
typedef char abi_stack_wrapper_size_must_be_270[
    sizeof(struct abi_stack_wrapper) == 270 ? 1 : -1];

abi_u32 tasking_stack_apply(abi_u16 seed,
                            volatile struct abi_stack_arena *arena);
abi_u32 tasking_stack_owner(abi_u16 seed);

#endif
