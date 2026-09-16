#include "kernel.inc"
#include "c166_test.h"

static volatile abi_s8 C166_FAR far_values[4];
static volatile abi_s8 C166_HUGE huge_values[4];

void main(void) {
  static const abi_u16 seeds[] = {0U, 1U, 0x7fU, 0x80U, 0xffU, 0x1234U};
  static const abi_u32 wide_values[] = {
      0x10203040UL, 0x01312151UL, 0x32021262UL,
      0x23130373UL, 0x54647404UL, 0x45756515UL};
  static const abi_u32 expected[] = {
      0x1018204cUL, 0x9890a811UL, 0x01093040UL,
      0x8981b9deUL, 0x323a027cUL, 0xbab28be4UL};
  abi_u16 index;

  for (index = 0U; index != 4U; ++index) {
    far_values[index] = (abi_s8)(index * 37U - 60U);
    huge_values[index] = (abi_s8)(index * 29U - 50U);
  }

  tap_plan(6U);
  for (index = 0U; index != 6U; ++index)
    tap_is_u32(c166_pseudo_eval(seeds[index], &far_values[index & 3U],
                               &huge_values[(index + 1U) & 3U],
                               wide_values[index]),
               expected[index], "pseudo expansion");
}
