#include "types.h"

#pragma fragment

abi_u16 tasking_pressure_selector(abi_u16 selector) { return selector; }

#define ABI_PRESSURE_WORD_FUNCTION tasking_pressure_words
#define ABI_PRESSURE_LONG_FUNCTION tasking_pressure_longs
#define ABI_PRESSURE_SELECTOR tasking_pressure_selector
#include "pressure-kernel.inc"

#define ABI_PRESSURE_WORD_FUNCTION tasking_pressure_words_llvm_leaf
#define ABI_PRESSURE_LONG_FUNCTION tasking_pressure_longs_llvm_leaf
#define ABI_PRESSURE_SELECTOR llvm_pressure_selector_bridge
#include "pressure-kernel.inc"
