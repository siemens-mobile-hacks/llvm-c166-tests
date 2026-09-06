typedef unsigned short u16;

float size_float_arithmetic(float left, float right, u16 operation) {
  switch (operation & 3u) {
  case 0:
    return left + right;
  case 1:
    return left - right;
  case 2:
    return left * right;
  default:
    return left / right;
  }
}
