#include "types.h"
#include "vectors.inc"

static void reset_canaries(volatile struct abi_stack_wrapper *local) {
  local->before = 0x13579bdfUL;
  local->after = 0x89abcdefUL;
}

static void check_canaries(volatile struct abi_stack_wrapper *local) {
  tap_is_u32(local->before, 0x13579bdfUL, "leading stack canary");
  tap_is_u32(local->after, 0x89abcdefUL, "trailing stack canary");
}

static void run_far_automatic_vector(abi_u16 seed, abi_u32 expected) {
  volatile struct abi_stack_wrapper local;
  volatile struct abi_stack_arena C166_FAR *arena =
      (volatile struct abi_stack_arena C166_FAR *)&local.arena;
  abi_stack_apply_fn indirect = stack_apply;

  reset_canaries(&local);
  tap_is_u32(stack_apply(seed, arena), expected,
             "far pointer to caller automatic");
  check_canaries(&local);

  reset_canaries(&local);
  tap_is_u32(stack_reverse_apply(seed, arena), expected,
             "nested far pointer call");
  check_canaries(&local);

  reset_canaries(&local);
  tap_is_u32(indirect(seed, arena), expected,
             "indirect far pointer call");
  check_canaries(&local);

  tap_is_u32(stack_owner(seed), expected, "callee-owned automatic");
}

#define RUN_FAR_AUTOMATIC(id, seed, expected)                               \
  run_far_automatic_vector(seed, expected);

void main(void) {
  volatile struct abi_stack_wrapper layout;
  volatile abi_u8 *base = (volatile abi_u8 *)&layout.arena;

  tap_plan(49U);
  tap_is_u32(sizeof(struct abi_stack_arena), 262UL, "arena size");
  tap_is_u32((volatile abi_u8 *)&layout.arena.byte0 - base, 0UL,
             "byte0 offset");
  tap_is_u32((volatile abi_u8 *)&layout.arena.byte1 - base, 1UL,
             "byte1 offset");
  tap_is_u32((volatile abi_u8 *)&layout.arena.word2 - base, 2UL,
             "word2 offset");
  tap_is_u32((volatile abi_u8 *)&layout.arena.long14 - base, 14UL,
             "long14 offset");
  tap_is_u32((volatile abi_u8 *)&layout.arena.middle - base, 18UL,
             "middle offset");
  tap_is_u32((volatile abi_u8 *)&layout.arena.word256 - base, 256UL,
             "word256 offset");
  tap_is_u32((volatile abi_u8 *)&layout.arena.long258 - base, 258UL,
             "long258 offset");
  tap_is_u32(sizeof(struct abi_stack_wrapper), 270UL, "wrapper size");
  ABI_FAR_AUTOMATIC_VECTORS(RUN_FAR_AUTOMATIC)
}
