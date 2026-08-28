#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_mixed_state_proxy(
    abi_u16 depth, abi_u16 word, abi_u16 *pointer, float value,
    struct abi_mixed_pair pair, abi_u16 tail);
extern abi_u32 tasking_mixed_state_proxy(
    abi_u16 depth, abi_u16 word, abi_u16 *pointer, float value,
    struct abi_mixed_pair pair, abi_u16 tail);

union abi_mixed_float_words {
  float value;
  abi_u16 words[2];
};

static void check_observed(abi_u16 base, abi_u32 golden,
                           abi_u32 actual, abi_u32 float_bits,
                           abi_u16 first, abi_u16 second,
                           abi_u16 pointed, abi_u16 tail) {
  union abi_mixed_float_words observed;
  observed.value = tasking_mixed_observed_float;
  c166_test_check_u32(base + 1U, golden, actual);
  c166_test_check_u32(base + 2U, float_bits >> 16, observed.words[0]);
  c166_test_check_u32(base + 3U, (abi_u16)float_bits, observed.words[1]);
  c166_test_check_u32(base + 4U, first,
                      tasking_mixed_observed_pair.first);
  c166_test_check_u32(base + 5U, second,
                      tasking_mixed_observed_pair.second);
  c166_test_check_u32(base + 6U, pointed, tasking_mixed_observed_pointed);
  c166_test_check_u32(base + 7U, tail, tasking_mixed_observed_tail);
}

static void run_vector(abi_u16 vector_id, abi_u16 depth, abi_u16 word,
                       abi_u16 pointed, abi_u16 first, abi_u16 second,
                       abi_u16 tail, abi_u32 float_bits, abi_u32 golden,
                       abi_u16 observed_first, abi_u16 observed_second,
                       abi_u16 observed_pointed, abi_u16 observed_tail) {
  abi_u16 pointer_value = pointed;
  struct abi_mixed_pair pair;
  float value = tasking_mixed_float_values[vector_id - 1U];
  abi_u16 base = (vector_id - 1U) * 14U;
  pair.first = first;
  pair.second = second;

  check_observed(base, golden,
                 llvm_mixed_state_proxy(depth, word, &pointer_value, value,
                                        pair, tail),
                 float_bits, observed_first, observed_second,
                 observed_pointed, observed_tail);
  check_observed(base + 7U, golden,
                 tasking_mixed_state_proxy(depth, word, &pointer_value, value,
                                           pair, tail),
                 float_bits, observed_first, observed_second,
                 observed_pointed, observed_tail);
}

#define RUN_VECTOR(id, depth, word, pointed, first, second, tail, float_bits, \
                   golden, observed_first, observed_second, \
                   observed_pointed, observed_tail) \
  run_vector(id, depth, word, pointed, first, second, tail, float_bits, \
             golden, observed_first, observed_second, observed_pointed, \
             observed_tail);

void main(void) {
  c166_test_begin(308, 0x16600134UL);
  ABI_MIXED_RECURSION_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}

