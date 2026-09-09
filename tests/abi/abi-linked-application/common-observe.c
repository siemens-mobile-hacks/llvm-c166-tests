#include "types.h"

abi_u16 linked_common;

__attribute__((noinline, section(".linked.text")))
abi_u16 linked_common_observe(abi_u16 value) {
  return linked_observe_expected(linked_common, value);
}
