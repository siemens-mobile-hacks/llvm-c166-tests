extern unsigned int tasking_five(unsigned int a, unsigned int b,
                                 unsigned int c, unsigned int d,
                                 unsigned int e);

unsigned int llvm_entry(unsigned int seed) {
  return tasking_five(seed, 2, 3, 4, 5);
}

