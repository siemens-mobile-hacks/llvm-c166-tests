#include <stdarg.h>
#include "types.h"

#define NOINLINE __attribute__((noinline))
#define CALLEE(n) llvm_float_varargs##n
#define CALLEE_ATTRIBUTE(n) __attribute__((noinline, section(".llvm_float_varargs" #n)))
#include "callee.inc"

#define TARGET(n) tasking_float_varargs##n
#define RAW_FUNCTION llvm_float_raw
#define PROMOTE_FUNCTION llvm_float_promote
#define RAW_ATTRIBUTE __attribute__((noinline, section(".llvm_float_raw")))
#define PROMOTE_ATTRIBUTE __attribute__((noinline, section(".llvm_float_promote")))
#include "caller.inc"
