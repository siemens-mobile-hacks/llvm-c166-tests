#ifndef ABI_VARARGS_MATRIX_TYPES_H
#define ABI_VARARGS_MATRIX_TYPES_H

typedef signed int abi_s16;
typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;
typedef unsigned long long abi_ull;

enum matrix_enum {
  MATRIX_ENUM_ZERO = 0,
  MATRIX_ENUM_ONE = 1,
  MATRIX_ENUM_MAX = 127
};

#define DECLARE_VARARGS(prefix) \
  abi_u32 prefix##1(abi_u16 fixed0, ...); \
  abi_u32 prefix##2(abi_u16 fixed0, abi_u16 fixed1, ...); \
  abi_u32 prefix##3(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, ...); \
  abi_u32 prefix##4(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, \
                     abi_u16 fixed3, ...); \
  abi_u32 prefix##5(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, \
                     abi_u16 fixed3, abi_u16 fixed4, ...)

DECLARE_VARARGS(tasking_varargs);
DECLARE_VARARGS(llvm_varargs);

abi_u32 tasking_varargs_stream(abi_u16 prefix_count, ...);
abi_u32 llvm_varargs_stream(abi_u16 prefix_count, ...);

abi_u32 llvm_reverse_entry(
    abi_u16 shape, abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
    abi_u16 fixed3, abi_u16 fixed4, signed char signed_value,
    unsigned char unsigned_byte, enum matrix_enum enum_value,
    abi_u16 word_value, abi_u32 long_value, abi_ull long_long_value,
    volatile abi_u16 *pointer_value);
abi_u32 llvm_reverse_stream_entry(abi_u16 prefix_count,
                                  volatile abi_u16 *pointer_value);

#endif
