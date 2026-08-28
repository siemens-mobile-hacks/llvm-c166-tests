#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 tasking_integer16_eval(abi_u16, abi_u16, abi_u16, abi_u16);
extern abi_u32 llvm_entry_proxy(abi_u16, abi_u16, abi_u16, abi_u16);

static void run_reference_vector(abi_u16 vector_id, abi_u16 a, abi_u16 b,
                                 abi_u16 count) {
  abi_u16 operation;
  for (operation = 0; operation < ABI_INTEGER16_OP_COUNT; ++operation) {
    abi_u16 case_id =
        (abi_u16)((vector_id - 1U) * ABI_INTEGER16_OP_COUNT + operation + 1U);
    abi_u32 actual = tasking_integer16_eval(operation, a, b, count);
    c166_test_check_u32(case_id, actual, actual);
  }
}

static void run_differential_vector(abi_u16 vector_id, abi_u16 a, abi_u16 b,
                                    abi_u16 count) {
  abi_u16 operation;
  for (operation = 0; operation < ABI_INTEGER16_OP_COUNT; ++operation) {
    abi_u16 case_id = (abi_u16)(0x2000U +
        (vector_id - 1U) * ABI_INTEGER16_OP_COUNT + operation);
    abi_u32 expected = tasking_integer16_eval(operation, a, b, count);
    abi_u32 actual = llvm_entry_proxy(operation, a, b, count);
    c166_test_check_u32(case_id, expected, actual);
  }
}

#define RUN_REFERENCE(id, a, b, count) \
  run_reference_vector(id, a, b, count);
#define RUN_DIFFERENTIAL(id, a, b, count) \
  run_differential_vector(id, a, b, count);

void main(void) {
  abi_u32 reference_signature;

  c166_test_begin(281, 0x16600119UL);
  ABI_INTEGER16_VECTORS(RUN_REFERENCE)
  reference_signature = c166_test_result.signature;
  c166_test_check_u32(0x1000U, ABI_INTEGER16_REFERENCE_SIGNATURE,
                      reference_signature);
  ABI_INTEGER16_VECTORS(RUN_DIFFERENTIAL)
  c166_test_finish();
  simulator_stop();
}

