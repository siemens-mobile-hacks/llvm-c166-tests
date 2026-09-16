#ifndef ABI_REGISTER_PRESSURE_TYPES_H
#define ABI_REGISTER_PRESSURE_TYPES_H

#include "c166_test.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

abi_u16 pressure_selector_primary(abi_u16 selector);
abi_u16 pressure_selector_alternate(abi_u16 selector);

#define DECLARE_PRESSURE_PAIR(suffix)                                       \
  abi_u16 pressure_words_##suffix(                                          \
      abi_u16, abi_u16, abi_u16, abi_u16, abi_u16, abi_u16, abi_u16,       \
      abi_u16, abi_u16);                                                    \
  abi_u32 pressure_longs_##suffix(abi_u32, abi_u32, abi_u32, abi_u32,      \
                                  abi_u32, abi_u32, abi_u16)

DECLARE_PRESSURE_PAIR(primary);
DECLARE_PRESSURE_PAIR(alternate);
DECLARE_PRESSURE_PAIR(primary_alternate_leaf);
DECLARE_PRESSURE_PAIR(alternate_primary_leaf);
DECLARE_PRESSURE_PAIR(forward_alternate);

#undef DECLARE_PRESSURE_PAIR

#endif
