#include "c166_test.h"
#include "vectors.inc"

C166_TEST_NOINLINE
signed char c166_test_entry(signed char value) {
  return value;
}

#define BYTE_RETURN_VECTOR(id, value, expected) value,
static const signed char vectors[] = {
    ABI_BYTE_RETURN_VECTORS(BYTE_RETURN_VECTOR)
};

unsigned long c166_test_case(unsigned int case_id) {
  return (unsigned char)c166_test_entry(vectors[case_id]);
}
