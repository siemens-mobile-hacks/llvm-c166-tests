#include "c166-test-runtime.h"
volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(void);

void main(void) {
  simulator_result = llvm_entry_proxy();
  simulator_stop();
}

