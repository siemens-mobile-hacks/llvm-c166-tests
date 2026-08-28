#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "corpus.inc"
#include "types.h"

extern abi_u32 tasking_seeded_integer_eval(abi_u16, abi_u32, abi_u32);
extern abi_u32 llvm_entry_proxy(abi_u16, abi_u32, abi_u32);

static abi_u32 prng_next(abi_u32 *state) {
  abi_u32 value = *state;
  value ^= value << 13;
  value ^= value >> 17;
  value ^= value << 5;
  *state = value;
  return value;
}

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 actual) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ actual ^ tag;
}

static abi_u32 reference_seed_signature(abi_u16 operation, abi_u32 seed,
                                        abi_u32 signature,
                                        abi_u16 vector_base) {
  abi_u32 state = seed;
  abi_u32 a;
  abi_u32 b;
  abi_u16 iteration;

  for (iteration = 0; iteration < ABI_SEEDED_INTEGER_ITERATIONS;
       ++iteration) {
    a = prng_next(&state);
    b = prng_next(&state);
    signature = signature_step(signature,
        (abi_u16)(vector_base + iteration + 1U),
        tasking_seeded_integer_eval(operation, a, b));
  }
  return signature;
}

static abi_u32 reference_expression_signature(abi_u16 operation) {
  abi_u32 signature =
      ABI_SEEDED_INTEGER_REFERENCE_SIGNATURE_SEED ^ operation;

#define ACCUMULATE_REFERENCE_SEED(seed_id, seed) \
  signature = reference_seed_signature(operation, seed, signature, \
      (abi_u16)((seed_id - 1U) * ABI_SEEDED_INTEGER_ITERATIONS));
  ABI_SEEDED_INTEGER_SEEDS(ACCUMULATE_REFERENCE_SEED)
#undef ACCUMULATE_REFERENCE_SEED

  return signature;
}

static void run_seed(abi_u16 seed_id, abi_u32 seed) {
  abi_u32 state = seed;
  abi_u32 a;
  abi_u32 b;
  abi_u32 expected;
  abi_u32 actual;
  abi_u16 iteration;
  abi_u16 operation;
  abi_u16 case_id;

  for (iteration = 0; iteration < ABI_SEEDED_INTEGER_ITERATIONS;
       ++iteration) {
    a = prng_next(&state);
    b = prng_next(&state);
    for (operation = 0; operation < ABI_SEEDED_INTEGER_OP_COUNT;
         ++operation) {
      case_id = (abi_u16)((((seed_id - 1U) *
          ABI_SEEDED_INTEGER_ITERATIONS) + iteration) *
          ABI_SEEDED_INTEGER_OP_COUNT + operation + 1U);
      c166_test_set_context(seed, operation, iteration, a, b);
      expected = tasking_seeded_integer_eval(operation, a, b);
      actual = llvm_entry_proxy(operation, a, b);
      c166_test_check_u32(case_id, expected, actual);
    }
  }
}

#define RUN_SEED(seed_id, seed) run_seed(seed_id, seed);
#define CHECK_REFERENCE_SIGNATURE(operation, name, expected)                \
  c166_test_set_context(0UL, operation, 0U, 0UL, 0UL);                      \
  c166_test_check_u32((abi_u16)(0x1000U + operation), expected,             \
      reference_expression_signature(operation));

void main(void) {
  c166_test_begin(ABI_SEEDED_INTEGER_SUITE_ID, 0x1660011cUL);
  ABI_SEEDED_INTEGER_REFERENCE_SIGNATURES(CHECK_REFERENCE_SIGNATURE)
  ABI_SEEDED_INTEGER_SEEDS(RUN_SEED)
  c166_test_finish();
  simulator_stop();
}

#undef CHECK_REFERENCE_SIGNATURE
#undef RUN_SEED

