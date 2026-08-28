#include "types.h"

__attribute__((noinline, section(".llvm_f64_to_i32")))
abi_s32 llvm_f64_to_i32(double value) { return (abi_s32)value; }

__attribute__((noinline, section(".llvm_f64_to_u32")))
abi_u32 llvm_f64_to_u32(double value) { return (abi_u32)value; }

__attribute__((noinline, section(".llvm_f64_to_i16")))
abi_s16 llvm_f64_to_i16(double value) { return (abi_s16)value; }

__attribute__((noinline, section(".llvm_f64_to_u16")))
abi_u16 llvm_f64_to_u16(double value) { return (abi_u16)value; }

__attribute__((noinline, section(".llvm_i32_to_f64")))
double llvm_i32_to_f64(abi_s32 value) { return (double)value; }

__attribute__((noinline, section(".llvm_u32_to_f64")))
double llvm_u32_to_f64(abi_u32 value) { return (double)value; }

__attribute__((noinline, section(".llvm_i16_to_f64")))
double llvm_i16_to_f64(abi_s16 value) { return (double)value; }

__attribute__((noinline, section(".llvm_u16_to_f64")))
double llvm_u16_to_f64(abi_u16 value) { return (double)value; }

__attribute__((noinline, section(".llvm_f64_compare")))
abi_s16 llvm_f64_compare(abi_u16 operation, double lhs, double rhs) {
  switch (operation) {
  case 0:
    return lhs == rhs;
  case 1:
    return lhs != rhs;
  case 2:
    return lhs < rhs;
  case 3:
    return lhs <= rhs;
  case 4:
    return lhs > rhs;
  case 5:
    return lhs >= rhs;
  default:
    return __builtin_isunordered(lhs, rhs);
  }
}
