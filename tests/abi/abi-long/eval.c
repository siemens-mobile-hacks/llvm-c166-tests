#include "c166_test.h"
#include "types.h"

volatile abi_u32 c166_long_global;
volatile abi_s32 c166_signed_long_global;

C166_NOINLINE
abi_u32 c166_long_entry(abi_u16 prefix, abi_u32 value, abi_u16 tail) {
  volatile struct long_record record;

  record.tag = prefix;
  record.value = value;
  c166_long_global = record.value;
  value = c166_long_global;
  if (record.tag != prefix)
    return 0;
  c166_signed_long_global = (abi_s32)value;
  if ((abi_u16)(c166_signed_long_global < 0) != (abi_u16)(value >> 31))
    return 0;
  return c166_long_mix(prefix, value, tail) ^ 0x13579bdfUL;
}

C166_NOINLINE
struct long_record c166_long_record_entry(struct long_record input,
                                          abi_u16 tail) {
  return c166_long_record_mix(input, tail);
}
