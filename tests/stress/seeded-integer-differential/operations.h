#ifndef SEEDED_INTEGER_OPERATIONS_H
#define SEEDED_INTEGER_OPERATIONS_H

#include "types.h"
#include "expressions.inc"

#define ABI_SEEDED_CASE(id, name, expression) \
    case id: return (abi_u32)(expression);

#define DEFINE_SEEDED_INTEGER_EVAL(name, attributes)                       \
  attributes abi_u32 name(abi_u16 operation, abi_u32 a, abi_u32 b) {       \
    abi_u8 ua8 = (abi_u8)a;                                                 \
    abi_u8 ub8 = (abi_u8)b;                                                 \
    abi_s8 sa8 = (abi_s8)ua8;                                               \
    abi_s8 sb8 = (abi_s8)ub8;                                               \
    abi_u16 ua16 = (abi_u16)a;                                              \
    abi_u16 ub16 = (abi_u16)b;                                              \
    abi_s16 sa16 = (abi_s16)ua16;                                           \
    abi_s16 sb16 = (abi_s16)ub16;                                           \
    abi_s32 sa32 = (abi_s32)a;                                              \
    abi_s32 sb32 = (abi_s32)b;                                              \
    switch (operation) {                                                     \
      ABI_SEEDED_INTEGER_EXPRESSIONS(ABI_SEEDED_CASE)                       \
    default: return 0UL;                                                     \
    }                                                                        \
  }

#endif
