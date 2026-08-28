#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_float32_eval_bits_proxy(abi_u16, abi_u16, abi_u16,
                                            abi_u16, abi_u16);
extern abi_u32 llvm_float32_reverse_proxy(abi_u32);
extern abi_u32 llvm_float32_load_external_proxy(abi_u32);
extern abi_u32 llvm_float32_store_external_proxy(abi_u32);
extern abi_u32 llvm_float32_load_own_initial_proxy(void);
extern abi_u32 llvm_float32_load_own_proxy(abi_u32);
extern abi_u32 llvm_float32_store_own_proxy(abi_u32);
extern abi_u32 llvm_float32_slot0_proxy(float, abi_u16);
extern abi_u32 llvm_float32_slot1_proxy(abi_u16, float, abi_u16);
extern abi_u32 llvm_float32_slot2_proxy(abi_u16, abi_u16, float, abi_u16);
extern abi_u32 llvm_float32_slot3_proxy(abi_u16, abi_u16, abi_u16, float,
                                       abi_u16);
extern abi_u32 llvm_float32_slot4_proxy(abi_u16, abi_u16, abi_u16, abi_u16,
                                       float, abi_u16);
extern abi_u32 llvm_float32_call_tasking_proxy(abi_u16, abi_u16, abi_u16,
                                               float, abi_u16);
extern abi_u32 llvm_float32_icall_tasking_proxy(abi_float_slot3_fn, abi_u16,
                                                abi_u16, abi_u16, float,
                                                abi_u16);
extern abi_u32 llvm_float32_cross_tu_proxy(abi_u16, abi_u16, abi_u16, float,
                                           abi_u16);
extern abi_u32 tasking_float32_nested_proxy(abi_u16, abi_u16, abi_u16, float,
                                            abi_u16);
extern const float tasking_float_call_values[];
extern float tasking_float_slot3(abi_u16, abi_u16, abi_u16, float, abi_u16);

typedef abi_u32 (*abi_float_slot3_proxy_fn)(abi_u16, abi_u16, abi_u16, float,
                                            abi_u16);

void simulator_stop(void) {
  for (;;)
    ;
}

static abi_u16 is_nan_bits(abi_u32 bits) {
  return (abi_u16)((bits & 0x7f800000UL) == 0x7f800000UL &&
                   (bits & 0x007fffffUL) != 0UL);
}

static abi_u32 normalize_nan(abi_u32 expected, abi_u32 actual,
                             abi_u16 any_nan_allowed) {
  if (any_nan_allowed != 0U && is_nan_bits(actual))
    return expected;
  return actual;
}

static void run_arithmetic_vector(abi_u16 case_id, abi_u16 operation,
                                  abi_u32 lhs_bits, abi_u32 rhs_bits,
                                  abi_u32 expected, abi_u16 any_nan_allowed) {
  abi_u32 actual;
  actual = llvm_float32_eval_bits_proxy(operation,
      (abi_u16)lhs_bits, (abi_u16)(lhs_bits >> 16),
      (abi_u16)rhs_bits, (abi_u16)(rhs_bits >> 16));
  c166_test_check_u32(case_id, expected,
      normalize_nan(expected, actual, any_nan_allowed));
}

#define RUN_ARITHMETIC(case_id, operation, name, lhs, rhs, expected, nan_ok) \
  run_arithmetic_vector(case_id, operation, lhs, rhs, expected, nan_ok);
#define RUN_IDENTITY(vector_id, bits) \
  c166_test_check_u32((abi_u16)(0x4000U + vector_id), bits, \
      llvm_float32_reverse_proxy(bits));
#define RUN_STORAGE(vector_id, bits)                                      \
  c166_test_check_u32((abi_u16)(0x5100U + (vector_id - 1U) * 4U), bits,   \
      llvm_float32_load_external_proxy(bits));                            \
  c166_test_check_u32((abi_u16)(0x5101U + (vector_id - 1U) * 4U), bits,   \
      llvm_float32_store_external_proxy(bits));                           \
  c166_test_check_u32((abi_u16)(0x5102U + (vector_id - 1U) * 4U), bits,   \
      llvm_float32_load_own_proxy(bits));                                 \
  c166_test_check_u32((abi_u16)(0x5103U + (vector_id - 1U) * 4U), bits,   \
      llvm_float32_store_own_proxy(bits));

static void run_call_vector(abi_u16 vector_id, abi_u32 bits) {
  abi_u16 base = (abi_u16)(0x6000U + (vector_id - 1U) * 10U);
  float value = tasking_float_call_values[vector_id - 1U];
  abi_float_slot3_proxy_fn tasking_indirect = llvm_float32_slot3_proxy;

  c166_test_check_u32(base + 0U, bits,
                      llvm_float32_slot0_proxy(value, 0x5a5aU));
  c166_test_check_u32(base + 1U, bits,
                      llvm_float32_slot1_proxy(0x1111U, value, 0x5a5aU));
  c166_test_check_u32(base + 2U, bits,
                      llvm_float32_slot2_proxy(0x1111U, 0x2222U, value,
                                               0x5a5aU));
  c166_test_check_u32(base + 3U, bits,
                      llvm_float32_slot3_proxy(0x1111U, 0x2222U, 0x3333U,
                                               value, 0x5a5aU));
  c166_test_check_u32(base + 4U, bits,
                      llvm_float32_slot4_proxy(0x1111U, 0x2222U, 0x3333U,
                                               0x4444U, value, 0x5a5aU));
  c166_test_check_u32(base + 5U, bits,
                      tasking_indirect(0x1111U, 0x2222U, 0x3333U, value,
                                       0x5a5aU));
  c166_test_check_u32(base + 6U, bits,
                      llvm_float32_call_tasking_proxy(
                          0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU));
  c166_test_check_u32(base + 7U, bits,
                      llvm_float32_icall_tasking_proxy(
                          tasking_float_slot3, 0x1111U, 0x2222U, 0x3333U,
                          value, 0x5a5aU));
  c166_test_check_u32(base + 8U, bits,
                      llvm_float32_cross_tu_proxy(
                          0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU));
  c166_test_check_u32(base + 9U, bits,
                      tasking_float32_nested_proxy(
                          0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU));
}

#define RUN_CALL(vector_id, bits) run_call_vector(vector_id, bits);

void main(void) {
  c166_test_begin(286, 0x1660011eUL);
  ABI_FLOAT32_VECTORS(RUN_ARITHMETIC)
  ABI_FLOAT32_IDENTITY_VECTORS(RUN_IDENTITY)
  c166_test_check_u32(0x5000U, 0x3f800000UL,
                      llvm_float32_load_own_initial_proxy());
  ABI_FLOAT32_IDENTITY_VECTORS(RUN_STORAGE)
  ABI_FLOAT32_CALL_VECTORS(RUN_CALL)
  c166_test_finish();
  simulator_stop();
}

