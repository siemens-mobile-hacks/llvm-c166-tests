#include "types.h"
#include "vectors.inc"

static abi_u16 fixed_mix(abi_u16 shape) {
  switch (shape) {
  case 1: return 0x1111U;
  case 2: return 0x3333U;
  case 3: return 0x7777U;
  case 4: return 0xffffU;
  default: return 0x5432U;
  }
}

static void check_capture(abi_u16 shape, abi_u32 result, abi_u16 first3,
                          abi_u16 first2, abi_u16 first1, abi_u16 first0,
                          abi_u16 middle, abi_u16 second3, abi_u16 second2,
                          abi_u16 second1, abi_u16 second0) {
  abi_u16 mix = fixed_mix(shape);

  tap_is_u32(result, 0xa5a50000UL | mix, "variadic return value");
  tap_is_u32(vararg_captured[0].words[0], first3, "first double word 3");
  tap_is_u32(vararg_captured[0].words[1], first2, "first double word 2");
  tap_is_u32(vararg_captured[0].words[2], first1, "first double word 1");
  tap_is_u32(vararg_captured[0].words[3], first0, "first double word 0");
  tap_is_u32(vararg_observed[0], middle, "middle word");
  tap_is_u32(vararg_captured[1].words[0], second3, "second double word 3");
  tap_is_u32(vararg_captured[1].words[1], second2, "second double word 2");
  tap_is_u32(vararg_captured[1].words[2], second1, "second double word 1");
  tap_is_u32(vararg_captured[1].words[3], second0, "second double word 0");
  tap_is_u32(vararg_observed[1], mix, "fixed-argument mix");
}

static void run_raw_vector(abi_u16 first3, abi_u16 first2, abi_u16 first1,
                           abi_u16 first0, abi_u16 middle, abi_u16 second3,
                           abi_u16 second2, abi_u16 second1,
                           abi_u16 second0) {
  union abi_double_words first;
  union abi_double_words second;
  abi_u16 shape;

  first.words[0] = first3;
  first.words[1] = first2;
  first.words[2] = first1;
  first.words[3] = first0;
  second.words[0] = second3;
  second.words[1] = second2;
  second.words[2] = second1;
  second.words[3] = second0;
  for (shape = 1; shape <= 5; ++shape) {
    check_capture(shape, float_raw(shape, first.value, middle, second.value),
                  first3, first2, first1, first0, middle, second3, second2,
                  second1, second0);
    check_capture(shape,
                  float_raw_indirect(shape, first.value, middle, second.value),
                  first3, first2, first1, first0, middle, second3, second2,
                  second1, second0);
  }
}

static void run_promotion_vector(abi_u16 float1, abi_u16 float0,
                                 abi_u16 double3, abi_u16 double2,
                                 abi_u16 double1, abi_u16 double0) {
  union abi_float_words input;
  abi_u16 shape;

  input.words[0] = float1;
  input.words[1] = float0;
  for (shape = 1; shape <= 5; ++shape) {
    check_capture(shape, float_promote(shape, input.value), double3, double2,
                  double1, double0, 0x55aaU, 0x3ff0U, 0x0000U, 0x0000U,
                  0x0000U);
    check_capture(shape, float_promote_indirect(shape, input.value), double3,
                  double2, double1, double0, 0x55aaU, 0x3ff0U, 0x0000U,
                  0x0000U, 0x0000U);
  }
}

#define RUN_RAW(id, a3, a2, a1, a0, m, b3, b2, b1, b0)                    \
  run_raw_vector(a3, a2, a1, a0, m, b3, b2, b1, b0);
#define RUN_PROMOTION(id, f1, f0, d3, d2, d1, d0)                          \
  run_promotion_vector(f1, f0, d3, d2, d1, d0);

void main(void) {
  tap_plan(ABI_FLOAT_VARARGS_CHECKS);
  ABI_FLOAT_VARARGS_RAW_VECTORS(RUN_RAW)
  ABI_FLOAT_VARARGS_PROMOTION_VECTORS(RUN_PROMOTION)
}
