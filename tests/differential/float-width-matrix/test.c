#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

typedef union {
  float value;
  abi_u16 words[2];
} float32_bits;

typedef union {
  double value;
  abi_u16 words[4];
} float64_bits;

C166_NOINLINE C166_SECTION(".float32_to_float64") double
float32_to_float64(float value) {
  return (double)value;
}

C166_NOINLINE C166_SECTION(".float64_to_float32") float
float64_to_float32(double value) {
  return (float)value;
}

struct f32_to_f64_case {
  abi_u16 input[2];
  abi_u32 expected[2];
};

struct f64_to_f32_case {
  abi_u16 input[4];
  abi_u32 expected;
};

#define F32_TO_F64_CASE(id, w1, w0, expected_hi, expected_lo)                \
  {{w1, w0}, {expected_hi, expected_lo}},
static const struct f32_to_f64_case f32_to_f64_cases[] = {
    ABI_F32_TO_F64_VECTORS(F32_TO_F64_CASE)};
#undef F32_TO_F64_CASE

#define F64_TO_F32_CASE(id, w3, w2, w1, w0, expected)                        \
  {{w3, w2, w1, w0}, expected},
static const struct f64_to_f32_case f64_to_f32_cases[] = {
    ABI_F64_TO_F32_VECTORS(F64_TO_F32_CASE)};
#undef F64_TO_F32_CASE

void main(void) {
  unsigned int index;

  tap_plan((unsigned int)(sizeof(f32_to_f64_cases) /
                              sizeof(f32_to_f64_cases[0]) *
                              4U +
                          sizeof(f64_to_f32_cases) /
                              sizeof(f64_to_f32_cases[0]) *
                              2U));
  for (index = 0; index != sizeof(f32_to_f64_cases) /
                                   sizeof(f32_to_f64_cases[0]);
       ++index) {
    float32_bits input;
    float64_bits output;

    input.words[0] = f32_to_f64_cases[index].input[0];
    input.words[1] = f32_to_f64_cases[index].input[1];
    output.value = float32_to_float64(input.value);
    tap_is_u32(output.words[0], f32_to_f64_cases[index].expected[0] >> 16,
               "float32 to float64 word 3");
    tap_is_u32(output.words[1],
               (abi_u16)f32_to_f64_cases[index].expected[0],
               "float32 to float64 word 2");
    tap_is_u32(output.words[2], f32_to_f64_cases[index].expected[1] >> 16,
               "float32 to float64 word 1");
    tap_is_u32(output.words[3],
               (abi_u16)f32_to_f64_cases[index].expected[1],
               "float32 to float64 word 0");
  }
  for (index = 0; index != sizeof(f64_to_f32_cases) /
                                   sizeof(f64_to_f32_cases[0]);
       ++index) {
    float64_bits input;
    float32_bits output;

    input.words[0] = f64_to_f32_cases[index].input[0];
    input.words[1] = f64_to_f32_cases[index].input[1];
    input.words[2] = f64_to_f32_cases[index].input[2];
    input.words[3] = f64_to_f32_cases[index].input[3];
    output.value = float64_to_float32(input.value);
    tap_is_u32(output.words[0], f64_to_f32_cases[index].expected >> 16,
               "float64 to float32 word 1");
    tap_is_u32(output.words[1],
               (abi_u16)f64_to_f32_cases[index].expected,
               "float64 to float32 word 0");
  }
}
