#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "c166-address-spaces.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

extern abi_u32 llvm_entry_proxy(
    abi_u16 seed, volatile abi_u32 _huge *huge_words,
    volatile abi_u32 _shuge *shuge_words);
extern abi_u32 tasking_qualifier_reference(
    abi_u16 seed, volatile abi_u32 _huge *huge_words,
    volatile abi_u32 _shuge *shuge_words);

static const abi_u16 seeds[] = {
    0x0000U, 0x0001U, 0x1234U, 0x7fffU,
    0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
};

void main(void) {
  volatile abi_u32 _huge *huge_reference =
      (volatile abi_u32 _huge *)0x40fff0UL;
  volatile abi_u32 _huge *huge_actual =
      (volatile abi_u32 _huge *)0x42fff0UL;
  volatile abi_u32 _shuge *shuge_reference =
      (volatile abi_u32 _shuge *)0x440100UL;
  volatile abi_u32 _shuge *shuge_actual =
      (volatile abi_u32 _shuge *)0x460100UL;
  abi_u16 vector;
  abi_u16 index;

  c166_test_begin(304, 0x16606875UL);
  for (vector = 0; vector != 8; ++vector) {
    abi_u32 reference = tasking_qualifier_reference(
        seeds[vector], huge_reference, shuge_reference);
    abi_u32 actual = llvm_entry_proxy(
        seeds[vector], huge_actual, shuge_actual);
    for (index = 0; index != 20; ++index) {
      c166_test_check_u32((abi_u16)(vector * 30U + index + 1U),
                          huge_reference[index], huge_actual[index]);
    }
    for (index = 0; index != 8; ++index) {
      c166_test_check_u32((abi_u16)(vector * 30U + index + 21U),
                          shuge_reference[index], shuge_actual[index]);
    }
    c166_test_check_u32((abi_u16)(vector * 30U + 29U), 1,
                        huge_reference != huge_actual &&
                        shuge_reference != shuge_actual);
    c166_test_check_u32((abi_u16)(vector * 30U + 30U), reference, actual);
  }
  c166_test_finish();
  simulator_stop();
}

