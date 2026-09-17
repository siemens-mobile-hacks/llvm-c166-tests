#include "c166_test.h"
#include "types.h"

C166_TEST_NOINLINE
abi_s32 c166_f32_to_i32(float value) { return (abi_s32)value; }

C166_TEST_NOINLINE
abi_u32 c166_f32_to_u32(float value) { return (abi_u32)value; }

C166_TEST_NOINLINE
abi_s16 c166_f32_to_i16(float value) { return (abi_s16)value; }

C166_TEST_NOINLINE
abi_u16 c166_f32_to_u16(float value) { return (abi_u16)value; }

C166_TEST_NOINLINE
float c166_i32_to_f32(abi_s32 value) { return (float)value; }

C166_TEST_NOINLINE
float c166_u32_to_f32(abi_u32 value) { return (float)value; }

C166_TEST_NOINLINE
float c166_i16_to_f32(abi_s16 value) { return (float)value; }

C166_TEST_NOINLINE
float c166_u16_to_f32(abi_u16 value) { return (float)value; }

C166_TEST_NOINLINE
abi_s16 c166_f32_compare(abi_u16 operation, float lhs, float rhs) {
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
    return lhs != lhs || rhs != rhs;
  }
}

#if defined(C166_TEST_LLVM)
extern abi_s32 __lesf2(abi_u32, abi_u32);
extern abi_s32 __gesf2(abi_u32, abi_u32);
extern abi_s32 __unordsf2(abi_u32, abi_u32);

C166_TEST_NOINLINE
abi_s32 c166_f32_compare_runtime(abi_u16 operation, abi_u32 lhs, abi_u32 rhs) {
  switch (operation) {
  case 0:
    return __lesf2(lhs, rhs);
  case 1:
    return __gesf2(lhs, rhs);
  default:
    return __unordsf2(lhs, rhs);
  }
}
#endif
