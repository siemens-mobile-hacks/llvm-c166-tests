struct pair {
  unsigned int low;
  unsigned int high;
};

volatile unsigned int simulator_result;

extern unsigned int llvm_entry_proxy(unsigned int head, struct pair value,
                                     unsigned int tail);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  struct pair value;
  value.low = 11;
  value.high = 13;
  simulator_result = llvm_entry_proxy(7, value, 17);
  simulator_stop();
}

