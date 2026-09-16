#include "c166_test.h"
#include "types.h"

abi_fn volatile global_slot
    C166_USED C166_SECTION(".global.function-pointer.slot");

C166_NOINLINE C166_SECTION(".global.function-pointer.target")
abi_u16 global_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a + 0x1357U) ^ b;
}

C166_NOINLINE C166_SECTION(".global.function-pointer.exchange")
abi_fn global_exchange(abi_fn target) {
  global_slot = target;
  return global_slot;
}

C166_NOINLINE C166_SECTION(".global.function-pointer.call")
abi_u16 call_global(abi_u16 a, abi_u16 b) {
  abi_fn loaded = global_slot;
  return loaded(a, b);
}

C166_NOINLINE C166_SECTION(".global.function-pointer.roundtrip")
abi_u16 global_roundtrip(abi_fn target, abi_u16 a, abi_u16 b) {
  abi_fn loaded = global_exchange(target);
  abi_u16 result = call_global(a, b);

  if (loaded != target)
    result ^= 0xffffU;
  return result;
}

C166_NOINLINE C166_SECTION(".global.function-pointer.get-target")
abi_fn get_global_target(void) {
  return global_target;
}
