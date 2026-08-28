#include "types.h"

__attribute__((noinline, section(".llvm_pressure_selector")))
abi_u16 llvm_pressure_selector(abi_u16 selector) { return selector; }
