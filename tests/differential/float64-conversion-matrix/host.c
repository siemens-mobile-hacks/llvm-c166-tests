#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern volatile abi_u16 llvm_f64_observed[4];

extern abi_s32 llvm_f64_to_i32_proxy(abi_u16, abi_u16, abi_u16, abi_u16);
extern abi_u32 llvm_f64_to_u32_proxy(abi_u16, abi_u16, abi_u16, abi_u16);
extern abi_s16 llvm_f64_to_i16_proxy(abi_u16, abi_u16, abi_u16, abi_u16);
extern abi_u16 llvm_f64_to_u16_proxy(abi_u16, abi_u16, abi_u16, abi_u16);
extern void llvm_i32_to_f64_proxy(abi_s32);
extern void llvm_u32_to_f64_proxy(abi_u32);
extern void llvm_i16_to_f64_proxy(abi_s16);
extern void llvm_u16_to_f64_proxy(abi_u16);
extern abi_s16 llvm_f64_compare_proxy(abi_u16, abi_u16, abi_u16, abi_u16,
                                      abi_u16, abi_u16, abi_u16, abi_u16,
                                      abi_u16);

static void check_observed(abi_u16 case_id, abi_u32 expected_hi,
                           abi_u32 expected_lo) {
  abi_u32 actual_hi = ((abi_u32)llvm_f64_observed[0] << 16) |
                      (abi_u32)llvm_f64_observed[1];
  abi_u32 actual_lo = ((abi_u32)llvm_f64_observed[2] << 16) |
                      (abi_u32)llvm_f64_observed[3];
  c166_test_check_u32(case_id, expected_hi, actual_hi);
  c166_test_check_u32((abi_u16)(case_id + 1U), expected_lo, actual_lo);
}

#define RUN_F64_I32(id, w3, w2, w1, w0, expected) do {                     \
  c166_test_check_u32((abi_u16)(0x1000U + (id) - 1U), expected,             \
      (abi_u32)llvm_f64_to_i32_proxy(w3, w2, w1, w0));                     \
} while (0);

#define RUN_F64_U32(id, w3, w2, w1, w0, expected) do {                     \
  c166_test_check_u32((abi_u16)(0x1100U + (id) - 1U), expected,             \
      llvm_f64_to_u32_proxy(w3, w2, w1, w0));                              \
} while (0);

#define RUN_F64_I16(id, w3, w2, w1, w0, expected) do {                     \
  c166_test_check_u32((abi_u16)(0x1200U + (id) - 1U), expected,             \
      (abi_u32)(abi_s32)llvm_f64_to_i16_proxy(w3, w2, w1, w0));            \
} while (0);

#define RUN_F64_U16(id, w3, w2, w1, w0, expected) do {                     \
  c166_test_check_u32((abi_u16)(0x1300U + (id) - 1U), expected,             \
      (abi_u32)llvm_f64_to_u16_proxy(w3, w2, w1, w0));                     \
} while (0);

#define RUN_I32_F64(id, value, expected_hi, expected_lo) do {               \
  llvm_i32_to_f64_proxy(value);                                             \
  check_observed((abi_u16)(0x2000U + ((id) - 1U) * 2U),                    \
                 expected_hi, expected_lo);                                \
} while (0);

#define RUN_U32_F64(id, value, expected_hi, expected_lo) do {               \
  llvm_u32_to_f64_proxy(value);                                             \
  check_observed((abi_u16)(0x2100U + ((id) - 1U) * 2U),                    \
                 expected_hi, expected_lo);                                \
} while (0);

#define RUN_I16_F64(id, value, expected_hi, expected_lo) do {               \
  llvm_i16_to_f64_proxy(value);                                             \
  check_observed((abi_u16)(0x2200U + ((id) - 1U) * 2U),                    \
                 expected_hi, expected_lo);                                \
} while (0);

#define RUN_U16_F64(id, value, expected_hi, expected_lo) do {               \
  llvm_u16_to_f64_proxy(value);                                             \
  check_observed((abi_u16)(0x2300U + ((id) - 1U) * 2U),                    \
                 expected_hi, expected_lo);                                \
} while (0);

static void check_compare(abi_u16 case_id, abi_u16 operation,
                          abi_u16 expected, abi_u16 lhs_w3, abi_u16 lhs_w2,
                          abi_u16 lhs_w1, abi_u16 lhs_w0, abi_u16 rhs_w3,
                          abi_u16 rhs_w2, abi_u16 rhs_w1, abi_u16 rhs_w0) {
  c166_test_check_u32(
      case_id, (abi_u32)expected,
      (abi_u32)llvm_f64_compare_proxy(operation, lhs_w3, lhs_w2, lhs_w1,
                                      lhs_w0, rhs_w3, rhs_w2, rhs_w1,
                                      rhs_w0));
}

#define RUN_COMPARE(id, lw3, lw2, lw1, lw0, rw3, rw2, rw1, rw0, eq, ne,    \
                    lt, le, gt, ge, unord)                                  \
  do {                                                                      \
    abi_u16 base = (abi_u16)(0x4000U + ((id) - 1U) * 7U);                  \
    check_compare(base + 0U, 0U, eq, lw3, lw2, lw1, lw0, rw3, rw2, rw1,   \
                  rw0);                                                     \
    check_compare(base + 1U, 1U, ne, lw3, lw2, lw1, lw0, rw3, rw2, rw1,   \
                  rw0);                                                     \
    check_compare(base + 2U, 2U, lt, lw3, lw2, lw1, lw0, rw3, rw2, rw1,   \
                  rw0);                                                     \
    check_compare(base + 3U, 3U, le, lw3, lw2, lw1, lw0, rw3, rw2, rw1,   \
                  rw0);                                                     \
    check_compare(base + 4U, 4U, gt, lw3, lw2, lw1, lw0, rw3, rw2, rw1,   \
                  rw0);                                                     \
    check_compare(base + 5U, 5U, ge, lw3, lw2, lw1, lw0, rw3, rw2, rw1,   \
                  rw0);                                                     \
    check_compare(base + 6U, 6U, unord, lw3, lw2, lw1, lw0, rw3, rw2,     \
                  rw1, rw0);                                                \
  } while (0);

void main(void) {
  c166_test_begin(294, 0x16600125UL);
  ABI_F64_TO_I32_VECTORS(RUN_F64_I32)
  ABI_F64_TO_U32_VECTORS(RUN_F64_U32)
  ABI_F64_TO_I16_VECTORS(RUN_F64_I16)
  ABI_F64_TO_U16_VECTORS(RUN_F64_U16)
  ABI_I32_TO_F64_VECTORS(RUN_I32_F64)
  ABI_U32_TO_F64_VECTORS(RUN_U32_F64)
  ABI_I16_TO_F64_VECTORS(RUN_I16_F64)
  ABI_U16_TO_F64_VECTORS(RUN_U16_F64)
  ABI_F64_COMPARE_VECTORS(RUN_COMPARE)
  c166_test_finish();
  simulator_stop();
}

