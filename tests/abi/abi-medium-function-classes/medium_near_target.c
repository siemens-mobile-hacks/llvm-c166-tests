#include "types.h"

#pragma fragment

abi_u16 tasking_medium_near_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x1357U) ^
                   (abi_u16)(b + (abi_u16)(a << 1)));
}
