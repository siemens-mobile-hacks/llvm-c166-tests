#include "types.h"

__attribute__((noinline, section(".linked.text")))
abi_u16 linked_selected_transform(abi_u16 value) {
  return linked_selected_expected(value);
}
