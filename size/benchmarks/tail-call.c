typedef unsigned short u16;
typedef unsigned long u32;

extern u32 size_tail_target(u16, u32);
extern double size_tail_double_target(double, double);

u32 size_tail_call(u16 selector, u32 state) {
  return size_tail_target((u16)(selector + 1u), state ^ 0x01020304UL);
}

double size_tail_double(double left, double right) {
  return size_tail_double_target(left, right);
}
