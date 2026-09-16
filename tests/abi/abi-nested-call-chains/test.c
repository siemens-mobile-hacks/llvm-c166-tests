#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

static void run_vector(abi_u16 id, abi_u16 a, abi_u32 b, abi_u16 c,
                       abi_u16 d, abi_u32 direct_a, abi_u32 direct_b,
                       abi_u32 indirect_direct, abi_u32 twice_a) {
  tap_is_u32(c166_nested_dispatch(ABI_NESTED_DIRECT_INDIRECT,
                                  c166_nested_leaf_a, a, b, c, d),
             direct_a, "direct to indirect call chain");
  tap_is_u32(c166_nested_dispatch(ABI_NESTED_DIRECT_INDIRECT,
                                  c166_nested_leaf_b, a, b, c, d),
             direct_b, "alternate indirect callback");
  tap_is_u32(c166_nested_dispatch(ABI_NESTED_INDIRECT_DIRECT,
                                  c166_nested_reentrant, a, b, c, d),
             indirect_direct, "indirect reentrant call chain");
  tap_is_u32(c166_nested_dispatch(ABI_NESTED_DIRECT_INDIRECT_TWICE,
                                  c166_nested_leaf_a, a, b, c, d),
             twice_a, "two indirect callbacks");
  tap_is_u32(c166_nested_call_count, (abi_u32)id * 5UL,
             "callback call count");
}

#define RUN_VECTOR(id, a, b, c, d, direct_a, direct_b, indirect_direct,      \
                   twice_a)                                                   \
  run_vector(id, a, b, c, d, direct_a, direct_b, indirect_direct, twice_a);

void main(void) {
  c166_nested_call_count = 0;
  tap_plan(31U);
  ABI_NESTED_CALL_VECTORS(RUN_VECTOR)
  tap_is_u32(c166_nested_call_count, 30UL, "final callback call count");
}
