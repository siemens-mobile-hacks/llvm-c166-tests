#include "c166-test-runtime.h"
volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(unsigned int);
extern unsigned int tasking_runtime_variant_probe(void);

void main(void) {
  unsigned int tasking_result = tasking_runtime_variant_probe();
  unsigned int llvm_result = llvm_entry_proxy(tasking_result);

  simulator_result = 0U;
  if (tasking_result != 0x003fU)
    simulator_result |= 1U;
  if (llvm_result != (0x003fU ^ 0x55aaU))
    simulator_result |= 2U;
  simulator_stop();
}
