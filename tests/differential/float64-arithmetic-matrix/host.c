#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern volatile abi_u16 llvm_float64_observed[4];
extern void llvm_float64_eval_proxy(abi_u16, abi_u16, abi_u16, abi_u16,
                                    abi_u16, abi_u16, abi_u16, abi_u16,
                                    abi_u16);

void simulator_stop(void) {
  for (;;)
    ;
}

static abi_u16 observed_is_nan(void) {
  return (abi_u16)((llvm_float64_observed[0] & 0x7ff0U) == 0x7ff0U &&
                   ((llvm_float64_observed[0] & 0x000fU) != 0U ||
                    llvm_float64_observed[1] != 0U ||
                    llvm_float64_observed[2] != 0U ||
                    llvm_float64_observed[3] != 0U));
}

static void run_vector(abi_u16 vector_id, abi_u16 operation,
                       abi_u16 l0, abi_u16 l1, abi_u16 l2, abi_u16 l3,
                       abi_u16 r0, abi_u16 r1, abi_u16 r2, abi_u16 r3,
                       abi_u16 e0, abi_u16 e1, abi_u16 e2, abi_u16 e3,
                       abi_u16 any_nan_allowed) {
  abi_u32 expected_hi;
  abi_u32 expected_lo;
  abi_u32 actual_hi;
  abi_u32 actual_lo;

  llvm_float64_eval_proxy(operation, l0, l1, l2, l3, r0, r1, r2, r3);
  expected_hi = ((abi_u32)e0 << 16) | e1;
  expected_lo = ((abi_u32)e2 << 16) | e3;
  actual_hi = ((abi_u32)llvm_float64_observed[0] << 16) |
              llvm_float64_observed[1];
  actual_lo = ((abi_u32)llvm_float64_observed[2] << 16) |
              llvm_float64_observed[3];
  if (any_nan_allowed != 0U && observed_is_nan()) {
    actual_hi = expected_hi;
    actual_lo = expected_lo;
  }
  c166_test_check_u32((abi_u16)(vector_id * 2U - 1U), expected_hi, actual_hi);
  c166_test_check_u32((abi_u16)(vector_id * 2U), expected_lo, actual_lo);
}

#define RUN_VECTOR(id, operation, name, l0, l1, l2, l3, r0, r1, r2, r3,    \
                   e0, e1, e2, e3, nan_ok)                                 \
  run_vector(id, operation, l0, l1, l2, l3, r0, r1, r2, r3, e0, e1, e2,   \
             e3, nan_ok);

void main(void) {
  c166_test_begin(292, 0x1660011eUL);
  ABI_FLOAT64_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}

