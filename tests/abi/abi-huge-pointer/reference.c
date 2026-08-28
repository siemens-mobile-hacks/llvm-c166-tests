#include "kernel.inc"

abi_u32 tasking_qualifier_reference(
    abi_u16 seed, volatile abi_u32 _huge *huge_words,
    volatile abi_u32 _shuge *shuge_words) {
  return qualifier_kernel(seed, huge_words, shuge_words);
}
