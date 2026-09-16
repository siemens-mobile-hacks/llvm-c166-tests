#include "c166_test.h"
#include "types.h"
#include "corpus.inc"

union abi_double_words {
  double value;
  struct {
    abi_u16 word3;
    abi_u16 word2;
    abi_u16 word1;
    abi_u16 word0;
  } words;
};

struct abi_u64_words {
  abi_u32 high;
  abi_u32 low;
};

volatile double C166_NEAR seeded_f64_inputs[2];

double seeded_f64_eval(abi_u16 expression);

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 value) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

static void xorshift64(struct abi_u64_words *value) {
  abi_u32 high = value->high;
  abi_u32 low = value->low;

  value->high ^= (high << 13) | (low >> 19);
  value->low ^= low << 13;
  high = value->high;
  low = value->low;
  value->high ^= high >> 7;
  value->low ^= (low >> 7) | (high << 25);
  high = value->high;
  low = value->low;
  value->high ^= (high << 17) | (low >> 15);
  value->low ^= low << 17;
}

static struct abi_u64_words finite_bits(struct abi_u64_words *state) {
  struct abi_u64_words bits;

  xorshift64(state);
  bits = *state;
  if (((bits.high >> 20) & 0x7ffUL) == 0x7ffUL)
    bits.high ^= 0x00100000UL;
  return bits;
}

static double double_from_bits(struct abi_u64_words bits) {
  union abi_double_words value;
  value.words.word3 = (abi_u16)(bits.high >> 16);
  value.words.word2 = (abi_u16)bits.high;
  value.words.word1 = (abi_u16)(bits.low >> 16);
  value.words.word0 = (abi_u16)bits.low;
  return value.value;
}

static struct abi_u64_words double_to_bits(double input) {
  union abi_double_words value;
  struct abi_u64_words bits;

  value.value = input;
  bits.high = ((abi_u32)value.words.word3 << 16) | value.words.word2;
  bits.low = ((abi_u32)value.words.word1 << 16) | value.words.word0;
  return bits;
}

static void write_inputs(struct abi_u64_words a, struct abi_u64_words b) {
  seeded_f64_inputs[0] = double_from_bits(a);
  seeded_f64_inputs[1] = double_from_bits(b);
}

static void run_group(abi_u16 seed_index, abi_u16 expression) {
  struct abi_u64_words state;
  abi_u32 signature;
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  state.high = abi_seeded_f64_seed_hi[seed_index];
  state.low = abi_seeded_f64_seed_lo[seed_index];
  signature = ABI_SEEDED_F64_GROUP_SIGNATURE_SEED ^
              ((abi_u32)(seed_index + 1U) << 8) ^ expression;

  for (iteration = 0U; iteration < ABI_SEEDED_F64_ITERATIONS; ++iteration) {
    struct abi_u64_words a = finite_bits(&state);
    struct abi_u64_words b = finite_bits(&state);
    struct abi_u64_words result;

    if ((iteration & 3U) == 0U) {
      abi_u16 edge = (abi_u16)((iteration + seed_index + 1U) & 15U);
      a.high = abi_seeded_f64_edge_hi[edge];
      a.low = abi_seeded_f64_edge_lo[edge];
    }
    if ((iteration & 3U) == 1U) {
      abi_u16 edge =
          (abi_u16)((iteration * 3U + seed_index + 1U) & 15U);
      b.high = abi_seeded_f64_edge_hi[edge];
      b.low = abi_seeded_f64_edge_lo[edge];
    }
    if ((b.high & 0x7fffffffUL) == 0UL && b.low == 0UL) {
      b.high = 0x3ff00000UL;
      b.low = 0UL;
    }
    if ((iteration & 15U) == 15U)
      b = a;

    write_inputs(a, b);
    result = double_to_bits(seeded_f64_eval(expression));
    signature = signature_step(signature, (abi_u16)(iteration * 2U + 1U),
                               result.high);
    signature = signature_step(signature, (abi_u16)(iteration * 2U + 2U),
                               result.low);

    if ((iteration & (ABI_SEEDED_F64_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_SEEDED_F64_CHECKPOINT_INTERVAL - 1U) {
      abi_u16 expected_index = (abi_u16)(
          seed_index * ABI_SEEDED_F64_EXPRESSION_COUNT *
                  ABI_SEEDED_F64_CHECKPOINTS_PER_GROUP +
          expression * ABI_SEEDED_F64_CHECKPOINTS_PER_GROUP + checkpoint);
      tap_is_u32(signature, abi_seeded_f64_expected[expected_index],
                 "binary64 arithmetic checkpoint");
      ++checkpoint;
    }
  }
}

void main(void) {
  abi_u16 seed_index;
  abi_u16 expression;

  tap_plan(ABI_SEEDED_F64_CHECKPOINT_COUNT);
  for (seed_index = 0U; seed_index < ABI_SEEDED_F64_SEED_COUNT; ++seed_index)
    for (expression = 0U; expression < ABI_SEEDED_F64_EXPRESSION_COUNT;
         ++expression)
      run_group(seed_index, expression);
}
