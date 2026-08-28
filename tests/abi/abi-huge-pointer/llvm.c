#include "kernel.inc"

__attribute__((noinline))
abi_u32 llvm_entry(abi_u16 seed, volatile abi_u32 _huge *huge_words,
                   volatile abi_u32 _shuge *shuge_words) {
  return qualifier_kernel(seed, huge_words, shuge_words);
}
