#include "c166-test-runtime.h"
#include "types.h"
#include "c166-address-spaces.h"

volatile unsigned int simulator_result;
volatile abi_s8 far_values[4];
volatile abi_s8 _huge huge_values[4];

extern abi_u32 c166_pseudo_eval(abi_u16, volatile abi_s8 _far *,
                               volatile abi_s8 _huge *, abi_u32);
extern abi_u32 llvm_entry_proxy(abi_u16, volatile abi_s8 _far *,
                               volatile abi_s8 _huge *, abi_u32);

void main(void) {
  static const abi_u16 seeds[] = {0U, 1U, 0x7fU, 0x80U, 0xffU, 0x1234U};
  static const abi_u32 wide_values[] = {
      0x10203040UL, 0x01312151UL, 0x32021262UL,
      0x23130373UL, 0x54647404UL, 0x45756515UL};
  abi_u16 index;
  abi_u32 expected;
  abi_u32 actual;

  for (index = 0U; index < 4U; ++index) {
    far_values[index] = (abi_s8)(index * 37U - 60U);
    huge_values[index] = (abi_s8)(index * 29U - 50U);
  }
  simulator_result = 1U;
  for (index = 0U; index < 6U; ++index) {
    abi_u32 wide = wide_values[index];
    expected = c166_pseudo_eval(seeds[index], &far_values[index & 3U],
                                &huge_values[(index + 1U) & 3U], wide);
    actual = llvm_entry_proxy(seeds[index], &far_values[index & 3U],
                              &huge_values[(index + 1U) & 3U], wide);
    if (actual != expected)
      simulator_result = (abi_u16)(0x100U + index);
  }
  if (simulator_result == 1U)
    simulator_result = 42U;
  simulator_stop();
}
