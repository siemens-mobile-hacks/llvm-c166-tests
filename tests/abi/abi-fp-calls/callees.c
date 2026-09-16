#include "types.h"

C166_NOINLINE C166_SECTION(".abi.fp.float")
float abi_fp_float(abi_u16 tag, double first, float value, abi_u16 tail,
                   abi_u8 *out) {
  abi_u16 index;

  for (index = 0U; index != 8U; ++index)
    out[index] = ((abi_u8 *)&first)[index];
  for (index = 0U; index != 4U; ++index)
    out[8U + index] = ((abi_u8 *)&value)[index];
  out[12] = (abi_u8)tag;
  out[13] = (abi_u8)(tag >> 8);
  out[14] = (abi_u8)tail;
  out[15] = (abi_u8)(tail >> 8);
  return value;
}

C166_NOINLINE C166_SECTION(".abi.fp.double")
double abi_fp_double(abi_u16 tag, float first, double value, abi_u16 tail,
                     abi_u8 *out) {
  abi_u16 index;

  for (index = 0U; index != 4U; ++index)
    out[index] = ((abi_u8 *)&first)[index];
  for (index = 0U; index != 8U; ++index)
    out[4U + index] = ((abi_u8 *)&value)[index];
  out[12] = (abi_u8)tag;
  out[13] = (abi_u8)(tag >> 8);
  out[14] = (abi_u8)tail;
  out[15] = (abi_u8)(tail >> 8);
  return value;
}
