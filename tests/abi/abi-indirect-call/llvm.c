typedef unsigned long (*classic_function)(unsigned int a, unsigned int b,
                                          unsigned int c, unsigned long pair,
                                          unsigned int tail);

__attribute__((noinline))
unsigned long llvm_entry(unsigned int prefix, classic_function target,
                         unsigned int a, unsigned int b, unsigned int c,
                         unsigned long pair, unsigned int tail) {
  return target(a, b, c, pair, tail) ^ prefix;
}
