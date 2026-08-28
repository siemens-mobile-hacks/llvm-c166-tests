#ifndef ABI_AGGREGATE_RETURN_HASH_COMMON_H
#define ABI_AGGREGATE_RETURN_HASH_COMMON_H

#define ROTATE5(value) \
  (abi_u16)(((abi_u16)(value) << 5) | ((abi_u16)(value) >> 11))

#define MIX_VALUE(value, size) \
  do { \
    for (index = 0; index < size; ++index) { \
      result = ROTATE5(result); \
      result ^= value.bytes[index]; \
      result ^= ((abi_u16)size << 8) | index; \
    } \
  } while (0)

#define TAIL_ARGUMENTS(seed) \
  (abi_u16)((seed) + 0x0101U), (abi_u16)((seed) ^ 0x2345U), \
  (abi_u16)((seed) + 0x4567U), (abi_u16)((seed) ^ 0x6789U), \
  (abi_u16)((seed) + 0x89abU), (abi_u16)((seed) ^ 0xabcdU), \
  (abi_u16)((seed) + 0xcdefU), (abi_u16)((seed) ^ 0xfedcU)

#endif
