#include "c166-test-result.h"
#include "c166-test-runtime.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 llvm_entry_proxy(abi_u16, abi_u16, abi_u16, abi_u16, abi_u16);

static void run_vector(abi_u16 id, abi_u16 operation, abi_u16 word3,
                       abi_u16 word2, abi_u16 word1, abi_u16 word0,
                       abi_u16 expected) {
  c166_test_check_u32(
      id, expected,
      llvm_entry_proxy(word3, word2, word1, word0, operation));
}

#define RUN_VECTOR(id, op, w3, w2, w1, w0, expected) \
  run_vector(id, op, w3, w2, w1, w0, expected);

void main(void) {
  c166_test_begin(322, 0);
  I64_CONVERSION_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}
