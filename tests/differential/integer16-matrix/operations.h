#ifndef INTEGER16_MATRIX_OPERATIONS_H
#define INTEGER16_MATRIX_OPERATIONS_H

#include "types.h"

#define DEFINE_INTEGER16_EVAL(name, attributes)                            \
  attributes abi_u32 name(abi_u16 operation, abi_u16 a, abi_u16 b,         \
                          abi_u16 count) {                                  \
    abi_s16 sa = (abi_s16)a;                                                \
    abi_s16 sb = (abi_s16)b;                                                \
    abi_u8 ua8 = (abi_u8)a;                                                 \
    abi_u8 ub8 = (abi_u8)b;                                                 \
    abi_s8 sa8 = (abi_s8)ua8;                                               \
    abi_s8 sb8 = (abi_s8)ub8;                                               \
    switch (operation) {                                                    \
    case 0: return (abi_u32)(abi_u16)(a + b);                               \
    case 1: return (abi_u32)(abi_u16)(a - b);                               \
    case 2: return (abi_u32)(abi_u16)(0U - a);                              \
    case 3: return (abi_u32)(a & b);                                        \
    case 4: return (abi_u32)(a | b);                                        \
    case 5: return (abi_u32)(a ^ b);                                        \
    case 6: return (abi_u32)(abi_u16)~a;                                    \
    case 7: return (abi_u32)(abi_u16)(a * b);                               \
    case 8: return (abi_u32)(a / b);                                        \
    case 9: return (abi_u32)(a % b);                                        \
    case 10: return (abi_u32)(abi_u16)(a << count);                         \
    case 11: return (abi_u32)(a >> count);                                  \
    case 12: return (abi_u32)(sa / sb);                                     \
    case 13: return (abi_u32)(sa % sb);                                     \
    case 14: return (abi_u32)(abi_s32)(sa >> count);                        \
    case 15: return (abi_u32)(a == b);                                      \
    case 16: return (abi_u32)(a != b);                                      \
    case 17: return (abi_u32)(a < b);                                       \
    case 18: return (abi_u32)(a <= b);                                      \
    case 19: return (abi_u32)(a > b);                                       \
    case 20: return (abi_u32)(a >= b);                                      \
    case 21: return (abi_u32)(sa < sb);                                     \
    case 22: return (abi_u32)(sa <= sb);                                    \
    case 23: return (abi_u32)(sa > sb);                                     \
    case 24: return (abi_u32)(sa >= sb);                                    \
    case 25: return (abi_u32)(ua8 + ub8);                                   \
    case 26: return (abi_u32)(abi_s32)(sa8 + sb8);                          \
    case 27: return (abi_u32)ua8;                                           \
    case 28: return (abi_u32)(abi_s32)sa8;                                  \
    case 29: return (abi_u32)(abi_u8)a;                                     \
    case 30: return (abi_u32)(abi_s32)(abi_s8)(abi_u8)a;                    \
    case 31: return (abi_u32)((a & 0xff00U) | ub8);                         \
    case 32: return (abi_u32)(abi_u8)(a >> 8);                              \
    case 33: return (abi_u32)(abi_s32)(abi_s8)(abi_u8)(a >> 8);             \
    case 34: return (abi_u32)(abi_s32)(sa8 * sb8);                          \
    case 35: return (abi_u32)(-(abi_s32)sa);                                \
    case 36: return (abi_u32)((abi_s32)sa + (abi_s32)sb);                   \
    case 37: return (abi_u32)((abi_s32)sa - (abi_s32)sb);                   \
    default: return (abi_u32)(abi_s32)(abi_s16)(                            \
        (abi_s16)((abi_s16)sa8 * (abi_s16)sb8) / 6 +                       \
        ((abi_s16)sa8 - (abi_s16)sb8));                                    \
    }                                                                       \
  }

#endif
