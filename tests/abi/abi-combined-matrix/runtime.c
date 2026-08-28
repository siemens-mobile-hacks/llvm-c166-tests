#include "types.h"

#define NOINLINE __attribute__((noinline))

static NOINLINE abi_u32 multiply(abi_u32 a, abi_u32 b) { return a * b; }
static NOINLINE abi_u32 unsigned_divide(abi_u32 a, abi_u32 b) { return a / b; }
static NOINLINE abi_u32 unsigned_remainder(abi_u32 a, abi_u32 b) {
  return a % b;
}
static NOINLINE abi_s32 signed_divide(abi_s32 a, abi_s32 b) { return a / b; }
static NOINLINE abi_s32 signed_remainder(abi_s32 a, abi_s32 b) {
  return a % b;
}
static NOINLINE abi_u32 shift_left(abi_u32 a, abi_u16 b) { return a << b; }
static NOINLINE abi_u32 shift_right(abi_u32 a, abi_u16 b) { return a >> b; }
static NOINLINE abi_s32 arithmetic_shift_right(abi_s32 a, abi_u16 b) {
  return a >> b;
}

abi_u16 runtime_probe(abi_u32 a, abi_u32 b, abi_u16 shift) {
  abi_u32 mix = multiply(a, b);
  mix ^= unsigned_divide(a, b);
  mix ^= unsigned_remainder(a, b);
  mix ^= (abi_u32)signed_divide((abi_s32)a, (abi_s32)b);
  mix ^= (abi_u32)signed_remainder((abi_s32)a, (abi_s32)b);
  mix ^= shift_left(a, shift);
  mix ^= shift_right(a, shift);
  mix ^= (abi_u32)arithmetic_shift_right((abi_s32)a, shift);
  return (abi_u16)mix ^ (abi_u16)(mix >> 16);
}
