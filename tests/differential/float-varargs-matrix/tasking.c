#include <stdarg.h>
#include "types.h"

#define NOINLINE
#define CALLEE(n) tasking_float_varargs##n
#define CALLEE_ATTRIBUTE(n)
#include "callee.inc"

#define TARGET(n) llvm_float_varargs_proxy##n
#define RAW_FUNCTION tasking_float_raw
#define PROMOTE_FUNCTION tasking_float_promote
#define RAW_ATTRIBUTE
#define PROMOTE_ATTRIBUTE
#include "caller.inc"
