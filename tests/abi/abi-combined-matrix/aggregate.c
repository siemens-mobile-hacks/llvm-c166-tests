#include "types.h"

abi_u16 aggregate_probe(abi_u16 prefix, struct abi_packed value,
                        abi_u16 tail) {
  if (prefix != 0x9abcU || value.first != 0x12U || value.word != 0x3456U ||
      value.last != 0x78U || tail != 0xdef0U)
    return 1U;
  return 42U;
}

struct abi_result make_result(abi_u16 seed) {
  struct abi_result result;
  result.first = 0x21U;
  result.word = seed ^ 0xa55aU;
  result.last = 0x43U;
  return result;
}
