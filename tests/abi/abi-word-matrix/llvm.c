#include "functions.h"

#define ATTR0 __attribute__((noinline, section(".llvm_words0")))
#define ATTR1 __attribute__((noinline, section(".llvm_words1")))
#define ATTR2 __attribute__((noinline, section(".llvm_words2")))
#define ATTR3 __attribute__((noinline, section(".llvm_words3")))
#define ATTR4 __attribute__((noinline, section(".llvm_words4")))
#define ATTR5 __attribute__((noinline, section(".llvm_words5")))
#define ATTR6 __attribute__((noinline, section(".llvm_words6")))
#define ATTR7 __attribute__((noinline, section(".llvm_words7")))
#define ATTR8 __attribute__((noinline, section(".llvm_words8")))
#define ATTR9 __attribute__((noinline, section(".llvm_words9")))
#define ATTR10 __attribute__((noinline, section(".llvm_words10")))

DEFINE_WORD_FUNCTIONS(llvm_words, ATTR0, ATTR1, ATTR2, ATTR3, ATTR4, ATTR5,
                      ATTR6, ATTR7, ATTR8, ATTR9, ATTR10)

__attribute__((noinline, section(".llvm_reverse")))
abi_u32 llvm_reverse_words(abi_u16 shape, abi_u16 a0, abi_u16 a1,
                           abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
                           abi_u16 a6, abi_u16 a7, abi_u16 a8, abi_u16 a9) {
  switch (shape) {
  case 0: return tasking_words0();
  case 1: return tasking_words1(a0);
  case 2: return tasking_words2(a0, a1);
  case 3: return tasking_words3(a0, a1, a2);
  case 4: return tasking_words4(a0, a1, a2, a3);
  case 5: return tasking_words5(a0, a1, a2, a3, a4);
  case 6: return tasking_words6(a0, a1, a2, a3, a4, a5);
  case 7: return tasking_words7(a0, a1, a2, a3, a4, a5, a6);
  case 8: return tasking_words8(a0, a1, a2, a3, a4, a5, a6, a7);
  case 9: return tasking_words9(a0, a1, a2, a3, a4, a5, a6, a7, a8);
  default: return tasking_words10(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
}
