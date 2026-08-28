#include "types.h"

#define LLVM_SECTION(name) __attribute__((noinline, section(name)))

LLVM_SECTION(".llvm_far_apply")
abi_u32 llvm_far_apply(abi_u16 seed,
                       volatile struct abi_far_arena *arena) {
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

LLVM_SECTION(".llvm_far_advance")
volatile abi_u8 *llvm_far_advance(volatile struct abi_far_arena *arena,
                                  abi_u16 offset) {
  return (volatile abi_u8 *)arena + offset;
}

LLVM_SECTION(".llvm_far_reverse_apply")
abi_u32 llvm_far_reverse_apply(abi_u16 seed,
                               volatile struct abi_far_arena *arena) {
  return tasking_far_apply(seed, arena);
}

LLVM_SECTION(".llvm_far_reverse_advance")
volatile abi_u8 *llvm_far_reverse_advance(
    volatile struct abi_far_arena *arena, abi_u16 offset) {
  return tasking_far_advance(arena, offset);
}

LLVM_SECTION(".llvm_far_retreat")
volatile abi_u8 *llvm_far_retreat(volatile abi_u8 *one_past,
                                  abi_u16 offset) {
  return one_past - offset;
}

LLVM_SECTION(".llvm_far_reverse_retreat")
volatile abi_u8 *llvm_far_reverse_retreat(volatile abi_u8 *one_past,
                                          abi_u16 offset) {
  return tasking_far_retreat(one_past, offset);
}

LLVM_SECTION(".llvm_far_equal")
abi_u16 llvm_far_equal(volatile abi_u8 *lhs, volatile abi_u8 *rhs) {
  return lhs == rhs;
}

LLVM_SECTION(".llvm_far_less")
abi_u16 llvm_far_less(volatile abi_u8 *lhs, volatile abi_u8 *rhs) {
  return lhs < rhs;
}

LLVM_SECTION(".llvm_far_is_null")
abi_u16 llvm_far_is_null(volatile abi_u8 *address) {
  return address == 0;
}

LLVM_SECTION(".llvm_far_not_equal")
abi_u16 llvm_far_not_equal(volatile abi_u8 *lhs, volatile abi_u8 *rhs) {
  return lhs != rhs;
}

LLVM_SECTION(".llvm_far_less_equal")
abi_u16 llvm_far_less_equal(volatile abi_u8 *lhs, volatile abi_u8 *rhs) {
  return lhs <= rhs;
}

LLVM_SECTION(".llvm_far_greater")
abi_u16 llvm_far_greater(volatile abi_u8 *lhs, volatile abi_u8 *rhs) {
  return lhs > rhs;
}

LLVM_SECTION(".llvm_far_greater_equal")
abi_u16 llvm_far_greater_equal(volatile abi_u8 *lhs,
                               volatile abi_u8 *rhs) {
  return lhs >= rhs;
}

LLVM_SECTION(".llvm_far_difference")
abi_s16 llvm_far_difference(volatile abi_u16 *lhs,
                            volatile abi_u16 *rhs) {
  return lhs - rhs;
}
