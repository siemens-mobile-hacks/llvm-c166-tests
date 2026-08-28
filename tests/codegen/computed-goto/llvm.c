__attribute__((noinline))
static unsigned int select_label(unsigned int selector) {
  static void *const labels[] = {&&first, &&second};
  goto *labels[selector & 1U];

first:
  return 11U;
second:
  return 22U;
}

unsigned int llvm_entry(unsigned int selector) {
  unsigned int first = select_label(selector);
  unsigned int second = select_label(selector + 1U);
  return (unsigned int)((first << 8) | second);
}
