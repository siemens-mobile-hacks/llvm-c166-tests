#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 llvm_entry_proxy(abi_u32);

#define CHECK_CLZ(vector_id, value, expected) \
  c166_test_check_u32(vector_id, expected, llvm_entry_proxy(value));

void main(void) {
  c166_test_begin(285, 0x1660011dUL);
  ABI_CLZ32_VECTORS(CHECK_CLZ)
  c166_test_finish();
  simulator_stop();
}

