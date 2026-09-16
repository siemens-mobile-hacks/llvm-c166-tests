#include "c166_test.h"
#include "types.h"

C166_NOINLINE abi_u16 alternate_target(abi_u16 a, abi_u16 b) {
  return (a ^ 0x5aa5U) + b;
}

C166_NOINLINE abi_u16 alternate_roundtrip(abi_fn target, abi_u16 a,
                                          abi_u16 b) {
  abi_fn loaded;
  abi_u16 result;

  global_slot = target;
  loaded = global_slot;
  result = loaded(a, b);
  if (loaded != target)
    result ^= 0xffffU;
  return result;
}

C166_NOINLINE abi_fn alternate_exchange(abi_fn target) {
  global_slot = target;
  return global_slot;
}

C166_NOINLINE abi_u16 forward_roundtrip(abi_fn target, abi_u16 a,
                                        abi_u16 b) {
  return alternate_roundtrip(target, a, b);
}
