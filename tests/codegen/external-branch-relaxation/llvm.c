extern unsigned int relax_probe(unsigned int value);

unsigned int llvm_entry(unsigned int unused) {
  unsigned int zero = relax_probe(0);
  unsigned int nonzero = relax_probe(1);

  (void)unused;
  return zero == 0x1234U && nonzero == 0x5678U ? 0xA55AU : 0xDEADU;
}
