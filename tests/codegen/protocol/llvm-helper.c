__attribute__((noinline))
unsigned long llvm_mix_helper(unsigned int a, unsigned long b, signed char c) {
  return (b ^ ((unsigned long)a << 16)) + (long)c;
}
