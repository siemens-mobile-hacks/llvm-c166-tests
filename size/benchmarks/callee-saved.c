typedef unsigned short u16;

extern u16 size_sink4(u16, u16, u16, u16);

u16 size_callee_saved(u16 a, u16 b, u16 c, u16 d) {
  u16 x0 = (u16)(a + 1u);
  u16 x1 = (u16)(b + 3u);
  u16 x2 = (u16)(c + 5u);
  u16 x3 = (u16)(d + 7u);
  u16 result = size_sink4(x0, x1, x2, x3);
  return (u16)(result + x0 + x1 + x2 + x3);
}
