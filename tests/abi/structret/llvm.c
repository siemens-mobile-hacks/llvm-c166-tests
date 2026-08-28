struct pair {
  unsigned int low;
  unsigned int high;
};

struct pair llvm_entry(unsigned int low, unsigned int high) {
  struct pair result;
  result.low = low;
  result.high = high;
  return result;
}

