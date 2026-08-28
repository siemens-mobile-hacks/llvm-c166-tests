#include "types.h"

#pragma fragment

abi_u16 ABI_HUGE_FUNCTION tasking_medium_huge_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0xa55aU) +
                   (abi_u16)(b ^ (abi_u16)(a >> 1)));
}
