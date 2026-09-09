#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int value, unsigned int bit_index,
                                     unsigned int flags, unsigned int form);
static const unsigned int words[] = {
    0, 0xffff, 0x5555, 0xaaaa, 1, 0x8000, 0x7fff, 0xfffe};

void main(void) {
  unsigned int form, bit_index, index, initial, value, mask, old, flags, result, id = 0;
  unsigned long want, actual;
#ifdef BIT_BRANCH_READ_ONLY
  c166_test_begin(384, 0x166UL);
#else
  c166_test_begin(374, 0x166UL);
#endif
  for (form = 0; form != 12; ++form)
    for (bit_index = 0; bit_index != 16; ++bit_index)
      for (index = 0; index != 8; ++index)
        for (initial = 0; initial != 32; ++initial) {
          value = words[index];
          mask = 1U << bit_index;
          old = (value & mask) != 0;
#ifdef BIT_BRANCH_READ_ONLY
          flags = initial;
          result = value;
#else
          flags = old ? 1U : 8U; /* N=old bit, Z=!old; E/V/C=0. */
          result = (form & 1U) ? value | mask : value & ~mask;
#endif
          if (old != (form & 1U)) flags |= 32U; /* Taken-path marker. */
          want = ((unsigned long)flags << 16) | result;
          c166_test_set_context(0x166UL, form, bit_index, value, initial);
          actual = llvm_entry_proxy(value, bit_index, initial, form);
          c166_test_check_u32(++id, want, actual);
          if (actual != want) goto done;
        }
done:
  c166_test_finish();
  simulator_stop();
}
