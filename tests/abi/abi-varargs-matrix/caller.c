#include "c166_test.h"
#include "matrix-common.h"

C166_NOINLINE C166_SECTION(".abi_varargs_dispatch")
abi_u32 varargs_dispatch(
    abi_u16 shape, abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
    abi_u16 fixed3, abi_u16 fixed4, signed char signed_value,
    unsigned char unsigned_byte, enum matrix_enum enum_value,
    abi_u16 word_value, abi_u32 long_value, abi_u32_second second_long_value,
    volatile abi_u16 *pointer_value) {
  switch (shape) {
  case 1:
    return varargs1(fixed0, signed_value, unsigned_byte, enum_value,
                    word_value, long_value, second_long_value, pointer_value);
  case 2:
    return varargs2(fixed0, fixed1, signed_value, unsigned_byte, enum_value,
                    word_value, long_value, second_long_value, pointer_value);
  case 3:
    return varargs3(fixed0, fixed1, fixed2, signed_value, unsigned_byte,
                    enum_value, word_value, long_value, second_long_value,
                    pointer_value);
  case 4:
    return varargs4(fixed0, fixed1, fixed2, fixed3, signed_value,
                    unsigned_byte, enum_value, word_value, long_value,
                    second_long_value, pointer_value);
  default:
    return varargs5(fixed0, fixed1, fixed2, fixed3, fixed4, signed_value,
                    unsigned_byte, enum_value, word_value, long_value,
                    second_long_value, pointer_value);
  }
}

C166_NOINLINE C166_SECTION(".abi_varargs_stream_call")
abi_u32 varargs_stream_call(abi_u16 prefix_count,
                            volatile abi_u16 *pointer_value) {
  abi_u32 result;
  CALL_VARARGS_STREAM(varargs_stream, prefix_count, pointer_value, result);
  return result;
}
