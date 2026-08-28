#include "types.h"

volatile abi_u16 far_words[4] = {0x1111U, 0x2222U, 0x3333U, 0x4444U};

abi_u16 pointer_target(abi_u16 first, abi_u16 second) {
  return (abi_u16)((first ^ second) + 0x1234U);
}

abi_u16 pointer_probe(abi_function function, volatile abi_u16 *address,
                      abi_u16 tail) {
  abi_u16 indirect;

  if (address != far_words || tail != 0x55aaU)
    return 1U;
  if (address[0] != 0x1111U || address[3] != 0x4444U)
    return 2U;
  address[2] = 0x5a5aU;
  if (*(address + 2) != 0x5a5aU)
    return 3U;
  indirect = function(0x1357U, 0x2468U);
  if (indirect != (abi_u16)((0x1357U ^ 0x2468U) + 0x1234U))
    return 4U;
  return 42U;
}
