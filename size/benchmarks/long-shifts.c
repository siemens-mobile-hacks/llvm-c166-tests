typedef unsigned short u16;
typedef unsigned long u32;

u32 size_long_shift_constant(u32 value) { return (value << 5) | (value >> 27); }

u32 size_long_shift_variable(u32 value, u16 amount) {
  return value << (amount & 31u);
}
