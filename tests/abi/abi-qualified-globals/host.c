volatile unsigned int simulator_result;

extern unsigned long llvm_entry_proxy(unsigned int seed);
extern unsigned long tasking_qualified_globals(unsigned int seed);

static const unsigned int seeds[] = {
    0x0000U, 0x0001U, 0x1234U, 0x7fffU,
    0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
};

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  unsigned int index;

  simulator_result = 42U;
  for (index = 0; index != 8; ++index) {
    unsigned long expected = tasking_qualified_globals(seeds[index]);
    unsigned long actual = llvm_entry_proxy(seeds[index]);
    if (actual != expected) {
      simulator_result = (unsigned int)(0x100U + index);
      break;
    }
  }
  simulator_stop();
}

