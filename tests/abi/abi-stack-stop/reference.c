unsigned long tasking_stop_reference(unsigned int a, unsigned int b,
                                     unsigned int c, unsigned long pair,
                                     unsigned int tail) {
  unsigned long value = pair;
  value += (unsigned long)a << 16;
  value += ((unsigned long)b << 1) + c;
  value ^= ((unsigned long)tail << 16) | b;
  return value;
}
