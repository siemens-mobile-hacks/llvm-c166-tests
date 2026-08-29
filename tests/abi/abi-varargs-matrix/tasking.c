#include "matrix-common.h"

#pragma fragment

abi_u32 tasking_varargs1(abi_u16 fixed0, ...) {
  VARARGS_BODY(fixed0, FIXED1(fixed0));
}

abi_u32 tasking_varargs2(abi_u16 fixed0, abi_u16 fixed1, ...) {
  VARARGS_BODY(fixed1, FIXED2(fixed0, fixed1));
}

abi_u32 tasking_varargs3(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, ...) {
  VARARGS_BODY(fixed2, FIXED3(fixed0, fixed1, fixed2));
}

abi_u32 tasking_varargs4(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                         abi_u16 fixed3, ...) {
  VARARGS_BODY(fixed3, FIXED4(fixed0, fixed1, fixed2, fixed3));
}

abi_u32 tasking_varargs5(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                         abi_u16 fixed3, abi_u16 fixed4, ...) {
  VARARGS_BODY(fixed4, FIXED5(fixed0, fixed1, fixed2, fixed3, fixed4));
}

abi_u32 tasking_varargs_stream(abi_u16 prefix_count, ...) {
  VARARGS_STREAM_BODY(prefix_count);
}
