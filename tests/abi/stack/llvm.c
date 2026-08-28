unsigned int llvm_entry(unsigned int a, unsigned int b, unsigned int c,
                        unsigned long d, unsigned int e) {
  return a + 3 * b + 5 * c + 7 * (unsigned int)d +
         11 * (unsigned int)(d >> 16) + 13 * e;
}

