#include "types.h"

extern double __adddf3(double, double);
extern double __subdf3(double, double);
extern double __muldf3(double, double);
extern double __divdf3(double, double);

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
  case 3:
    return lhs / rhs;
  case 4:
    return __adddf3(lhs, rhs);
  case 5:
    return __subdf3(lhs, rhs);
  case 6:
    return __muldf3(lhs, rhs);
  default:
    return __divdf3(lhs, rhs);
  }
}
