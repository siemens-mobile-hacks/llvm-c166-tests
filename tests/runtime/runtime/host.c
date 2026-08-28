volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(unsigned long a, unsigned long b,
                                     unsigned int shift);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = llvm_entry_proxy(0x87654321UL, 0x1234UL, 7);
  simulator_stop();
}

