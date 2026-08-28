#ifndef ABI_RECURSION_FUNCTION
#error ABI_RECURSION_FUNCTION must name the generated recursive function
#endif
#ifndef ABI_RECURSION_LEAF
#error ABI_RECURSION_LEAF must name the leaf function used at every depth
#endif
#ifndef ABI_RECURSION_ATTRIBUTE
#define ABI_RECURSION_ATTRIBUTE
#endif

ABI_RECURSION_ATTRIBUTE
abi_u32 ABI_RECURSION_FUNCTION(abi_u16 depth, abi_u16 seed) {
  volatile struct abi_recursion_frame local;
  abi_u32 child;
  abi_u32 result;
  abi_u32 odd_product;
  abi_u16 leaf;
  abi_u16 next_seed;
  abi_u16 index;
  abi_u16 repeat;

  local.before = 0x13579bdfUL;
  local.after = 0x89abcdefUL;
  for (index = 0; index != 8U; ++index) {
    local.words[index] =
        (abi_u16)(seed + (depth << 8) + depth + (index << 8) +
                  (index << 5) + (index << 1) + index) ^
        (abi_u16)(0xa55aU + (index << 12) + (index << 8) +
                  (index << 4) + index);
  }

  leaf = ABI_RECURSION_LEAF(depth, seed);
  next_seed = seed + (seed << 5) + (depth << 8) + depth + 0x005bU;
  child = depth ? ABI_RECURSION_FUNCTION(depth - 1U, next_seed)
                : 0x13579bdfUL;

  result = child ^ ((abi_u32)seed << 16) ^ leaf;
  for (index = 0; index != 8U; ++index) {
    odd_product = local.words[7U - index];
    result += (abi_u32)local.words[index] << (index & 7U);
    for (repeat = 0; repeat != index; ++repeat)
      odd_product += (abi_u32)local.words[7U - index] << 1;
    result ^= odd_product;
  }
  result ^= ((abi_u32)leaf << 16) + leaf;
  result = (result << 5) | (result >> 27);

  if (local.before != 0x13579bdfUL || local.after != 0x89abcdefUL)
    return 0xdeadc0deUL;
  return result;
}

#undef ABI_RECURSION_ATTRIBUTE
#undef ABI_RECURSION_LEAF
#undef ABI_RECURSION_FUNCTION
