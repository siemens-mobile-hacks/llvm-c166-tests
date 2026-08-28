#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "corpus.inc"

extern volatile struct abi_seeded_far_arena seeded_far_arena;
extern abi_u32 llvm_entry_proxy(
    abi_u16, volatile struct abi_seeded_far_arena *,
    abi_u16, abi_u16, abi_u32);

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

static abi_u16 operation_count(abi_u16 operation) {
  if (operation <= 2U || (operation >= 12U && operation <= 13U) ||
      operation == 15U)
    return ABI_SEEDED_FAR_BYTE_COUNT;
  if (operation <= 5U || operation == 14U)
    return ABI_SEEDED_FAR_WORD_COUNT;
  if (operation <= 8U)
    return ABI_SEEDED_FAR_LONG_COUNT;
  return ABI_SEEDED_FAR_RECORD_COUNT;
}

static void operation_indices(abi_u16 iteration, abi_u16 operation,
                              abi_u16 raw_index, abi_u16 raw_other,
                              abi_u16 *index, abi_u16 *other) {
  abi_u16 count = operation_count(operation);

  switch (iteration & 7U) {
  case 0U:
    *index = 0U;
    *other = count - 1U;
    return;
  case 1U:
    *index = count - 1U;
    *other = 0U;
    return;
  case 2U:
    *index = 1U;
    *other = count - 2U;
    return;
  case 3U:
    *index = count - 2U;
    *other = 1U;
    return;
  default:
    *index = (abi_u16)(raw_index + operation * 0x1111U) % count;
    *other = (abi_u16)(raw_other ^ operation * 0x0101U) % count;
    return;
  }
}

static void initialize_arena(abi_u16 seed) {
  abi_u16 index;
  abi_u32 repeated = ((abi_u32)seed << 16) | seed;

  for (index = 0U; index < ABI_SEEDED_FAR_BYTE_COUNT; ++index)
    seeded_far_arena.bytes[index] =
        (abi_u8)(seed + index * 37U + (index >> 4) * 11U);
  for (index = 0U; index < ABI_SEEDED_FAR_WORD_COUNT; ++index)
    seeded_far_arena.words[index] =
        (abi_u16)(seed ^ index * 257U ^ 0xa55aU);
  for (index = 0U; index < ABI_SEEDED_FAR_LONG_COUNT; ++index)
    seeded_far_arena.longs[index] =
        repeated ^ ((abi_u32)index << 8) ^ index ^ 0x13579bdfUL;
  for (index = 0U; index < ABI_SEEDED_FAR_RECORD_COUNT; ++index) {
    volatile struct abi_seeded_far_record *record =
        &seeded_far_arena.records[index];
    record->first = (abi_u8)(seed + index * 13U + 0x31U);
    record->second = (abi_u8)((seed >> 8) + index * 29U + 0x7bU);
    record->word = (abi_u16)(seed ^ index * 193U ^ 0x5aa5U);
    record->wide = repeated + ((abi_u32)index << 16) +
                   ((abi_u32)index << 9) + index + 0x2468ace0UL;
  }
}

static void run_seed(abi_u16 seed_index, abi_u32 seed) {
  struct prng_state state;
  abi_u32 signatures[ABI_SEEDED_FAR_OPERATION_COUNT];
  abi_u32 raw0;
  abi_u32 value;
  abi_u32 actual;
  abi_u16 raw_index;
  abi_u16 raw_other;
  abi_u16 index;
  abi_u16 other;
  abi_u16 iteration;
  abi_u16 operation;
  abi_u16 checkpoint = 0U;

  initialize_arena((abi_u16)seed);
  state.low = (abi_u16)seed;
  state.high = (abi_u16)(seed >> 16);
  for (operation = 0U; operation < ABI_SEEDED_FAR_OPERATION_COUNT;
       ++operation)
    signatures[operation] = ABI_SEEDED_FAR_GROUP_SIGNATURE_SEED ^
                            ((abi_u32)(seed_index + 1U) << 8) ^ operation;

  for (iteration = 0U; iteration < ABI_SEEDED_FAR_ITERATIONS; ++iteration) {
    raw0 = next_u32(&state);
    value = next_u32(&state);
    raw_index = (abi_u16)raw0;
    raw_other = (abi_u16)(raw0 >> 16);
    for (operation = 0U; operation < ABI_SEEDED_FAR_OPERATION_COUNT;
         ++operation) {
      abi_u16 expected_index;
      abi_u16 case_id;

      operation_indices(iteration, operation, raw_index, raw_other,
                        &index, &other);
      actual = llvm_entry_proxy(operation, &seeded_far_arena,
                                index, other, value);
      signatures[operation] = signature_step(
          signatures[operation], (abi_u16)(iteration + 1U), actual);
      if ((iteration & (ABI_SEEDED_FAR_CHECKPOINT_INTERVAL - 1U)) ==
          ABI_SEEDED_FAR_CHECKPOINT_INTERVAL - 1U) {
        expected_index = (abi_u16)(
            seed_index * ABI_SEEDED_FAR_OPERATION_COUNT *
                ABI_SEEDED_FAR_CHECKPOINTS_PER_OPERATION +
            operation * ABI_SEEDED_FAR_CHECKPOINTS_PER_OPERATION +
            checkpoint);
        case_id = (abi_u16)(
            seed_index * ABI_SEEDED_FAR_OPERATION_COUNT *
                ABI_SEEDED_FAR_CHECKPOINTS_PER_OPERATION +
            checkpoint * ABI_SEEDED_FAR_OPERATION_COUNT + operation + 1U);
        c166_test_set_context(seed, operation, iteration,
                              ((abi_u32)index << 16) | other, value);
        c166_test_check_u32(case_id, abi_seeded_far_expected[expected_index],
                            signatures[operation]);
      }
    }
    if ((iteration & (ABI_SEEDED_FAR_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_SEEDED_FAR_CHECKPOINT_INTERVAL - 1U)
      ++checkpoint;
  }
}

void main(void) {
  abi_u16 seed_index;

  c166_test_begin(ABI_SEEDED_FAR_SUITE_ID, 0x16600141UL);
  for (seed_index = 0U; seed_index < ABI_SEEDED_FAR_SEED_COUNT; ++seed_index)
    run_seed(seed_index, abi_seeded_far_seeds[seed_index]);
  c166_test_finish();
  simulator_stop();
}

