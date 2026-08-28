#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 tasking_integer32_eval(abi_u16, abi_u32, abi_u32, abi_u16);
extern abi_u32 llvm_entry_proxy(abi_u16, abi_u32, abi_u32, abi_u16);

static void run_reference_vector(abi_u16 vector_id, abi_u32 a, abi_u32 b,
                                 abi_u16 count) {
  abi_u16 operation;
  abi_u16 case_id;
  abi_u32 actual;
  for (operation = 0; operation < ABI_INTEGER32_OP_COUNT; ++operation) {
    if (operation >= 7U && operation <= 12U)
      continue;
    case_id = (abi_u16)((vector_id - 1U) * ABI_INTEGER32_OP_COUNT +
                        operation + 1U);
    actual = tasking_integer32_eval(operation, a, b, count);
    c166_test_check_u32(case_id, actual, actual);
  }
}

static void run_differential_vector(abi_u16 vector_id, abi_u32 a, abi_u32 b,
                                    abi_u16 count) {
  abi_u16 operation;
  abi_u16 case_id;
  abi_u32 expected;
  abi_u32 actual;
  for (operation = 0; operation < ABI_INTEGER32_OP_COUNT; ++operation) {
    if (operation >= 7U && operation <= 12U)
      continue;
    case_id = (abi_u16)(0x2000U +
        (vector_id - 1U) * ABI_INTEGER32_OP_COUNT + operation);
    expected = tasking_integer32_eval(operation, a, b, count);
    actual = llvm_entry_proxy(operation, a, b, count);
    c166_test_check_u32(case_id, expected, actual);
  }
}

#define RUN_REFERENCE(id, a, b, count) \
  run_reference_vector(id, a, b, count);
#define RUN_DIFFERENTIAL(id, a, b, count) \
  run_differential_vector(id, a, b, count);
#define RUN_HELPER(case_id, operation, a, b, count, golden) \
  c166_test_check_u32(case_id, golden, \
      llvm_entry_proxy(operation, a, b, count));

void main(void) {
  abi_u32 reference_signature;

  c166_test_begin(282, 0x1660011aUL);
  ABI_INTEGER32_VECTORS(RUN_REFERENCE)
  reference_signature = c166_test_result.signature;
  c166_test_check_u32(0x1000U, ABI_INTEGER32_REFERENCE_SIGNATURE,
                      reference_signature);
  ABI_INTEGER32_VECTORS(RUN_DIFFERENTIAL)
  ABI_INTEGER32_HELPER_GOLDENS(RUN_HELPER)
  c166_test_finish();
  simulator_stop();
}

