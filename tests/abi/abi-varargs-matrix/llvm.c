#include "matrix-common.h"

__attribute__((noinline, section(".llvm_varargs1")))
abi_u32 llvm_varargs1(abi_u16 fixed0, ...) {
  VARARGS_BODY(fixed0, FIXED1(fixed0));
}

__attribute__((noinline, section(".llvm_varargs2")))
abi_u32 llvm_varargs2(abi_u16 fixed0, abi_u16 fixed1, ...) {
  VARARGS_BODY(fixed1, FIXED2(fixed0, fixed1));
}

__attribute__((noinline, section(".llvm_varargs3")))
abi_u32 llvm_varargs3(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, ...) {
  VARARGS_BODY(fixed2, FIXED3(fixed0, fixed1, fixed2));
}

__attribute__((noinline, section(".llvm_varargs4")))
abi_u32 llvm_varargs4(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                      abi_u16 fixed3, ...) {
  VARARGS_BODY(fixed3, FIXED4(fixed0, fixed1, fixed2, fixed3));
}

__attribute__((noinline, section(".llvm_varargs5")))
abi_u32 llvm_varargs5(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                      abi_u16 fixed3, abi_u16 fixed4, ...) {
  VARARGS_BODY(fixed4, FIXED5(fixed0, fixed1, fixed2, fixed3, fixed4));
}

__attribute__((noinline, section(".llvm_varargs_stream")))
abi_u32 llvm_varargs_stream(abi_u16 prefix_count, ...) {
  VARARGS_STREAM_BODY(prefix_count);
}

__attribute__((noinline, section(".llvm_reverse")))
abi_u32 llvm_reverse_entry(
    abi_u16 shape, abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
    abi_u16 fixed3, abi_u16 fixed4, signed char signed_value,
    unsigned char unsigned_byte, enum matrix_enum enum_value,
    abi_u16 word_value, abi_u32 long_value, abi_ull long_long_value,
    volatile abi_u16 *pointer_value) {
  switch (shape) {
  case 1:
    return tasking_varargs1(fixed0, signed_value, unsigned_byte, enum_value,
                            word_value, long_value, long_long_value,
                            pointer_value);
  case 2:
    return tasking_varargs2(fixed0, fixed1, signed_value, unsigned_byte,
                            enum_value, word_value, long_value, long_long_value,
                            pointer_value);
  case 3:
    return tasking_varargs3(fixed0, fixed1, fixed2, signed_value,
                            unsigned_byte, enum_value, word_value, long_value,
                            long_long_value, pointer_value);
  case 4:
    return tasking_varargs4(fixed0, fixed1, fixed2, fixed3, signed_value,
                            unsigned_byte, enum_value, word_value, long_value,
                            long_long_value, pointer_value);
  default:
    return tasking_varargs5(fixed0, fixed1, fixed2, fixed3, fixed4,
                            signed_value, unsigned_byte, enum_value,
                            word_value, long_value, long_long_value,
                            pointer_value);
  }
}

__attribute__((noinline, section(".llvm_reverse_stream")))
abi_u32 llvm_reverse_stream_entry(abi_u16 prefix_count,
                                  volatile abi_u16 *pointer_value) {
  abi_u32 result;
  CALL_VARARGS_STREAM(tasking_varargs_stream, prefix_count, pointer_value,
                      result);
  return result;
}
