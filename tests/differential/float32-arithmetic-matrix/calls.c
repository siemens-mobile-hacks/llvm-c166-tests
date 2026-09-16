#include "c166_test.h"
#include "types.h"

extern float c166_float32_slot3(abi_u16, abi_u16, abi_u16, float, abi_u16);

C166_TEST_NOINLINE
float c166_float32_roundtrip(abi_u16 head, float value, abi_u16 tail) {
  return head == 0x1357U && tail == 0x2468U ? value : 12345.0f;
}

C166_TEST_NOINLINE
float c166_float32_call_slot3(abi_u16 a, abi_u16 b, abi_u16 c, float value,
                              abi_u16 tail) {
  return c166_float32_slot3(a, b, c, value, tail);
}

C166_TEST_NOINLINE
float c166_float32_icall_slot3(abi_float_slot3_fn callee, abi_u16 a, abi_u16 b,
                               abi_u16 c, float value, abi_u16 tail) {
  return callee(a, b, c, value, tail);
}

C166_TEST_NOINLINE
float c166_float32_cross_tu(abi_u16 a, abi_u16 b, abi_u16 c, float value,
                            abi_u16 tail) {
  return c166_float32_slot3(a, b, c, value, tail);
}

C166_TEST_NOINLINE
float c166_float32_nested_slot3(abi_u16 a, abi_u16 b, abi_u16 c, float value,
                                abi_u16 tail) {
  return c166_float32_call_slot3(a, b, c, value, tail);
}
