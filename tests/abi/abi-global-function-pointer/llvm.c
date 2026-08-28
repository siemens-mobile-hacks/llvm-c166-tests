#include "types.h"

#define LLVM_SECTION(name) __attribute__((noinline, section(name)))

abi_fn volatile llvm_global_slot
    __attribute__((section(".bss.llvm_global_slot"), used));

LLVM_SECTION(".llvm_target")
abi_u16 llvm_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)(a + 0x1357U) ^ b;
}

LLVM_SECTION(".llvm_exchange")
abi_fn llvm_global_exchange(abi_fn target) {
  llvm_global_slot = target;
  return llvm_global_slot;
}

LLVM_SECTION(".llvm_call_global")
abi_u16 llvm_call_global(abi_u16 a, abi_u16 b) {
  abi_fn loaded = llvm_global_slot;
  return loaded(a, b);
}

LLVM_SECTION(".llvm_roundtrip")
abi_u16 llvm_global_roundtrip(abi_fn target, abi_u16 a, abi_u16 b) {
  abi_fn loaded = llvm_global_exchange(target);
  abi_u16 result = llvm_call_global(a, b);
  if (loaded != target)
    result ^= 0xffffU;
  return result;
}

LLVM_SECTION(".llvm_get_target")
abi_fn llvm_get_target(void) {
  return llvm_target;
}

LLVM_SECTION(".llvm_reverse_roundtrip")
abi_u16 llvm_reverse_roundtrip(abi_fn target, abi_u16 a, abi_u16 b) {
  return tasking_global_roundtrip(target, a, b);
}

LLVM_SECTION(".llvm_reverse_exchange")
abi_fn llvm_reverse_exchange(abi_fn target) {
  return tasking_global_exchange(target);
}
