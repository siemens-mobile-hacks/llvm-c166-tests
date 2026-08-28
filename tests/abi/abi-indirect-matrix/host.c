#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_call0_proxy(abi_fn0 volatile *slot, abi_fn0 target);
extern abi_u32 llvm_call2_proxy(abi_fn2 volatile *slot, abi_fn2 target,
                               abi_u16 a, abi_u16 b);
extern abi_u32 llvm_call5_proxy(abi_fn5 volatile *slot, abi_fn5 target,
                               abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                               abi_u16 e);
extern abi_fn0 llvm_get0_proxy(void);
extern abi_fn2 llvm_get2_proxy(void);
extern abi_fn5 llvm_get5_proxy(void);
extern abi_u32 llvm_canary0_proxy(void);
extern abi_u32 llvm_canary2_proxy(abi_u16 a, abi_u16 b);
extern abi_u32 llvm_canary5_proxy(abi_u16 a, abi_u16 b, abi_u16 c,
                                 abi_u16 d, abi_u16 e);

static void run_indirect_matrix(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                                abi_u16 e, abi_u32 golden0, abi_u32 golden2,
                                abi_u32 golden5) {
  abi_fn0 llvm0;
  abi_fn2 llvm2;
  abi_fn5 llvm5;
  abi_fn0 volatile slot0;
  abi_fn2 volatile slot2;
  abi_fn5 volatile slot5;

  c166_test_check_u32(1, golden0, tasking_indirect0());
  c166_test_check_u32(2, golden0,
                       llvm_call0_proxy(&slot0, tasking_indirect0));
  c166_test_check_u32(3, 1UL, slot0 == tasking_indirect0);
  llvm0 = llvm_get0_proxy();
  c166_test_check_u32(4, 1UL, llvm0 == llvm_get0_proxy());
  slot0 = llvm0;
  c166_test_check_u32(5, golden0, slot0());
  c166_test_check_u32(6, golden0, llvm_canary0_proxy());

  c166_test_check_u32(7, golden2, tasking_indirect2(a, b));
  c166_test_check_u32(8, golden2,
                       llvm_call2_proxy(&slot2, tasking_indirect2, a, b));
  c166_test_check_u32(9, 1UL, slot2 == tasking_indirect2);
  llvm2 = llvm_get2_proxy();
  c166_test_check_u32(10, 1UL, llvm2 == llvm_get2_proxy());
  slot2 = llvm2;
  c166_test_check_u32(11, golden2, slot2(a, b));
  c166_test_check_u32(12, golden2, llvm_canary2_proxy(a, b));

  c166_test_check_u32(13, golden5, tasking_indirect5(a, b, c, d, e));
  c166_test_check_u32(
      14, golden5,
      llvm_call5_proxy(&slot5, tasking_indirect5, a, b, c, d, e));
  c166_test_check_u32(15, 1UL, slot5 == tasking_indirect5);
  llvm5 = llvm_get5_proxy();
  c166_test_check_u32(16, 1UL, llvm5 == llvm_get5_proxy());
  slot5 = llvm5;
  c166_test_check_u32(17, golden5, slot5(a, b, c, d, e));
  c166_test_check_u32(18, golden5, llvm_canary5_proxy(a, b, c, d, e));
}

#define RUN_INDIRECT_MATRIX(a, b, c, d, e, golden0, golden2, golden5) \
  run_indirect_matrix(a, b, c, d, e, golden0, golden2, golden5);

void main(void) {
  c166_test_begin(270, 0x1660010eUL);
  ABI_INDIRECT_MATRIX_VECTOR(RUN_INDIRECT_MATRIX)
  c166_test_finish();
  simulator_stop();
}

