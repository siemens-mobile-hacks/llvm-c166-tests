#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 llvm_entry_proxy(abi_u16 seed);
extern abi_u16 hash_tasking_returns(abi_u16 seed);
extern abi_u16 hash_llvm_returns(abi_u16 seed);

static void run_return_vector(abi_u16 vector_id, abi_u16 seed,
                              unsigned long golden) {
  c166_test_check_u32(vector_id * 3 - 2, golden,
                      hash_tasking_returns(seed));
  c166_test_check_u32(vector_id * 3 - 1, golden,
                      hash_llvm_returns(seed));
  c166_test_check_u32(vector_id * 3, golden, llvm_entry_proxy(seed));
}

#define RUN_RETURN(id, seed, golden) run_return_vector(id, seed, golden);

void main(void) {
  c166_test_begin(266, 0x1660010aUL);
  ABI_AGGREGATE_RETURN_VECTORS(RUN_RETURN)
  c166_test_finish();
  simulator_stop();
}

