#include "types.h"

__attribute__((section(".linked.unused.data")))
abi_u16 linked_unused_data = 0xcafeU;

__attribute__((noinline, section(".linked.unused.text")))
abi_u16 linked_unused_archive(abi_u16 value) {
  return (abi_u16)(value ^ linked_unused_data);
}
