#include "c166-test-compat.h"
#include "operations.h"
#include "vectors.inc"

DEFINE_INTEGER32_EVAL(
    llvm_integer32_eval,
    C166_TEST_NOINLINE)

struct integer32_vector {
  abi_u32 a;
  abi_u32 b;
  abi_u16 count;
};

#define INTEGER32_VECTOR(id, a, b, count) {a, b, count},
static const struct integer32_vector vectors[] = {
    ABI_INTEGER32_VECTORS(INTEGER32_VECTOR)
};

C166_TEST_NOINLINE
abi_u32 c166_test_case(abi_u16 case_id) {
  const struct integer32_vector *vector =
      &vectors[case_id / ABI_INTEGER32_OP_COUNT];
  abi_u16 operation = case_id % ABI_INTEGER32_OP_COUNT;
  return llvm_integer32_eval(operation, vector->a, vector->b, vector->count);
}
