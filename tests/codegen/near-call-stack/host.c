#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned int llvm_entry_proxy(unsigned int word, unsigned int flags,
                                    unsigned int expected_flags);

void main(void) {
  static const unsigned int words[] = {
    0, 1, 0x7fffU, 0x8000U, 0xffffU, 0xa55aU, 0x5555U, 0xfffeU
  };
  unsigned int i, flags, expected_flags, actual;
  c166_test_begin(367, 0x166UL);
  for (i = 0; i != sizeof(words) / sizeof(words[0]); ++i)
    for (flags = 0; flags != 32; ++flags) {
      /* PCALL/RETP preserve C/V and derive N/Z/E from the saved word. */
      expected_flags = (flags & 6U) | ((words[i] >> 15) & 1U);
      if (words[i] == 0) expected_flags |= 8U;
      if (words[i] == 0x8000U) expected_flags |= 16U;
      actual = llvm_entry_proxy(words[i], flags, expected_flags);
      c166_test_check_u32(i * 32U + flags + 1U, 0, actual);
      if (actual != 0) goto done;
    }
done:
  c166_test_finish();
  simulator_stop();
}
