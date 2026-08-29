#ifndef INTEGER32_MATRIX_OPERATIONS_H
#define INTEGER32_MATRIX_OPERATIONS_H

#include "types.h"

#define INTEGER32_HELPER_CASES                                              \
    case 7: return a * b;                                                   \
    case 8: return a / b;                                                   \
    case 9: return a % b;                                                   \
    case 10: return (abi_u32)((abi_s32)slow_a * (abi_s32)slow_b);           \
    case 11: return (abi_u32)(sa / sb);                                     \
    case 12: return (abi_u32)(sa % sb);

#define DEFINE_INTEGER32_EVAL(name, attributes)                            \
  attributes abi_u32 name(abi_u16 operation, abi_u32 a, abi_u32 b,         \
                          abi_u16 count) {                                  \
    abi_s32 sa = (abi_s32)a;                                                \
    abi_s32 sb = (abi_s32)b;                                                \
    abi_u16 low_a = (abi_u16)a;                                             \
    abi_u16 low_b = (abi_u16)b;                                             \
    abi_s16 slow_a = (abi_s16)low_a;                                        \
    abi_s16 slow_b = (abi_s16)low_b;                                        \
    switch (operation) {                                                    \
    case 0: return a + b;                                                   \
    case 1: return a - b;                                                   \
    case 2: return 0UL - a;                                                 \
    case 3: return a & b;                                                   \
    case 4: return a | b;                                                   \
    case 5: return a ^ b;                                                   \
    case 6: return ~a;                                                      \
    INTEGER32_HELPER_CASES                                                  \
    case 13: return a << count;                                             \
    case 14: return a >> count;                                             \
    case 15: return (abi_u32)(sa >> count);                                 \
    case 16: return (abi_u32)(a == b);                                      \
    case 17: return (abi_u32)(a != b);                                      \
    case 18: return (abi_u32)(a < b);                                       \
    case 19: return (abi_u32)(a <= b);                                      \
    case 20: return (abi_u32)(a > b);                                       \
    case 21: return (abi_u32)(a >= b);                                      \
    case 22: return (abi_u32)(sa < sb);                                     \
    case 23: return (abi_u32)(sa <= sb);                                    \
    case 24: return (abi_u32)(sa > sb);                                     \
    case 25: return (abi_u32)(sa >= sb);                                    \
    case 26: return (abi_u32)low_a;                                         \
    case 27: return a >> 16;                                                \
    case 28: return (abi_u32)(abi_u16)a;                                    \
    case 29: return (abi_u32)(abi_s32)(abi_s16)(abi_u16)a;                  \
    case 30: return (a & 0xffff0000UL) | (abi_u32)low_b;                    \
    case 31: return (abi_u32)low_a | ((abi_u32)low_b << 16);                \
    case 32: return (abi_u32)((a + b) < a);                                 \
    case 33: return (abi_u32)(a < b);                                       \
    case 34: return sa < sb ? a : b;                                        \
    default: return a > b ? a : b;                                          \
    }                                                                       \
  }

#endif
