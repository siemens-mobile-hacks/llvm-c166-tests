#include "types.h"

#define LLVM_SECTION(name) __attribute__((noinline, section(name)))

#define ABI_PRESSURE_WORD_FUNCTION llvm_pressure_words
#define ABI_PRESSURE_LONG_FUNCTION llvm_pressure_longs
#define ABI_PRESSURE_SELECTOR llvm_pressure_selector
#define ABI_PRESSURE_WORD_ATTRIBUTE LLVM_SECTION(".llvm_pressure_words")
#define ABI_PRESSURE_LONG_ATTRIBUTE LLVM_SECTION(".llvm_pressure_longs")
#include "pressure-kernel.inc"

#define ABI_PRESSURE_WORD_FUNCTION llvm_pressure_words_tasking_leaf
#define ABI_PRESSURE_LONG_FUNCTION llvm_pressure_longs_tasking_leaf
#define ABI_PRESSURE_SELECTOR tasking_pressure_selector
#define ABI_PRESSURE_WORD_ATTRIBUTE \
  LLVM_SECTION(".llvm_pressure_words_tasking_leaf")
#define ABI_PRESSURE_LONG_ATTRIBUTE \
  LLVM_SECTION(".llvm_pressure_longs_tasking_leaf")
#include "pressure-kernel.inc"

LLVM_SECTION(".llvm_call_tasking_pressure_words")
abi_u16 llvm_call_tasking_pressure_words(
    abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
    abi_u16 a6, abi_u16 a7, abi_u16 selector) {
  return tasking_pressure_words(a0, a1, a2, a3, a4, a5, a6, a7, selector);
}

LLVM_SECTION(".llvm_call_tasking_pressure_longs")
abi_u32 llvm_call_tasking_pressure_longs(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                                         abi_u32 a3, abi_u32 a4, abi_u32 a5,
                                         abi_u16 selector) {
  return tasking_pressure_longs(a0, a1, a2, a3, a4, a5, selector);
}
