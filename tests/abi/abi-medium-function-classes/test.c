#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

static abi_u16 expected_callback_near(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x1357U) ^
                   (abi_u16)(b + (abi_u16)(a << 1)));
}

static abi_u16 expected_callback_huge(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0xa55aU) +
                   (abi_u16)(b ^ (abi_u16)(a >> 1)));
}

static abi_u16 expected_target_near(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a + 0x2468U) ^
                   (abi_u16)(b + (abi_u16)(a << 2)));
}

static abi_u16 expected_target_huge(abi_u16 a, abi_u16 b) {
  return (abi_u16)((abi_u16)(a ^ 0x5aa5U) +
                   (abi_u16)(b ^ (abi_u16)(a >> 2)));
}

static void run_vector(abi_u16 a, abi_u16 b) {
  abi_near_fn2 near_target = c166_medium_near_target;
  abi_huge_fn2 huge_target = c166_medium_huge_target;
  abi_u16 callback_near = expected_callback_near(a, b);
  abi_u16 callback_huge = expected_callback_huge(a, b);
  abi_u16 target_near = expected_target_near(a, b);
  abi_u16 target_huge = expected_target_huge(a, b);
  abi_u32 expected_dispatch =
      (abi_u32)(callback_near ^ (abi_u16)(callback_near + 0x1111U)) |
      ((abi_u32)(callback_huge + (abi_u16)(callback_huge ^ 0x2222U)) << 16);

  tap_is_u32(c166_medium_near_callback(a, b), callback_near,
             "direct near callback");
  tap_is_u32(c166_medium_huge_callback(a, b), callback_huge,
             "direct huge callback");
  tap_is_u32(c166_medium_dispatch(c166_medium_near_callback,
                                  c166_medium_huge_callback, a, b),
             expected_dispatch, "mixed function-class dispatch");
  tap_is_u32(c166_medium_near_target(a, b), target_near,
             "direct near target");
  tap_is_u32(near_target(a, b), target_near, "indirect near target");
  tap_is_u32(huge_target(a, b), target_huge, "indirect huge target");
  tap_is_u32(c166_medium_huge_target(a, b), target_huge,
             "direct huge target");
  tap_ok(near_target == c166_medium_near_target,
         "near function-pointer identity");
  tap_ok(huge_target == c166_medium_huge_target,
         "huge function-pointer identity");
}

#define RUN_VECTOR(a, b) run_vector(a, b);

void main(void) {
  tap_plan(56U);
  tap_is_u32(sizeof(abi_near_fn2), 2UL, "near function-pointer size");
  tap_is_u32(sizeof(abi_huge_fn2), 4UL, "huge function-pointer size");
  ABI_MEDIUM_FUNCTION_CLASS_VECTORS(RUN_VECTOR)
}
