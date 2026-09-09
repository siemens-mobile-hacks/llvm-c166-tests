#include "types.h"
#include "callback-state.inc"

#pragma fragment

abi_aggregate_callback volatile aggregate_callback;
volatile struct abi_live_state aggregate_live;

struct shape1 tasking_return1(abi_u16 seed) {
  struct shape1 value;
  value.bytes[0] = (abi_u8)(seed + 0x11U);
  return value;
}

struct shape2 tasking_return2(abi_u16 seed) {
  struct shape2 value;
  abi_u16 index;
  for (index = 0; index < 2; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x22U + index * 0x23U);
  return value;
}

struct shape3 tasking_return3(abi_u16 seed) {
  struct shape3 value;
  abi_u16 index;
	mutate_callback_state();
  for (index = 0; index < 3; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x33U + index * 0x23U);
  return value;
}

struct shape4 tasking_return4(abi_u16 seed) {
  struct shape4 value;
  abi_u16 index;
  for (index = 0; index < 4; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x44U + index * 0x23U);
  return value;
}

struct shape5 tasking_return5(abi_u16 seed) {
  struct shape5 value;
  abi_u16 index;
  for (index = 0; index < 5; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x55U + index * 0x23U);
  return value;
}

struct shape6 tasking_return6(abi_u16 seed) {
  struct shape6 value;
  abi_u16 index;
  for (index = 0; index < 6; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x66U + index * 0x23U);
  return value;
}

struct shape7 tasking_return7(abi_u16 seed) {
  struct shape7 value;
  abi_u16 index;
  for (index = 0; index < 7; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x77U + index * 0x23U);
  return value;
}

struct shape8 tasking_return8(abi_u16 seed) {
  struct shape8 value;
  abi_u16 index;
  for (index = 0; index < 8; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x88U + index * 0x23U);
  return value;
}

struct shape8 tasking_return_tail(
    abi_u16 first, abi_u16 second, abi_u16 third, abi_u16 fourth,
    abi_u16 fifth, abi_u16 sixth, abi_u16 seventh, abi_u16 eighth) {
#include "tail.inc"
}
