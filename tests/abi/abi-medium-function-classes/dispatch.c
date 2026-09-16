#include "types.h"

C166_NOINLINE C166_SECTION(".medium.near.dispatch")
abi_u32 c166_medium_dispatch(abi_near_fn2 near_target,
                             abi_huge_fn2 huge_target, abi_u16 a,
                             abi_u16 b) {
  abi_u16 direct_near = c166_medium_near_callback(a, b);
  abi_u16 indirect_near = near_target(a, b);
  abi_u16 direct_huge = c166_medium_huge_callback(a, b);
  abi_u16 indirect_huge = huge_target(a, b);

  return (abi_u32)(direct_near ^ (abi_u16)(indirect_near + 0x1111U)) |
         ((abi_u32)(direct_huge + (abi_u16)(indirect_huge ^ 0x2222U))
          << 16);
}
