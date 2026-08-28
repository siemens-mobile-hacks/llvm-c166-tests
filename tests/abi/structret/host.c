struct pair {
  unsigned int low;
  unsigned int high;
};

volatile unsigned int simulator_result;

extern struct pair llvm_entry_proxy(unsigned int low, unsigned int high);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  struct pair value = llvm_entry_proxy(21, 34);
  simulator_result = value.low + 2 * value.high;
  simulator_stop();
}

