#include "types.h"
#include "vectors.inc"

static void run_pressure_word_vector(
    abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
    abi_u16 a6, abi_u16 a7, abi_u16 selector, abi_u16 golden) {
  tap_is_u32(pressure_words_primary(a0, a1, a2, a3, a4, a5, a6, a7,
                                    selector),
             golden, "primary word pressure");
  tap_is_u32(pressure_words_alternate(a0, a1, a2, a3, a4, a5, a6, a7,
                                      selector),
             golden, "alternate word pressure");
  tap_is_u32(pressure_words_primary_alternate_leaf(
                 a0, a1, a2, a3, a4, a5, a6, a7, selector),
             golden, "primary word pressure with alternate leaf");
  tap_is_u32(pressure_words_alternate_primary_leaf(
                 a0, a1, a2, a3, a4, a5, a6, a7, selector),
             golden, "alternate word pressure with primary leaf");
  tap_is_u32(pressure_words_forward_alternate(
                 a0, a1, a2, a3, a4, a5, a6, a7, selector),
             golden, "forwarded alternate word pressure");
}

static void run_pressure_long_vector(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                                     abi_u32 a3, abi_u32 a4, abi_u32 a5,
                                     abi_u16 selector, abi_u32 golden) {
  tap_is_u32(pressure_longs_primary(a0, a1, a2, a3, a4, a5, selector),
             golden, "primary long pressure");
  tap_is_u32(pressure_longs_alternate(a0, a1, a2, a3, a4, a5, selector),
             golden, "alternate long pressure");
  tap_is_u32(pressure_longs_primary_alternate_leaf(
                 a0, a1, a2, a3, a4, a5, selector),
             golden, "primary long pressure with alternate leaf");
  tap_is_u32(pressure_longs_alternate_primary_leaf(
                 a0, a1, a2, a3, a4, a5, selector),
             golden, "alternate long pressure with primary leaf");
  tap_is_u32(pressure_longs_forward_alternate(a0, a1, a2, a3, a4, a5,
                                              selector),
             golden, "forwarded alternate long pressure");
}

#define RUN_PRESSURE_WORD(id, a0, a1, a2, a3, a4, a5, a6, a7, selector,   \
                          golden)                                           \
  run_pressure_word_vector(a0, a1, a2, a3, a4, a5, a6, a7, selector,      \
                           golden);
#define RUN_PRESSURE_LONG(id, a0, a1, a2, a3, a4, a5, selector, golden)    \
  run_pressure_long_vector(a0, a1, a2, a3, a4, a5, selector, golden);

void main(void) {
  tap_plan(40U);
  ABI_PRESSURE_WORD_VECTORS(RUN_PRESSURE_WORD)
  ABI_PRESSURE_LONG_VECTORS(RUN_PRESSURE_LONG)
}
