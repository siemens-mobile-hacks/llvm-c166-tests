#include "types.h"

C166_NOINLINE abi_u32 far_apply(abi_u16 seed,
                                abi_far_arena_pointer arena) {
  abi_u16 dynamic = 1U + ((seed >> 4) & 0x000fU);
  abi_u32 result;

  arena->byte0 = (abi_u8)seed;
  arena->byte1 = (abi_u8)(seed >> 8);
  arena->word0 = seed ^ 0xa55aU;
  arena->long0 = 0x13579bdfUL ^ ((abi_u32)seed << 8);
  arena->middle[0] ^= (abi_u8)seed ^ 0x3cU;
  arena->middle[dynamic] ^= (abi_u8)(seed >> 8) ^ 0xc3U;
  arena->middle[27] = (abi_u8)(seed + 0x71U);
  arena->tail_long = 0x89abcdefUL + seed;

  result = arena->long0 ^ arena->tail_long;
  result += arena->word0;
  result ^= (abi_u32)arena->byte0 << 24;
  result ^= (abi_u32)arena->byte1 << 16;
  result += arena->middle[0];
  result ^= (abi_u32)arena->middle[dynamic] << 8;
  result += (abi_u32)arena->middle[27] << 16;
  return result;
}

C166_NOINLINE abi_far_byte_pointer
far_advance(abi_far_arena_pointer arena, abi_u16 offset) {
  return (abi_far_byte_pointer)arena + offset;
}

C166_NOINLINE abi_far_byte_pointer
far_reverse_advance(abi_far_arena_pointer arena, abi_u16 offset) {
  return far_advance(arena, offset);
}

C166_NOINLINE abi_far_byte_pointer
far_retreat(abi_far_byte_pointer one_past, abi_u16 offset) {
  return one_past - offset;
}

C166_NOINLINE abi_far_byte_pointer
far_reverse_retreat(abi_far_byte_pointer one_past, abi_u16 offset) {
  return far_retreat(one_past, offset);
}

C166_NOINLINE abi_u32 far_reverse_apply(abi_u16 seed,
                                        abi_far_arena_pointer arena) {
  return far_apply(seed, arena);
}

C166_NOINLINE abi_u16 far_equal(abi_far_byte_pointer lhs,
                                abi_far_byte_pointer rhs) {
  return lhs == rhs;
}

C166_NOINLINE abi_u16 far_not_equal(abi_far_byte_pointer lhs,
                                    abi_far_byte_pointer rhs) {
  return lhs != rhs;
}

C166_NOINLINE abi_u16 far_less(abi_far_byte_pointer lhs,
                               abi_far_byte_pointer rhs) {
  return lhs < rhs;
}

C166_NOINLINE abi_u16 far_less_equal(abi_far_byte_pointer lhs,
                                     abi_far_byte_pointer rhs) {
  return lhs <= rhs;
}

C166_NOINLINE abi_u16 far_greater(abi_far_byte_pointer lhs,
                                  abi_far_byte_pointer rhs) {
  return lhs > rhs;
}

C166_NOINLINE abi_u16 far_greater_equal(abi_far_byte_pointer lhs,
                                        abi_far_byte_pointer rhs) {
  return lhs >= rhs;
}

C166_NOINLINE abi_s16 far_difference(abi_far_word_pointer lhs,
                                     abi_far_word_pointer rhs) {
  return lhs - rhs;
}

C166_NOINLINE abi_u16 far_is_null(abi_far_byte_pointer address) {
  return address == 0;
}
