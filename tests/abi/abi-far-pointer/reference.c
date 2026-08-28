volatile unsigned int *tasking_far_reference(
    unsigned int tag0, unsigned int tag1, volatile unsigned int *address,
    unsigned int value) {
  *address = (value ^ tag0) + tag1;
  return address;
}
