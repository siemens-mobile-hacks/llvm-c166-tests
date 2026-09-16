#include "c166_test.h"
#include "types.h"

C166_NOINLINE C166_SECTION(".seeded_f32_to_i32")
abi_s32 c166_f32_to_i32(float value) {
  return (abi_s32)value;
}

C166_NOINLINE C166_SECTION(".seeded_f32_to_u32")
abi_u32 c166_f32_to_u32(float value) {
  return (abi_u32)value;
}
