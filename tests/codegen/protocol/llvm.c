extern unsigned long llvm_mix_helper(unsigned int a, unsigned long b,
                                     signed char c);

volatile unsigned long llvm_protocol_state;

__attribute__((noinline))
unsigned long llvm_entry(unsigned int a, unsigned long b, signed char c) {
  llvm_protocol_state = llvm_mix_helper(a, b, c) ^ 0x5aa5a55aUL;
  return llvm_protocol_state ^ 0x5aa5a55aUL;
}
