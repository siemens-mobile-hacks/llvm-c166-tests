unsigned int tasking_pair_r12_reference(unsigned long pair,
                                        unsigned int word14,
                                        signed char byte15,
                                        unsigned int stack0,
                                        unsigned long stack1) {
  unsigned int result = (unsigned int)pair;
  result += (unsigned int)(pair >> 16) * 3U;
  result += word14 * 5U;
  result += byte15;
  result += stack0 * 7U;
  result += (unsigned int)stack1 * 11U;
  result += (unsigned int)(stack1 >> 16) * 13U;
  return result;
}
