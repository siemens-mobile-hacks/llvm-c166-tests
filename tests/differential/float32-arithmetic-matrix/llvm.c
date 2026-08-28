#include "types.h"

extern float tasking_float_roundtrip(abi_u16, float, abi_u16);
extern volatile float tasking_float_slot;

__attribute__((section(".llvm_float32_data")))
volatile float llvm_float32_slot = 1.0f;

__attribute__((noinline, section(".llvm_float32_eval")))
float llvm_float32_eval(abi_u16 operation, float lhs, float rhs,
                        abi_u16 tail) {
  if (tail != 0x5a5aU)
    return __builtin_nanf("");

  switch (operation) {
  case 0: return lhs + rhs;
  case 1: return lhs - rhs;
  case 2: return lhs * rhs;
  default: return lhs / rhs;
  }
}

__attribute__((noinline, section(".llvm_float32_reverse")))
float llvm_float32_reverse(float value) {
  return tasking_float_roundtrip(0x1357U, value, 0x2468U);
}

__attribute__((noinline, section(".llvm_float32_load_external")))
float llvm_float32_load_external(void) {
  return tasking_float_slot;
}

__attribute__((noinline, section(".llvm_float32_store_external")))
void llvm_float32_store_external(float value) {
  tasking_float_slot = value;
}

__attribute__((noinline, section(".llvm_float32_load_own")))
float llvm_float32_load_own(void) {
  return llvm_float32_slot;
}

__attribute__((noinline, section(".llvm_float32_store_own")))
void llvm_float32_store_own(float value) {
  llvm_float32_slot = value;
}

__attribute__((noinline, section(".llvm_float32_slot0")))
float llvm_float32_slot0(float value, abi_u16 tail) {
  return tail == 0x5a5aU ? value : 12345.0f;
}

__attribute__((noinline, section(".llvm_float32_slot1")))
float llvm_float32_slot1(abi_u16 a, float value, abi_u16 tail) {
  return a == 0x1111U && tail == 0x5a5aU ? value : 12345.0f;
}

__attribute__((noinline, section(".llvm_float32_slot2")))
float llvm_float32_slot2(abi_u16 a, abi_u16 b, float value, abi_u16 tail) {
  return a == 0x1111U && b == 0x2222U && tail == 0x5a5aU
             ? value
             : 12345.0f;
}

__attribute__((noinline, section(".llvm_float32_slot3")))
float llvm_float32_slot3(abi_u16 a, abi_u16 b, abi_u16 c, float value,
                         abi_u16 tail) {
  return a == 0x1111U && b == 0x2222U && c == 0x3333U && tail == 0x5a5aU
             ? value
             : 12345.0f;
}

__attribute__((noinline, section(".llvm_float32_slot4")))
float llvm_float32_slot4(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                         float value, abi_u16 tail) {
  return a == 0x1111U && b == 0x2222U && c == 0x3333U && d == 0x4444U &&
                 tail == 0x5a5aU
             ? value
             : 12345.0f;
}
