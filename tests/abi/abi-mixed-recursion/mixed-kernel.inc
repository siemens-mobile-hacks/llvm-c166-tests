#ifndef ABI_MIXED_FUNCTION
#error ABI_MIXED_FUNCTION must name the generated function
#endif
#ifndef ABI_MIXED_OTHER
#error ABI_MIXED_OTHER must name the alternating compiler function
#endif
#ifndef ABI_MIXED_ATTRIBUTE
#define ABI_MIXED_ATTRIBUTE
#endif

ABI_MIXED_ATTRIBUTE
abi_u32 ABI_MIXED_FUNCTION(abi_u16 depth, abi_u16 word, abi_u16 *pointer,
                           float value, struct abi_mixed_pair pair,
                           abi_u16 tail) {
  volatile abi_u16 canary[6];
  struct abi_mixed_pair next_pair;
  abi_u32 child;
  abi_u32 result;
  abi_u16 index;
  abi_u16 pointed = *pointer;

  for (index = 0; index != 6U; ++index)
    canary[index] = (abi_u16)(0xa55aU + word + (depth << 8) +
                              (index << 12) + (index << 4) + index);

  if (depth != 0U) {
    next_pair.first =
        (abi_u16)(pair.second ^ (abi_u16)(0x0101U + depth));
    next_pair.second =
        (abi_u16)(pair.first + (abi_u16)(0x0202U + depth));
    child = ABI_MIXED_OTHER(
        depth - 1U,
        (abi_u16)(word + 0x0111U + depth + (depth << 1)), pointer,
        value, next_pair,
        (abi_u16)(tail ^
                  (abi_u16)(0x1001U + depth + (depth << 4))));
  } else {
    tasking_mixed_observe(value, pair, pointed, tail);
    child = 0x13579bdfUL ^ ((abi_u32)pointed << 16) ^ word;
  }

  result = child ^ ((abi_u32)word << 16) ^ pointed;
  result += ((abi_u32)pair.first << 16) | pair.second;
  result ^= (abi_u32)tail << (depth & 7U);
  result ^= ((abi_u32)depth << 24) | ((abi_u32)depth << 16) |
            ((abi_u32)depth << 8) | depth;
  result = (result << 7) | (result >> 25);

  for (index = 0; index != 6U; ++index) {
    abi_u16 expected =
        (abi_u16)(0xa55aU + word + (depth << 8) +
                  (index << 12) + (index << 4) + index);
    if (canary[index] != expected)
      return 0xdeadc0deUL;
  }
  return result;
}

#undef ABI_MIXED_ATTRIBUTE
#undef ABI_MIXED_OTHER
#undef ABI_MIXED_FUNCTION
