#include "functions.h"
#include "vectors.inc"

static volatile abi_u16 cell;

static abi_u16 callback(abi_u16 value) {
  return value ^ 0x5aa5U;
}

#define ABI_DISPATCH_DIRECT(prefix)                                          \
  switch (shape) {                                                           \
  case 0: return prefix##s8_0(signed_byte, tail);                            \
  case 1: return prefix##s8_1(p0, signed_byte, tail);                        \
  case 2: return prefix##s8_2(p0, p1, signed_byte, tail);                    \
  case 3: return prefix##s8_3(p0, p1, p2, signed_byte, tail);                \
  case 4: return prefix##s8_4(p0, p1, p2, p3, signed_byte, tail);            \
  case 5: return prefix##u8_0(unsigned_byte, tail);                          \
  case 6: return prefix##u8_1(p0, unsigned_byte, tail);                      \
  case 7: return prefix##u8_2(p0, p1, unsigned_byte, tail);                  \
  case 8: return prefix##u8_3(p0, p1, p2, unsigned_byte, tail);              \
  case 9: return prefix##u8_4(p0, p1, p2, p3, unsigned_byte, tail);          \
  case 10: return prefix##u32_0(long_value, tail);                           \
  case 11: return prefix##u32_1(p0, long_value, tail);                       \
  case 12: return prefix##u32_2(p0, p1, long_value, tail);                   \
  case 13: return prefix##u32_3(p0, p1, p2, long_value, tail);               \
  case 14: return prefix##ptr_0(address, tail);                              \
  case 15: return prefix##ptr_1(p0, address, tail);                          \
  case 16: return prefix##ptr_2(p0, p1, address, tail);                      \
  case 17: return prefix##ptr_3(p0, p1, p2, address, tail);                  \
  case 18: return prefix##fn_0(function, tail);                              \
  case 19: return prefix##fn_1(p0, function, tail);                          \
  case 20: return prefix##fn_2(p0, p1, function, tail);                      \
  default: return prefix##fn_3(p0, p1, p2, function, tail);                 \
  }

static abi_u32 call_direct(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail) {
  ABI_DISPATCH_DIRECT(mixed_)
}

#undef ABI_DISPATCH_DIRECT

static abi_u32 golden_for_shape(abi_u16 shape) {
  switch (shape) {
#define ABI_GOLDEN_CASE(id, value) case id: return value;
  ABI_MIXED_SLOT_GOLDENS(ABI_GOLDEN_CASE)
#undef ABI_GOLDEN_CASE
  default: return 0UL;
  }
}

void main(void) {
  abi_u16 shape;

  tap_plan(52U);
  cell = ABI_MIXED_SLOT_CELL;
  for (shape = 0; shape < 22U; ++shape) {
    abi_u32 expected = golden_for_shape(shape);
    tap_is_u32(call_direct(
                   shape, ABI_MIXED_SLOT_P0, ABI_MIXED_SLOT_P1,
                   ABI_MIXED_SLOT_P2, ABI_MIXED_SLOT_P3, ABI_MIXED_SLOT_S8,
                   ABI_MIXED_SLOT_U8, ABI_MIXED_SLOT_U32, &cell, callback,
                   ABI_MIXED_SLOT_TAIL),
               expected, "mixed-slot direct call");
    tap_is_u32(mixed_dispatch(
                   shape, ABI_MIXED_SLOT_P0, ABI_MIXED_SLOT_P1,
                   ABI_MIXED_SLOT_P2, ABI_MIXED_SLOT_P3, ABI_MIXED_SLOT_S8,
                   ABI_MIXED_SLOT_U8, ABI_MIXED_SLOT_U32, &cell, callback,
                   ABI_MIXED_SLOT_TAIL),
               expected, "mixed-slot separate caller");
  }

#define CHECK_ENUM_VECTOR(id, a, b, c, d, e, expected)                      \
  tap_is_u32(mixed_enum5((enum abi_enum)(a), (enum abi_enum)(b),            \
                         (enum abi_enum)(c), (enum abi_enum)(d),             \
                         (enum abi_enum)(e)),                                \
             (expected), "enum slots direct call");                        \
  tap_is_u32(mixed_enum5_dispatch(                                           \
                 (enum abi_enum)(a), (enum abi_enum)(b),                     \
                 (enum abi_enum)(c), (enum abi_enum)(d),                     \
                 (enum abi_enum)(e)),                                        \
             (expected), "enum slots separate caller");

  ABI_ENUM_SLOT_VECTORS(CHECK_ENUM_VECTOR)
#undef CHECK_ENUM_VECTOR
}
