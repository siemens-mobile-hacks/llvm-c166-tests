struct pair {
  unsigned int low;
  unsigned int high;
};

unsigned int llvm_entry(unsigned int head, struct pair value,
                        unsigned int tail) {
  return head + 2 * value.low + 3 * value.high + 4 * tail;
}

