#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

typedef unsigned long long u64;
typedef signed long long s64;

/* TASKING has no corresponding 64-bit integer type; this is LLVM-only. */

static u64 make_u64(abi_u16 word3, abi_u16 word2, abi_u16 word1,
                    abi_u16 word0) {
  return (u64)word3 << 48 | (u64)word2 << 32 | (u64)word1 << 16 | word0;
}

static abi_u16 fold_u64(u64 value) {
  return (abi_u16)value ^ (abi_u16)(value >> 16) * 3U ^
         (abi_u16)(value >> 32) * 5U ^ (abi_u16)(value >> 48) * 7U;
}

C166_NOINLINE static abi_u16 fold_less(u64 value) {
  return fold_u64(value) ^ 0x1357U;
}

C166_NOINLINE static abi_u16 fold_not_less(u64 value) {
  return fold_u64(value) ^ 0x2468U;
}

C166_NOINLINE C166_SECTION(".llvm_i64_compare_eval") abi_u16
llvm_i64_compare_eval(abi_u16 left3, abi_u16 left2, abi_u16 left1,
                      abi_u16 left0, abi_u16 right3, abi_u16 right2,
                      abi_u16 right1, abi_u16 right0, abi_u16 operation) {
  u64 left = make_u64(left3, left2, left1, left0);
  u64 right = make_u64(right3, right2, right1, right0);

  switch (operation) {
  case 0:
    return left == right;
  case 1:
    return left != right;
  case 2:
    return left < right;
  case 3:
    return left <= right;
  case 4:
    return left > right;
  case 5:
    return left >= right;
  case 6:
    return (s64)left < (s64)right;
  case 7:
    return (s64)left <= (s64)right;
  case 8:
    return (s64)left > (s64)right;
  case 9:
    return (s64)left >= (s64)right;
  case 10:
    return fold_u64(left >> 1);
  case 11:
    return fold_u64((u64)((s64)left >> 1));
  case 12:
    if (left < right)
      return fold_less(left);
    return fold_not_less(right);
  default:
    return 0;
  }
}

static abi_u16 fold_words(abi_u16 word3, abi_u16 word2, abi_u16 word1,
                          abi_u16 word0) {
  return word0 ^ word1 * 3U ^ word2 * 5U ^ word3 * 7U;
}

static abi_u16 words_less(abi_u16 left3, abi_u16 left2, abi_u16 left1,
                          abi_u16 left0, abi_u16 right3, abi_u16 right2,
                          abi_u16 right1, abi_u16 right0) {
  if (left3 != right3)
    return left3 < right3;
  if (left2 != right2)
    return left2 < right2;
  if (left1 != right1)
    return left1 < right1;
  return left0 < right0;
}

static void run_vector(abi_u16 left3, abi_u16 left2, abi_u16 left1,
                       abi_u16 left0, abi_u16 right3, abi_u16 right2,
                       abi_u16 right1, abi_u16 right0, abi_u16 expected) {
  abi_u16 actual = 0;
  abi_u16 operation;

  for (operation = 0; operation != 10; ++operation)
    actual |= llvm_i64_compare_eval(left3, left2, left1, left0, right3,
                                    right2, right1, right0, operation)
              << operation;
  tap_is_u32(actual, expected, "comparison bitmask");
  tap_is_u32(llvm_i64_compare_eval(left3, left2, left1, left0, right3,
                                   right2, right1, right0, 10),
             fold_words(left3 >> 1, (left2 >> 1) | (left3 << 15),
                        (left1 >> 1) | (left2 << 15),
                        (left0 >> 1) | (left1 << 15)),
             "logical shift");
  tap_is_u32(llvm_i64_compare_eval(left3, left2, left1, left0, right3,
                                   right2, right1, right0, 11),
             fold_words((left3 >> 1) | (left3 & 0x8000U),
                        (left2 >> 1) | (left3 << 15),
                        (left1 >> 1) | (left2 << 15),
                        (left0 >> 1) | (left1 << 15)),
             "arithmetic shift");
  tap_is_u32(
      llvm_i64_compare_eval(left3, left2, left1, left0, right3, right2,
                            right1, right0, 12),
      words_less(left3, left2, left1, left0, right3, right2, right1, right0)
          ? fold_words(left3, left2, left1, left0) ^ 0x1357U
          : fold_words(right3, right2, right1, right0) ^ 0x2468U,
      "conditional call");
}

#define RUN_VECTOR(id, l3, l2, l1, l0, r3, r2, r1, r0, expected) \
  run_vector(l3, l2, l1, l0, r3, r2, r1, r0, expected);

void main(void) {
  tap_plan(60);
  I64_COMPARISON_VECTORS(RUN_VECTOR)
}
