#include "c166-test-result.h"
#include "types.h"
#include "corpus.inc"

struct abi_u64_words {
  abi_u32 hi;
  abi_u32 lo;
};

extern volatile abi_u16 seeded_f64_inputs[12];
extern volatile abi_u16 seeded_f64_observed[4];
extern void llvm_seeded_f64_eval_proxy(abi_u16 expression);

void simulator_stop(void) {
  for (;;)
    ;
}

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

static void finite_bits(struct abi_u64_words *state,
                        struct abi_u64_words *bits) {
  xorshift64(state);
  bits->hi = state->hi;
  bits->lo = state->lo;
  if (((bits->hi >> 20) & 0x7ffUL) == 0x7ffUL)
    bits->hi ^= 0x00100000UL;
}

static void edge_bits(abi_u16 index, struct abi_u64_words *bits) {
  bits->hi = abi_seeded_f64_edge_hi[index];
  bits->lo = abi_seeded_f64_edge_lo[index];
}

static void write_input(abi_u16 index, const struct abi_u64_words *bits) {
  abi_u16 offset = index * 4U;
  seeded_f64_inputs[offset] = (abi_u16)(bits->hi >> 16);
  seeded_f64_inputs[offset + 1U] = (abi_u16)bits->hi;
  seeded_f64_inputs[offset + 2U] = (abi_u16)(bits->lo >> 16);
  seeded_f64_inputs[offset + 3U] = (abi_u16)bits->lo;
}

static void run_group(abi_u16 seed_index, abi_u16 expression) {
  struct abi_u64_words state;
  struct abi_u64_words a;
  struct abi_u64_words b;
  struct abi_u64_words c;
  abi_u32 signature;
  abi_u16 iteration;
  abi_u16 checkpoint = 0U;

  state.hi = abi_seeded_f64_seed_hi[seed_index];
  state.lo = abi_seeded_f64_seed_lo[seed_index];
  signature = ABI_SEEDED_F64_GROUP_SIGNATURE_SEED ^
              ((abi_u32)(seed_index + 1U) << 8) ^ expression;

  for (iteration = 0U; iteration < ABI_SEEDED_F64_ITERATIONS; ++iteration) {
    abi_u16 expected_index;
    abi_u16 case_id;
    abi_u32 actual_hi;
    abi_u32 actual_lo;

    finite_bits(&state, &a);
    finite_bits(&state, &b);
    finite_bits(&state, &c);
    if ((iteration & 3U) == 0U)
      edge_bits((abi_u16)((iteration + seed_index + 1U) & 15U), &a);
    if ((iteration & 3U) == 1U)
      edge_bits((abi_u16)((iteration * 3U + seed_index + 1U) & 15U), &b);
    if ((iteration & 3U) == 2U)
      edge_bits((abi_u16)((iteration * 5U + seed_index + 1U) & 15U), &c);
    if ((b.hi & 0x7fffffffUL) == 0UL && b.lo == 0UL) {
      b.hi = 0x3ff00000UL;
      b.lo = 0UL;
    }

    write_input(0U, &a);
    write_input(1U, &b);
    write_input(2U, &c);
    llvm_seeded_f64_eval_proxy(expression);
    actual_hi = ((abi_u32)seeded_f64_observed[0] << 16) |
                seeded_f64_observed[1];
    actual_lo = ((abi_u32)seeded_f64_observed[2] << 16) |
                seeded_f64_observed[3];
    signature = signature_step(signature, (abi_u16)(iteration * 2U + 1U),
                               actual_hi);
    signature = signature_step(signature, (abi_u16)(iteration * 2U + 2U),
                               actual_lo);

    if ((iteration & (ABI_SEEDED_F64_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_SEEDED_F64_CHECKPOINT_INTERVAL - 1U) {
      expected_index = (abi_u16)(
          seed_index * ABI_SEEDED_F64_EXPRESSION_COUNT *
                  ABI_SEEDED_F64_CHECKPOINTS_PER_GROUP +
          expression * ABI_SEEDED_F64_CHECKPOINTS_PER_GROUP + checkpoint);
      case_id = (abi_u16)(expected_index + 1U);
      c166_test_set_context(
          abi_seeded_f64_seed_hi[seed_index] ^
              abi_seeded_f64_seed_lo[seed_index],
          (abi_u16)(((seed_index + 1U) << 8) | expression), iteration, a.hi,
          b.hi);
      c166_test_check_u32(case_id, abi_seeded_f64_expected[expected_index],
                          signature);
      ++checkpoint;
    }
  }
}

void main(void) {
  abi_u16 seed_index;
  abi_u16 expression;

  c166_test_begin(ABI_SEEDED_F64_SUITE_ID, 0x1660012eUL);
  for (seed_index = 0U; seed_index < ABI_SEEDED_F64_SEED_COUNT; ++seed_index)
#ifdef C166_SEEDED_F64_EXPRESSION
    run_group(seed_index, C166_SEEDED_F64_EXPRESSION);
#else
    for (expression = 0U; expression < ABI_SEEDED_F64_EXPRESSION_COUNT;
         ++expression)
      run_group(seed_index, expression);
#endif
  c166_test_finish();
  simulator_stop();
}

