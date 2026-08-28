#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "corpus.inc"

extern abi_u32 llvm_entry_proxy(abi_u16, abi_u32, abi_u32, abi_u16);

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
  abi_u32 a;
  abi_u32 b;
  abi_u32 selector_word;
  abi_u32 actual;
  abi_u16 selector;
  abi_u16 iteration;
  abi_u16 kernel;
  abi_u16 checkpoint = 0U;

  state.low = (abi_u16)seed;
  state.high = (abi_u16)(seed >> 16);
  for (kernel = 0U; kernel < ABI_GENERATED_TYPE_KERNEL_COUNT; ++kernel)
    signatures[kernel] = ABI_GENERATED_TYPE_GROUP_SIGNATURE_SEED ^
                         ((abi_u32)(seed_index + 1U) << 12) ^ kernel;

  for (iteration = 0U; iteration < ABI_GENERATED_TYPE_ITERATIONS;
       ++iteration) {
    a = next_u32(&state);
    b = next_u32(&state);
    selector_word = next_u32(&state);
    selector = (abi_u16)selector_word;
    if ((iteration & 7U) == 0U) {
      a = 0UL; b = 0UL; selector = 0U;
    } else if ((iteration & 7U) == 1U) {
      a = 0xffffffffUL; b = 1UL; selector = 0xffffU;
    } else if ((iteration & 7U) == 2U) {
      a = 0x80000000UL; b = 0x7fffffffUL; selector = 0x8000U;
    } else if ((iteration & 7U) == 3U) {
      a = 0x0000ffffUL; b = 0xffff0000UL; selector = 0x7fffU;
    }

    for (kernel = 0U; kernel < ABI_GENERATED_TYPE_KERNEL_COUNT; ++kernel) {
      abi_u16 expected_index;
      abi_u16 case_id;

      actual = llvm_entry_proxy(kernel, a, b, selector);
      signatures[kernel] = signature_step(
          signatures[kernel], (abi_u16)(iteration + 1U), actual);
      if ((iteration & (ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U)) ==
          ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U) {
        expected_index = (abi_u16)(
            seed_index * ABI_GENERATED_TYPE_KERNEL_COUNT *
                ABI_GENERATED_TYPE_CHECKPOINTS_PER_KERNEL +
            kernel * ABI_GENERATED_TYPE_CHECKPOINTS_PER_KERNEL + checkpoint);
        case_id = (abi_u16)(
            seed_index * ABI_GENERATED_TYPE_KERNEL_COUNT *
                ABI_GENERATED_TYPE_CHECKPOINTS_PER_KERNEL +
            checkpoint * ABI_GENERATED_TYPE_KERNEL_COUNT + kernel + 1U);
        c166_test_set_context(seed, kernel, iteration, a,
                              ((abi_u32)(abi_u16)b << 16) | selector);
        c166_test_check_u32(case_id,
            abi_generated_type_expected[expected_index], signatures[kernel]);
      }
    }
    if ((iteration & (ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_GENERATED_TYPE_CHECKPOINT_INTERVAL - 1U)
      ++checkpoint;
  }
}

void main(void) {
  abi_u16 seed_index;

  c166_test_begin(ABI_GENERATED_TYPE_SUITE_ID, 0x16600145UL);
  for (seed_index = 0U; seed_index < ABI_GENERATED_TYPE_SEED_COUNT;
       ++seed_index)
    run_seed(seed_index, abi_generated_type_seeds[seed_index]);
  c166_test_finish();
  simulator_stop();
}

