#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "corpus.inc"

union abi_float_words {
  float value;
  struct {
    abi_u16 high;
    abi_u16 low;
  } words;
};

extern abi_s32 llvm_f32_to_i32_proxy(float);
extern abi_u32 llvm_f32_to_u32_proxy(float);

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

static void run_group(abi_u16 seed_index) {
  abi_u32 state = abi_f32_to_i32_seeds[seed_index];
  abi_u32 signature = ABI_F32_TO_I32_GROUP_SIGNATURE_SEED ^
                      ((abi_u32)(seed_index + 1U) << 8);
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  for (iteration = 0U; iteration < ABI_F32_TO_I32_ITERATIONS; ++iteration) {
    abi_u16 expected_index;
    abi_u16 case_id = (abi_u16)(iteration * 2U + 1U);
    abi_u32 bits;
    float value;

    state = xorshift32(state);
    bits = state;
    if ((iteration & 7U) == 0U) {
      abi_u16 edge =
          (abi_u16)((iteration / 8U + seed_index) &
                    (ABI_F32_TO_I32_EDGE_COUNT - 1U));
      bits = abi_f32_to_i32_edges[edge];
    }
    value = float_from_bits(bits);

    signature = signature_step(
        signature, case_id, (abi_u32)llvm_f32_to_i32_proxy(value));
    signature = signature_step(
        signature, (abi_u16)(case_id + 1U),
        llvm_f32_to_u32_proxy(value));

    if ((iteration & (ABI_F32_TO_I32_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_F32_TO_I32_CHECKPOINT_INTERVAL - 1U) {
      expected_index =
          (abi_u16)(seed_index * ABI_F32_TO_I32_CHECKPOINTS_PER_GROUP +
                    checkpoint);
      c166_test_set_context(abi_f32_to_i32_seeds[seed_index], 0U, iteration,
                            bits, 0U);
      c166_test_check_u32(
          (abi_u16)(expected_index + 1U),
          abi_f32_to_i32_expected[expected_index], signature);
      ++checkpoint;
    }
  }
}

void main(void) {
  abi_u16 seed_index;

  c166_test_begin(ABI_F32_TO_I32_SUITE_ID, 0x16600183UL);
  for (seed_index = 0U; seed_index < ABI_F32_TO_I32_SEED_COUNT; ++seed_index)
    run_group(seed_index);
  c166_test_finish();
  simulator_stop();
}
