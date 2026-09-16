#include "c166_test.h"
#include "types.h"

C166_NOINLINE
abi_u32 c166_long_mix(abi_u16 prefix, abi_u32 value, abi_u16 tail) {
  value += (abi_u32)prefix << 16;
  value ^= ((abi_u32)tail << 1) | prefix;
  return value;
}

C166_NOINLINE
struct long_record c166_long_record_mix(struct long_record input,
                                        abi_u16 tail) {
  input.tag ^= tail;
  input.value += ((abi_u32)tail << 16) | input.tag;
  return input;
}
