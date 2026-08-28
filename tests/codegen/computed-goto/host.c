volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(unsigned int selector);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = llvm_entry_proxy(0U);
  simulator_stop();
}

