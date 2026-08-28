#include "types.h"

#define NOINLINE_SECTION(name) __attribute__((noinline, section(name)))

NOINLINE_SECTION(".llvm_target0")
abi_u32 llvm_target0(void) {
  return 0x13572468UL;
}

NOINLINE_SECTION(".llvm_target2")
abi_u32 llvm_target2(abi_u16 a, abi_u16 b) {
  return 0x24680000UL ^ ((abi_u32)a << 16) ^ b;
}

NOINLINE_SECTION(".llvm_target5")
abi_u32 llvm_target5(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                     abi_u16 e) {
  abi_u32 value = 0x5a5a0000UL;
  value += a;
  value ^= (abi_u32)b << 16;
  value += (abi_u32)c << 1;
  value ^= d;
  value += (abi_u32)e << 16;
  return value;
}

NOINLINE_SECTION(".llvm_call0")
abi_u32 llvm_call0(abi_fn0 volatile *slot, abi_fn0 target) {
  abi_fn0 loaded;
  *slot = target;
  loaded = *slot;
  return loaded() ^ (loaded == target ? 0UL : 0xffffffffUL);
}

NOINLINE_SECTION(".llvm_call2")
abi_u32 llvm_call2(abi_fn2 volatile *slot, abi_fn2 target, abi_u16 a,
                   abi_u16 b) {
  abi_fn2 loaded;
  *slot = target;
  loaded = *slot;
  return loaded(a, b) ^ (loaded == target ? 0UL : 0xffffffffUL);
}

NOINLINE_SECTION(".llvm_call5")
abi_u32 llvm_call5(abi_fn5 volatile *slot, abi_fn5 target, abi_u16 a,
                   abi_u16 b, abi_u16 c, abi_u16 d, abi_u16 e) {
  abi_fn5 loaded;
  *slot = target;
  loaded = *slot;
  return loaded(a, b, c, d, e) ^
         (loaded == target ? 0UL : 0xffffffffUL);
}

NOINLINE_SECTION(".llvm_get0") abi_fn0 llvm_get0(void) {
  return llvm_target0;
}

NOINLINE_SECTION(".llvm_get2") abi_fn2 llvm_get2(void) {
  return llvm_target2;
}

NOINLINE_SECTION(".llvm_get5") abi_fn5 llvm_get5(void) {
  return llvm_target5;
}
