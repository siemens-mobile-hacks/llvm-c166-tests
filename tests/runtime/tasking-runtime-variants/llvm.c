unsigned int llvm_runtime_variant_probe(unsigned int value) {
  return value ^ 0x55aaU;
}
