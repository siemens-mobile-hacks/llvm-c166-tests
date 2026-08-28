#include "types.h"

extern volatile float volatile_f32_a[4];
extern volatile float volatile_f32_b[4];
extern volatile double volatile_f64_a[4];
extern volatile double volatile_f64_b[4];

__attribute__((noinline, section(".llvm_volatile_f32_eval")))
float llvm_volatile_f32_eval(abi_u16 operation, abi_u16 index) {
  volatile float local;

  switch (operation) {
  case 0:
    return volatile_f32_a[index];
  case 1:
    volatile_f32_b[index] = volatile_f32_a[index];
    return volatile_f32_b[index];
  case 2:
    volatile_f32_b[index] += volatile_f32_a[index];
    return volatile_f32_b[index];
  case 3:
    volatile_f32_b[index] *= volatile_f32_a[index];
    return volatile_f32_b[index];
  case 4:
    local = volatile_f32_a[index];
    local += volatile_f32_b[index];
    return local;
  case 5:
    local = volatile_f32_a[index];
    local *= volatile_f32_b[index];
    return local;
  default:
    return __c11_atomic_is_lock_free(index ? 8U : 4U) ? 1.0f : 0.0f;
  }
}

__attribute__((noinline, section(".llvm_volatile_f64_eval")))
double llvm_volatile_f64_eval(abi_u16 operation, abi_u16 index) {
  volatile double local;

  switch (operation) {
  case 0:
    return volatile_f64_a[index];
  case 1:
    volatile_f64_b[index] = volatile_f64_a[index];
    return volatile_f64_b[index];
  case 2:
    volatile_f64_b[index] += volatile_f64_a[index];
    return volatile_f64_b[index];
  case 3:
    volatile_f64_b[index] *= volatile_f64_a[index];
    return volatile_f64_b[index];
  case 4:
    local = volatile_f64_a[index];
    local += volatile_f64_b[index];
    return local;
  default:
    local = volatile_f64_a[index];
    local *= volatile_f64_b[index];
    return local;
  }
}
