#include "c166_test.h"
#include "types.h"
#include "corpus.inc"

abi_u32 c166_seeded_integer_eval(abi_u16 operation, abi_u32 a, abi_u32 b);

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

static abi_u32 seed_signature(abi_u16 operation, abi_u32 seed,
                              abi_u32 signature, abi_u16 vector_base) {
  abi_u32 state = seed;
  abi_u16 iteration;

  for (iteration = 0U; iteration < ABI_SEEDED_INTEGER_ITERATIONS;
       ++iteration) {
    abi_u32 a = prng_next(&state);
    abi_u32 b = prng_next(&state);
    signature = signature_step(
        signature, (abi_u16)(vector_base + iteration + 1U),
        c166_seeded_integer_eval(operation, a, b));
  }
  return signature;
}

static abi_u32 operation_signature(abi_u16 operation) {
  abi_u32 signature =
      ABI_SEEDED_INTEGER_REFERENCE_SIGNATURE_SEED ^ operation;

#define ACCUMULATE_SEED(seed_id, seed)                                     \
  signature = seed_signature(                                              \
      operation, seed, signature,                                          \
      (abi_u16)((seed_id - 1U) * ABI_SEEDED_INTEGER_ITERATIONS));
  ABI_SEEDED_INTEGER_SEEDS(ACCUMULATE_SEED)
#undef ACCUMULATE_SEED

  return signature;
}

#define CHECK_OPERATION(operation, name, expected)                         \
  tap_is_u32(operation_signature(operation), expected, #name);

void main(void) {
  tap_plan(ABI_SEEDED_INTEGER_OP_COUNT);
  ABI_SEEDED_INTEGER_REFERENCE_SIGNATURES(CHECK_OPERATION)
}

#undef CHECK_OPERATION
