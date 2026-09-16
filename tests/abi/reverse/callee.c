#include "c166_test.h"

C166_NOINLINE
unsigned int c166_five(unsigned int a, unsigned int b, unsigned int c,
                       unsigned int d, unsigned int e) {
  return a + 2U * b + 3U * c + 4U * d + 5U * e;
}
