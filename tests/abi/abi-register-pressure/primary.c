#include "types.h"

#define ABI_PRESSURE_WORD_FUNCTION pressure_words_primary
#define ABI_PRESSURE_LONG_FUNCTION pressure_longs_primary
#define ABI_PRESSURE_SELECTOR pressure_selector_primary
#define ABI_PRESSURE_WORD_ATTRIBUTE                                         \
  C166_NOINLINE C166_SECTION(".abi.pressure.primary.words")
#define ABI_PRESSURE_LONG_ATTRIBUTE                                         \
  C166_NOINLINE C166_SECTION(".abi.pressure.primary.longs")
#include "pressure-kernel.inc"

#define ABI_PRESSURE_WORD_FUNCTION pressure_words_primary_alternate_leaf
#define ABI_PRESSURE_LONG_FUNCTION pressure_longs_primary_alternate_leaf
#define ABI_PRESSURE_SELECTOR pressure_selector_alternate
#define ABI_PRESSURE_WORD_ATTRIBUTE                                         \
  C166_NOINLINE C166_SECTION(".abi.pressure.primary-alternate.words")
#define ABI_PRESSURE_LONG_ATTRIBUTE                                         \
  C166_NOINLINE C166_SECTION(".abi.pressure.primary-alternate.longs")
#include "pressure-kernel.inc"
