#include "c166-test-runtime.h"
volatile unsigned int simulator_result;
volatile unsigned int far_value;

extern unsigned int llvm_entry_proxy(volatile unsigned int *address,
                                     unsigned int value);

void main(void) {
  simulator_result = llvm_entry_proxy(&far_value, 0x1234);
  simulator_stop();
}

