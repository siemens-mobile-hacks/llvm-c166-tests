#include "c166_test.h"
#include "kernel.inc"

static const abi_u16 seeds[] = {
    0x0000U, 0x0001U, 0x1234U, 0x7fffU,
    0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
};

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 value) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

static abi_u32 near_linear_address(abi_u16 direct, abi_u16 dpp) {
  return ((abi_u32)dpp << 14) | (direct & 0x3fffU);
}

static abi_u32 normalized_near_address(abi_u32 address) {
  return (address & 0x3fffUL) | 0x4000UL;
}

void main(void) {
  volatile abi_u32 C166_HUGE *huge_reference =
      (volatile abi_u32 C166_HUGE *)0x40fff0UL;
  volatile abi_u32 C166_HUGE *huge_actual =
      (volatile abi_u32 C166_HUGE *)0x42fff0UL;
  volatile abi_u32 C166_SHUGE *shuge_reference =
      (volatile abi_u32 C166_SHUGE *)0x440100UL;
  volatile abi_u32 C166_SHUGE *shuge_actual =
      (volatile abi_u32 C166_SHUGE *)0x460100UL;
  abi_u32 signature = 0x16606875UL;
  abi_u16 vector;
  abi_u16 index;

  tap_plan(305U);
  for (vector = 0; vector != 8; ++vector) {
    abi_u16 base = vector * 30U;
    abi_u16 near_raw = (abi_u16)(0x8010U + (seeds[vector] & 0x000fU));
    abi_u16 xnear_raw = (abi_u16)(0x4010U + (seeds[vector] & 0x000fU));
    abi_u32 near_linear = near_linear_address(near_raw, C166_DPP2_VALUE);
    abi_u32 xnear_linear = near_linear_address(xnear_raw, C166_DPP1_VALUE);
    abi_u32 reference = huge_pointer_kernel(
        seeds[vector], huge_reference, shuge_reference);
    abi_u32 actual = huge_pointer_kernel(
        seeds[vector], huge_actual, shuge_actual);

    for (index = 0; index != 20; ++index) {
      abi_u32 signature_value = huge_actual[index];

      tap_is_u32(huge_actual[index], huge_reference[index], "huge word");
      if (index >= 11U && index <= 18U)
        signature_value = normalized_near_address(signature_value);
      signature =
          signature_step(signature, base + index + 1U, signature_value);
    }
    for (index = 0; index != 8; ++index) {
      tap_is_u32(shuge_actual[index], shuge_reference[index], "shuge word");
      signature = signature_step(signature, base + index + 21U,
                                 shuge_actual[index]);
    }
    tap_ok(huge_reference != huge_actual &&
               shuge_reference != shuge_actual,
           "distinct pointer ranges");
    signature = signature_step(signature, base + 29U, 1U);
    tap_is_u32(actual, reference, "kernel digest");
    signature = signature_step(signature, base + 30U, actual);

    tap_is_u32(huge_actual[11], near_linear, "near to huge");
    tap_is_u32(huge_actual[12], near_linear, "near to shuge");
    tap_is_u32(huge_actual[13], xnear_linear, "xnear to huge");
    tap_is_u32(huge_actual[14], xnear_linear, "xnear to shuge");
    tap_is_u32(huge_actual[15], (abi_u16)near_linear, "huge to near");
    tap_is_u32(huge_actual[16], (abi_u16)near_linear, "shuge to near");
    tap_is_u32(huge_actual[17], (abi_u16)xnear_linear, "huge to xnear");
    tap_is_u32(huge_actual[18], (abi_u16)xnear_linear, "shuge to xnear");
  }
  tap_is_u32(signature, 0x59bd39cbUL, "golden signature");
}
