#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_recursive_state_proxy(abi_u16 depth, abi_u16 seed);
extern abi_u32 llvm_recursive_tasking_state_proxy(abi_u16 depth,
                                                   abi_u16 seed);
extern abi_u32 llvm_call_tasking_state_proxy(abi_u16 depth, abi_u16 seed);

void simulator_stop(void) {
  for (;;)
    ;
}

static void run_recursion_state_vector(abi_u16 vector_id, abi_u16 depth,
                                       abi_u16 seed, abi_u32 golden) {
  abi_u16 base = (vector_id - 1U) * ABI_RECURSION_STATE_CASES_PER_VECTOR;

  c166_test_check_u32(base + 1U, golden, tasking_recursive(depth, seed));
  c166_test_check_u32(base + 2U, golden,
                      llvm_recursive_state_proxy(depth, seed));
  c166_test_check_u32(base + 3U, golden,
                      llvm_recursive_tasking_state_proxy(depth, seed));
  c166_test_check_u32(base + 4U, golden,
                      tasking_recursive_llvm_leaf(depth, seed));
  c166_test_check_u32(base + 5U, golden,
                      llvm_call_tasking_state_proxy(depth, seed));
}

#define RUN_RECURSION_STATE(id, depth, seed, golden) \
  run_recursion_state_vector(id, depth, seed, golden);

void main(void) {
  c166_test_begin(275, 0x16600113UL);
  ABI_RECURSION_STATE_VECTORS(RUN_RECURSION_STATE)
  c166_test_finish();
  simulator_stop();
}

