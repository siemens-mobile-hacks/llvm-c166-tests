typedef unsigned short u16;

extern u16 size_sink6(u16, u16, u16, u16, u16, u16);

u16 size_outgoing_stack(u16 a, u16 b, u16 c, u16 d) {
  return size_sink6(a, b, c, d, a, b);
}
