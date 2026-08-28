#ifndef ABI_WORD_MATRIX_FUNCTIONS_H
#define ABI_WORD_MATRIX_FUNCTIONS_H

#include "types.h"

#define WORD_INIT(count) \
  abi_u32 value = 0xa5a50000UL ^ ((abi_u32)(count) * 0x01010101UL)
#define WORD_STEP(argument, left, right) \
  do { \
    value += (abi_u32)(argument) << (left); \
    value ^= (abi_u32)(argument) << (right); \
  } while (0)

#define DEFINE_WORD_FUNCTIONS(prefix, A0, A1, A2, A3, A4, A5, A6, A7, A8, \
                              A9, A10) \
  A0 abi_u32 prefix##0(void) { \
    WORD_INIT(0); \
    return value; \
  } \
  A1 abi_u32 prefix##1(abi_u16 a0) { \
    WORD_INIT(1); WORD_STEP(a0, 1, 15); return value; \
  } \
  A2 abi_u32 prefix##2(abi_u16 a0, abi_u16 a1) { \
    WORD_INIT(2); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); return value; \
  } \
  A3 abi_u32 prefix##3(abi_u16 a0, abi_u16 a1, abi_u16 a2) { \
    WORD_INIT(3); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); return value; \
  } \
  A4 abi_u32 prefix##4(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3) { \
    WORD_INIT(4); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); WORD_STEP(a3, 4, 12); return value; \
  } \
  A5 abi_u32 prefix##5(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                        abi_u16 a4) { \
    WORD_INIT(5); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); WORD_STEP(a3, 4, 12); WORD_STEP(a4, 5, 11); \
    return value; \
  } \
  A6 abi_u32 prefix##6(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                        abi_u16 a4, abi_u16 a5) { \
    WORD_INIT(6); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); WORD_STEP(a3, 4, 12); WORD_STEP(a4, 5, 11); \
    WORD_STEP(a5, 6, 10); return value; \
  } \
  A7 abi_u32 prefix##7(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                        abi_u16 a4, abi_u16 a5, abi_u16 a6) { \
    WORD_INIT(7); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); WORD_STEP(a3, 4, 12); WORD_STEP(a4, 5, 11); \
    WORD_STEP(a5, 6, 10); WORD_STEP(a6, 7, 9); return value; \
  } \
  A8 abi_u32 prefix##8(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                        abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7) { \
    WORD_INIT(8); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); WORD_STEP(a3, 4, 12); WORD_STEP(a4, 5, 11); \
    WORD_STEP(a5, 6, 10); WORD_STEP(a6, 7, 9); WORD_STEP(a7, 8, 8); \
    return value; \
  } \
  A9 abi_u32 prefix##9(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                        abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7, \
                        abi_u16 a8) { \
    WORD_INIT(9); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); WORD_STEP(a3, 4, 12); WORD_STEP(a4, 5, 11); \
    WORD_STEP(a5, 6, 10); WORD_STEP(a6, 7, 9); WORD_STEP(a7, 8, 8); \
    WORD_STEP(a8, 9, 7); return value; \
  } \
  A10 abi_u32 prefix##10(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                          abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7, \
                          abi_u16 a8, abi_u16 a9) { \
    WORD_INIT(10); WORD_STEP(a0, 1, 15); WORD_STEP(a1, 2, 14); \
    WORD_STEP(a2, 3, 13); WORD_STEP(a3, 4, 12); WORD_STEP(a4, 5, 11); \
    WORD_STEP(a5, 6, 10); WORD_STEP(a6, 7, 9); WORD_STEP(a7, 8, 8); \
    WORD_STEP(a8, 9, 7); WORD_STEP(a9, 10, 6); return value; \
  }

#endif
