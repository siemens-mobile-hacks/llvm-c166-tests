#include "types.h"

C166_NOINLINE C166_SECTION(".abi.pressure.forward.words")
abi_u16 pressure_words_forward_alternate(
    abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
    abi_u16 a6, abi_u16 a7, abi_u16 selector) {
  return pressure_words_alternate(a0, a1, a2, a3, a4, a5, a6, a7, selector);
}

C166_NOINLINE C166_SECTION(".abi.pressure.forward.longs")
abi_u32 pressure_longs_forward_alternate(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                                         abi_u32 a3, abi_u32 a4, abi_u32 a5,
                                         abi_u16 selector) {
  return pressure_longs_alternate(a0, a1, a2, a3, a4, a5, selector);
}
