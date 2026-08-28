#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

static volatile abi_far_u8 far_alias_arena[FAR_ALIAS_ARENA_SIZE];

extern abi_u32 llvm_entry_proxy(abi_u16 operation,
                                volatile abi_far_u8 *lhs,
                                volatile abi_far_u8 *rhs, abi_u32 value,
                                abi_u16 aux, abi_u16 tail);

static void initialize_arena(abi_u16 seed) {
  abi_u16 index;
  for (index = 0U; index != FAR_ALIAS_ARENA_SIZE; ++index)
    far_alias_arena[index] =
        (abi_u8)(seed + index * 37U + (index >> 3) * 11U);
}

static abi_u32 arena_digest(void) {
  abi_u16 low = 0x4a39U;
  abi_u16 high = 0x6d2bU;
  abi_u16 index;
  for (index = 0U; index != FAR_ALIAS_ARENA_SIZE; ++index) {
    low = (abi_u16)(low * 33U + far_alias_arena[index] + index);
    high = (abi_u16)(high * 257U);
    high ^= (abi_u16)(((abi_u16)far_alias_arena[index] << 8) | index);
  }
  return ((abi_u32)high << 16) | low;
}

static void run_vector(abi_u16 id, abi_u16 operation, abi_u16 lhs_offset,
                       abi_u16 rhs_offset, abi_u32 value, abi_u16 aux,
                       abi_u16 tail, abi_u16 seed, abi_u32 expected,
                       abi_u32 expected_digest) {
  volatile abi_far_u8 *lhs;
  volatile abi_far_u8 *rhs;

  initialize_arena(seed);
  /* Obtain both pointers independently in TASKING-compiled code. */
  lhs = &far_alias_arena[lhs_offset];
  rhs = &far_alias_arena[rhs_offset];
  c166_test_set_context(seed, operation, id,
                        ((abi_u32)lhs_offset << 16) | rhs_offset, value);
  c166_test_check_u32(id * 2U - 1U, expected,
                      llvm_entry_proxy(operation, lhs, rhs, value, aux,
                                       tail));
  c166_test_check_u32(id * 2U, expected_digest, arena_digest());
}

#define RUN_VECTOR(id, operation, lhs, rhs, value, aux, tail, seed,          \
                   expected, digest)                                         \
  run_vector(id, operation, lhs, rhs, value, aux, tail, seed, expected,      \
             digest);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  c166_test_begin(322, 0x16600142UL);
  C166_FAR_ALIAS_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}
