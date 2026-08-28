#include "types.h"

extern volatile float seeded_f32_inputs[2];

__attribute__((noinline, section(".llvm_seeded_f32_eval")))
float llvm_seeded_f32_eval(abi_u16 expression) {
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
