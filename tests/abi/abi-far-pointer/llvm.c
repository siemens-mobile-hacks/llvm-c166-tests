__attribute__((noinline))
volatile unsigned int *llvm_entry(unsigned int tag0, unsigned int tag1,
                                  volatile unsigned int *address,
                                  unsigned int value) {
  *address = (value ^ tag0) + tag1;
  return address;
}
