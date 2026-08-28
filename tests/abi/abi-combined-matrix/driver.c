#include "types.h"

abi_u16 test_main(void) {
  struct abi_packed packed_value;
  struct abi_result result;

  if (mixed_probe((abi_s8)-5, (abi_u8)250, 0x12345678UL, 0x9abcU,
                  0xdef0U) != 42U)
    return 0x0101U;

  packed_value.first = 0x12U;
  packed_value.word = 0x3456U;
  packed_value.last = 0x78U;
  if (aggregate_probe(0x9abcU, packed_value, 0xdef0U) != 42U)
    return 0x0102U;

  result = make_result(0x1357U);
  if (result.first != 0x21U || result.word != 0xb60dU ||
      result.last != 0x43U)
    return 0x0103U;

  if (pointer_probe(pointer_target, far_words, 0x55aaU) != 42U)
    return 0x0104U;

  if (runtime_probe(0x87654321UL, 0x1234UL, 7U) != 26999U)
    return 0x0105U;

  if (float_probe(1.5F, 2.25F, 6.5, 2.0, 0xa55aU) != 42U)
    return 0x0106U;

  return 42U;
}
