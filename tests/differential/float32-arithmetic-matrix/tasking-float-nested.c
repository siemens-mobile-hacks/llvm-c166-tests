#include "types.h"

extern float tasking_float_slot3(abi_u16, abi_u16, abi_u16, float, abi_u16);

float tasking_float_nested_slot3(abi_u16 a, abi_u16 b, abi_u16 c,
                                 float value, abi_u16 tail) {
  return tasking_float_slot3(a, b, c, value, tail);
}
