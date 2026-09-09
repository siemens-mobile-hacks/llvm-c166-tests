#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int value, unsigned int flags,
                                     unsigned int form);

void main(void) {
  unsigned int form, value, initial, flags, result, id;
  unsigned long want, actual;
  c166_test_begin(373, 0x166UL);
  for (form = 0; form != 10; ++form) {
    id = 0;
    for (value = 0; value != 256; ++value)
      for (initial = 0; initial != 32; ++initial) {
        result = value;
        flags = initial & 6U; /* C/V preserved; E cleared. */
        if (value == 0) flags |= 8U;
        if (form < 5 && value >= 128) {
          result |= 0xff00U;
          flags |= 1U;
        }
        want = ((unsigned long)flags << 16) | result;
        c166_test_set_context(0x166UL, form, initial, value, 0);
        actual = llvm_entry_proxy(value, initial, form);
        c166_test_check_u32(++id, want, actual);
        if (actual != want) goto done;
      }
  }
done:
  c166_test_finish();
  simulator_stop();
}
