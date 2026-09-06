typedef unsigned short u16;

u16 size_double_comparison(double left, double right) {
  u16 result = 0;
  if (left == right)
    result |= 1u;
  if (left != right)
    result |= 2u;
  if (left < right)
    result |= 4u;
  if (left <= right)
    result |= 8u;
  if (left > right)
    result |= 16u;
  if (left >= right)
    result |= 32u;
  return result;
}
