#include "c166_test.h"
#include "kernel.inc"

volatile abi_u32 C166_NEAR C166_SECTION(".near_words") near_words[8];
volatile abi_u32 C166_XNEAR C166_SECTION(".xnear_words") xnear_words[8];

static const abi_u16 seeds[] = {
    0x0000U, 0x0001U, 0x1234U, 0x7fffU,
    0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
};

static const abi_u32 expected_digests[] = {
    0xf007cb6fUL, 0xad034fa4UL, 0xad42c18eUL, 0x8525676aUL,
    0xd9a2cbabUL, 0xa14377d8UL, 0xeafc9942UL, 0xa0886769UL,
};

C166_NOINLINE
abi_u32 c166_test_entry(abi_u16 seed,
                        volatile abi_u32 C166_NEAR *near_values,
                        volatile abi_u32 C166_XNEAR *xnear_values) {
  return qualifier_kernel(seed, near_values, xnear_values);
}

static void check_buffers(abi_u16 seed) {
  abi_u16 index;

  for (index = 0; index != 8U; ++index) {
    abi_u32 low = (abi_u32)(abi_u16)(seed + index * 0x1111U);
    abi_u32 high = (abi_u32)(abi_u16)(seed ^ (index * 0x2222U));

    tap_is_u32(near_words[index], low | (high << 16), "near buffer");
    tap_is_u32(xnear_words[index],
               (low ^ 0xa55aUL) | ((high ^ 0x5aa5UL) << 16),
               "xnear buffer");
  }
}

void main(void) {
  abi_u16 vector;

  tap_plan(136U);
  for (vector = 0; vector != 8U; ++vector) {
    abi_u32 digest = c166_test_entry(seeds[vector], near_words, xnear_words);
    tap_is_u32(digest, expected_digests[vector], "near pointer digest");
    check_buffers(seeds[vector]);
  }
}
