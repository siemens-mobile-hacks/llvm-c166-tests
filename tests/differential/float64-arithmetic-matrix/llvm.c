#include "types.h"

__attribute__((noinline, section(".llvm_float64_eval")))
double llvm_float64_eval(abi_u16 operation, double lhs, double rhs,
                         abi_u16 tail) {
  if (tail != 0x5a5aU)
    return __builtin_nan("");

  switch (operation) {
  case 0:
    return lhs + rhs;
  case 1:
    return lhs - rhs;
  case 2:
    return lhs * rhs;
  default:
    return lhs / rhs;
  }
}
