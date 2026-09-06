typedef unsigned long u32;

u32 size_long_arithmetic(u32 left, u32 right) {
  left += 0x12345678UL;
  right -= 0x00010001UL;
  return (left ^ right) + 0x0000ff00UL;
}
