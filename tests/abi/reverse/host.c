volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(unsigned int seed);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = llvm_entry_proxy(1);
  simulator_stop();
}

