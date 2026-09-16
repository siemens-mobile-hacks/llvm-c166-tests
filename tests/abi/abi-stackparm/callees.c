#include "types.h"

C166_NOINLINE C166_SECTION(".abi.stackparm.callee")
abi_u16 C166_STACKPARM stackparm_callee(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 C166_FAR *p,
    struct abi_stackparm_pair pair, abi_u16 d) {
  return (abi_u16)((abi_u16)(a * 19U) + b +
                   (abi_u16)((abi_u16)c * 3U) +
                   (abi_u16)((abi_u16)(c >> 16) * 7U) +
                   (abi_u16)(*p * 5U) + pair.lo +
                   (abi_u16)(pair.hi * 11U) + (abi_u16)(d * 13U));
}

C166_NOINLINE C166_SECTION(".abi.stackparm.float-callee")
float C166_STACKPARM stackparm_float_callee(float value, abi_u16 tail) {
  (void)tail;
  return value;
}

C166_NOINLINE C166_SECTION(".abi.stackparm.double-callee")
double C166_STACKPARM stackparm_double_callee(double value, abi_u16 tail) {
  (void)tail;
  return value;
}
