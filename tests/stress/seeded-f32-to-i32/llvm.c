#include "types.h"

__attribute__((noinline, section(".llvm_f32_to_i32")))
abi_s32 llvm_f32_to_i32(float value) { return (abi_s32)value; }

__attribute__((noinline, section(".llvm_f32_to_u32")))
abi_u32 llvm_f32_to_u32(float value) { return (abi_u32)value; }
