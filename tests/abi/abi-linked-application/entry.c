#include "types.h"

extern const linked_operation linked_operations[3];
extern abi_u16 linked_common;
extern abi_u16 linked_marker;
extern abi_u16 linked_common_observe(abi_u16 value);
extern abi_u16 linked_host_callback(abi_u16 value, abi_u16 state);

__attribute__((noinline, section(".linked.text")))
abi_u32 linked_application(abi_u16 seed) {
  abi_u16 value = (abi_u16)(seed ^ linked_marker);
  abi_u16 index;

  linked_common = linked_initial_state(seed);
  for (index = 0; index != 3; ++index)
    value = linked_operations[index](value);
  value = linked_common_observe(value);
  value = linked_host_callback(value, linked_common);
  return ((abi_u32)linked_common << 16) | value;
}
