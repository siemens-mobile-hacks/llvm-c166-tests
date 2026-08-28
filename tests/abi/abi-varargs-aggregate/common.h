#ifndef ABI_VARARGS_AGGREGATE_COMMON_H
#define ABI_VARARGS_AGGREGATE_COMMON_H

#include <stdarg.h>
#include "types.h"

#define ABI_AGGREGATE_VARARGS_FIXED 0x1357U

#define ABI_INIT_AGGREGATES(seed, pair_value, chars_value, packed_value, tail) \
  do { \
    (pair_value).first = (abi_u16)((seed) ^ 0x1111U); \
    (pair_value).second = (abi_u16)((seed) + 0x2222U); \
    (chars_value).first = (abi_u8)((seed) + 0x01U); \
    (chars_value).second = (abi_u8)((seed) + 0x23U); \
    (chars_value).third = (abi_u8)((seed) + 0x45U); \
    (packed_value).first = (abi_u8)((seed) + 0x67U); \
    (packed_value).second = (abi_u16)((seed) ^ 0x8888U); \
    (tail) = (abi_u16)((seed) ^ 0xCAFEU); \
  } while (0)

#define ABI_AGGREGATE_VARARGS_BODY(last_fixed) \
  va_list arguments; \
  struct pair2 pair_value; \
  struct chars3 chars_value; \
  ABI_PACKED_REF(packed3) packed_value; \
  abi_u16 tail; \
  abi_u16 result; \
  va_start(arguments, last_fixed); \
  pair_value = va_arg(arguments, struct pair2); \
  chars_value = va_arg(arguments, struct chars3); \
  packed_value = va_arg(arguments, ABI_PACKED_REF(packed3)); \
  tail = va_arg(arguments, unsigned int); \
  va_end(arguments); \
  result = (abi_u16)((last_fixed) * 3U); \
  result = (abi_u16)(result + pair_value.first * 5U); \
  result = (abi_u16)(result + pair_value.second * 7U); \
  result = (abi_u16)(result + (abi_u16)chars_value.first * 11U); \
  result = (abi_u16)(result + (abi_u16)chars_value.second * 13U); \
  result = (abi_u16)(result + (abi_u16)chars_value.third * 17U); \
  result = (abi_u16)(result + (abi_u16)packed_value.first * 19U); \
  result = (abi_u16)(result + packed_value.second * 23U); \
  result = (abi_u16)(result + tail * 29U); \
  return result

#endif
