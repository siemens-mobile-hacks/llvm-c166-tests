#include "c166-test-compat.h"

C166_TEST_NOINLINE
volatile unsigned int *c166_test_entry(
    unsigned int tag0, unsigned int tag1, volatile unsigned int *address,
    unsigned int value) {
  *address = (value ^ tag0) + tag1;
  return address;
}
