typedef unsigned short u16;
typedef unsigned long u32;

u32 size_multiply_divide(u16 left, u16 right, u16 divisor) {
  u32 product = (u32)left * (u32)right;
  return product / (u32)(divisor | 1u);
}
