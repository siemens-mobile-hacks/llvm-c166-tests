#include "types.h"

__attribute__((noinline, section(".linked.text")))
abi_u16 linked_archive_transform(abi_u16 value) {
  return linked_archive_expected(value);
}

__attribute__((section(".linked.dead.data")))
abi_u16 linked_dead_data = 0xbad0U;

__attribute__((noinline, section(".linked.dead.text")))
abi_u16 linked_dead_function(abi_u16 value) {
  return (abi_u16)(value + linked_dead_data);
}
