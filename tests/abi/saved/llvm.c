static unsigned int helper(unsigned int a, unsigned int b, unsigned int c,
                           unsigned int d) __attribute__((noinline));

unsigned int llvm_entry(unsigned int a, unsigned int b, unsigned int c,
                        unsigned int d) {
  unsigned int x0 = a + 10;
  unsigned int x1 = b + 20;
  unsigned int x2 = c + 30;
  unsigned int x3 = d + 40;
  unsigned int x4 = a ^ 0x55;
  unsigned int x5 = b ^ 0xaa;
  unsigned int x6 = c + 5;
  unsigned int x7 = d + 6;
  unsigned int middle = helper(x0, x1, x2, x3);

  return middle + x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7;
}

static unsigned int helper(unsigned int a, unsigned int b, unsigned int c,
                           unsigned int d) {
  return a + 2 * b + 3 * c + 4 * d;
}

