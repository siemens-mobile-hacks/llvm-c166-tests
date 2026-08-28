#include "c166-test-runtime.h"
volatile unsigned int simulator_result;

extern unsigned int run_llvm_interrupt(void);

void main(void) {
  simulator_result = run_llvm_interrupt();
  simulator_stop();
}

