#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern volatile abi_u16 llvm_vararg_inputs[14];
extern volatile abi_u16 llvm_vararg_captured_words[8];
extern volatile abi_u16 llvm_vararg_observed[2];

extern abi_u32 llvm_float_varargs_proxy1(void);
extern abi_u32 llvm_float_varargs_proxy2(void);
extern abi_u32 llvm_float_varargs_proxy3(void);
extern abi_u32 llvm_float_varargs_proxy4(void);
extern abi_u32 llvm_float_varargs_proxy5(void);
extern abi_u32 llvm_float_promote_proxy(abi_u16, abi_u16, abi_u16);

void simulator_stop(void) {
  for (;;)
    ;
}

static abi_u16 fixed_mix(abi_u16 shape) {
  switch (shape) {
  case 1: return 0x1111U;
  case 2: return 0x3333U;
  case 3: return 0x7777U;
  case 4: return 0xffffU;
  default: return 0x5432U;
  }
}

static abi_u32 call_raw(abi_u16 shape) {
  switch (shape) {
  case 1: return llvm_float_varargs_proxy1();
  case 2: return llvm_float_varargs_proxy2();
  case 3: return llvm_float_varargs_proxy3();
  case 4: return llvm_float_varargs_proxy4();
  default: return llvm_float_varargs_proxy5();
  }
}

static void check_capture(abi_u16 base, abi_u16 shape, abi_u32 result,
                          abi_u16 first3, abi_u16 first2,
                          abi_u16 first1, abi_u16 first0, abi_u16 middle,
                          abi_u16 second3, abi_u16 second2,
                          abi_u16 second1, abi_u16 second0) {
  abi_u16 mix = fixed_mix(shape);
  c166_test_check_u32(base, 0xa5a50000UL | mix, result);
  c166_test_check_u32(base + 1U, first3, llvm_vararg_captured_words[0]);
  c166_test_check_u32(base + 2U, first2, llvm_vararg_captured_words[1]);
  c166_test_check_u32(base + 3U, first1, llvm_vararg_captured_words[2]);
  c166_test_check_u32(base + 4U, first0, llvm_vararg_captured_words[3]);
  c166_test_check_u32(base + 5U, middle, llvm_vararg_observed[0]);
  c166_test_check_u32(base + 6U, second3, llvm_vararg_captured_words[4]);
  c166_test_check_u32(base + 7U, second2, llvm_vararg_captured_words[5]);
  c166_test_check_u32(base + 8U, second1, llvm_vararg_captured_words[6]);
  c166_test_check_u32(base + 9U, second0, llvm_vararg_captured_words[7]);
  c166_test_check_u32(base + 10U, mix, llvm_vararg_observed[1]);
}

static void prepare_raw(abi_u16 first3, abi_u16 first2,
                        abi_u16 first1, abi_u16 first0, abi_u16 middle,
                        abi_u16 second3, abi_u16 second2,
                        abi_u16 second1, abi_u16 second0) {
  llvm_vararg_inputs[0] = 0x1111U;
  llvm_vararg_inputs[1] = 0x2222U;
  llvm_vararg_inputs[2] = 0x4444U;
  llvm_vararg_inputs[3] = 0x8888U;
  llvm_vararg_inputs[4] = 0xabcdU;
  llvm_vararg_inputs[5] = first3;
  llvm_vararg_inputs[6] = first2;
  llvm_vararg_inputs[7] = first1;
  llvm_vararg_inputs[8] = first0;
  llvm_vararg_inputs[9] = middle;
  llvm_vararg_inputs[10] = second3;
  llvm_vararg_inputs[11] = second2;
  llvm_vararg_inputs[12] = second1;
  llvm_vararg_inputs[13] = second0;
}

static void run_raw_vector(abi_u16 vector_id,
                           abi_u16 first3, abi_u16 first2,
                           abi_u16 first1, abi_u16 first0, abi_u16 middle,
                           abi_u16 second3, abi_u16 second2,
                           abi_u16 second1, abi_u16 second0) {
  abi_u16 shape;
  (void)vector_id;
  prepare_raw(first3, first2, first1, first0, middle,
              second3, second2, second1, second0);
  for (shape = 1; shape <= 5; ++shape)
    check_capture((abi_u16)(0x7000U +
                  ((vector_id - 1U) * 5U + shape - 1U) * 11U),
                  shape, call_raw(shape), first3, first2, first1, first0,
                  middle, second3, second2, second1, second0);
}

static void run_promotion_vector(abi_u16 vector_id, abi_u16 float1,
                                 abi_u16 float0, abi_u16 double3,
                                 abi_u16 double2, abi_u16 double1,
                                 abi_u16 double0) {
  abi_u16 shape;
  (void)vector_id;
  for (shape = 1; shape <= 5; ++shape) {
    abi_u32 result = llvm_float_promote_proxy(shape, float1, float0);
    check_capture((abi_u16)(0x7000U + 275U +
                  ((vector_id - 1U) * 5U + shape - 1U) * 11U),
                  shape, result, double3, double2, double1, double0, 0x55aaU,
                  0x3ff0U, 0x0000U, 0x0000U, 0x0000U);
  }
}

#define RUN_RAW(id, a3, a2, a1, a0, middle, b3, b2, b1, b0)                \
  run_raw_vector(id, a3, a2, a1, a0, middle, b3, b2, b1, b0);
#define RUN_PROMOTION(id, f1, f0, d3, d2, d1, d0)                           \
  run_promotion_vector(id, f1, f0, d3, d2, d1, d0);

void main(void) {
  c166_test_begin(300, 0x1660012cUL);
  ABI_FLOAT_VARARGS_RAW_VECTORS(RUN_RAW)
  ABI_FLOAT_VARARGS_PROMOTION_VECTORS(RUN_PROMOTION)
  c166_test_finish();
  simulator_stop();
}

