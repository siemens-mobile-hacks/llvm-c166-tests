#ifndef ABI_MIXED_SLOT_MATRIX_FUNCTIONS_H
#define ABI_MIXED_SLOT_MATRIX_FUNCTIONS_H

typedef signed char abi_s8;
typedef unsigned char abi_u8;
typedef signed long abi_s32;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;
typedef abi_u16 (*abi_callback)(abi_u16);

enum abi_enum {
  ABI_ENUM_0 = 0,
  ABI_ENUM_15 = 15
};

union abi_u32_words {
  abi_u32 whole;
  abi_u16 word[2];
};

#define ABI_MIX_RESULT(tag, p0, p1, p2, p3, value, tail) \
  do { \
    union abi_u32_words input; \
    union abi_u32_words result; \
    input.whole = (abi_u32)(value); \
    result.word[0] = (abi_u16)(tag) ^ ((abi_u16)(p0) << 1) ^ \
                     ((abi_u16)(p2) << 3) ^ input.word[0]; \
    result.word[1] = 0x6d2bU ^ (abi_u16)(tag) ^ (abi_u16)(p1) ^ \
                     ((abi_u16)(p3) << 8) ^ input.word[1]; \
    result.word[0] += (abi_u16)(tail); \
    result.word[1] += (abi_u16)(tail); \
    return result.whole; \
  } while (0)

#define ABI_MIXED_FUNCTION_LIST(X, prefix) \
  X(prefix, 0, s8_0, (abi_s8 value, abi_u16 tail), \
    ABI_MIX_RESULT(0, 0, 0, 0, 0, (abi_s32)value, tail)) \
  X(prefix, 1, s8_1, (abi_u16 p0, abi_s8 value, abi_u16 tail), \
    ABI_MIX_RESULT(1, p0, 0, 0, 0, (abi_s32)value, tail)) \
  X(prefix, 2, s8_2, \
    (abi_u16 p0, abi_u16 p1, abi_s8 value, abi_u16 tail), \
    ABI_MIX_RESULT(2, p0, p1, 0, 0, (abi_s32)value, tail)) \
  X(prefix, 3, s8_3, \
    (abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_s8 value, abi_u16 tail), \
    ABI_MIX_RESULT(3, p0, p1, p2, 0, (abi_s32)value, tail)) \
  X(prefix, 4, s8_4, \
    (abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3, abi_s8 value, \
     abi_u16 tail), \
    ABI_MIX_RESULT(4, p0, p1, p2, p3, (abi_s32)value, tail)) \
  X(prefix, 5, u8_0, (abi_u8 value, abi_u16 tail), \
    ABI_MIX_RESULT(5, 0, 0, 0, 0, value, tail)) \
  X(prefix, 6, u8_1, (abi_u16 p0, abi_u8 value, abi_u16 tail), \
    ABI_MIX_RESULT(6, p0, 0, 0, 0, value, tail)) \
  X(prefix, 7, u8_2, \
    (abi_u16 p0, abi_u16 p1, abi_u8 value, abi_u16 tail), \
    ABI_MIX_RESULT(7, p0, p1, 0, 0, value, tail)) \
  X(prefix, 8, u8_3, \
    (abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u8 value, abi_u16 tail), \
    ABI_MIX_RESULT(8, p0, p1, p2, 0, value, tail)) \
  X(prefix, 9, u8_4, \
    (abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3, abi_u8 value, \
     abi_u16 tail), \
    ABI_MIX_RESULT(9, p0, p1, p2, p3, value, tail)) \
  X(prefix, 10, u32_0, (abi_u32 value, abi_u16 tail), \
    ABI_MIX_RESULT(10, 0, 0, 0, 0, value, tail)) \
  X(prefix, 11, u32_1, (abi_u16 p0, abi_u32 value, abi_u16 tail), \
    ABI_MIX_RESULT(11, p0, 0, 0, 0, value, tail)) \
  X(prefix, 12, u32_2, \
    (abi_u16 p0, abi_u16 p1, abi_u32 value, abi_u16 tail), \
    ABI_MIX_RESULT(12, p0, p1, 0, 0, value, tail)) \
  X(prefix, 13, u32_3, \
    (abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u32 value, abi_u16 tail), \
    ABI_MIX_RESULT(13, p0, p1, p2, 0, value, tail)) \
  X(prefix, 14, ptr_0, (volatile abi_u16 *address, abi_u16 tail), \
    ABI_MIX_RESULT(14, 0, 0, 0, 0, *address, tail)) \
  X(prefix, 15, ptr_1, \
    (abi_u16 p0, volatile abi_u16 *address, abi_u16 tail), \
    ABI_MIX_RESULT(15, p0, 0, 0, 0, *address, tail)) \
  X(prefix, 16, ptr_2, \
    (abi_u16 p0, abi_u16 p1, volatile abi_u16 *address, abi_u16 tail), \
    ABI_MIX_RESULT(16, p0, p1, 0, 0, *address, tail)) \
  X(prefix, 17, ptr_3, \
    (abi_u16 p0, abi_u16 p1, abi_u16 p2, volatile abi_u16 *address, \
     abi_u16 tail), \
    ABI_MIX_RESULT(17, p0, p1, p2, 0, *address, tail)) \
  X(prefix, 18, fn_0, (abi_callback function, abi_u16 tail), \
    ABI_MIX_RESULT(18, 0, 0, 0, 0, function(tail ^ 18U), tail)) \
  X(prefix, 19, fn_1, \
    (abi_u16 p0, abi_callback function, abi_u16 tail), \
    ABI_MIX_RESULT(19, p0, 0, 0, 0, function(tail ^ 19U), tail)) \
  X(prefix, 20, fn_2, \
    (abi_u16 p0, abi_u16 p1, abi_callback function, abi_u16 tail), \
    ABI_MIX_RESULT(20, p0, p1, 0, 0, function(tail ^ 20U), tail)) \
  X(prefix, 21, fn_3, \
    (abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_callback function, \
     abi_u16 tail), \
    ABI_MIX_RESULT(21, p0, p1, p2, 0, function(tail ^ 21U), tail))

#define ABI_MIX_DECLARE(prefix, id, name, parameters, body) \
  abi_u32 prefix##name parameters;

ABI_MIXED_FUNCTION_LIST(ABI_MIX_DECLARE, tasking_)
ABI_MIXED_FUNCTION_LIST(ABI_MIX_DECLARE, llvm_)
ABI_MIXED_FUNCTION_LIST(ABI_MIX_DECLARE, llvm_proxy_)

abi_u32 llvm_reverse_mixed(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail);

#define ABI_ENUM5_BODY(a, b, c, d, e)                                     \
  return ((abi_u32)(a) & 0x0fUL) | (((abi_u32)(b) & 0x0fUL) << 4) |       \
         (((abi_u32)(c) & 0x0fUL) << 8) |                                 \
         (((abi_u32)(d) & 0x0fUL) << 12) |                                \
         (((abi_u32)(e) & 0x0fUL) << 16)

abi_u32 tasking_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                      enum abi_enum d, enum abi_enum e);
abi_u32 llvm_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                   enum abi_enum d, enum abi_enum e);
abi_u32 llvm_proxy_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                         enum abi_enum d, enum abi_enum e);
abi_u32 llvm_reverse_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                           enum abi_enum d, enum abi_enum e);
abi_u32 llvm_reverse_enum5_proxy(
    enum abi_enum a, enum abi_enum b, enum abi_enum c, enum abi_enum d,
    enum abi_enum e);

#endif
