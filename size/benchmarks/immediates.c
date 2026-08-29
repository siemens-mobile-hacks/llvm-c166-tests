typedef unsigned short u16;

u16 size_immediates(u16 value) {
  value = (u16)(value + 0x1234u);
  value = (u16)(value & 0x3fffu);
  value = (u16)(value - 0x0080u);
  if (value < 0x0800u)
    value = (u16)(value + 7u);
  return value;
}
