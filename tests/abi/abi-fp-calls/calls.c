#include "types.h"

C166_NOINLINE C166_SECTION(".abi.fp.reverse")
void abi_fp_reverse(abi_u16 indirect, float single, double wide,
                    abi_u8 *out) {
  abi_float_fn volatile float_target = abi_fp_float;
  abi_double_fn volatile double_target = abi_fp_double;
  float float_result;
  double double_result;
  abi_u16 index;

  if (indirect) {
    float_result = float_target(0x1357U, wide, single, 0x9bdfU, out);
    double_result =
        double_target(0x1357U, single, wide, 0x9bdfU, out + 20U);
  } else {
    float_result = abi_fp_float(0x1357U, wide, single, 0x9bdfU, out);
    double_result =
        abi_fp_double(0x1357U, single, wide, 0x9bdfU, out + 20U);
  }
  for (index = 0U; index != 4U; ++index)
    out[16U + index] = ((abi_u8 *)&float_result)[index];
  for (index = 0U; index != 8U; ++index)
    out[36U + index] = ((abi_u8 *)&double_result)[index];
}
