volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(void);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = llvm_entry_proxy();
  simulator_stop();
}

