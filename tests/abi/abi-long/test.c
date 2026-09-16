#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

static void run_scalar(abi_u16 prefix, abi_u32 value, abi_u16 tail,
                       abi_u32 expected) {
  tap_is_u32(c166_long_entry(prefix, value, tail), expected ^ 0x13579bdfUL,
             "long scalar arguments and return");
}

#define RUN_SCALAR(id, prefix, value, tail, expected, record_tag,              \
                   record_value)                                               \
  run_scalar(prefix, value, tail, expected);

static void run_record(abi_u16 prefix, abi_u32 value, abi_u16 tail,
                       abi_u16 expected_tag, abi_u32 expected_value) {
  struct long_record input;
  struct long_record actual;

  input.tag = prefix;
  input.value = value;
  actual = c166_long_record_entry(input, tail);
  tap_is_u32(actual.tag, expected_tag, "long record tag return");
  tap_is_u32(actual.value, expected_value, "long record value return");
}

#define RUN_RECORD(id, prefix, value, tail, expected, record_tag,              \
                   record_value)                                               \
  run_record(prefix, value, tail, record_tag, record_value);

void main(void) {
  tap_plan(24U);
  ABI_LONG_VECTORS(RUN_SCALAR)
  ABI_LONG_VECTORS(RUN_RECORD)
}
