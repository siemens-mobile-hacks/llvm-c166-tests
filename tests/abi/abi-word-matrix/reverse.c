#include "types.h"

C166_NOINLINE C166_SECTION(".abi.word.reverse")
abi_u32 reverse_words(abi_u16 shape, abi_u16 a0, abi_u16 a1, abi_u16 a2,
                      abi_u16 a3, abi_u16 a4, abi_u16 a5, abi_u16 a6,
                      abi_u16 a7, abi_u16 a8, abi_u16 a9) {
  switch (shape) {
  case 0:
    return words0();
  case 1:
    return words1(a0);
  case 2:
    return words2(a0, a1);
  case 3:
    return words3(a0, a1, a2);
  case 4:
    return words4(a0, a1, a2, a3);
  case 5:
    return words5(a0, a1, a2, a3, a4);
  case 6:
    return words6(a0, a1, a2, a3, a4, a5);
  case 7:
    return words7(a0, a1, a2, a3, a4, a5, a6);
  case 8:
    return words8(a0, a1, a2, a3, a4, a5, a6, a7);
  case 9:
    return words9(a0, a1, a2, a3, a4, a5, a6, a7, a8);
  default:
    return words10(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
}
