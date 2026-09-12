#include "types.h"

typedef unsigned long long u64;
typedef signed long long s64;

static u64 make_u64(abi_u16 word3, abi_u16 word2, abi_u16 word1,
                    abi_u16 word0) {
  return (u64)word3 << 48 | (u64)word2 << 32 | (u64)word1 << 16 | word0;
}

static abi_u16 fold_u64(u64 value) {
  return (abi_u16)value ^ (abi_u16)(value >> 16) * 3U ^
         (abi_u16)(value >> 32) * 5U ^ (abi_u16)(value >> 48) * 7U;
}

__attribute__((noinline)) static abi_u16 fold_less(u64 value) {
  return fold_u64(value) ^ 0x1357U;
}

__attribute__((noinline)) static abi_u16 fold_not_less(u64 value) {
  return fold_u64(value) ^ 0x2468U;
}

__attribute__((noinline, section(".llvm_i64_compare_eval"))) abi_u16
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
