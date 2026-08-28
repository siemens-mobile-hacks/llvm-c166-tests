#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_entry_proxy(abi_u16 operation, abi_chain_fn callback,
                                abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);
extern volatile abi_u16 tasking_nested_call_count;

abi_u32 tasking_nested_leaf_a(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);
abi_u32 tasking_nested_leaf_b(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);
abi_u32 tasking_nested_reentrant(abi_u16 a, abi_u32 b, abi_u16 c,
                                 abi_u16 d);

static void run_vector(abi_u16 id, abi_u16 a, abi_u32 b, abi_u16 c,
                       abi_u16 d, abi_u32 direct_a, abi_u32 direct_b,
                       abi_u32 indirect_direct, abi_u32 twice_a) {
  abi_u16 base = (id - 1U) * 5U;

  c166_test_check_u32(
      base + 1U, direct_a,
      llvm_entry_proxy(ABI_NESTED_DIRECT_INDIRECT, tasking_nested_leaf_a, a,
                       b, c, d));
  c166_test_check_u32(
      base + 2U, direct_b,
      llvm_entry_proxy(ABI_NESTED_DIRECT_INDIRECT, tasking_nested_leaf_b, a,
                       b, c, d));
  c166_test_check_u32(
      base + 3U, indirect_direct,
      llvm_entry_proxy(ABI_NESTED_INDIRECT_DIRECT,
                       tasking_nested_reentrant, a, b, c, d));
  c166_test_check_u32(
      base + 4U, twice_a,
      llvm_entry_proxy(ABI_NESTED_DIRECT_INDIRECT_TWICE,
                       tasking_nested_leaf_a, a, b, c, d));
  c166_test_check_u32(base + 5U, (abi_u32)id * 5UL,
                     tasking_nested_call_count);
}

#define RUN_VECTOR(id, a, b, c, d, direct_a, direct_b, indirect_direct,      \
                   twice_a)                                                   \
  run_vector(id, a, b, c, d, direct_a, direct_b, indirect_direct, twice_a);

void main(void) {
  tasking_nested_call_count = 0;
  c166_test_begin(320, 0x16600140UL);
  ABI_NESTED_CALL_VECTORS(RUN_VECTOR)
  c166_test_check_u32(31, 30UL, tasking_nested_call_count);
  c166_test_finish();
  simulator_stop();
}
