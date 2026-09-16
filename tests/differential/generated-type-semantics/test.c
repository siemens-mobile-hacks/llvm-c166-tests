#include "c166_test.h"
#include "types.h"

#include "corpus.inc"

extern abi_u32 c166_generated_type_eval(abi_u16, abi_u32, abi_u32, abi_u16);

struct prng_state {
  abi_u16 low;
  abi_u16 high;
};

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

static void run_seed(abi_u16 seed_index, abi_u32 seed) {
  struct prng_state state;
  abi_u32 signatures[ABI_GENERATED_TYPE_KERNEL_COUNT];
  abi_u16 checkpoint = 0U;
  abi_u16 iteration;
  abi_u16 kernel;

  state.low = (abi_u16)seed;
  state.high = (abi_u16)(seed >> 16);
  for (kernel = 0U; kernel < ABI_GENERATED_TYPE_KERNEL_COUNT; ++kernel)
    signatures[kernel] = ABI_GENERATED_TYPE_GROUP_SIGNATURE_SEED ^
                         ((abi_u32)(seed_index + 1U) << 12) ^ kernel;

  for (iteration = 0U; iteration < ABI_GENERATED_TYPE_ITERATIONS; ++iteration) {
    abi_u32 a = next_u32(&state);
    abi_u32 b = next_u32(&state);
    abi_u16 selector = (abi_u16)next_u32(&state);

    if ((iteration & 7U) == 0U) {
      a = 0UL;
      b = 0UL;
      selector = 0U;
    } else if ((iteration & 7U) == 1U) {
      a = 0xffffffffUL;
      b = 1UL;
      selector = 0xffffU;
    } else if ((iteration & 7U) == 2U) {
      a = 0x80000000UL;
      b = 0x7fffffffUL;
      selector = 0x8000U;
    } else if ((iteration & 7U) == 3U) {
      a = 0x0000ffffUL;
      b = 0xffff0000UL;
      selector = 0x7fffU;
    }

    for (kernel = 0U; kernel < ABI_GENERATED_TYPE_KERNEL_COUNT; ++kernel) {
      signatures[kernel] =
          signature_step(signatures[kernel], (abi_u16)(iteration + 1U),
                         c166_generated_type_eval(kernel, a, b, selector));
      if ((iteration & (ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U)) ==
          ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U) {
        abi_u16 expected_index =
            (abi_u16)(seed_index * ABI_GENERATED_TYPE_KERNEL_COUNT *
                          ABI_GENERATED_TYPE_CHECKPOINTS_PER_KERNEL +
                      kernel * ABI_GENERATED_TYPE_CHECKPOINTS_PER_KERNEL +
                      checkpoint);
        tap_is_u32(signatures[kernel],
                   abi_generated_type_expected[expected_index],
                   "generated type semantics");
      }
    }
    if ((iteration & (ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U)
      ++checkpoint;
  }
}

void main(void) {
  abi_u16 seed_index;

  tap_plan(ABI_GENERATED_TYPE_CHECKPOINT_COUNT);
  for (seed_index = 0U; seed_index < ABI_GENERATED_TYPE_SEED_COUNT;
       ++seed_index)
    run_seed(seed_index, abi_generated_type_seeds[seed_index]);
}
