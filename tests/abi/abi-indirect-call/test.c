#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

static void run_vector(abi_u16 prefix, abi_u16 a, abi_u16 b, abi_u16 c,
                       abi_u32 pair, abi_u16 tail, abi_u32 expected) {
  tap_is_u32(c166_indirect_target(a, b, c, pair, tail), expected,
             "direct target call");
  tap_is_u32(c166_indirect_entry(prefix, c166_indirect_target, a, b, c, pair,
                                 tail),
             expected ^ prefix, "indirect function-pointer call");
}

#define RUN_VECTOR(id, prefix, a, b, c, pair, tail, expected)                 \
  run_vector(prefix, a, b, c, pair, tail, expected);

void main(void) {
  tap_plan(16U);
  ABI_INDIRECT_VECTORS(RUN_VECTOR)
}
