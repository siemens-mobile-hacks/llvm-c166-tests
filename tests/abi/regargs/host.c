#include "c166-test-runtime.h"
volatile unsigned int simulator_result;

extern int llvm_entry_proxy(int lhs, int rhs);

void main(void) {
  simulator_result = llvm_entry_proxy(19, 23);
  simulator_stop();
}

