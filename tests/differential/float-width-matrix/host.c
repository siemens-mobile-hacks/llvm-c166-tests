#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern volatile abi_u16 llvm_float_width_observed[4];

extern void llvm_f32_to_f64_proxy(abi_u16, abi_u16);
extern void llvm_f64_to_f32_proxy(abi_u16, abi_u16, abi_u16, abi_u16);

static void check_f64(abi_u16 case_id, abi_u32 expected_hi,
                      abi_u32 expected_lo) {
  abi_u32 actual_hi = ((abi_u32)llvm_float_width_observed[0] << 16) |
                      (abi_u32)llvm_float_width_observed[1];
  abi_u32 actual_lo = ((abi_u32)llvm_float_width_observed[2] << 16) |
                      (abi_u32)llvm_float_width_observed[3];
  c166_test_check_u32(case_id, expected_hi, actual_hi);
  c166_test_check_u32((abi_u16)(case_id + 1U), expected_lo, actual_lo);
}

#define RUN_F32_TO_F64(id, w1, w0, expected_hi, expected_lo)                 \
  do {                                                                        \
    llvm_f32_to_f64_proxy(w1, w0);                                            \
    check_f64((abi_u16)(0x5000U + ((id) - 1U) * 2U), expected_hi,            \
              expected_lo);                                                   \
  } while (0);

#define RUN_F64_TO_F32(id, w3, w2, w1, w0, expected)                         \
  do {                                                                        \
    abi_u32 actual;                                                           \
    llvm_f64_to_f32_proxy(w3, w2, w1, w0);                                   \
    actual = ((abi_u32)llvm_float_width_observed[0] << 16) |                  \
             (abi_u32)llvm_float_width_observed[1];                           \
    c166_test_check_u32((abi_u16)(0x6000U + (id) - 1U), expected, actual);    \
  } while (0);

void main(void) {
  c166_test_begin(295, 0x16600126UL);
  ABI_F32_TO_F64_VECTORS(RUN_F32_TO_F64)
  ABI_F64_TO_F32_VECTORS(RUN_F64_TO_F32)
  c166_test_finish();
  simulator_stop();
}

