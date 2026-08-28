#include "types.h"

#define NOINLINE_SECTION(name) __attribute__((noinline, section(name)))

NOINLINE_SECTION(".medium.near.target")
abi_u16 llvm_medium_near_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x2468U) ^
                   (abi_u16)(b + (abi_u16)(a << 2)));
}

abi_u16 ABI_HUGE_FUNCTION
    NOINLINE_SECTION(".medium.huge.target")
    llvm_medium_huge_target(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0x5aa5U) +
                   (abi_u16)(b ^ (abi_u16)(a >> 2)));
}

NOINLINE_SECTION(".medium.near.dispatch")
abi_u32 llvm_medium_dispatch(abi_near_fn2 near_target,
                             abi_huge_fn2 huge_target,
                             abi_u16 a, abi_u16 b) {
  abi_u16 direct_near = tasking_medium_near_target(a, b);
  abi_u16 indirect_near = near_target(a, b);
  abi_u16 direct_huge = tasking_medium_huge_target(a, b);
  abi_u16 indirect_huge = huge_target(a, b);

  return (abi_u32)(direct_near ^ (abi_u16)(indirect_near + 0x1111U)) |
         ((abi_u32)(direct_huge + (abi_u16)(indirect_huge ^ 0x2222U))
          << 16);
}
