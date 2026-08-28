#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "corpus.inc"

struct prng_state {
  abi_u16 low;
  abi_u16 high;
};

extern volatile abi_u16 seeded_f32_inputs[4];
extern abi_u32 llvm_seeded_f32_eval_proxy(abi_u16 expression);

static abi_u16 xorshift16(abi_u16 value) {
  value ^= (abi_u16)(value << 7);
  value ^= value >> 9;
  value ^= (abi_u16)(value << 8);
  return value;
}

static abi_u32 next_u32(struct prng_state *state) {
  state->low = xorshift16(state->low);
  state->high = xorshift16(state->high);
  if (state->low == 0U && state->high == 0U)
    state->high = 1U;
  return ((abi_u32)state->high << 16) | state->low;
}

static abi_u32 finite_bits(struct prng_state *state) {
  abi_u32 bits = next_u32(state);
  if (((bits >> 23) & 0xffUL) == 0xffUL)
    bits ^= 0x00800000UL;
  return bits;
}

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 value) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

static void write_inputs(abi_u32 a, abi_u32 b) {
  seeded_f32_inputs[0] = (abi_u16)(a >> 16);
  seeded_f32_inputs[1] = (abi_u16)a;
  seeded_f32_inputs[2] = (abi_u16)(b >> 16);
  seeded_f32_inputs[3] = (abi_u16)b;
}

static void run_group(abi_u16 seed_index, abi_u16 expression) {
  abi_u32 seed = abi_seeded_f32_seeds[seed_index];
  struct prng_state state;
  abi_u32 signature;
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  state.low = (abi_u16)seed;
  state.high = (abi_u16)(seed >> 16);
  signature = ABI_SEEDED_F32_GROUP_SIGNATURE_SEED ^
              ((abi_u32)(seed_index + 1U) << 8) ^ expression;

  for (iteration = 0U; iteration < ABI_SEEDED_F32_ITERATIONS; ++iteration) {
    abi_u32 a = finite_bits(&state);
    abi_u32 b = finite_bits(&state);
    abi_u32 actual;

    if ((iteration & 3U) == 0U)
      a = abi_seeded_f32_edges[(iteration + seed_index) & 15U];
    if ((iteration & 3U) == 1U)
      b = abi_seeded_f32_edges[(iteration * 3U + seed_index) & 15U];
    if ((b & 0x7fffffffUL) == 0UL)
      b = 0x3f800000UL;

    write_inputs(a, b);
    actual = llvm_seeded_f32_eval_proxy(expression);
    signature = signature_step(signature, (abi_u16)(iteration + 1U), actual);

    if ((iteration & (ABI_SEEDED_F32_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_SEEDED_F32_CHECKPOINT_INTERVAL - 1U) {
      abi_u16 expected_index = (abi_u16)(
          seed_index * ABI_SEEDED_F32_EXPRESSION_COUNT *
                  ABI_SEEDED_F32_CHECKPOINTS_PER_GROUP +
          expression * ABI_SEEDED_F32_CHECKPOINTS_PER_GROUP + checkpoint);
      abi_u16 case_id = (abi_u16)(expected_index + 1U);
      c166_test_set_context(seed, expression, iteration, a, b);
      c166_test_check_u32(case_id, abi_seeded_f32_expected[expected_index],
                          signature);
      ++checkpoint;
    }
  }
}

void main(void) {
  abi_u16 seed_index;
  abi_u16 expression;

  c166_test_begin(ABI_SEEDED_F32_SUITE_ID, 0x16600137UL);
  for (seed_index = 0U; seed_index < ABI_SEEDED_F32_SEED_COUNT; ++seed_index)
    for (expression = 0U; expression < ABI_SEEDED_F32_EXPRESSION_COUNT;
         ++expression)
      run_group(seed_index, expression);
  c166_test_finish();
  simulator_stop();
}

