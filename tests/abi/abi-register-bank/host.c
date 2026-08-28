volatile unsigned int simulator_result;

extern unsigned int run_llvm_interrupt(void);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = run_llvm_interrupt();
  simulator_stop();
}

