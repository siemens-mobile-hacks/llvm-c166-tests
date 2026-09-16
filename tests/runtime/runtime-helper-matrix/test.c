#include "c166_test.h"
#include "types.h"
#include "corpus.inc"

struct prng_state {
  abi_u16 low;
  abi_u16 high;
};

abi_u32 runtime_helper_eval(abi_u16 operation, abi_u32 a, abi_u32 b,
                            abi_u16 shift);

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

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 value) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

static abi_u16 run_group(abi_u16 seed_index, abi_u16 operation) {
  abi_u32 seed = abi_runtime_helper_seeds[seed_index];
  struct prng_state state;
  abi_u32 signature;
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  state.low = (abi_u16)seed;
  state.high = (abi_u16)(seed >> 16);
  signature = ABI_RUNTIME_HELPER_GROUP_SIGNATURE_SEED ^
              ((abi_u32)(seed_index + 1U) << 8) ^ operation;

  for (iteration = 0U; iteration < ABI_RUNTIME_HELPER_ITERATIONS;
       ++iteration) {
    abi_u32 a = next_u32(&state);
    abi_u32 b = next_u32(&state);
    abi_u32 shift_source = next_u32(&state);
    abi_u16 shift;
    abi_u32 actual;

    if ((iteration & 7U) == 0U)
      a = abi_runtime_helper_edges[
          ((iteration >> 3) + seed_index) & 15U];
    if ((iteration & 7U) == 1U)
      b = abi_runtime_helper_edges[
          ((iteration >> 3) * 3U + seed_index) & 15U];
    if ((iteration & 7U) == 2U)
      shift_source = abi_runtime_helper_shift_edges[
          ((iteration >> 3) + seed_index) & 7U];
    shift = (abi_u16)shift_source & 31U;
    if (b == 0UL)
      b = 1UL;
    if (a == 0x80000000UL && b == 0xffffffffUL)
      b = 1UL;

    actual = runtime_helper_eval(operation, a, b, shift);
    signature = signature_step(signature, (abi_u16)(iteration + 1U), actual);

    if ((iteration & (ABI_RUNTIME_HELPER_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_RUNTIME_HELPER_CHECKPOINT_INTERVAL - 1U) {
      abi_u16 expected_index = (abi_u16)(
          seed_index * ABI_RUNTIME_HELPER_OPERATION_COUNT *
                  ABI_RUNTIME_HELPER_CHECKPOINTS_PER_GROUP +
          operation * ABI_RUNTIME_HELPER_CHECKPOINTS_PER_GROUP + checkpoint);

      if (signature != abi_runtime_helper_expected[expected_index])
        return checkpoint + 1U;
      ++checkpoint;
    }
  }
  return 0U;
}

void main(void) {
  abi_u16 seed_index;
  abi_u16 operation;

  tap_plan(ABI_RUNTIME_HELPER_SEED_COUNT *
           ABI_RUNTIME_HELPER_OPERATION_COUNT);
  for (seed_index = 0U; seed_index < ABI_RUNTIME_HELPER_SEED_COUNT;
       ++seed_index)
    for (operation = 0U; operation < ABI_RUNTIME_HELPER_OPERATION_COUNT;
         ++operation)
      tap_is_u32(run_group(seed_index, operation), 0UL,
                 "runtime helper group");
}
