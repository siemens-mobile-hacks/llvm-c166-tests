#include "c166_test.h"
#include "types.h"
#include "corpus.inc"

union abi_float_words {
  float value;
  struct {
    abi_u16 high;
    abi_u16 low;
  } words;
};

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

float seeded_f64_to_f32(double value);

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

static double double_from_bits(struct abi_u64_words bits) {
  union abi_double_words value;
  value.words.word3 = (abi_u16)(bits.high >> 16);
  value.words.word2 = (abi_u16)bits.high;
  value.words.word1 = (abi_u16)(bits.low >> 16);
  value.words.word0 = (abi_u16)bits.low;
  return value.value;
}

static abi_u32 float_to_bits(float input) {
  union abi_float_words value;
  value.value = input;
  return ((abi_u32)value.words.high << 16) | value.words.low;
}

static void run_group(abi_u16 seed_index) {
  struct abi_u64_words state;
  abi_u32 signature;
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  state.high = abi_f64_to_f32_seed_hi[seed_index];
  state.low = abi_f64_to_f32_seed_lo[seed_index];
  signature = ABI_F64_TO_F32_GROUP_SIGNATURE_SEED ^
              ((abi_u32)(seed_index + 1U) << 8);

  for (iteration = 0U; iteration < ABI_F64_TO_F32_ITERATIONS; ++iteration) {
    struct abi_u64_words bits;

    xorshift64(&state);
    bits = state;
    if ((iteration & 7U) == 0U) {
      abi_u16 edge =
          (abi_u16)((iteration / 8U + seed_index) &
                    (ABI_F64_TO_F32_EDGE_COUNT - 1U));
      bits.high = abi_f64_to_f32_edge_hi[edge];
      bits.low = abi_f64_to_f32_edge_lo[edge];
    }

    signature = signature_step(
        signature, (abi_u16)(iteration + 1U),
        float_to_bits(seeded_f64_to_f32(double_from_bits(bits))));

    if ((iteration & (ABI_F64_TO_F32_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_F64_TO_F32_CHECKPOINT_INTERVAL - 1U) {
      abi_u16 expected_index =
          (abi_u16)(seed_index * ABI_F64_TO_F32_CHECKPOINTS_PER_GROUP +
                    checkpoint);
      tap_is_u32(signature, abi_f64_to_f32_expected[expected_index],
                 "binary64 to binary32 checkpoint");
      ++checkpoint;
    }
  }
}

void main(void) {
  abi_u16 seed_index;

  tap_plan(ABI_F64_TO_F32_CHECKPOINT_COUNT);
  for (seed_index = 0U; seed_index < ABI_F64_TO_F32_SEED_COUNT; ++seed_index)
    run_group(seed_index);
}
