#include "c166-test-runtime.h"

volatile unsigned int simulator_result;
extern unsigned int snapshot_eval(unsigned int);
extern unsigned int llvm_entry_proxy(unsigned int);

void main(void) {
  unsigned int mode;
  simulator_result = 1U;
  for (mode = 0U; mode != 4U; ++mode) {
    unsigned int expected = snapshot_eval(mode);
    unsigned int actual = llvm_entry_proxy(mode);
    if (expected != 1U || actual != expected) {
      simulator_result = 0x100U + mode;
      simulator_stop();
      return;
    }
  }
  simulator_result = 42U;
  simulator_stop();
}
