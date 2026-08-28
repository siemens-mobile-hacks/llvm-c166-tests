unsigned int llvm_entry(volatile unsigned int *address,
                        unsigned int value) {
  *address = value;
  return *address;
}

