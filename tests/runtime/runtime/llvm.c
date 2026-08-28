typedef unsigned long u32;
typedef signed long s32;

#define NOINLINE __attribute__((noinline))

static NOINLINE u32 multiply(u32 a, u32 b) { return a * b; }
static NOINLINE u32 unsigned_divide(u32 a, u32 b) { return a / b; }
static NOINLINE u32 unsigned_remainder(u32 a, u32 b) { return a % b; }
static NOINLINE s32 signed_divide(s32 a, s32 b) { return a / b; }
static NOINLINE s32 signed_remainder(s32 a, s32 b) { return a % b; }
static NOINLINE u32 shift_left(u32 a, unsigned int b) { return a << b; }
static NOINLINE u32 shift_right(u32 a, unsigned int b) { return a >> b; }
static NOINLINE s32 arithmetic_shift_right(s32 a, unsigned int b) {
  return a >> b;
}

unsigned int llvm_entry(u32 a, u32 b, unsigned int shift) {
  u32 mix = multiply(a, b);
  mix ^= unsigned_divide(a, b);
  mix ^= unsigned_remainder(a, b);
  mix ^= (u32)signed_divide((s32)a, (s32)b);
  mix ^= (u32)signed_remainder((s32)a, (s32)b);
  mix ^= shift_left(a, shift);
  mix ^= shift_right(a, shift);
  mix ^= (u32)arithmetic_shift_right((s32)a, shift);
  return (unsigned int)mix ^ (unsigned int)(mix >> 16);
}
