#include "types.h"
#include "vectors.inc"

static void run_tuple_reload_vector(
    abi_u32 a0, abi_u32 a1, abi_u32 a2, abi_u32 a3, abi_u32 a4, abi_u32 a5,
    abi_u16 selector, abi_u16 low, abi_u16 high) {
  tap_is_u32(tuple_reload_low(a0, a1, a2, a3, a4, a5, selector), low,
             "direct low tuple half");
  tap_is_u32(tuple_reload_high(a0, a1, a2, a3, a4, a5, selector), high,
             "direct high tuple half");
  tap_is_u32(tuple_reload_low_probe(a0, a1, a2, a3, a4, a5, selector), low,
             "low tuple half with preserved state");
  tap_is_u32(tuple_reload_high_probe(a0, a1, a2, a3, a4, a5, selector), high,
             "high tuple half with preserved state");
}

#define RUN_TUPLE_RELOAD(id, a0, a1, a2, a3, a4, a5, selector, low, high)   \
  run_tuple_reload_vector(a0, a1, a2, a3, a4, a5, selector, low, high);

void main(void) {
  tap_plan(48U);
  ABI_TUPLE_RELOAD_VECTORS(RUN_TUPLE_RELOAD)
}
