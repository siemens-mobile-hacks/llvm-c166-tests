extern unsigned long tasking_stop(unsigned int a, unsigned int b,
                                  unsigned int c, unsigned long pair,
                                  unsigned int tail);

__attribute__((noinline))
unsigned long llvm_entry(unsigned int a, unsigned int b, unsigned int c,
                         unsigned long pair, unsigned int tail) {
  return tasking_stop(a, b, c, pair, tail) ^ 0x13579bdfUL;
}
