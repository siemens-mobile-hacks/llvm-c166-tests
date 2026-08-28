#include "types.h"

#pragma fragment

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
  struct shape8 value;
  abi_u16 word0 = first + fifth;
  abi_u16 word1 = second + sixth;
  abi_u16 word2 = third + seventh;
  abi_u16 word3 = fourth + eighth;
  value.bytes[0] = (abi_u8)word0;
  value.bytes[1] = (abi_u8)(word0 >> 8);
  value.bytes[2] = (abi_u8)word1;
  value.bytes[3] = (abi_u8)(word1 >> 8);
  value.bytes[4] = (abi_u8)word2;
  value.bytes[5] = (abi_u8)(word2 >> 8);
  value.bytes[6] = (abi_u8)word3;
  value.bytes[7] = (abi_u8)(word3 >> 8);
  return value;
}
