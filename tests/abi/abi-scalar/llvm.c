__attribute__((noinline))
unsigned long llvm_entry(unsigned int head, unsigned long pair,
                         signed char register_tail, unsigned int stack0,
                         unsigned long stack1, unsigned int stack2) {
  unsigned long value = pair;
  value += (unsigned long)head << 16;
  value += (long)register_tail;
  value ^= (unsigned long)stack0 << 1;
  value += stack1;
  value ^= ((unsigned long)stack2 << 16) | stack0;
  return value;
}
