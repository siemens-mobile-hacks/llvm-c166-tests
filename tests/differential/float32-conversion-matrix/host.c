#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern const float tasking_float_values[];
extern abi_s32 llvm_f32_to_i32_proxy(float);
extern abi_u32 llvm_f32_to_u32_proxy(float);
extern abi_s16 llvm_f32_to_i16_proxy(float);
extern abi_u16 llvm_f32_to_u16_proxy(float);
extern abi_u32 llvm_i32_to_f32_proxy(abi_s32);
extern abi_u32 llvm_u32_to_f32_proxy(abi_u32);
extern abi_u32 llvm_i16_to_f32_proxy(abi_s16);
extern abi_u32 llvm_u16_to_f32_proxy(abi_u16);
extern abi_s16 llvm_f32_compare_proxy(abi_u16, float, float);

#define RUN_F32_I32(id, pool, bits, expected) do {                          \
  c166_test_check_u32((abi_u16)(0x1000U + (id) - 1U), expected,             \
      (abi_u32)llvm_f32_to_i32_proxy(tasking_float_values[pool]));          \
} while (0);

#define RUN_F32_U32(id, pool, bits, expected) do {                          \
  c166_test_check_u32((abi_u16)(0x1100U + (id) - 1U), expected,             \
      llvm_f32_to_u32_proxy(tasking_float_values[pool]));                   \
} while (0);

#define RUN_F32_I16(id, pool, bits, expected) do {                          \
  c166_test_check_u32((abi_u16)(0x1200U + (id) - 1U), expected,             \
      (abi_u32)(abi_s32)llvm_f32_to_i16_proxy(tasking_float_values[pool])); \
} while (0);

#define RUN_F32_U16(id, pool, bits, expected) do {                          \
  c166_test_check_u32((abi_u16)(0x1300U + (id) - 1U), expected,             \
      (abi_u32)llvm_f32_to_u16_proxy(tasking_float_values[pool]));          \
} while (0);

#define RUN_I32_F32(id, value, expected) do {                               \
  c166_test_check_u32((abi_u16)(0x2000U + (id) - 1U), expected,             \
                      llvm_i32_to_f32_proxy(value));                        \
} while (0);

#define RUN_U32_F32(id, value, expected) do {                               \
  c166_test_check_u32((abi_u16)(0x2100U + (id) - 1U), expected,             \
                      llvm_u32_to_f32_proxy(value));                        \
} while (0);

#define RUN_I16_F32(id, value, expected) do {                               \
  c166_test_check_u32((abi_u16)(0x2200U + (id) - 1U), expected,             \
                      llvm_i16_to_f32_proxy(value));                        \
} while (0);

#define RUN_U16_F32(id, value, expected) do {                               \
  c166_test_check_u32((abi_u16)(0x2300U + (id) - 1U), expected,             \
                      llvm_u16_to_f32_proxy(value));                        \
} while (0);

static void check_compare(abi_u16 case_id, abi_u16 operation,
                          abi_u16 expected, abi_u16 lhs_pool,
                          abi_u16 rhs_pool) {
  c166_test_check_u32(case_id, (abi_u32)expected,
      (abi_u32)llvm_f32_compare_proxy(operation,
          tasking_float_values[lhs_pool], tasking_float_values[rhs_pool]));
}

#define RUN_COMPARE(id, lhs_pool, rhs_pool, lhs_bits, rhs_bits,             \
                    eq, ne, lt, le, gt, ge, unord) do {                     \
  abi_u16 base = (abi_u16)(0x4000U + ((id) - 1U) * 7U);                    \
  check_compare(base + 0U, 0U, eq, lhs_pool, rhs_pool);                    \
  check_compare(base + 1U, 1U, ne, lhs_pool, rhs_pool);                    \
  check_compare(base + 2U, 2U, lt, lhs_pool, rhs_pool);                    \
  check_compare(base + 3U, 3U, le, lhs_pool, rhs_pool);                    \
  check_compare(base + 4U, 4U, gt, lhs_pool, rhs_pool);                    \
  check_compare(base + 5U, 5U, ge, lhs_pool, rhs_pool);                    \
  check_compare(base + 6U, 6U, unord, lhs_pool, rhs_pool);                 \
} while (0);

void main(void) {
  c166_test_begin(291, 0x16600123UL);
  ABI_F32_TO_I32_VECTORS(RUN_F32_I32)
  ABI_F32_TO_U32_VECTORS(RUN_F32_U32)
  ABI_F32_TO_I16_VECTORS(RUN_F32_I16)
  ABI_F32_TO_U16_VECTORS(RUN_F32_U16)
  ABI_I32_TO_F32_VECTORS(RUN_I32_F32)
  ABI_U32_TO_F32_VECTORS(RUN_U32_F32)
  ABI_I16_TO_F32_VECTORS(RUN_I16_F32)
  ABI_U16_TO_F32_VECTORS(RUN_U16_F32)
  ABI_F32_COMPARE_VECTORS(RUN_COMPARE)
  c166_test_finish();
  simulator_stop();
}

