#include "c166_test.h"
#include "vectors.inc"

C166_TEST_NOINLINE
unsigned int c166_test_entry(unsigned long pair, unsigned int word14,
                             signed char byte15, unsigned int stack0,
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

struct pair_r12_vector {
  unsigned long pair;
  unsigned int word14;
  signed char byte15;
  unsigned int stack0;
  unsigned long stack1;
};

#define PAIR_R12_VECTOR(id, pair, word14, byte15, stack0, stack1, expected) \
  {pair, word14, byte15, stack0, stack1},

static const struct pair_r12_vector vectors[] = {
    ABI_PAIR_R12_VECTORS(PAIR_R12_VECTOR)
};

unsigned long c166_test_case(unsigned int case_id) {
  const struct pair_r12_vector *vector = &vectors[case_id];
  return c166_test_entry(vector->pair, vector->word14, vector->byte15,
                         vector->stack0, vector->stack1);
}
