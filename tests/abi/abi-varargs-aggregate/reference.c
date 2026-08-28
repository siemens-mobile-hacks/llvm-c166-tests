#include "common.h"

#pragma fragment

abi_u16 tasking_varargs_aggregate(abi_u16 fixed, ...) {
  ABI_AGGREGATE_VARARGS_BODY(fixed);
}
