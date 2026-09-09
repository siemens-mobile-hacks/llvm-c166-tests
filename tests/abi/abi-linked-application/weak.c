#include "types.h"

__attribute__((section(".linked.weak.data")))
const abi_u16 linked_weak_cookie = 0xdeadU;

__attribute__((weak, noinline, section(".linked.weak.text")))
abi_u16 linked_selected_transform(abi_u16 value) {
  return (abi_u16)(value ^ linked_weak_cookie);
}
