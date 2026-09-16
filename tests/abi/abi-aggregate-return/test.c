#include "types.h"
#include "vectors.inc"

static void run_vector(abi_u16 seed, abi_u16 expected) {
  tap_is_u32(aggregate_return_hash(seed), expected,
             "aggregate return values");
  tap_ok(aggregate_return_indirect_ok(seed),
         "indirect aggregate return values");
}

#define RUN_VECTOR(id, seed, expected) run_vector(seed, (abi_u16)expected);

void main(void) {
  tap_plan(8U);
  ABI_AGGREGATE_RETURN_VECTORS(RUN_VECTOR)
}
