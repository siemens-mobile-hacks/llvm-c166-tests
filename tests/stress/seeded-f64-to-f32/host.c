#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "corpus.inc"

struct abi_u64_words {
  abi_u32 hi;
  abi_u32 lo;
};

extern volatile abi_u16 llvm_float_width_observed[4];
extern void llvm_f64_to_f32_proxy(abi_u16, abi_u16, abi_u16, abi_u16);

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 value) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

static void xorshift64(struct abi_u64_words *value) {
  abi_u32 hi = value->hi;
  abi_u32 lo = value->lo;

  value->hi ^= (hi << 13) | (lo >> 19);
  value->lo ^= lo << 13;
  hi = value->hi;
  lo = value->lo;
  value->hi ^= hi >> 7;
  value->lo ^= (lo >> 7) | (hi << 25);
  hi = value->hi;
  lo = value->lo;
  value->hi ^= (hi << 17) | (lo >> 15);
  value->lo ^= lo << 17;
}

static void run_group(abi_u16 seed_index) {
  struct abi_u64_words state;
  struct abi_u64_words bits;
  abi_u32 signature;
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  state.hi = abi_f64_to_f32_seed_hi[seed_index];
  state.lo = abi_f64_to_f32_seed_lo[seed_index];
  signature = ABI_F64_TO_F32_GROUP_SIGNATURE_SEED ^
              ((abi_u32)(seed_index + 1U) << 8);

  for (iteration = 0U; iteration < ABI_F64_TO_F32_ITERATIONS; ++iteration) {
    abi_u16 expected_index;
    abi_u16 case_id;
    abi_u32 actual;

    xorshift64(&state);
    bits = state;
    if ((iteration & 7U) == 0U) {
      abi_u16 edge =
          (abi_u16)((iteration / 8U + seed_index) &
                    (ABI_F64_TO_F32_EDGE_COUNT - 1U));
      bits.hi = abi_f64_to_f32_edge_hi[edge];
      bits.lo = abi_f64_to_f32_edge_lo[edge];
    }

    llvm_f64_to_f32_proxy((abi_u16)(bits.hi >> 16), (abi_u16)bits.hi,
                          (abi_u16)(bits.lo >> 16), (abi_u16)bits.lo);
    actual = ((abi_u32)llvm_float_width_observed[0] << 16) |
             llvm_float_width_observed[1];
    signature = signature_step(signature, (abi_u16)(iteration + 1U), actual);

    if ((iteration & (ABI_F64_TO_F32_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_F64_TO_F32_CHECKPOINT_INTERVAL - 1U) {
      expected_index =
          (abi_u16)(seed_index * ABI_F64_TO_F32_CHECKPOINTS_PER_GROUP +
                    checkpoint);
      case_id = (abi_u16)(expected_index + 1U);
      c166_test_set_context(
          abi_f64_to_f32_seed_hi[seed_index] ^
              abi_f64_to_f32_seed_lo[seed_index],
          0U, iteration, bits.hi, bits.lo);
      c166_test_check_u32(case_id, abi_f64_to_f32_expected[expected_index],
                          signature);
      ++checkpoint;
    }
  }
}

void main(void) {
  abi_u16 seed_index;

  c166_test_begin(ABI_F64_TO_F32_SUITE_ID, 0x16600132UL);
  for (seed_index = 0U; seed_index < ABI_F64_TO_F32_SEED_COUNT; ++seed_index)
    run_group(seed_index);
  c166_test_finish();
  simulator_stop();
}
