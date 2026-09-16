#include "c166_test.h"
#include "types.h"

extern volatile float C166_NEAR seeded_f32_inputs[2];

C166_NOINLINE C166_SECTION(".seeded_f32_eval")
float seeded_f32_eval(abi_u16 expression) {
  float a = seeded_f32_inputs[0];
  float b = seeded_f32_inputs[1];

  switch (expression) {
  case 0:
    return a + b;
  case 1:
    return a - b;
  case 2:
    return a * b;
  default:
    return a / b;
  }
}
