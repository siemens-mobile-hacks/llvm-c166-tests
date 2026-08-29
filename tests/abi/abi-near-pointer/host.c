#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "c166-address-spaces.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

extern abi_u32 llvm_entry_proxy(
    abi_u16 seed, volatile abi_u32 _near *near_words,
    volatile abi_u32 _xnear *xnear_words);
extern abi_u32 c166_test_entry(
    abi_u16 seed, volatile abi_u32 _near *near_words,
    volatile abi_u32 _xnear *xnear_words);

volatile abi_u32 _near near_reference[8];
volatile abi_u32 _near near_actual[8];
volatile abi_u32 _xnear xnear_reference[8];
volatile abi_u32 _xnear xnear_actual[8];

static const abi_u16 seeds[] = {
    0x0000U, 0x0001U, 0x1234U, 0x7fffU,
    0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
};

void main(void) {
  abi_u16 vector;
  abi_u16 index;

  c166_test_begin(278, 0x16604e58UL);
  for (vector = 0; vector != 8; ++vector) {
    abi_u32 reference = c166_test_entry(
        seeds[vector], near_reference, xnear_reference);
    abi_u32 actual = llvm_entry_proxy(
        seeds[vector], near_actual, xnear_actual);
    c166_test_check_u32((abi_u16)(vector * 18U + 1U), reference, actual);
    for (index = 0; index != 8; ++index) {
      c166_test_check_u32((abi_u16)(vector * 18U + index * 2U + 2U),
                          near_reference[index], near_actual[index]);
      c166_test_check_u32((abi_u16)(vector * 18U + index * 2U + 3U),
                          xnear_reference[index], xnear_actual[index]);
    }
    c166_test_check_u32((abi_u16)(vector * 18U + 18U), 1,
                        near_reference != near_actual &&
                        xnear_reference != xnear_actual);
  }
  c166_test_finish();
  simulator_stop();
}
