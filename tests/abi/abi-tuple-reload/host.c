#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

typedef abi_u16 (*tuple_reload_function)(abi_u32, abi_u32, abi_u32, abi_u32,
                                         abi_u32, abi_u32, abi_u16);

extern abi_u16 llvm_tuple_reload_low_state_proxy(
    abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, abi_u16);
extern abi_u16 llvm_tuple_reload_high_state_proxy(
    abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, abi_u16);

static void run_tuple_reload_vector(
    abi_u16 vector_id, abi_u32 a0, abi_u32 a1, abi_u32 a2, abi_u32 a3,
    abi_u32 a4, abi_u32 a5, abi_u16 selector, abi_u16 low, abi_u16 high) {
  abi_u16 base = (vector_id - 1U) * ABI_TUPLE_RELOAD_CASES_PER_VECTOR;

  c166_test_check_u32(base + 1U, low, llvm_tuple_reload_low_state_proxy(
                                          a0, a1, a2, a3, a4, a5, selector));
  c166_test_check_u32(base + 2U, high, llvm_tuple_reload_high_state_proxy(
                                           a0, a1, a2, a3, a4, a5, selector));
}

#define RUN_TUPLE_RELOAD(id, a0, a1, a2, a3, a4, a5, selector, low, high) \
  run_tuple_reload_vector(id, a0, a1, a2, a3, a4, a5, selector, low, high);

void main(void) {
  c166_test_begin(279, 0x16600117UL);
  ABI_TUPLE_RELOAD_VECTORS(RUN_TUPLE_RELOAD)
  c166_test_finish();
  simulator_stop();
}

