#include "c166-test-result.h"
#include "common.h"
#include "vectors.inc"

void simulator_stop(void) {
  for (;;)
    ;
}

static void run_vector(abi_u16 vector_id, abi_u16 seed, abi_u16 expected,
                       abi_u32 repeated) {
  struct pair2 pair_value;
  struct chars3 chars_value;
  ABI_PACKED_REF(packed3) packed_value;
  abi_u16 tail;
  abi_u16 tasking_result;
  abi_u16 llvm_result;
  abi_u32 reverse_result;
  abi_u16 base = (abi_u16)((vector_id - 1U) * 3U);

  ABI_INIT_AGGREGATES(seed, pair_value, chars_value, packed_value, tail);
  tasking_result = tasking_varargs_aggregate_abi_proxy(
      &pair_value, &chars_value, &packed_value, tail);
  llvm_result = llvm_varargs_aggregate_abi_proxy(
      &pair_value, &chars_value, &packed_value, tail);
  reverse_result = llvm_varargs_aggregate_reverse_proxy(seed);

  c166_test_check_u32(base + 1U, expected, tasking_result);
  c166_test_check_u32(base + 2U, expected, llvm_result);
  c166_test_check_u32(base + 3U, repeated, reverse_result);
}

#define RUN_VECTOR(id, seed, expected, repeated) \
  run_vector(id, seed, expected, repeated);

void main(void) {
  c166_test_begin(307, 0x16600131UL);
  ABI_AGGREGATE_VARARGS_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}

