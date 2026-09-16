#include "types.h"
#include "vectors.inc"

static void run_recursion_state_vector(abi_u16 depth, abi_u16 seed,
                                       abi_u32 golden) {
  tap_is_u32(recursion_primary(depth, seed), golden,
             "primary recursive call");
  tap_is_u32(recursion_alternate(depth, seed), golden,
             "alternate recursive call");
  tap_is_u32(recursion_primary_alternate_leaf(depth, seed), golden,
             "primary recursion with alternate leaf");
  tap_is_u32(recursion_alternate_primary_leaf(depth, seed), golden,
             "alternate recursion with primary leaf");
  tap_is_u32(recursion_forward_alternate(depth, seed), golden,
             "forwarded recursive call");
}

#define RUN_RECURSION_STATE(id, depth, seed, golden)                         \
  run_recursion_state_vector(depth, seed, golden);

void main(void) {
  tap_plan(25U);
  ABI_RECURSION_STATE_VECTORS(RUN_RECURSION_STATE)
}
