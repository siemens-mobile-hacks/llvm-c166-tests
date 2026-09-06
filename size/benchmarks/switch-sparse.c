typedef unsigned short u16;

u16 size_switch_sparse(u16 selector, u16 value) {
  switch (selector) {
  case 1:
    return (u16)(value + 1u);
  case 17:
    return (u16)(value + 2u);
  case 257:
    return (u16)(value + 3u);
  case 4096:
    return (u16)(value + 4u);
  case 0x7fffu:
    return (u16)(value + 5u);
  default:
    return value;
  }
}
