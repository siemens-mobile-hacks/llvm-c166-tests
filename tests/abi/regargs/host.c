volatile unsigned int simulator_result;

extern int llvm_entry_proxy(int lhs, int rhs);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  simulator_result = llvm_entry_proxy(19, 23);
  simulator_stop();
}

