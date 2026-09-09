#include "types.h"

abi_u16 linked_common;

__attribute__((noinline, section(".linked.text")))
abi_u16 linked_common_update(abi_u16 value) {
  linked_common = linked_common_expected(linked_common, value);
  return linked_common;
}
