typedef unsigned short u16;

u16 size_switch_dense(u16 selector, u16 value) {
  switch (selector) {
  case 0:
    return (u16)(value + 3u);
  case 1:
    return (u16)(value - 5u);
  case 2:
    return (u16)(value ^ 0x1234u);
  case 3:
    return (u16)(value | 0x0080u);
  case 4:
    return (u16)(value & 0x3fffu);
  case 5:
    return (u16)(value + 11u);
  case 6:
    return (u16)(value - 13u);
  case 7:
    return (u16)(value ^ 0x00ffu);
  default:
    return value;
  }
}
