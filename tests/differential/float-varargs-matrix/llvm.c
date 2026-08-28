#include <stdarg.h>
#include "types.h"

extern volatile double llvm_vararg_captured[2];
extern volatile abi_u16 llvm_vararg_observed[2];

static __attribute__((noinline)) abi_u32 capture_varargs(va_list arguments,
                                                         abi_u16 fixed_mix) {
  abi_u16 middle;

  llvm_vararg_captured[0] = va_arg(arguments, double);
  middle = va_arg(arguments, unsigned int);
  llvm_vararg_captured[1] = va_arg(arguments, double);

  llvm_vararg_observed[0] = middle;
  llvm_vararg_observed[1] = fixed_mix;
  return 0xa5a50000UL | fixed_mix;
}

#define CAPTURE(last, mix)                                                    \
  do {                                                                        \
    va_list arguments;                                                        \
    abi_u32 result;                                                           \
    va_start(arguments, last);                                                \
    result = capture_varargs(arguments, mix);                                 \
    va_end(arguments);                                                        \
    return result;                                                            \
  } while (0)

__attribute__((noinline, section(".llvm_float_varargs1")))
abi_u32 llvm_float_varargs1(abi_u16 fixed0, ...) {
  CAPTURE(fixed0, fixed0);
}

__attribute__((noinline, section(".llvm_float_varargs2")))
abi_u32 llvm_float_varargs2(abi_u16 fixed0, abi_u16 fixed1, ...) {
  CAPTURE(fixed1, fixed0 ^ fixed1);
}

__attribute__((noinline, section(".llvm_float_varargs3")))
abi_u32 llvm_float_varargs3(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                            ...) {
  CAPTURE(fixed2, fixed0 ^ fixed1 ^ fixed2);
}

__attribute__((noinline, section(".llvm_float_varargs4")))
abi_u32 llvm_float_varargs4(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                            abi_u16 fixed3, ...) {
  CAPTURE(fixed3, fixed0 ^ fixed1 ^ fixed2 ^ fixed3);
}

__attribute__((noinline, section(".llvm_float_varargs5")))
abi_u32 llvm_float_varargs5(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                            abi_u16 fixed3, abi_u16 fixed4, ...) {
  CAPTURE(fixed4, fixed0 ^ fixed1 ^ fixed2 ^ fixed3 ^ fixed4);
}

__attribute__((noinline, section(".llvm_float_promote")))
abi_u32 llvm_float_promote(abi_u16 shape, float value) {
  switch (shape) {
  case 1:
    return llvm_float_varargs1(0x1111U, value, 0x55aaU, 1.0);
  case 2:
    return llvm_float_varargs2(0x1111U, 0x2222U, value, 0x55aaU, 1.0);
  case 3:
    return llvm_float_varargs3(0x1111U, 0x2222U, 0x4444U, value, 0x55aaU,
                               1.0);
  case 4:
    return llvm_float_varargs4(0x1111U, 0x2222U, 0x4444U, 0x8888U, value,
                               0x55aaU, 1.0);
  default:
    return llvm_float_varargs5(0x1111U, 0x2222U, 0x4444U, 0x8888U, 0xabcdU,
                               value, 0x55aaU, 1.0);
  }
}

#undef CAPTURE
