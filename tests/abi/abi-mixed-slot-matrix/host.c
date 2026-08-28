#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "functions.h"
#include "vectors.inc"

extern abi_u32 llvm_reverse_mixed_proxy(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail);

static abi_u16 tasking_callback(abi_u16 value) {
  return value ^ 0x5aa5U;
}

#define ABI_DISPATCH(prefix) \
  switch (shape) { \
  case 0: return prefix##s8_0(signed_byte, tail); \
  case 1: return prefix##s8_1(p0, signed_byte, tail); \
  case 2: return prefix##s8_2(p0, p1, signed_byte, tail); \
  case 3: return prefix##s8_3(p0, p1, p2, signed_byte, tail); \
  case 4: return prefix##s8_4(p0, p1, p2, p3, signed_byte, tail); \
  case 5: return prefix##u8_0(unsigned_byte, tail); \
  case 6: return prefix##u8_1(p0, unsigned_byte, tail); \
  case 7: return prefix##u8_2(p0, p1, unsigned_byte, tail); \
  case 8: return prefix##u8_3(p0, p1, p2, unsigned_byte, tail); \
  case 9: return prefix##u8_4(p0, p1, p2, p3, unsigned_byte, tail); \
  case 10: return prefix##u32_0(long_value, tail); \
  case 11: return prefix##u32_1(p0, long_value, tail); \
  case 12: return prefix##u32_2(p0, p1, long_value, tail); \
  case 13: return prefix##u32_3(p0, p1, p2, long_value, tail); \
  case 14: return prefix##ptr_0(address, tail); \
  case 15: return prefix##ptr_1(p0, address, tail); \
  case 16: return prefix##ptr_2(p0, p1, address, tail); \
  case 17: return prefix##ptr_3(p0, p1, p2, address, tail); \
  case 18: return prefix##fn_0(function, tail); \
  case 19: return prefix##fn_1(p0, function, tail); \
  case 20: return prefix##fn_2(p0, p1, function, tail); \
  default: return prefix##fn_3(p0, p1, p2, function, tail); \
  }

static abi_u32 call_tasking(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail) {
  ABI_DISPATCH(tasking_)
}

static abi_u32 call_llvm(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail) {
  ABI_DISPATCH(llvm_proxy_)
}

#undef ABI_DISPATCH

static abi_u32 golden_for_shape(abi_u16 shape) {
  switch (shape) {
#define ABI_GOLDEN_CASE(id, value) case id: return value;
  ABI_MIXED_SLOT_GOLDENS(ABI_GOLDEN_CASE)
#undef ABI_GOLDEN_CASE
  default: return 0;
  }
}

void main(void) {
  volatile abi_u16 cell = ABI_MIXED_SLOT_CELL;
  abi_u16 shape;
  abi_u16 base;
  abi_u32 golden;
  abi_u32 actual;

  c166_test_begin(271, 0x1660010fUL);
  for (shape = 0; shape < 22; ++shape) {
    base = shape * 3;
    golden = golden_for_shape(shape);
    actual = call_tasking(
        shape, ABI_MIXED_SLOT_P0, ABI_MIXED_SLOT_P1, ABI_MIXED_SLOT_P2,
        ABI_MIXED_SLOT_P3, ABI_MIXED_SLOT_S8, ABI_MIXED_SLOT_U8,
        ABI_MIXED_SLOT_U32, &cell, tasking_callback, ABI_MIXED_SLOT_TAIL);
    c166_test_check_u32(base + 1, golden, actual);
    actual = call_llvm(
        shape, ABI_MIXED_SLOT_P0, ABI_MIXED_SLOT_P1, ABI_MIXED_SLOT_P2,
        ABI_MIXED_SLOT_P3, ABI_MIXED_SLOT_S8, ABI_MIXED_SLOT_U8,
        ABI_MIXED_SLOT_U32, &cell, tasking_callback, ABI_MIXED_SLOT_TAIL);
    c166_test_check_u32(base + 2, golden, actual);
    actual = llvm_reverse_mixed_proxy(
        shape, ABI_MIXED_SLOT_P0, ABI_MIXED_SLOT_P1, ABI_MIXED_SLOT_P2,
        ABI_MIXED_SLOT_P3, ABI_MIXED_SLOT_S8, ABI_MIXED_SLOT_U8,
        ABI_MIXED_SLOT_U32, &cell, tasking_callback, ABI_MIXED_SLOT_TAIL);
    c166_test_check_u32(base + 3, golden, actual);
  }

#define CHECK_ENUM_VECTOR(id, a, b, c, d, e, expected)                    \
  c166_test_check_u32(66U + (id - 1U) * 3U + 1U, expected,                \
      tasking_enum5((enum abi_enum)a, (enum abi_enum)b, (enum abi_enum)c,  \
                    (enum abi_enum)d, (enum abi_enum)e));                   \
  c166_test_check_u32(66U + (id - 1U) * 3U + 2U, expected,                \
      llvm_proxy_enum5((enum abi_enum)a, (enum abi_enum)b,                 \
                       (enum abi_enum)c, (enum abi_enum)d,                  \
                       (enum abi_enum)e));                                  \
  c166_test_check_u32(66U + (id - 1U) * 3U + 3U, expected,                \
      llvm_reverse_enum5_proxy((enum abi_enum)a, (enum abi_enum)b,         \
                               (enum abi_enum)c, (enum abi_enum)d,          \
                               (enum abi_enum)e));

  ABI_ENUM_SLOT_VECTORS(CHECK_ENUM_VECTOR)
#undef CHECK_ENUM_VECTOR
  c166_test_finish();
  simulator_stop();
}

