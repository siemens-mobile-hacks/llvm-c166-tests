#include "c166_test.h"
#include "types.h"

extern volatile double C166_NEAR seeded_f64_inputs[2];

C166_NOINLINE C166_SECTION(".seeded_f64_eval")
double seeded_f64_eval(abi_u16 expression) {
  double a = seeded_f64_inputs[0];
  double b = seeded_f64_inputs[1];

  switch (expression) {
  case 0:
    return a + b;
  case 1:
    return a - b;
  case 2:
    return a * b;
  case 3:
    return a / b;
  case 4:
    if (a < b)
      return 1.0;
    if (a == b)
      return 2.0;
    if (a > b)
      return 4.0;
    return 8.0;
  default:
    return 0.0;
  }
}
