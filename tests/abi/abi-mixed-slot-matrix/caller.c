#include "functions.h"

C166_NOINLINE C166_SECTION(".abi_mixed_dispatch")
abi_u32 mixed_dispatch(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail) {
  switch (shape) {
  case 0: return mixed_s8_0(signed_byte, tail);
  case 1: return mixed_s8_1(p0, signed_byte, tail);
  case 2: return mixed_s8_2(p0, p1, signed_byte, tail);
  case 3: return mixed_s8_3(p0, p1, p2, signed_byte, tail);
  case 4: return mixed_s8_4(p0, p1, p2, p3, signed_byte, tail);
  case 5: return mixed_u8_0(unsigned_byte, tail);
  case 6: return mixed_u8_1(p0, unsigned_byte, tail);
  case 7: return mixed_u8_2(p0, p1, unsigned_byte, tail);
  case 8: return mixed_u8_3(p0, p1, p2, unsigned_byte, tail);
  case 9: return mixed_u8_4(p0, p1, p2, p3, unsigned_byte, tail);
  case 10: return mixed_u32_0(long_value, tail);
  case 11: return mixed_u32_1(p0, long_value, tail);
  case 12: return mixed_u32_2(p0, p1, long_value, tail);
  case 13: return mixed_u32_3(p0, p1, p2, long_value, tail);
  case 14: return mixed_ptr_0(address, tail);
  case 15: return mixed_ptr_1(p0, address, tail);
  case 16: return mixed_ptr_2(p0, p1, address, tail);
  case 17: return mixed_ptr_3(p0, p1, p2, address, tail);
  case 18: return mixed_fn_0(function, tail);
  case 19: return mixed_fn_1(p0, function, tail);
  case 20: return mixed_fn_2(p0, p1, function, tail);
  default: return mixed_fn_3(p0, p1, p2, function, tail);
  }
}

C166_NOINLINE C166_SECTION(".abi_mixed_enum5_dispatch")
abi_u32 mixed_enum5_dispatch(enum abi_enum a, enum abi_enum b,
                             enum abi_enum c, enum abi_enum d,
                             enum abi_enum e) {
  return mixed_enum5(a, b, c, d, e);
}
