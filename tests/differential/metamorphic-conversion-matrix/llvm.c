#include "types.h"

#define LLVM_FUNCTION __attribute__((noinline, used))

static volatile abi_u16 volatile_canary_lo;
static volatile abi_u32 volatile_word;
static volatile abi_u16 volatile_canary_hi;

static abi_u32 rotate_left(abi_u32 value, abi_u16 count) {
  return (value << count) | (value >> (32U - count));
}

static abi_u32 rotate_right(abi_u32 value, abi_u16 count) {
  return (value >> count) | (value << (32U - count));
}

LLVM_FUNCTION
abi_u32 llvm_metamorphic_entry(abi_u16 operation, abi_u32 x, abi_s32 sx,
                               abi_u32 y, abi_u16 aux, abi_s8 sc, abi_u8 uc,
                               abi_u16 tail) {
  abi_u16 count;
  abi_u32 first;
  abi_u32 second;
  volatile abi_u32 vx;
  volatile abi_u32 vy;

  switch (operation) {
  case META_ADD_SUB:
    return (x + y) - y;
  case META_XOR_ROUNDTRIP:
    return (x ^ y) ^ y;
  case META_ROTATE_ROUNDTRIP:
    count = (abi_u16)(((aux ^ tail) % 31U) + 1U);
    return rotate_right(rotate_left(x, count), count);
  case META_DIVISION_DECOMPOSITION:
    if (y == 0UL)
      return 0xbad30000UL;
    return (x / y) * y + x % y;
  case META_DE_MORGAN:
    vx = x;
    vy = y;
    first = ~(vx & vy);
    second = ~vx | ~vy;
    return first == second ? first : first ^ 0xbad40000UL;
  case META_COMPLEMENTARY_COMPARE:
    vx = x;
    vy = y;
    first = vx < vy;
    second = !(vx >= vy);
    return first | (second << 16);
  case META_BRANCH_SELECT: {
    volatile abi_u16 side_effect = tail;
    abi_u32 branch_value;
    abi_u32 select_value;
    if ((aux & 1U) != 0U) {
      side_effect += 3U;
      branch_value = x;
    } else {
      side_effect += 5U;
      branch_value = y;
    }
    select_value = (aux & 1U) != 0U ? x : y;
    return (branch_value ^ select_value) |
           ((abi_u32)side_effect << 16) | side_effect;
  }
  case META_VOLATILE_ROUNDTRIP:
    volatile_canary_lo = 0x1357U;
    volatile_word = x ^ rotate_left(y, 1U);
    volatile_canary_hi = 0x2468U;
    first = volatile_word;
    if (volatile_canary_lo != 0x1357U)
      first ^= 0xbad70001UL;
    if (volatile_canary_hi != 0x2468U)
      first ^= 0xbad70002UL;
    return first;
  case META_DISTRIBUTION:
    second = ((abi_u32)aux << 16) | tail;
    first = x & (y | second);
    second = (x & y) | (x & second);
    return first == second ? first : first ^ 0xbad80000UL;
  case META_SHIFT_RECONSTRUCTION:
    count = (abi_u16)(((aux ^ tail) % 31U) + 1U);
    second = (1UL << count) - 1UL;
    return ((x >> count) << count) | (x & second);
  case META_S8_TO_S32:
    return (abi_u32)(abi_s32)sc;
  case META_U8_TO_U32:
    return (abi_u32)uc;
  case META_S8_S32_S8: {
    abi_s32 widened = (abi_s32)sc;
    abi_s8 narrowed = (abi_s8)widened;
    return (abi_u32)(abi_s32)narrowed;
  }
  case META_U8_U32_U8: {
    abi_u32 widened = (abi_u32)uc;
    abi_u8 narrowed = (abi_u8)widened;
    return (abi_u32)narrowed;
  }
  case META_S32_S8_S32: {
    abi_s8 narrowed = (abi_s8)sx;
    return (abi_u32)(abi_s32)narrowed;
  }
  case META_U32_U8_U32:
    return (abi_u32)(abi_u8)x;
  case META_S16_TO_S32:
    return (abi_u32)(abi_s32)(abi_s16)sx;
  default:
    return 0xbad00000UL | operation;
  }
}
