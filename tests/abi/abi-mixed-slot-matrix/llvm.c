#include "functions.h"

#define ABI_STRING_1(value) #value
#define ABI_STRING(value) ABI_STRING_1(value)
#define ABI_MIX_DEFINE(prefix, id, name, parameters, body) \
  __attribute__((noinline, section(".llvm_mixed" ABI_STRING(id)))) \
  abi_u32 prefix##name parameters { body; }

ABI_MIXED_FUNCTION_LIST(ABI_MIX_DEFINE, llvm_)

__attribute__((noinline, section(".llvm_reverse")))
abi_u32 llvm_reverse_mixed(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail) {
  switch (shape) {
  case 0: return tasking_s8_0(signed_byte, tail);
  case 1: return tasking_s8_1(p0, signed_byte, tail);
  case 2: return tasking_s8_2(p0, p1, signed_byte, tail);
  case 3: return tasking_s8_3(p0, p1, p2, signed_byte, tail);
  case 4: return tasking_s8_4(p0, p1, p2, p3, signed_byte, tail);
  case 5: return tasking_u8_0(unsigned_byte, tail);
  case 6: return tasking_u8_1(p0, unsigned_byte, tail);
  case 7: return tasking_u8_2(p0, p1, unsigned_byte, tail);
  case 8: return tasking_u8_3(p0, p1, p2, unsigned_byte, tail);
  case 9: return tasking_u8_4(p0, p1, p2, p3, unsigned_byte, tail);
  case 10: return tasking_u32_0(long_value, tail);
  case 11: return tasking_u32_1(p0, long_value, tail);
  case 12: return tasking_u32_2(p0, p1, long_value, tail);
  case 13: return tasking_u32_3(p0, p1, p2, long_value, tail);
  case 14: return tasking_ptr_0(address, tail);
  case 15: return tasking_ptr_1(p0, address, tail);
  case 16: return tasking_ptr_2(p0, p1, address, tail);
  case 17: return tasking_ptr_3(p0, p1, p2, address, tail);
  case 18: return tasking_fn_0(function, tail);
  case 19: return tasking_fn_1(p0, function, tail);
  case 20: return tasking_fn_2(p0, p1, function, tail);
  default: return tasking_fn_3(p0, p1, p2, function, tail);
  }
}

__attribute__((noinline, section(".llvm_enum5")))
abi_u32 llvm_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                   enum abi_enum d, enum abi_enum e) {
  ABI_ENUM5_BODY(a, b, c, d, e);
}

__attribute__((noinline, section(".llvm_reverse_enum5")))
abi_u32 llvm_reverse_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                           enum abi_enum d, enum abi_enum e) {
  return tasking_enum5(a, b, c, d, e);
}
