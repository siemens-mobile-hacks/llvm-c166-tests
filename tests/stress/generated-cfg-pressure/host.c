#include "c166-test-result.h"
#include "types.h"
#include "corpus.inc"

struct prng_state {
  u16 low;
  u16 high;
};

extern u32 llvm_entry_proxy(u16, u32, u32, u16);

void simulator_stop(void) {
  for (;;)
    ;
}

static u16 xorshift16(u16 value) {
  value ^= (u16)(value << 7);
  value ^= value >> 9;
  value ^= (u16)(value << 8);
  return value;
}

static u32 next_u32(struct prng_state *state) {
  state->low = xorshift16(state->low);
  state->high = xorshift16(state->high);
  if (state->low == 0U && state->high == 0U)
    state->high = 1U;
  return ((u32)state->high << 16) | state->low;
}

static void run_group(u16 kernel, u16 seed_index) {
  u32 seed = abi_generated_cfg_seeds[seed_index];
  struct prng_state state;
  u16 iteration;

  state.low = (u16)seed;
  state.high = (u16)(seed >> 16);
  for (iteration = 0U; iteration < ABI_GENERATED_CFG_ITERATIONS; ++iteration) {
    u32 a = next_u32(&state);
    u32 b = next_u32(&state);
    u32 selector_source = next_u32(&state);
    u16 selector = (u16)selector_source;
    u16 case_id;
    u32 actual;

    if ((iteration & 3U) == 0U)
      a = abi_generated_cfg_edges[(iteration + seed_index) & 15U];
    if ((iteration & 3U) == 1U)
      b = abi_generated_cfg_edges[(iteration * 3U + seed_index) & 15U];
    if ((iteration & 3U) == 2U)
      selector = abi_generated_cfg_selector_edges[
          (iteration + seed_index) & 15U];

    case_id = (u16)(kernel * ABI_GENERATED_CFG_SEED_COUNT *
                            ABI_GENERATED_CFG_ITERATIONS +
                    seed_index * ABI_GENERATED_CFG_ITERATIONS + iteration +
                    1U);
    actual = llvm_entry_proxy(kernel, a, b, selector);
    c166_test_set_context(seed, kernel, iteration, a, b);
    c166_test_check_u32(case_id,
                        abi_generated_cfg_expected[case_id - 1U], actual);
  }
}

void main(void) {
  u16 kernel;
  u16 seed_index;

  c166_test_begin(ABI_GENERATED_CFG_SUITE_ID, 0x16600136UL);
  for (kernel = 0U; kernel < ABI_GENERATED_CFG_KERNEL_COUNT; ++kernel)
    for (seed_index = 0U; seed_index < ABI_GENERATED_CFG_SEED_COUNT;
         ++seed_index)
      run_group(kernel, seed_index);
  c166_test_finish();
  simulator_stop();
}

