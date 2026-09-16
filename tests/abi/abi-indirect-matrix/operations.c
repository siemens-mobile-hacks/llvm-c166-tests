#include "types.h"

C166_NOINLINE C166_SECTION(".abi.indirect.call0")
abi_u32 indirect_call0(abi_fn0 volatile *slot, abi_fn0 target) {
  abi_fn0 loaded;
  *slot = target;
  loaded = *slot;
  return loaded() ^ (loaded == target ? 0UL : 0xffffffffUL);
}

C166_NOINLINE C166_SECTION(".abi.indirect.call2")
abi_u32 indirect_call2(abi_fn2 volatile *slot, abi_fn2 target, abi_u16 a,
                       abi_u16 b) {
  abi_fn2 loaded;
  *slot = target;
  loaded = *slot;
  return loaded(a, b) ^ (loaded == target ? 0UL : 0xffffffffUL);
}

C166_NOINLINE C166_SECTION(".abi.indirect.call5")
abi_u32 indirect_call5(abi_fn5 volatile *slot, abi_fn5 target, abi_u16 a,
                       abi_u16 b, abi_u16 c, abi_u16 d, abi_u16 e) {
  abi_fn5 loaded;
  *slot = target;
  loaded = *slot;
  return loaded(a, b, c, d, e) ^
         (loaded == target ? 0UL : 0xffffffffUL);
}

C166_NOINLINE C166_SECTION(".abi.indirect.get0")
abi_fn0 indirect_get0(void) {
  return indirect_target0;
}

C166_NOINLINE C166_SECTION(".abi.indirect.get2")
abi_fn2 indirect_get2(void) {
  return indirect_target2;
}

C166_NOINLINE C166_SECTION(".abi.indirect.get5")
abi_fn5 indirect_get5(void) {
  return indirect_target5;
}
