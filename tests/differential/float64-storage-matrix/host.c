#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern volatile abi_u16 tasking_double_array[];
extern volatile abi_u16 tasking_double_record_src[];
extern volatile abi_u16 tasking_double_record_dst[];
extern volatile abi_u16 llvm_double_array[];
extern volatile abi_u16 llvm_double_record[];
extern volatile abi_u16 llvm_double_observed[4];
extern volatile abi_u16 float64_storage_next_case;

extern void llvm_float64_storage_eval_proxy(abi_u16 operation, abi_u16 index,
                                            abi_u16 w3, abi_u16 w2,
                                            abi_u16 w1, abi_u16 w0,
                                            abi_u16 tail);

static void write_words(volatile abi_u16 *base, abi_u16 offset, abi_u16 w3,
                        abi_u16 w2, abi_u16 w1, abi_u16 w0) {
  base[offset + 0U] = w3;
  base[offset + 1U] = w2;
  base[offset + 2U] = w1;
  base[offset + 3U] = w0;
}

static void check_word(abi_u16 expected, abi_u16 actual) {
  c166_test_check_u32(float64_storage_next_case++, expected, actual);
}

static void check_words(volatile abi_u16 *base, abi_u16 offset, abi_u16 w3,
                        abi_u16 w2, abi_u16 w1, abi_u16 w0) {
  check_word(w3, base[offset + 0U]);
  check_word(w2, base[offset + 1U]);
  check_word(w1, base[offset + 2U]);
  check_word(w0, base[offset + 3U]);
}

static void call_and_check(abi_u16 operation, abi_u16 index, abi_u16 w3,
                           abi_u16 w2, abi_u16 w1, abi_u16 w0) {
  llvm_float64_storage_eval_proxy(operation, index, w3, w2, w1, w0,
                                  0x5a5aU);
  check_words(llvm_double_observed, 0U, w3, w2, w1, w0);
}

static void check_initializers(void) {
  call_and_check(2U, 0U, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U);
  check_words(llvm_double_array, 0U, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U);
  call_and_check(2U, 1U, 0xc004U, 0x0000U, 0x0000U, 0x0000U);
  check_words(llvm_double_array, 4U, 0xc004U, 0x0000U, 0x0000U, 0x0000U);
  call_and_check(2U, 2U, 0x0000U, 0x0000U, 0x0000U, 0x0001U);
  check_words(llvm_double_array, 8U, 0x0000U, 0x0000U, 0x0000U, 0x0001U);
  call_and_check(2U, 3U, 0x8000U, 0x0000U, 0x0000U, 0x0000U);
  check_words(llvm_double_array, 12U, 0x8000U, 0x0000U, 0x0000U, 0x0000U);
  call_and_check(6U, 0U, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U);
  check_words(llvm_double_record, 1U, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U);
  call_and_check(6U, 1U, 0xc004U, 0x0000U, 0x0000U, 0x0000U);
  check_words(llvm_double_record, 6U, 0xc004U, 0x0000U, 0x0000U, 0x0000U);
}

static void run_storage_vector(abi_u16 vector_id, abi_u16 w3, abi_u16 w2,
                               abi_u16 w1, abi_u16 w0) {
  abi_u16 index = (abi_u16)((vector_id - 1U) & 3U);
  abi_u16 which = (abi_u16)((vector_id - 1U) & 1U);

  write_words(tasking_double_array, index * 4U, w3, w2, w1, w0);
  call_and_check(0U, index, w3, w2, w1, w0);
  call_and_check(1U, index, w3, w2, w1, w0);
  check_words(tasking_double_array, index * 4U, w3, w2, w1, w0);

  write_words(llvm_double_array, index * 4U, w3, w2, w1, w0);
  call_and_check(2U, index, w3, w2, w1, w0);
  call_and_check(3U, index, w3, w2, w1, w0);
  check_words(llvm_double_array, index * 4U, w3, w2, w1, w0);

  write_words(tasking_double_record_src, which ? 6U : 1U, w3, w2, w1, w0);
  call_and_check(4U, which, w3, w2, w1, w0);
  call_and_check(5U, which, w3, w2, w1, w0);
  check_words(tasking_double_record_src, which ? 6U : 1U, w3, w2, w1, w0);

  write_words(llvm_double_record, which ? 6U : 1U, w3, w2, w1, w0);
  call_and_check(6U, which, w3, w2, w1, w0);
  call_and_check(7U, which, w3, w2, w1, w0);
  check_words(llvm_double_record, which ? 6U : 1U, w3, w2, w1, w0);

  tasking_double_record_src[0] = 0x1357U;
  write_words(tasking_double_record_src, 1U, w3, w2, w1, w0);
  tasking_double_record_src[5] = 0x2468U;
  write_words(tasking_double_record_src, 6U, w3, w2, w1, w0);
  tasking_double_record_src[10] = 0xcdefU;
  tasking_double_record_src[11] = 0x89abU;
  call_and_check(8U, 0U, w3, w2, w1, w0);
  call_and_check(8U, 1U, w3, w2, w1, w0);
  check_words(tasking_double_record_dst, 1U, w3, w2, w1, w0);
  check_words(tasking_double_record_dst, 6U, w3, w2, w1, w0);
  check_word(0x1357U, tasking_double_record_dst[0]);
  check_word(0x2468U, tasking_double_record_dst[5]);
  check_word(0xcdefU, tasking_double_record_dst[10]);
  check_word(0x89abU, tasking_double_record_dst[11]);

  call_and_check(9U, 0U, w3, w2, w1, w0);
  call_and_check(9U, 1U, w3, w2, w1, w0);
}

#define RUN_STORAGE(vector_id, w3, w2, w1, w0)                             \
  run_storage_vector(vector_id, w3, w2, w1, w0);

void main(void) {
  c166_test_begin(293, 0x16600124UL);
  float64_storage_next_case = 1U;
  check_initializers();
  ABI_FLOAT64_STORAGE_VECTORS(RUN_STORAGE)
  c166_test_finish();
  simulator_stop();
}

