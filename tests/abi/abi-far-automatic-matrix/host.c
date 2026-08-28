#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_stack_apply_proxy(
    abi_u16 seed, volatile struct abi_stack_arena *arena);
extern abi_u32 llvm_stack_owner_local_proxy(abi_u16 seed);
extern abi_u32 llvm_stack_owner_tasking_proxy(abi_u16 seed);
extern abi_u32 llvm_stack_reverse_apply_proxy(
    abi_u16 seed, volatile struct abi_stack_arena *arena);

static void reset_canaries(volatile struct abi_stack_wrapper *local) {
  local->before = 0x13579bdfUL;
  local->after = 0x89abcdefUL;
}

static void check_canaries(abi_u16 first_case,
                           volatile struct abi_stack_wrapper *local) {
  c166_test_check_u32(first_case, 0x13579bdfUL, local->before);
  c166_test_check_u32(first_case + 1U, 0x89abcdefUL, local->after);
}

static void run_far_automatic_vector(abi_u16 vector_id, abi_u16 seed,
                                     abi_u32 golden) {
  volatile struct abi_stack_wrapper local;
  abi_u16 base = 9U + (vector_id - 1U) *
      ABI_FAR_AUTOMATIC_CASES_PER_VECTOR;

  reset_canaries(&local);
  c166_test_check_u32(base + 1U, golden,
                      tasking_stack_apply(seed, &local.arena));
  check_canaries(base + 2U, &local);

  reset_canaries(&local);
  c166_test_check_u32(base + 4U, golden,
                      llvm_stack_apply_proxy(seed, &local.arena));
  check_canaries(base + 5U, &local);

  reset_canaries(&local);
  c166_test_check_u32(base + 7U, golden,
                      llvm_stack_reverse_apply_proxy(seed, &local.arena));
  check_canaries(base + 8U, &local);

  c166_test_check_u32(base + 10U, golden,
                      llvm_stack_owner_local_proxy(seed));
  c166_test_check_u32(base + 11U, golden,
                      llvm_stack_owner_tasking_proxy(seed));
}

#define RUN_FAR_AUTOMATIC(id, seed, golden) \
  run_far_automatic_vector(id, seed, golden);

void main(void) {
  volatile struct abi_stack_wrapper layout;
  volatile abi_u8 *base = (volatile abi_u8 *)&layout.arena;

  c166_test_begin(274, 0x16600112UL);
  c166_test_check_u32(1, 262UL, sizeof(struct abi_stack_arena));
  c166_test_check_u32(2, 0UL,
      (volatile abi_u8 *)&layout.arena.byte0 - base);
  c166_test_check_u32(3, 1UL,
      (volatile abi_u8 *)&layout.arena.byte1 - base);
  c166_test_check_u32(4, 2UL,
      (volatile abi_u8 *)&layout.arena.word2 - base);
  c166_test_check_u32(5, 14UL,
      (volatile abi_u8 *)&layout.arena.long14 - base);
  c166_test_check_u32(6, 18UL,
      (volatile abi_u8 *)&layout.arena.middle - base);
  c166_test_check_u32(7, 256UL,
      (volatile abi_u8 *)&layout.arena.word256 - base);
  c166_test_check_u32(8, 258UL,
      (volatile abi_u8 *)&layout.arena.long258 - base);
  c166_test_check_u32(9, 270UL, sizeof(struct abi_stack_wrapper));
  ABI_FAR_AUTOMATIC_VECTORS(RUN_FAR_AUTOMATIC)
  c166_test_finish();
  simulator_stop();
}

