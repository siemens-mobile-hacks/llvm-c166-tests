typedef unsigned short u16;

u16 size_high_bit_is_clear(u16 value) { return (u16)((value & 0x8000u) == 0u); }

u16 size_middle_bit_is_clear(u16 value) {
  return (u16)((value & 0x0020u) == 0u);
}
