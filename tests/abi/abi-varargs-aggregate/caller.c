#include "c166_test.h"
#include "common.h"

C166_NOINLINE C166_SECTION(".abi_varargs_aggregate_repeat")
abi_u32 varargs_aggregate_repeat(abi_u16 seed) {
  struct pair2 pair_value;
  struct chars3 chars_value;
  ABI_PACKED_REF(packed3) packed_value;
  abi_u16 tail;
  abi_u16 first;
  abi_u16 second;

  ABI_INIT_AGGREGATES(seed, pair_value, chars_value, packed_value, tail);
  first = varargs_aggregate(ABI_AGGREGATE_VARARGS_FIXED, pair_value,
                            chars_value, packed_value, tail);
  second = varargs_aggregate(ABI_AGGREGATE_VARARGS_FIXED, pair_value,
                             chars_value, packed_value, tail);
  return ((abi_u32)first << 16) | second;
}
