typedef unsigned short u16;

extern u16 size_sink1(u16);

u16 size_stack_arguments(u16 a, u16 b, u16 c, u16 d, u16 e, u16 f) {
  u16 first = size_sink1((u16)(a + b));
  u16 second = size_sink1((u16)(c + d));
  return (u16)(first + second + e + f);
}
