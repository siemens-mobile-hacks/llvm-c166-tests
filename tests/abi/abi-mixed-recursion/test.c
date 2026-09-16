#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

union abi_mixed_float_words {
  float value;
  struct {
    abi_u16 high;
    abi_u16 low;
  } words;
};

float mixed_observed_float;
struct abi_mixed_pair mixed_observed_pair;
abi_u16 mixed_observed_pointed;
abi_u16 mixed_observed_tail;

void mixed_observe(float value, struct abi_mixed_pair pair,
                   abi_u16 pointed, abi_u16 tail) {
  mixed_observed_float = value;
  mixed_observed_pair = pair;
  mixed_observed_pointed = pointed;
  mixed_observed_tail = tail;
}

static float float_from_bits(abi_u32 bits) {
  union abi_mixed_float_words value;
  value.words.high = (abi_u16)(bits >> 16);
  value.words.low = (abi_u16)bits;
  return value.value;
}

static abi_u32 float_to_bits(float input) {
  union abi_mixed_float_words value;
  value.value = input;
  return ((abi_u32)value.words.high << 16) | value.words.low;
}

static void check_observed(abi_u32 golden, abi_u32 actual,
                           abi_u32 float_bits, abi_u16 first,
                           abi_u16 second, abi_u16 pointed, abi_u16 tail) {
  tap_is_u32(actual, golden, "recursive result");
  tap_is_u32(float_to_bits(mixed_observed_float), float_bits,
             "float argument");
  tap_is_u32(mixed_observed_pair.first, first, "aggregate first word");
  tap_is_u32(mixed_observed_pair.second, second, "aggregate second word");
  tap_is_u32(mixed_observed_pointed, pointed, "pointed value");
  tap_is_u32(mixed_observed_tail, tail, "stack tail");
  tap_is_u32(actual != 0xdeadc0deUL, 1U, "stack canaries");
}

static void run_vector(abi_u16 depth, abi_u16 word, abi_u16 pointed,
                       abi_u16 first, abi_u16 second, abi_u16 tail,
                       abi_u32 float_bits, abi_u32 golden,
                       abi_u16 observed_first, abi_u16 observed_second,
                       abi_u16 observed_pointed, abi_u16 observed_tail) {
  abi_u16 pointer_value = pointed;
  struct abi_mixed_pair pair;
  float value = float_from_bits(float_bits);

  pair.first = first;
  pair.second = second;
  check_observed(golden,
                 mixed_recursion_a(depth, word, &pointer_value, value, pair,
                                   tail),
                 float_bits, observed_first, observed_second,
                 observed_pointed, observed_tail);
  check_observed(golden,
                 mixed_recursion_b(depth, word, &pointer_value, value, pair,
                                   tail),
                 float_bits, observed_first, observed_second,
                 observed_pointed, observed_tail);
}

#define RUN_VECTOR(id, depth, word, pointed, first, second, tail, float_bits, \
                   golden, observed_first, observed_second,                 \
                   observed_pointed, observed_tail)                         \
  run_vector(depth, word, pointed, first, second, tail, float_bits, golden,  \
             observed_first, observed_second, observed_pointed,             \
             observed_tail);

void main(void) {
  tap_plan(56U);
  ABI_MIXED_RECURSION_VECTORS(RUN_VECTOR)
}
