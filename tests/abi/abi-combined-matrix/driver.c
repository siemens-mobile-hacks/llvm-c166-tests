#include "types.h"

void main(void) {
  ABI_PACKED_REF packed_value;
  struct abi_result result;

  tap_plan(9U);

  tap_is_u32(mixed_probe((abi_s8)-5, (abi_u8)250, 0x12345678UL, 0x9abcU,
                         0xdef0U),
             42U, "mixed scalar arguments");

  packed_value.first = 0x12U;
  packed_value.word = 0x3456U;
  packed_value.last = 0x78U;
  tap_is_u32(aggregate_probe(0x9abcU, packed_value, 0xdef0U), 42U,
             "packed aggregate argument");

  result = make_result(0x1357U);
  tap_is_u32(result.first, 0x21U, "aggregate return first");
  tap_is_u32(result.word, 0xb60dU, "aggregate return word");
  tap_is_u32(result.last, 0x43U, "aggregate return last");

  tap_is_u32(pointer_probe(pointer_target, far_words, 0x55aaU), 42U,
             "data and function pointers");

  tap_is_u32(far_words[2], 0x5a5aU, "pointer mutation");

  tap_is_u32(runtime_probe(0x87654321UL, 0x1234UL, 7U), 26999U,
             "integer runtime operations");

  tap_is_u32(float_probe(1.5F, 2.25F, 6.5, 2.0, 0xa55aU), 42U,
             "floating arguments and operations");
}
