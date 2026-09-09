#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int psw, unsigned int vector);
static const unsigned int vectors[] = {1, 63, 127};

void main(void) {
  unsigned int index, priority, flags, psw, id = 0;
  unsigned long actual, expected;
  c166_test_begin(375, 0x166UL);
  for (index = 0; index != 3; ++index)
    for (priority = 0; priority != 16; ++priority)
      for (flags = 0; flags != 32; ++flags) {
        psw = (priority << 12) | flags;
        expected = (unsigned long)psw << 16;
        c166_test_set_context(0x166UL, vectors[index], priority, flags, 0);
        actual = llvm_entry_proxy(psw, vectors[index]);
        c166_test_check_u32(++id, expected, actual);
        if (actual != expected) goto done;
      }
done:
  c166_test_finish();
  simulator_stop();
}
