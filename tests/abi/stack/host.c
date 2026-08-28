volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(unsigned int a, unsigned int b,
                                     unsigned int c, unsigned long d,
                                     unsigned int e);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = llvm_entry_proxy(1, 2, 3, 0x00040005UL, 6);
  simulator_stop();
}

