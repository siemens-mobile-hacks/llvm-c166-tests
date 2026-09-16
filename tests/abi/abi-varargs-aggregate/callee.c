#include "c166_test.h"
#include "common.h"

C166_NOINLINE C166_SECTION(".abi_varargs_aggregate")
abi_u16 varargs_aggregate(abi_u16 fixed, ...) {
  ABI_AGGREGATE_VARARGS_BODY(fixed);
}
