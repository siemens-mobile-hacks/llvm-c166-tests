#include "common.h"

__attribute__((noinline, section(".llvm_varargs_aggregate")))
abi_u16 llvm_varargs_aggregate(abi_u16 fixed, ...) {
  ABI_AGGREGATE_VARARGS_BODY(fixed);
}

__attribute__((noinline, section(".llvm_varargs_aggregate_reverse")))
abi_u32 llvm_varargs_aggregate_reverse(abi_u16 seed) {
  struct pair2 pair_value;
  struct chars3 chars_value;
  ABI_PACKED_REF(packed3) packed_value;
  abi_u16 tail;
  abi_u16 tasking_result;
  abi_u16 llvm_result;

  ABI_INIT_AGGREGATES(seed, pair_value, chars_value, packed_value, tail);
  tasking_result = tasking_varargs_aggregate(
      ABI_AGGREGATE_VARARGS_FIXED, pair_value, chars_value, packed_value, tail);
  llvm_result = llvm_varargs_aggregate(
      ABI_AGGREGATE_VARARGS_FIXED, pair_value, chars_value, packed_value, tail);
  return ((abi_u32)tasking_result << 16) | (abi_u32)llvm_result;
}
