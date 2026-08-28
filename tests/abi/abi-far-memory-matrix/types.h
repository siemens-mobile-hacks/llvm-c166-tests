#ifndef ABI_FAR_MEMORY_TYPES_H
#define ABI_FAR_MEMORY_TYPES_H

typedef unsigned char abi_u8;
typedef signed int abi_s16;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_far_arena {
  abi_u8 prefix[4];
  abi_u8 byte0;
  abi_u8 byte1;
  abi_u16 word0;
  abi_u8 gap0[4];
  abi_u32 long0;
  abi_u8 middle[28];
  abi_u32 tail_long;
};

typedef char abi_far_arena_size_must_be_48[
    sizeof(struct abi_far_arena) == 48 ? 1 : -1];

abi_u32 tasking_far_apply(abi_u16 seed,
                          volatile struct abi_far_arena *arena);
volatile abi_u8 *tasking_far_advance(
    volatile struct abi_far_arena *arena, abi_u16 offset);
volatile abi_u8 *tasking_far_retreat(
    volatile abi_u8 *one_past, abi_u16 offset);
abi_u16 tasking_far_equal(volatile abi_u8 *lhs, volatile abi_u8 *rhs);
abi_u16 tasking_far_not_equal(volatile abi_u8 *lhs, volatile abi_u8 *rhs);
abi_u16 tasking_far_less(volatile abi_u8 *lhs, volatile abi_u8 *rhs);
abi_u16 tasking_far_less_equal(volatile abi_u8 *lhs, volatile abi_u8 *rhs);
abi_u16 tasking_far_greater(volatile abi_u8 *lhs, volatile abi_u8 *rhs);
abi_u16 tasking_far_greater_equal(volatile abi_u8 *lhs,
                                  volatile abi_u8 *rhs);
abi_s16 tasking_far_difference(volatile abi_u16 *lhs,
                               volatile abi_u16 *rhs);
abi_u16 tasking_far_is_null(volatile abi_u8 *address);

#endif
