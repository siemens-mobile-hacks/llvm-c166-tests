#include "types.h"

float tasking_float_slot3(abi_u16 a, abi_u16 b, abi_u16 c, float value,
                          abi_u16 tail) {
  if (a == 0x1111U && b == 0x2222U && c == 0x3333U && tail == 0x5a5aU)
    return value;
  return 12345.0f;
}
