#ifndef CONTROL_FLOW_MATRIX_OPERATIONS_H
#define CONTROL_FLOW_MATRIX_OPERATIONS_H

#include "types.h"

#ifdef CONTROL_FLOW_LLVM
extern abi_u32 llvm_control_padding_step(abi_u32);
#define CONTROL_FLOW_PAD1(value) value = llvm_control_padding_step(value);
#define CONTROL_FLOW_PAD2(value) CONTROL_FLOW_PAD1(value) CONTROL_FLOW_PAD1(value)
#define CONTROL_FLOW_PAD4(value) CONTROL_FLOW_PAD2(value) CONTROL_FLOW_PAD2(value)
#define CONTROL_FLOW_PAD8(value) CONTROL_FLOW_PAD4(value) CONTROL_FLOW_PAD4(value)
#define CONTROL_FLOW_PAD16(value) CONTROL_FLOW_PAD8(value) CONTROL_FLOW_PAD8(value)
#define CONTROL_FLOW_PAD32(value) CONTROL_FLOW_PAD16(value) CONTROL_FLOW_PAD16(value)
#define CONTROL_FLOW_PAD64(value) CONTROL_FLOW_PAD32(value) CONTROL_FLOW_PAD32(value)
#define CONTROL_FLOW_PADDING(value)                                        \
  do { CONTROL_FLOW_PAD64(value) CONTROL_FLOW_PAD16(value) } while (0)
#else
#define CONTROL_FLOW_PADDING(value) do { (void)(value); } while (0)
#endif

#define CONTROL_ROL32(value, count)                                        \
  (((value) << (count)) | ((value) >> (32U - (count))))

#define DEFINE_CONTROL_FLOW_EVAL(name, attributes)                         \
  attributes abi_u32 name(abi_u16 operation, abi_u32 a, abi_u32 b,         \
                          abi_u16 count) {                                  \
    volatile abi_u16 hits = 0U;                                            \
    abi_u32 accumulator;                                                   \
    abi_u32 minimum;                                                       \
    abi_u32 maximum;                                                       \
    abi_u16 index;                                                         \
    abi_u16 iterations = (abi_u16)(count & 31U);                           \
    abi_s16 low_a = (abi_s16)(abi_u16)a;                                   \
    abi_s32 sa = (abi_s32)a;                                               \
    abi_s32 sb = (abi_s32)b;                                               \
    switch (operation) {                                                   \
    case 0:                                                               \
      if (a == b) return 0x10000000UL ^ a;                                 \
      return 0x20000000UL ^ b;                                             \
    case 1:                                                               \
      if (a != b) return 0x30000000UL ^ a;                                 \
      return 0x40000000UL ^ b;                                             \
    case 2:                                                               \
      if (a < b) return 0x50000000UL ^ a;                                  \
      return 0x60000000UL ^ b;                                             \
    case 3:                                                               \
      if (a <= b) return 0x70000000UL ^ a;                                 \
      return 0x71000000UL ^ b;                                             \
    case 4:                                                               \
      if (a > b) return 0x72000000UL ^ a;                                  \
      return 0x73000000UL ^ b;                                             \
    case 5:                                                               \
      if (a >= b) return 0x74000000UL ^ a;                                 \
      return 0x75000000UL ^ b;                                             \
    case 6:                                                               \
      if (sa < sb) return 0x76000000UL ^ a;                                \
      return 0x77000000UL ^ b;                                             \
    case 7:                                                               \
      if (sa <= sb) return 0x78000000UL ^ a;                               \
      return 0x79000000UL ^ b;                                             \
    case 8:                                                               \
      if (sa > sb) return 0x7a000000UL ^ a;                                \
      return 0x7b000000UL ^ b;                                             \
    case 9:                                                               \
      if (sa >= sb) return 0x7c000000UL ^ a;                               \
      return 0x7d000000UL ^ b;                                             \
    case 10:                                                              \
      if ((abi_u16)a != 0U && (++hits, (abi_u16)b != 0U))                  \
        accumulator = 0xa1000001UL;                                        \
      else                                                                \
        accumulator = 0xa2000002UL;                                        \
      return accumulator ^ ((abi_u32)hits << 16);                          \
    case 11:                                                              \
      if ((abi_u16)a != 0U || (++hits, (abi_u16)b != 0U))                  \
        accumulator = 0xa3000003UL;                                        \
      else                                                                \
        accumulator = 0xa4000004UL;                                        \
      return accumulator ^ ((abi_u32)hits << 16);                          \
    case 12:                                                              \
      if ((abi_u16)a != 0U && (++hits, (abi_u16)b != 0U) &&                \
          (++hits, iterations != 0U))                                      \
        accumulator = 0xa5000005UL;                                        \
      else                                                                \
        accumulator = 0xa6000006UL;                                        \
      return accumulator ^ ((abi_u32)hits << 16);                          \
    case 13:                                                              \
      return a < b ? (iterations ? a ^ 0xb1000001UL                       \
                                 : b ^ 0xb2000002UL)                       \
                   : (iterations ? a ^ 0xb3000003UL                       \
                                 : b ^ 0xb4000004UL);                      \
    case 14:                                                              \
      minimum = sa < sb ? a : b;                                           \
      maximum = sa > sb ? a : b;                                           \
      return minimum ^ CONTROL_ROL32(maximum, 7U);                         \
    case 15:                                                              \
      accumulator = 0x13579bdfUL;                                          \
      for (index = 0U; index < iterations; ++index) {                      \
        accumulator = CONTROL_ROL32(accumulator, 5U) ^                    \
                      (a + ((abi_u32)index << 16) + index);                 \
      }                                                                    \
      return accumulator;                                                  \
    case 16:                                                              \
      accumulator = 0x2468ace0UL;                                          \
      index = 0U;                                                          \
      while (index < iterations) {                                         \
        accumulator = CONTROL_ROL32(accumulator, 3U) + (b ^ index);        \
        ++index;                                                           \
      }                                                                    \
      return accumulator;                                                  \
    case 17:                                                              \
      accumulator = 0x0f0ff0f0UL;                                         \
      index = 0U;                                                          \
      do {                                                                 \
        accumulator = CONTROL_ROL32(accumulator, 7U) ^                    \
                      (a + b + index);                                      \
        ++index;                                                           \
      } while (index < iterations);                                        \
      return accumulator;                                                  \
    case 18:                                                              \
      switch (low_a) {                                                     \
      case -2: return 0xd0020002UL;                                        \
      case -1: return 0xd0010001UL;                                        \
      case 0: return 0xd0000000UL;                                         \
      case 1: return 0xd1010001UL;                                         \
      case 2: return 0xd2020002UL;                                         \
      case 3: return 0xd3030003UL;                                         \
      case 4: return 0xd4040004UL;                                         \
      default: return 0xdfff00ffUL;                                        \
      }                                                                    \
    case 19:                                                              \
      switch (low_a) {                                                     \
      case -32768: return 0xe0008000UL;                                    \
      case -257: return 0xe000feffUL;                                      \
      case 0: return 0xe0000000UL;                                         \
      case 257: return 0xe0000101UL;                                       \
      case 32767: return 0xe0007fffUL;                                     \
      default: return 0xefff00ffUL;                                        \
      }                                                                    \
    case 20:                                                              \
      switch (a) {                                                         \
      case 0x00000000UL: return 0xf0000000UL;                              \
      case 0x00010000UL: return 0xf0010000UL;                              \
      case 0x7fffffffUL: return 0xf07fffffUL;                              \
      case 0x80000000UL: return 0xf0800000UL;                              \
      case 0xffffffffUL: return 0xf0ffffffUL;                              \
      default: return 0xf0abcdefUL;                                        \
      }                                                                    \
    case 21:                                                              \
      accumulator = 0x10203040UL;                                          \
      for (index = 0U; index < iterations; ++index) {                      \
        if ((index & 3U) == 1U)                                            \
          continue;                                                        \
        accumulator += a ^ index;                                          \
        if ((index & 7U) == 6U)                                            \
          break;                                                           \
      }                                                                    \
      return accumulator;                                                  \
    case 22:                                                              \
      if ((abi_u16)a == 0U) {                                              \
        accumulator = 0xc1000001UL ^ b;                                    \
        CONTROL_FLOW_PADDING(accumulator);                                 \
        return accumulator;                                                \
      }                                                                    \
      return 0xc2000002UL ^ a;                                             \
    default:                                                              \
      if (sa < sb) {                                                       \
        if ((iterations & 1U) != 0U) return a ^ 0x01010101UL;              \
        return b ^ 0x02020202UL;                                           \
      }                                                                    \
      if (a == b) return a ^ 0x03030303UL;                                 \
      if ((iterations & 1U) != 0U) return b ^ 0x04040404UL;                \
      return a ^ 0x05050505UL;                                             \
    }                                                                      \
  }

#endif
