#include "c166-test-result.h"
#include "c166-test-runtime.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 llvm_entry_proxy(abi_u16, abi_u16, abi_u16, abi_u16, abi_u16,
                                abi_u16, abi_u16, abi_u16, abi_u16);

static void run_vector(abi_u16 id, abi_u16 operation, abi_u16 left3,
                       abi_u16 left2, abi_u16 left1, abi_u16 left0,
                       abi_u16 right3, abi_u16 right2, abi_u16 right1,
                       abi_u16 right0, abi_u16 expected) {
  c166_test_check_u32(id, expected,
                      llvm_entry_proxy(left3, left2, left1, left0, right3,
                                       right2, right1, right0, operation));
}

#define RUN_VECTOR(id, op, l3, l2, l1, l0, r3, r2, r1, r0, expected) \
  run_vector(id, op, l3, l2, l1, l0, r3, r2, r1, r0, expected);

void main(void) {
  c166_test_begin(321, 0);
  I64_ARITHMETIC_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}
