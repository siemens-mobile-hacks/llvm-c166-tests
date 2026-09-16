#include "c166_test.h"
#include "common.h"
#include "vectors.inc"

static void run_vector(abi_u16 seed, abi_u16 expected, abi_u32 repeated) {
  struct pair2 pair_value;
  struct chars3 chars_value;
  ABI_PACKED_REF(packed3) packed_value;
  abi_u16 tail;

  ABI_INIT_AGGREGATES(seed, pair_value, chars_value, packed_value, tail);
  tap_is_u32(varargs_aggregate(ABI_AGGREGATE_VARARGS_FIXED, pair_value,
                               chars_value, packed_value, tail),
             expected, "aggregate variadic arguments");
  tap_is_u32(varargs_aggregate_repeat(seed), repeated,
             "aggregate variadic caller");
}

#define RUN_VECTOR(id, seed, expected, repeated) \
  run_vector(seed, expected, repeated);

void main(void) {
  tap_plan(12U);
  ABI_AGGREGATE_VARARGS_VECTORS(RUN_VECTOR)
}
