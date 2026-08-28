#include "types.h"

extern volatile double seeded_f64_inputs[3];

__attribute__((noinline, section(".llvm_seeded_f64_eval")))
double llvm_seeded_f64_eval(abi_u16 expression) {
  double a = seeded_f64_inputs[0];
  double b = seeded_f64_inputs[1];

#ifdef C166_SEEDED_F64_EXPRESSION
#if C166_SEEDED_F64_EXPRESSION == 0
  return a + b;
#elif C166_SEEDED_F64_EXPRESSION == 1
  return a - b;
#elif C166_SEEDED_F64_EXPRESSION == 2
  return a * b;
#elif C166_SEEDED_F64_EXPRESSION == 3
  return a / b;
#else
#error C166_SEEDED_F64_EXPRESSION must be in 0..3
#endif
#else
  switch (expression) {
  case 0:
    return a + b;
  case 1:
    return a - b;
  case 2:
    return a * b;
  case 3:
    return a / b;
  default:
    return 0.0;
  }
#endif
}
