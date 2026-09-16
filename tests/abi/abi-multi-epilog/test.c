#include "types.h"
#include "vectors.inc"

static void run_multi_epilog_vector(
    abi_u32 a0, abi_u32 a1, abi_u32 a2, abi_u32 a3, abi_u32 a4, abi_u32 a5,
    abi_u16 mode, abi_u16 rounds, abi_u32 golden) {
  tap_is_u32(multi_epilog_pressure(a0, a1, a2, a3, a4, a5, mode, rounds),
             golden, "direct multi-epilog result");
  tap_is_u32(
      multi_epilog_pressure_probe(a0, a1, a2, a3, a4, a5, mode, rounds),
      golden, "multi-epilog result with preserved state");
}

#define RUN_MULTI_EPILOG(id, a0, a1, a2, a3, a4, a5, mode, rounds, golden)  \
  run_multi_epilog_vector(a0, a1, a2, a3, a4, a5, mode, rounds, golden);

void main(void) {
  tap_plan(24U);
  ABI_MULTI_EPILOG_VECTORS(RUN_MULTI_EPILOG)
}
