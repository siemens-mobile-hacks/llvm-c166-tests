#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

extern abi_u32 copy_pair_up_state_proxy(abi_u16 low, abi_u16 high);
extern abi_u32 copy_pair_down_state_proxy(abi_u16 low, abi_u16 high);
extern abi_u32 zext_pair_high_overlap_state_proxy(abi_u16 value);
extern abi_u32 sext_pair_high_overlap_state_proxy(abi_u16 value);

static void run_tuple_pair_vector(abi_u16 vector_id, abi_u16 low,
                                  abi_u16 high, abi_u32 golden) {
  abi_u16 base = (vector_id - 1U) * 2U;

  c166_test_check_u32(base + 1U, golden,
                      copy_pair_up_state_proxy(low, high));
  c166_test_check_u32(base + 2U, golden,
                      copy_pair_down_state_proxy(low, high));
}

static void run_tuple_scalar_vector(abi_u16 vector_id, abi_u16 value,
                                    abi_u32 zext_golden,
                                    abi_u32 sext_golden) {
  abi_u16 base = ABI_TUPLE_PAIR_CASES + (vector_id - 1U) * 2U;

  c166_test_check_u32(base + 1U, zext_golden,
                      zext_pair_high_overlap_state_proxy(value));
  c166_test_check_u32(base + 2U, sext_golden,
                      sext_pair_high_overlap_state_proxy(value));
}

#define RUN_TUPLE_PAIR(id, low, high, golden) \
  run_tuple_pair_vector(id, low, high, golden);
#define RUN_TUPLE_SCALAR(id, value, zext, sext) \
  run_tuple_scalar_vector(id, value, zext, sext);

void main(void) {
  c166_test_begin(278, 0x16600116UL);
  ABI_TUPLE_PAIR_VECTORS(RUN_TUPLE_PAIR)
  ABI_TUPLE_SCALAR_VECTORS(RUN_TUPLE_SCALAR)
  c166_test_finish();
  simulator_stop();
}

