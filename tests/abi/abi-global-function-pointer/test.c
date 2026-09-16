#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

static void run_vector(abi_u16 a, abi_u16 b, abi_u16 alternate_expected,
                       abi_u16 global_expected) {
  abi_fn exchanged;
  abi_fn target;

  tap_is_u32(alternate_roundtrip(alternate_target, a, b),
             alternate_expected, "alternate roundtrip");
  tap_is_u32(global_roundtrip(alternate_target, a, b), alternate_expected,
             "global roundtrip");
  tap_is_u32(forward_roundtrip(alternate_target, a, b), alternate_expected,
             "forwarded roundtrip");

  exchanged = global_exchange(alternate_target);
  tap_ok(exchanged == alternate_target, "alternate exchange identity");
  tap_is_u32(exchanged(a, b), alternate_expected, "alternate exchanged call");

  target = get_global_target();
  tap_ok(target == get_global_target(), "global target identity");
  exchanged = global_exchange(target);
  tap_ok(exchanged == target, "global exchange identity");
  tap_is_u32(call_global(a, b), global_expected, "global slot call");
  tap_is_u32(exchanged(a, b), global_expected, "global exchanged call");

  exchanged = alternate_exchange(alternate_target);
  tap_ok(exchanged == alternate_target, "reverse exchange identity");
  tap_is_u32(exchanged(a, b), alternate_expected, "reverse exchanged call");
}

#define RUN_VECTOR(a, b, alternate_expected, global_expected)                 \
  run_vector(a, b, alternate_expected, global_expected);

void main(void) {
  abi_fn exchanged;

  tap_plan(48U);
  tap_ok(global_slot == (abi_fn)0, "global slot zero initialized");
  ABI_GLOBAL_FUNCTION_POINTER_VECTORS(RUN_VECTOR)

  exchanged = global_exchange((abi_fn)0);
  tap_ok(exchanged == (abi_fn)0, "global null exchange");
  exchanged = alternate_exchange((abi_fn)0);
  tap_ok(exchanged == (abi_fn)0, "alternate null exchange");
  exchanged = global_exchange(alternate_target);
  tap_ok(exchanged == alternate_target, "global slot restored");
}
