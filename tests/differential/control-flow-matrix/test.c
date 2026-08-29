#include "c166-test-compat.h"

#if defined(C166_TEST_LLVM)
#define CONTROL_FLOW_LLVM 1
#endif

#include "operations.h"
#include "vectors.inc"

DEFINE_CONTROL_FLOW_EVAL(
    llvm_control_flow_eval,
    C166_TEST_NOINLINE)

struct control_flow_vector {
  abi_u32 a;
  abi_u32 b;
  abi_u16 count;
};

#define CONTROL_FLOW_VECTOR(id, a, b, count) {a, b, count},
static const struct control_flow_vector vectors[] = {
    ABI_CONTROL_FLOW_VECTORS(CONTROL_FLOW_VECTOR)
};

C166_TEST_NOINLINE
abi_u32 c166_test_case(abi_u16 case_id) {
  const struct control_flow_vector *vector =
      &vectors[case_id / ABI_CONTROL_FLOW_OP_COUNT];
  abi_u16 operation = case_id % ABI_CONTROL_FLOW_OP_COUNT;
  return llvm_control_flow_eval(operation, vector->a, vector->b,
                                vector->count);
}
