#include "c166_test.h"
#include "vectors.inc"

C166_TEST_NOINLINE
unsigned long c166_test_entry(unsigned int head, unsigned long pair,
                              signed char register_tail,
                              unsigned int stack0, unsigned long stack1,
                              unsigned int stack2) {
  unsigned long value = pair;
  value += (unsigned long)head << 16;
  value += (long)register_tail;
  value ^= (unsigned long)stack0 << 1;
  value += stack1;
  value ^= ((unsigned long)stack2 << 16) | stack0;
  return value;
}

struct scalar_vector {
  unsigned int head;
  unsigned long pair;
  signed char register_tail;
  unsigned int stack0;
  unsigned long stack1;
  unsigned int stack2;
};

#define SCALAR_VECTOR(id, head, pair, register_tail, stack0, stack1, stack2,  \
                      expected)                                               \
  {head, pair, register_tail, stack0, stack1, stack2},

static const struct scalar_vector vectors[] = {
    ABI_SCALAR_VECTORS(SCALAR_VECTOR)
};

unsigned long c166_test_case(unsigned int case_id) {
  const struct scalar_vector *vector = &vectors[case_id];
  return c166_test_entry(vector->head, vector->pair, vector->register_tail,
                         vector->stack0, vector->stack1, vector->stack2);
}
