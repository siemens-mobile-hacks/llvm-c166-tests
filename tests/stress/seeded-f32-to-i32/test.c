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

abi_s32 c166_f32_to_i32(float value);
abi_u32 c166_f32_to_u32(float value);

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 value) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

static abi_u32 xorshift32(abi_u32 state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

static float float_from_bits(abi_u32 bits) {
  union abi_float_words value;
  value.words.high = (abi_u16)(bits >> 16);
  value.words.low = (abi_u16)bits;
  return value.value;
}

static void check_defined_edges(void) {
  float zero = float_from_bits(0x00000000UL);
  float one = float_from_bits(0x3f800000UL);
  float two_and_half = float_from_bits(0x40200000UL);
  float minus_one = float_from_bits(0xbf800000UL);
  float signed_max = float_from_bits(0x4effffffUL);
  float unsigned_midpoint = float_from_bits(0x4f000000UL);

  tap_is_u32((abi_u32)c166_f32_to_i32(zero), 0UL, "signed zero");
  tap_is_u32(c166_f32_to_u32(zero), 0UL, "unsigned zero");
  tap_is_u32((abi_u32)c166_f32_to_i32(one), 1UL, "signed one");
  tap_is_u32(c166_f32_to_u32(one), 1UL, "unsigned one");
  tap_is_u32((abi_u32)c166_f32_to_i32(two_and_half), 2UL,
             "signed fraction truncation");
  tap_is_u32(c166_f32_to_u32(two_and_half), 2UL,
             "unsigned fraction truncation");
  tap_is_u32((abi_u32)c166_f32_to_i32(minus_one), 0xffffffffUL,
             "signed negative one");
  tap_is_u32((abi_u32)c166_f32_to_i32(signed_max), 0x7fffff80UL,
             "signed positive limit");
  tap_is_u32(c166_f32_to_u32(signed_max), 0x7fffff80UL,
             "unsigned signed-limit value");
  tap_is_u32(c166_f32_to_u32(unsigned_midpoint), 0x80000000UL,
             "unsigned midpoint");
}

static void run_group(abi_u16 seed_index) {
  abi_u32 state = abi_f32_to_i32_seeds[seed_index];
  abi_u32 signature = ABI_F32_TO_I32_GROUP_SIGNATURE_SEED ^
                      ((abi_u32)(seed_index + 1U) << 8);
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  for (iteration = 0U; iteration < ABI_F32_TO_I32_ITERATIONS; ++iteration) {
    abi_u16 case_id = (abi_u16)(iteration * 2U + 1U);
    abi_u32 bits;
    float value;

    state = xorshift32(state);
    bits = state & ABI_F32_TO_I32_RANDOM_MASK;
    if ((iteration & 7U) == 0U) {
      abi_u16 edge =
          (abi_u16)((iteration / 8U + seed_index) &
                    (ABI_F32_TO_I32_EDGE_COUNT - 1U));
      bits = abi_f32_to_i32_edges[edge];
    }
    value = float_from_bits(bits);

    signature = signature_step(
        signature, case_id, (abi_u32)c166_f32_to_i32(value));
    signature = signature_step(
        signature, (abi_u16)(case_id + 1U), c166_f32_to_u32(value));

    if ((iteration & (ABI_F32_TO_I32_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_F32_TO_I32_CHECKPOINT_INTERVAL - 1U) {
      abi_u16 expected_index =
          (abi_u16)(seed_index * ABI_F32_TO_I32_CHECKPOINTS_PER_GROUP +
                    checkpoint);
      tap_is_u32(signature, abi_f32_to_i32_expected[expected_index],
                 "binary32 to integer checkpoint");
      ++checkpoint;
    }
  }
}

void main(void) {
  abi_u16 seed_index;

  tap_plan(10U + ABI_F32_TO_I32_SEED_COUNT *
                    ABI_F32_TO_I32_CHECKPOINTS_PER_GROUP);
  check_defined_edges();
  for (seed_index = 0U; seed_index < ABI_F32_TO_I32_SEED_COUNT; ++seed_index)
    run_group(seed_index);
}
