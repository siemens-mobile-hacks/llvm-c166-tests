#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int value, unsigned int flags,
                                     unsigned int form);
static const unsigned int words[] = {
    0, 1, 2, 0x7fff, 0x8000, 0xfffe, 0xffff, 0xa55a,
    0x0100, 0x0200, 0xfe00, 0x5a00};

void main(void) {
  unsigned int form, index, initial, value, source, flags, result, id = 0;
  unsigned long want, actual;
  c166_test_begin(372, 0x166UL);
  for (form = 0; form != 15; ++form)
    for (index = 0; index != sizeof(words) / sizeof(words[0]); ++index)
      for (initial = 0; initial != 32; ++initial) {
        value = words[index];
        /* MOV sets N/Z/E from the source, preserving C/V. */
        source = form < 6 ? value : value >> 8;
        flags = (initial & 6U) | (source >> (form < 6 ? 15 : 7));
        if (source == 0) flags |= 8U;
        if (source == (form < 6 ? 0x8000U : 0x80U)) flags |= 16U;
        result = form < 6 ? value : (value & 0xff00U) | (~value & 0xffU);
        if (form == 9)
          result = (~value & 0xff00U) | (value >> 8);
        /* A byte write to an SFR clears its complementary high byte. */
        if (form == 10 || form == 12 || form == 13)
          result = value >> 8;
        want = ((unsigned long)flags << 16) | result;
        c166_test_set_context(0x166UL, form, initial, value, 0);
        actual = llvm_entry_proxy(value, initial, form);
        c166_test_check_u32(++id, want, actual);
      }
  c166_test_finish();
  simulator_stop();
}
