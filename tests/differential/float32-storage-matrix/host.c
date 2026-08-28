#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_float_array_load_external_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_array_store_external_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_array_load_own_initial_proxy(abi_u16);
extern abi_u32 llvm_float_array_load_own_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_array_store_own_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_record_load_external_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_record_store_external_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_record_load_own_initial_proxy(abi_u16);
extern abi_u32 llvm_float_record_load_own_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_record_store_own_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_record_copy_external_proxy(abi_u32, abi_u16);
extern abi_u32 llvm_float_record_local_proxy(abi_u32, abi_u16);

static void run_storage_vector(abi_u16 vector_id, abi_u32 bits) {
  abi_u16 base = (abi_u16)(0x2000U + (vector_id - 1U) * 12U);
  abi_u16 index = (abi_u16)((vector_id - 1U) & 3U);
  abi_u16 which = (abi_u16)((vector_id - 1U) & 1U);

  c166_test_check_u32(base + 0U, bits,
                      llvm_float_array_load_external_proxy(bits, index));
  c166_test_check_u32(base + 1U, bits,
                      llvm_float_array_store_external_proxy(bits, index));
  c166_test_check_u32(base + 2U, bits,
                      llvm_float_array_load_own_proxy(bits, index));
  c166_test_check_u32(base + 3U, bits,
                      llvm_float_array_store_own_proxy(bits, index));
  c166_test_check_u32(base + 4U, bits,
                      llvm_float_record_load_external_proxy(bits, which));
  c166_test_check_u32(base + 5U, bits,
                      llvm_float_record_store_external_proxy(bits, which));
  c166_test_check_u32(base + 6U, bits,
                      llvm_float_record_load_own_proxy(bits, which));
  c166_test_check_u32(base + 7U, bits,
                      llvm_float_record_store_own_proxy(bits, which));
  c166_test_check_u32(base + 8U, bits,
                      llvm_float_record_copy_external_proxy(bits, 0U));
  c166_test_check_u32(base + 9U, bits,
                      llvm_float_record_copy_external_proxy(bits, 1U));
  c166_test_check_u32(base + 10U, bits,
                      llvm_float_record_local_proxy(bits, 0U));
  c166_test_check_u32(base + 11U, bits,
                      llvm_float_record_local_proxy(bits, 1U));
}

#define RUN_STORAGE(vector_id, bits) run_storage_vector(vector_id, bits);

void main(void) {
  c166_test_begin(290, 0x16600122UL);
  c166_test_check_u32(0x1000U, 0x3f800000UL,
                      llvm_float_array_load_own_initial_proxy(0U));
  c166_test_check_u32(0x1001U, 0xc0200000UL,
                      llvm_float_array_load_own_initial_proxy(1U));
  c166_test_check_u32(0x1002U, 0x00000001UL,
                      llvm_float_array_load_own_initial_proxy(2U));
  c166_test_check_u32(0x1003U, 0x80000000UL,
                      llvm_float_array_load_own_initial_proxy(3U));
  c166_test_check_u32(0x1004U, 0x3f800000UL,
                      llvm_float_record_load_own_initial_proxy(0U));
  c166_test_check_u32(0x1005U, 0xc0200000UL,
                      llvm_float_record_load_own_initial_proxy(1U));
  ABI_FLOAT32_STORAGE_VECTORS(RUN_STORAGE)
  c166_test_finish();
  simulator_stop();
}

