#include "types.h"

#define ABI_PRESSURE_WORD_FUNCTION pressure_words_alternate
#define ABI_PRESSURE_LONG_FUNCTION pressure_longs_alternate
#define ABI_PRESSURE_SELECTOR pressure_selector_alternate
#define ABI_PRESSURE_WORD_ATTRIBUTE                                         \
  C166_NOINLINE C166_SECTION(".abi.pressure.alternate.words")
#define ABI_PRESSURE_LONG_ATTRIBUTE                                         \
  C166_NOINLINE C166_SECTION(".abi.pressure.alternate.longs")
#include "pressure-kernel.inc"

#define ABI_PRESSURE_WORD_FUNCTION pressure_words_alternate_primary_leaf
#define ABI_PRESSURE_LONG_FUNCTION pressure_longs_alternate_primary_leaf
#define ABI_PRESSURE_SELECTOR pressure_selector_primary
#include "pressure-kernel.inc"
