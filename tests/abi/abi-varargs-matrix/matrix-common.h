#ifndef ABI_VARARGS_MATRIX_COMMON_H
#define ABI_VARARGS_MATRIX_COMMON_H

#include <stdarg.h>
#include "types.h"

#define FIXED1(f0) ((abi_u32)(f0))
#define FIXED2(f0, f1) (FIXED1(f0) + (abi_u32)(f1) * 3UL)
#define FIXED3(f0, f1, f2) (FIXED2(f0, f1) + (abi_u32)(f2) * 5UL)
#define FIXED4(f0, f1, f2, f3) \
  (FIXED3(f0, f1, f2) + (abi_u32)(f3) * 7UL)
#define FIXED5(f0, f1, f2, f3, f4) \
  (FIXED4(f0, f1, f2, f3) + (abi_u32)(f4) * 11UL)

#define STREAM_PREFIX0 0x0000U
#define STREAM_PREFIX1 0x0001U
#define STREAM_PREFIX2 0x7fffU
#define STREAM_PREFIX3 0x8000U
#define STREAM_PREFIX4 0xffffU
#define STREAM_PREFIX5 0x1234U
#define STREAM_PREFIX6 0xa5a5U
#define STREAM_LONG0 0x1234abcdUL
#define STREAM_WORD0 0xbeefU
#define STREAM_LONG1 0x8000ffffUL
#define STREAM_WORD1 0x1357U
#define STREAM_LONG_LONG ((abi_ull)0xa5a55a5aUL)
#define STREAM_TAIL 0x2468U

static abi_u32 stream_rotate_left(abi_u32 value, abi_u16 count) {
  return (value << count) | (value >> (32U - count));
}

#define VARARGS_STREAM_PAYLOAD(pointer_value) \
  STREAM_LONG0, STREAM_WORD0, STREAM_LONG1, STREAM_WORD1, \
  STREAM_LONG_LONG, (pointer_value), STREAM_TAIL

#define CALL_VARARGS_STREAM(function, prefix_count, pointer_value, result) \
  do { \
    switch (prefix_count) { \
    case 0: \
      (result) = (function)(0U, VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    case 1: \
      (result) = (function)(1U, STREAM_PREFIX0, \
                            VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    case 2: \
      (result) = (function)(2U, STREAM_PREFIX0, STREAM_PREFIX1, \
                            VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    case 3: \
      (result) = (function)(3U, STREAM_PREFIX0, STREAM_PREFIX1, \
                            STREAM_PREFIX2, \
                            VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    case 4: \
      (result) = (function)(4U, STREAM_PREFIX0, STREAM_PREFIX1, \
                            STREAM_PREFIX2, STREAM_PREFIX3, \
                            VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    case 5: \
      (result) = (function)(5U, STREAM_PREFIX0, STREAM_PREFIX1, \
                            STREAM_PREFIX2, STREAM_PREFIX3, STREAM_PREFIX4, \
                            VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    case 6: \
      (result) = (function)(6U, STREAM_PREFIX0, STREAM_PREFIX1, \
                            STREAM_PREFIX2, STREAM_PREFIX3, STREAM_PREFIX4, \
                            STREAM_PREFIX5, \
                            VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    default: \
      (result) = (function)(7U, STREAM_PREFIX0, STREAM_PREFIX1, \
                            STREAM_PREFIX2, STREAM_PREFIX3, STREAM_PREFIX4, \
                            STREAM_PREFIX5, STREAM_PREFIX6, \
                            VARARGS_STREAM_PAYLOAD(pointer_value)); \
      break; \
    } \
  } while (0)

#define VARARGS_BODY(last_fixed, fixed_mix) \
  va_list arguments; \
  abi_s16 signed_value; \
  abi_u16 unsigned_byte; \
  abi_u16 enum_value; \
  abi_u16 word_value; \
  abi_u32 long_value; \
  abi_ull long_long_value; \
  volatile abi_u16 *pointer_value; \
  abi_u32 result; \
  va_start(arguments, last_fixed); \
  signed_value = va_arg(arguments, signed int); \
  unsigned_byte = (abi_u16)va_arg(arguments, signed int); \
  enum_value = (abi_u16)va_arg(arguments, signed int); \
  word_value = va_arg(arguments, unsigned int); \
  long_value = va_arg(arguments, unsigned long); \
  long_long_value = va_arg(arguments, unsigned long long); \
  pointer_value = va_arg(arguments, volatile unsigned int *); \
  va_end(arguments); \
  result = long_value + (fixed_mix) + signed_value; \
  result ^= (abi_u32)unsigned_byte << 8; \
  result += (abi_u32)enum_value * 0x101UL; \
  result ^= (abi_u32)word_value << 16; \
  result += *pointer_value; \
  result ^= (abi_u32)long_long_value; \
  result += ((abi_u32)long_long_value << 7) | \
            ((abi_u32)long_long_value >> 25); \
  return result

#define VARARGS_STREAM_BODY(last_fixed) \
  va_list arguments; \
  abi_u16 index; \
  abi_u16 word_value; \
  abi_u32 long0; \
  abi_u32 long1; \
  abi_ull long_long_value; \
  volatile abi_u16 *pointer_value; \
  abi_u16 tail; \
  abi_u32 result; \
  va_start(arguments, last_fixed); \
  result = 0x6d2b79f5UL ^ (abi_u32)(last_fixed); \
  for (index = 0; index < (last_fixed); ++index) { \
    word_value = va_arg(arguments, unsigned int); \
    result = stream_rotate_left(result, 5U); \
    result ^= (abi_u32)word_value + (abi_u32)index * 0x0101UL; \
  } \
  long0 = va_arg(arguments, unsigned long); \
  word_value = va_arg(arguments, unsigned int); \
  long1 = va_arg(arguments, unsigned long); \
  tail = va_arg(arguments, unsigned int); \
  long_long_value = va_arg(arguments, unsigned long long); \
  pointer_value = va_arg(arguments, volatile unsigned int *); \
  index = va_arg(arguments, unsigned int); \
  va_end(arguments); \
  result += long0; \
  result ^= (abi_u32)word_value << 16; \
  result = stream_rotate_left(result, 7U) + long1; \
  result ^= (abi_u32)tail; \
  result += (abi_u32)long_long_value; \
  result ^= stream_rotate_left((abi_u32)long_long_value, 11U); \
  result += *pointer_value; \
  result ^= ((abi_u32)index << 16) | (abi_u32)(last_fixed); \
  return result

#endif
