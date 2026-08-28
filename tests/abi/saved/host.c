volatile unsigned int simulator_result;

extern unsigned int llvm_entry_saved_proxy(unsigned int a, unsigned int b,
                                           unsigned int c, unsigned int d);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = llvm_entry_saved_proxy(1, 2, 3, 4);
  simulator_stop();
}

