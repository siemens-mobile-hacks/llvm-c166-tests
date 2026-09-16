#ifndef ABI_FAR_MEMORY_TYPES_H
#define ABI_FAR_MEMORY_TYPES_H

#include "c166_test.h"

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

typedef volatile abi_u8 C166_FAR *abi_far_byte_pointer;
typedef volatile abi_u16 C166_FAR *abi_far_word_pointer;
typedef volatile struct abi_far_arena C166_FAR *abi_far_arena_pointer;
typedef abi_far_byte_pointer (*abi_far_advance_fn)(abi_far_arena_pointer,
                                                   abi_u16);
typedef abi_far_byte_pointer (*abi_far_retreat_fn)(abi_far_byte_pointer,
                                                   abi_u16);
typedef abi_u16 (*abi_far_relation_fn)(abi_far_byte_pointer,
                                       abi_far_byte_pointer);
typedef abi_s16 (*abi_far_difference_fn)(abi_far_word_pointer,
                                         abi_far_word_pointer);
typedef abi_u16 (*abi_far_null_fn)(abi_far_byte_pointer);

abi_u32 far_apply(abi_u16 seed, abi_far_arena_pointer arena);
abi_far_byte_pointer far_advance(abi_far_arena_pointer arena,
                                 abi_u16 offset);
abi_far_byte_pointer far_reverse_advance(abi_far_arena_pointer arena,
                                         abi_u16 offset);
abi_far_byte_pointer far_retreat(abi_far_byte_pointer one_past,
                                 abi_u16 offset);
abi_far_byte_pointer far_reverse_retreat(abi_far_byte_pointer one_past,
                                         abi_u16 offset);
abi_u32 far_reverse_apply(abi_u16 seed, abi_far_arena_pointer arena);
abi_u16 far_equal(abi_far_byte_pointer lhs, abi_far_byte_pointer rhs);
abi_u16 far_not_equal(abi_far_byte_pointer lhs, abi_far_byte_pointer rhs);
abi_u16 far_less(abi_far_byte_pointer lhs, abi_far_byte_pointer rhs);
abi_u16 far_less_equal(abi_far_byte_pointer lhs, abi_far_byte_pointer rhs);
abi_u16 far_greater(abi_far_byte_pointer lhs, abi_far_byte_pointer rhs);
abi_u16 far_greater_equal(abi_far_byte_pointer lhs,
                          abi_far_byte_pointer rhs);
abi_s16 far_difference(abi_far_word_pointer lhs, abi_far_word_pointer rhs);
abi_u16 far_is_null(abi_far_byte_pointer address);

#endif
