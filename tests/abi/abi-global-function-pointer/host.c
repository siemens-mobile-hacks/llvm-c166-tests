#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 llvm_global_roundtrip_proxy(abi_fn target, abi_u16 a,
                                            abi_u16 b);
extern abi_fn llvm_global_exchange_proxy(abi_fn target);
extern abi_u16 llvm_call_global_proxy(abi_u16 a, abi_u16 b);
extern abi_fn llvm_get_target_proxy(void);
extern abi_u16 llvm_reverse_roundtrip_proxy(abi_fn target, abi_u16 a,
                                             abi_u16 b);
extern abi_fn llvm_reverse_exchange_proxy(abi_fn target);

static abi_u16 tasking_target(abi_u16 a, abi_u16 b) {
  return (a ^ 0x5aa5U) + b;
}

static void run_global_function_pointer_vector(
    abi_u16 vector_id, abi_u16 a, abi_u16 b, abi_u32 tasking_golden,
    abi_u32 llvm_golden) {
  abi_fn exchanged;
  abi_fn llvm_target;
  abi_fn reverse_exchanged;
  abi_u16 base = (vector_id - 1) * 11;

  c166_test_check_u32(base + 1, tasking_golden,
                       tasking_global_roundtrip(tasking_target, a, b));
  c166_test_check_u32(base + 2, tasking_golden,
                       llvm_global_roundtrip_proxy(tasking_target, a, b));
  c166_test_check_u32(base + 3, tasking_golden,
                       llvm_reverse_roundtrip_proxy(tasking_target, a, b));

  exchanged = llvm_global_exchange_proxy(tasking_target);
  c166_test_check_u32(base + 4, 1UL, exchanged == tasking_target);
  c166_test_check_u32(base + 5, tasking_golden, exchanged(a, b));

  llvm_target = llvm_get_target_proxy();
  c166_test_check_u32(base + 6, 1UL,
                       llvm_target == llvm_get_target_proxy());
  exchanged = llvm_global_exchange_proxy(llvm_target);
  c166_test_check_u32(base + 7, 1UL, exchanged == llvm_target);
  c166_test_check_u32(base + 8, llvm_golden,
                       llvm_call_global_proxy(a, b));
  c166_test_check_u32(base + 9, llvm_golden, exchanged(a, b));

  reverse_exchanged = llvm_reverse_exchange_proxy(tasking_target);
  c166_test_check_u32(base + 10, 1UL,
                       reverse_exchanged == tasking_target);
  c166_test_check_u32(base + 11, tasking_golden,
                       reverse_exchanged(a, b));
}

#define RUN_GLOBAL_FUNCTION_POINTER(id, a, b, tasking_golden, llvm_golden) \
  run_global_function_pointer_vector(id, a, b, tasking_golden, llvm_golden);

void main(void) {
  abi_fn exchanged;

  c166_test_begin(272, 0x16600110UL);
  ABI_GLOBAL_FUNCTION_POINTER_VECTORS(RUN_GLOBAL_FUNCTION_POINTER)

  exchanged = llvm_global_exchange_proxy((abi_fn)0);
  c166_test_check_u32(45, 1UL, exchanged == (abi_fn)0);
  exchanged = llvm_reverse_exchange_proxy((abi_fn)0);
  c166_test_check_u32(46, 1UL, exchanged == (abi_fn)0);
  exchanged = llvm_global_exchange_proxy(tasking_target);
  c166_test_check_u32(47, 1UL, exchanged == tasking_target);
  c166_test_check_u32(48, 0x5aa5UL, llvm_call_global_proxy(0, 0));

  c166_test_finish();
  simulator_stop();
}

