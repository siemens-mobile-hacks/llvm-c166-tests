#include "types.h"

C166_NOINLINE static float add_float(float first, float second) {
  return first + second;
}

C166_NOINLINE static double affine_double(double first, double second) {
  return first * second + 1.0;
}

C166_NOINLINE static double forward_double(double first, double second) {
  return affine_double(first, second);
}

abi_u16 float_probe(float first, float second, double third, double fourth,
                    abi_u16 tail) {
  if (first != 1.5F || second != 2.25F || third != 6.5 || fourth != 2.0 ||
      tail != 0xa55aU)
    return 1U;
  if (add_float(first, second) != 3.75F)
    return 2U;
  if (forward_double(third, fourth) != 14.0)
    return 3U;
  return 42U;
}
