#include "c166-test-compat.h"
#include "operations.h"
#include "vectors.inc"

DEFINE_INTEGER16_EVAL(
    integer16_eval,
    C166_TEST_NOINLINE)

struct integer16_vector {
  abi_u16 a;
  abi_u16 b;
  abi_u16 count;
};

#define INTEGER16_VECTOR(id, a, b, count) {a, b, count},
static const struct integer16_vector vectors[] = {
    ABI_INTEGER16_VECTORS(INTEGER16_VECTOR)
};

C166_TEST_NOINLINE
abi_u32 c166_test_case(abi_u16 case_id) {
  const struct integer16_vector *vector =
      &vectors[case_id / ABI_INTEGER16_OP_COUNT];
  abi_u16 operation = case_id % ABI_INTEGER16_OP_COUNT;
  return integer16_eval(operation, vector->a, vector->b, vector->count);
}
