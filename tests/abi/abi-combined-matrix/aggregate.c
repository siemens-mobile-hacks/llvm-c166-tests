#include "types.h"

__attribute__((noinline)) static struct abi_result
adjust_result(struct abi_result value) {
  value.word ^= 0x55aaU;
  return value;
}

__attribute__((noinline)) static struct abi_result
forward_result(struct abi_result value) {
  return adjust_result(value);
}

abi_u16 aggregate_probe(abi_u16 prefix, struct abi_packed value, abi_u16 tail) {
  if (prefix != 0x9abcU || value.first != 0x12U || value.word != 0x3456U ||
      value.last != 0x78U || tail != 0xdef0U)
    return 1U;
  struct abi_result forwarded = {value.first, value.word, value.last};
  forwarded = forward_result(forwarded);
  if (forwarded.first != value.first ||
      forwarded.word != (abi_u16)(value.word ^ 0x55aaU) ||
      forwarded.last != value.last)
    return 2U;
  return 42U;
}

struct abi_result make_result(abi_u16 seed) {
  struct abi_result result;
  result.first = 0x21U;
  result.word = seed ^ 0xa55aU;
  result.last = 0x43U;
  return result;
}
