unsigned long tasking_mix_reference(unsigned int a, unsigned long b,
                                    signed char c) {
  return (b ^ ((unsigned long)a << 16)) + (long)c;
}
