#include "c166_test.h"
#include "matrix-common.h"

C166_NOINLINE C166_SECTION(".abi_varargs1")
abi_u32 varargs1(abi_u16 fixed0, ...) {
  VARARGS_BODY(fixed0, FIXED1(fixed0));
}

C166_NOINLINE C166_SECTION(".abi_varargs2")
abi_u32 varargs2(abi_u16 fixed0, abi_u16 fixed1, ...) {
  VARARGS_BODY(fixed1, FIXED2(fixed0, fixed1));
}

C166_NOINLINE C166_SECTION(".abi_varargs3")
abi_u32 varargs3(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, ...) {
  VARARGS_BODY(fixed2, FIXED3(fixed0, fixed1, fixed2));
}

C166_NOINLINE C166_SECTION(".abi_varargs4")
abi_u32 varargs4(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                 abi_u16 fixed3, ...) {
  VARARGS_BODY(fixed3, FIXED4(fixed0, fixed1, fixed2, fixed3));
}

C166_NOINLINE C166_SECTION(".abi_varargs5")
abi_u32 varargs5(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                 abi_u16 fixed3, abi_u16 fixed4, ...) {
  VARARGS_BODY(fixed4, FIXED5(fixed0, fixed1, fixed2, fixed3, fixed4));
}

C166_NOINLINE C166_SECTION(".abi_varargs_stream")
abi_u32 varargs_stream(abi_u16 prefix_count, ...) {
  {
    va_list original;
    va_list copied;
    abi_u16 prefix;
    abi_u16 valid = 1U;
    volatile abi_u16 *original_pointer;

    va_start(original, prefix_count);
    for (prefix = 0U; prefix < prefix_count; ++prefix)
      (void)va_arg(original, unsigned int);
    C166_VA_COPY(copied, original);

    valid &= va_arg(original, unsigned long) == STREAM_LONG0;
    valid &= va_arg(copied, unsigned long) == STREAM_LONG0;
    valid &= va_arg(copied, unsigned int) == STREAM_WORD0;
    valid &= va_arg(original, unsigned int) == STREAM_WORD0;
    valid &= va_arg(original, unsigned long) == STREAM_LONG1;
    valid &= va_arg(original, unsigned int) == STREAM_WORD1;
    valid &= va_arg(original, unsigned long) == STREAM_SECOND_LONG;
    original_pointer = va_arg(original, volatile abi_u16 *);
    valid &= va_arg(original, unsigned int) == STREAM_TAIL;
    va_end(original);

    valid &= va_arg(copied, unsigned long) == STREAM_LONG1;
    valid &= va_arg(copied, unsigned int) == STREAM_WORD1;
    valid &= va_arg(copied, unsigned long) == STREAM_SECOND_LONG;
    valid &= va_arg(copied, volatile abi_u16 *) == original_pointer;
    valid &= va_arg(copied, unsigned int) == STREAM_TAIL;
    va_end(copied);
    if (!valid)
      return 0UL;
  }

  {
    VARARGS_STREAM_BODY(prefix_count);
  }
}
