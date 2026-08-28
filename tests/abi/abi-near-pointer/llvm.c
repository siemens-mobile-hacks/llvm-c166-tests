#include "kernel.inc"

__attribute__((noinline))
abi_u32 llvm_entry(abi_u16 seed, volatile abi_u32 _near *near_words,
                   volatile abi_u32 _xnear *xnear_words) {
  return qualifier_kernel(seed, near_words, xnear_words);
}
