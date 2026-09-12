#include "c166-test-result.h"
#include "c166-test-runtime.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 llvm_entry_proxy(abi_u16, abi_u16, abi_u16, abi_u16, abi_u16,
                                abi_u16, abi_u16, abi_u16, abi_u16);

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

static void run_vector(abi_u16 vector_id, abi_u16 left3, abi_u16 left2,
                       abi_u16 left1, abi_u16 left0, abi_u16 right3,
                       abi_u16 right2, abi_u16 right1, abi_u16 right0,
                       abi_u16 expected) {
  abi_u16 actual = 0;
  abi_u16 operation;

  for (operation = 0; operation != 10; ++operation)
    actual |= llvm_entry_proxy(left3, left2, left1, left0, right3, right2,
                               right1, right0, operation)
              << operation;
  c166_test_check_u32(vector_id, expected, actual);

  c166_test_check_u32(
      100U + vector_id,
      fold_words(left3 >> 1, (left2 >> 1) | (left3 << 15),
                 (left1 >> 1) | (left2 << 15),
                 (left0 >> 1) | (left1 << 15)),
      llvm_entry_proxy(left3, left2, left1, left0, right3, right2, right1,
                       right0, 10));
  c166_test_check_u32(
      200U + vector_id,
      fold_words((left3 >> 1) | (left3 & 0x8000U),
                 (left2 >> 1) | (left3 << 15),
                 (left1 >> 1) | (left2 << 15),
                 (left0 >> 1) | (left1 << 15)),
      llvm_entry_proxy(left3, left2, left1, left0, right3, right2, right1,
                       right0, 11));
  c166_test_check_u32(
      300U + vector_id,
      (words_less(left3, left2, left1, left0, right3, right2, right1, right0)
           ? fold_words(left3, left2, left1, left0) ^ 0x1357U
           : fold_words(right3, right2, right1, right0) ^ 0x2468U),
      llvm_entry_proxy(left3, left2, left1, left0, right3, right2, right1,
                       right0, 12));
}

#define RUN_VECTOR(id, l3, l2, l1, l0, r3, r2, r1, r0, expected)               \
  run_vector(id, l3, l2, l1, l0, r3, r2, r1, r0, expected);

void main(void) {
  c166_test_begin(320, 0);
  I64_COMPARISON_VECTORS(RUN_VECTOR)
  c166_test_finish();
  simulator_stop();
}
