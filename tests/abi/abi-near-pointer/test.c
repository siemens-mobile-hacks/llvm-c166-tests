#include "c166-test-compat.h"
#include "kernel.inc"

C166_TEST_NOINLINE
abi_u32 c166_test_entry(abi_u16 seed,
                        volatile abi_u32 _near *near_words,
                        volatile abi_u32 _xnear *xnear_words) {
  return qualifier_kernel(seed, near_words, xnear_words);
}
