#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

#define DECLARE_WORD_PROXY(name)                                             \
  extern abi_u16 name(abi_u16, abi_u16, abi_u16, abi_u16, abi_u16, abi_u16, \
                      abi_u16, abi_u16, abi_u16)
#define DECLARE_LONG_PROXY(name)                                             \
  extern abi_u32 name(abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, \
                      abi_u16)

DECLARE_WORD_PROXY(tasking_pressure_words_state_proxy);
DECLARE_WORD_PROXY(llvm_pressure_words_state_proxy);
DECLARE_WORD_PROXY(llvm_pressure_words_tasking_leaf_state_proxy);
DECLARE_WORD_PROXY(tasking_pressure_words_llvm_leaf_state_proxy);
DECLARE_WORD_PROXY(llvm_call_tasking_pressure_words_state_proxy);
DECLARE_LONG_PROXY(tasking_pressure_longs_state_proxy);
DECLARE_LONG_PROXY(llvm_pressure_longs_state_proxy);
DECLARE_LONG_PROXY(llvm_pressure_longs_tasking_leaf_state_proxy);
DECLARE_LONG_PROXY(tasking_pressure_longs_llvm_leaf_state_proxy);
DECLARE_LONG_PROXY(llvm_call_tasking_pressure_longs_state_proxy);

void simulator_stop(void) {
  for (;;)
    ;
}

static void run_pressure_word_vector(
    abi_u16 vector_id, abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3,
    abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7, abi_u16 selector,
    abi_u16 golden) {
  abi_u16 base = (vector_id - 1U) * ABI_PRESSURE_CASES_PER_VECTOR;

  c166_test_check_u32(
      base + 1U, golden, tasking_pressure_words_state_proxy(
                              a0, a1, a2, a3, a4, a5, a6, a7, selector));
  c166_test_check_u32(
      base + 2U, golden, llvm_pressure_words_state_proxy(
                              a0, a1, a2, a3, a4, a5, a6, a7, selector));
  c166_test_check_u32(
      base + 3U, golden, llvm_pressure_words_tasking_leaf_state_proxy(
                              a0, a1, a2, a3, a4, a5, a6, a7, selector));
  c166_test_check_u32(
      base + 4U, golden, tasking_pressure_words_llvm_leaf_state_proxy(
                              a0, a1, a2, a3, a4, a5, a6, a7, selector));
  c166_test_check_u32(
      base + 5U, golden, llvm_call_tasking_pressure_words_state_proxy(
                              a0, a1, a2, a3, a4, a5, a6, a7, selector));
}

static void run_pressure_long_vector(
    abi_u16 vector_id, abi_u32 a0, abi_u32 a1, abi_u32 a2, abi_u32 a3,
    abi_u32 a4, abi_u32 a5, abi_u16 selector, abi_u32 golden) {
  abi_u16 base = ABI_PRESSURE_WORD_CASES +
                 (vector_id - 1U) * ABI_PRESSURE_CASES_PER_VECTOR;

  c166_test_check_u32(base + 1U, golden,
                      tasking_pressure_longs_state_proxy(
                          a0, a1, a2, a3, a4, a5, selector));
  c166_test_check_u32(base + 2U, golden,
                      llvm_pressure_longs_state_proxy(
                          a0, a1, a2, a3, a4, a5, selector));
  c166_test_check_u32(base + 3U, golden,
                      llvm_pressure_longs_tasking_leaf_state_proxy(
                          a0, a1, a2, a3, a4, a5, selector));
  c166_test_check_u32(base + 4U, golden,
                      tasking_pressure_longs_llvm_leaf_state_proxy(
                          a0, a1, a2, a3, a4, a5, selector));
  c166_test_check_u32(base + 5U, golden,
                      llvm_call_tasking_pressure_longs_state_proxy(
                          a0, a1, a2, a3, a4, a5, selector));
}

#define RUN_PRESSURE_WORD(id, a0, a1, a2, a3, a4, a5, a6, a7, selector,   \
                          golden)                                           \
  run_pressure_word_vector(id, a0, a1, a2, a3, a4, a5, a6, a7, selector,  \
                           golden);
#define RUN_PRESSURE_LONG(id, a0, a1, a2, a3, a4, a5, selector, golden)    \
  run_pressure_long_vector(id, a0, a1, a2, a3, a4, a5, selector, golden);

void main(void) {
  c166_test_begin(277, 0x16600115UL);
  ABI_PRESSURE_WORD_VECTORS(RUN_PRESSURE_WORD)
  ABI_PRESSURE_LONG_VECTORS(RUN_PRESSURE_LONG)
  c166_test_finish();
  simulator_stop();
}

