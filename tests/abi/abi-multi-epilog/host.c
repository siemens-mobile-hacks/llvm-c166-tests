#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_multi_epilog_pressure_state_proxy(
    abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, abi_u32, abi_u16, abi_u16);

static void run_multi_epilog_vector(
    abi_u16 vector_id, abi_u32 a0, abi_u32 a1, abi_u32 a2, abi_u32 a3,
    abi_u32 a4, abi_u32 a5, abi_u16 mode, abi_u16 rounds, abi_u32 golden) {
  c166_test_check_u32(vector_id, golden,
                      llvm_multi_epilog_pressure_state_proxy(
                          a0, a1, a2, a3, a4, a5, mode, rounds));
}

#define RUN_MULTI_EPILOG(id, a0, a1, a2, a3, a4, a5, mode, rounds, golden) \
  run_multi_epilog_vector(id, a0, a1, a2, a3, a4, a5, mode, rounds, golden);

void main(void) {
  c166_test_begin(280, 0x16600118UL);
  ABI_MULTI_EPILOG_VECTORS(RUN_MULTI_EPILOG)
  c166_test_finish();
  simulator_stop();
}

