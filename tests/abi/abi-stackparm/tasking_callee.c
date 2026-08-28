#include "types.h"

abi_u16 ABI_STACKPARM tasking_stackparm_callee(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d) {
  return (abi_u16)((abi_u16)a + (abi_u16)(b * 3U) + (abi_u16)c +
                   (abi_u16)((abi_u16)(c >> 16) * 5U) +
                   (abi_u16)(*p * 7U) + (abi_u16)(pair.lo * 11U) +
                   (abi_u16)(pair.hi * 13U) + (abi_u16)(d * 17U));
}
