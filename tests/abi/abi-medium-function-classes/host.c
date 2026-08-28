#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_entry_proxy(abi_near_fn2 near_target,
                                abi_huge_fn2 huge_target,
                                abi_u16 a, abi_u16 b);
extern abi_u16 llvm_medium_near_proxy(abi_u16 a, abi_u16 b);
extern abi_u16 ABI_HUGE_FUNCTION llvm_medium_huge_proxy(abi_u16 a,
                                                        abi_u16 b);

static abi_u16 expected_tasking_near(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x1357U) ^
                   (abi_u16)(b + (abi_u16)(a << 1)));
}

static abi_u16 expected_tasking_huge(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0xa55aU) +
                   (abi_u16)(b ^ (abi_u16)(a >> 1)));
}

static abi_u16 expected_llvm_near(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x2468U) ^
                   (abi_u16)(b + (abi_u16)(a << 2)));
}

static abi_u16 expected_llvm_huge(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0x5aa5U) +
                   (abi_u16)(b ^ (abi_u16)(a >> 2)));
}

static void run_vector(abi_u16 vector_id, abi_u16 a, abi_u16 b) {
  abi_near_fn2 llvm_near = llvm_medium_near_proxy;
  abi_huge_fn2 llvm_huge = (abi_huge_fn2)0x180100UL;
  abi_u16 tasking_near = expected_tasking_near(a, b);
  abi_u16 tasking_huge = expected_tasking_huge(a, b);
  abi_u16 llvm_near_value = expected_llvm_near(a, b);
  abi_u16 llvm_huge_value = expected_llvm_huge(a, b);
  abi_u32 expected_dispatch =
      (abi_u32)(tasking_near ^ (abi_u16)(tasking_near + 0x1111U)) |
      ((abi_u32)(tasking_huge + (abi_u16)(tasking_huge ^ 0x2222U)) << 16);
  abi_u16 base = (vector_id - 1U) * 9U;

  c166_test_check_u32(base + 1U, tasking_near,
                       tasking_medium_near_target(a, b));
  c166_test_check_u32(base + 2U, tasking_huge,
                       tasking_medium_huge_target(a, b));
  c166_test_check_u32(base + 3U, expected_dispatch,
                       llvm_entry_proxy(tasking_medium_near_target,
                                        tasking_medium_huge_target, a, b));
  c166_test_check_u32(base + 4U, llvm_near_value,
                       llvm_medium_near_proxy(a, b));
  c166_test_check_u32(base + 5U, llvm_near_value, llvm_near(a, b));
  c166_test_check_u32(base + 6U, llvm_huge_value, llvm_huge(a, b));
  c166_test_check_u32(base + 7U, llvm_huge_value,
                       llvm_medium_huge_proxy(a, b));
  c166_test_check_u32(base + 8U, 1UL,
                       llvm_near == llvm_medium_near_proxy);
  c166_test_check_u32(base + 9U, 1UL,
                       llvm_huge == (abi_huge_fn2)0x180100UL);
}

#define RUN_VECTOR(a, b) run_vector(vector_id++, a, b);

void main(void) {
  abi_u16 vector_id = 1;

  c166_test_begin(306, 0xabc16707UL);
  c166_test_check_u32(55, 2UL, sizeof(abi_near_fn2));
  c166_test_check_u32(56, 4UL, sizeof(abi_huge_fn2));
  ABI_MEDIUM_FUNCTION_CLASS_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}

