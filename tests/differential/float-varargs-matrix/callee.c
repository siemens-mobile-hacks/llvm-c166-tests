#include <stdarg.h>

#include "types.h"

volatile union abi_double_words vararg_captured[2];
volatile abi_u16 vararg_observed[2];

static C166_NOINLINE abi_u32 capture_varargs(va_list arguments,
                                             abi_u16 fixed_mix) {
  vararg_captured[0].value = va_arg(arguments, double);
  vararg_observed[0] = va_arg(arguments, unsigned int);
  vararg_captured[1].value = va_arg(arguments, double);
  vararg_observed[1] = fixed_mix;
  return 0xa5a50000UL | fixed_mix;
}

#define DEFINE_FLOAT_VARARGS(number, parameters, last, mix)                  \
  C166_NOINLINE C166_SECTION(".float_varargs" #number)                     \
  abi_u32 float_varargs##number parameters {                                 \
    va_list arguments;                                                       \
    abi_u32 result;                                                          \
    va_start(arguments, last);                                               \
    result = capture_varargs(arguments, mix);                                \
    va_end(arguments);                                                       \
    return result;                                                           \
  }

DEFINE_FLOAT_VARARGS(1, (abi_u16 fixed0, ...), fixed0, fixed0)
DEFINE_FLOAT_VARARGS(2, (abi_u16 fixed0, abi_u16 fixed1, ...), fixed1,
                     fixed0 ^ fixed1)
DEFINE_FLOAT_VARARGS(3,
                     (abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, ...),
                     fixed2, fixed0 ^ fixed1 ^ fixed2)
DEFINE_FLOAT_VARARGS(4,
                     (abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                      abi_u16 fixed3, ...),
                     fixed3, fixed0 ^ fixed1 ^ fixed2 ^ fixed3)
DEFINE_FLOAT_VARARGS(5,
                     (abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                      abi_u16 fixed3, abi_u16 fixed4, ...),
                     fixed4, fixed0 ^ fixed1 ^ fixed2 ^ fixed3 ^ fixed4)

#undef DEFINE_FLOAT_VARARGS
