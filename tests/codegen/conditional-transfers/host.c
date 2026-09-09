#include "c166-test-runtime.h"
#include "c166-test-result.h"
extern unsigned int llvm_entry_proxy(unsigned int condition,
                                    unsigned int flags, unsigned int form);

/* ISA condition table, independent of the assembly dispatch. */
static unsigned int condition_true(unsigned int cc, unsigned int flags) {
  unsigned int n = flags & 1U;
  unsigned int c = (flags >> 1) & 1U;
  unsigned int v = (flags >> 2) & 1U;
  unsigned int z = (flags >> 3) & 1U;
  unsigned int e = (flags >> 4) & 1U;
  switch (cc) {
  case 0: return 1;
  case 1: return !(z || e);
  case 2: return z;
  case 3: return !z;
  case 4: return v;
  case 5: return !v;
  case 6: return n;
  case 7: return !n;
  case 8: return c;
  case 9: return !c;
  case 10: return !(z || (n ^ v));
  case 11: return z || (n ^ v);
  case 12: return n ^ v;
  case 13: return !(n ^ v);
  case 14: return !(z || c);
  default: return z || c;
  }
}

void main(void) {
  unsigned int cc, flags, form, expected, actual;
  c166_test_begin(366, 0x166UL);
  for (form = 0; form != 6; ++form)
    for (cc = 0; cc != 16; ++cc)
      for (flags = 0; flags != 32; ++flags) {
        expected = flags | (condition_true(cc, flags) << 8);
        actual = llvm_entry_proxy(cc, flags, form);
        c166_test_check_u32(1U + (form * 16U + cc) * 32U + flags,
                           expected, actual);
        if (actual != expected) {
          goto done;
        }
      }
done:
  c166_test_finish();
  simulator_stop();
}
