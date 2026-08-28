#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_entry_proxy(abi_u16 operation, abi_u32 x, abi_s32 sx,
                                abi_u32 y, abi_u16 aux, abi_s8 sc, abi_u8 uc,
                                abi_u16 tail);

static void run_vector(abi_u16 id, abi_u16 operation, abi_u32 x, abi_s32 sx,
                       abi_u32 y, abi_u16 aux, abi_s8 sc, abi_u8 uc,
                       abi_u16 tail, abi_u32 expected) {
  c166_test_set_context(0x16600141UL, operation, id, x, y);
  c166_test_check_u32(id, expected,
                      llvm_entry_proxy(operation, x, sx, y, aux, sc, uc,
                                       tail));
}

#define RUN_VECTOR(id, operation, x, sx, y, aux, sc, uc, tail, expected)    \
  run_vector(id, operation, x, sx, y, aux, sc, uc, tail, expected);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  c166_test_begin(321, 0x16600141UL);
  C166_METAMORPHIC_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}
