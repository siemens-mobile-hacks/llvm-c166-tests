#include "types.h"
#include <stdarg.h>

typedef unsigned long long u64;
typedef signed long long s64;

struct wide_record {
  abi_u16 tag;
  u64 value;
};

static u64 make_u64(abi_u16 w3, abi_u16 w2, abi_u16 w1, abi_u16 w0) {
  volatile union {
    u64 all;
    abi_u16 word[4];
  } value;
  value.word[0] = w0;
  value.word[1] = w1;
  value.word[2] = w2;
  value.word[3] = w3;
  return value.all;
}

static abi_u16 fold_u64(u64 value) {
  volatile union {
    u64 all;
    abi_u16 word[4];
  } words = {value};
  return words.word[0] ^ words.word[1] * 3U ^ words.word[2] * 5U ^
         words.word[3] * 7U;
}

__attribute__((noinline)) static u64 select_variadic(abi_u16 index, ...) {
  va_list arguments;
  u64 value;

  va_start(arguments, index);
  value = va_arg(arguments, u64);
  if (index != 0)
    value = va_arg(arguments, u64);
  va_end(arguments);
  return value;
}

__attribute__((noinline)) static struct wide_record
transform_record(struct wide_record input) {
  struct wide_record result = {
      (abi_u16)(input.tag ^ 0x55aaU), input.value + input.tag};
  return result;
}

__attribute__((noinline)) static u64 evaluate(u64 left, u64 right,
                                               abi_u16 operation) {
  switch (operation) {
  case 0: return left + right;
  case 1: return left - right;
  case 2: return left * right;
  case 3: return left / right;
  case 4: return left % right;
  case 5: return (u64)((s64)left / (s64)right);
  case 6: return (u64)((s64)left % (s64)right);
  case 7: return left << (abi_u16)right;
  case 8: return left >> (abi_u16)right;
  case 9: return (u64)((s64)left >> (abi_u16)right);
  case 10:
  case 11:
  case 12:
  case 13: return left * right;
  case 14:
  case 15:
  case 16:
  case 17: return left + right;
  case 18: return select_variadic(1, left, right);
  case 20: return left << 5;
  default: return 0;
  }
}

__attribute__((noinline, section(".llvm_i64_arithmetic_eval"))) abi_u16
llvm_i64_arithmetic_eval(abi_u16 left3, abi_u16 left2, abi_u16 left1,
                         abi_u16 left0, abi_u16 right3, abi_u16 right2,
                         abi_u16 right1, abi_u16 right0,
                         abi_u16 operation) {
  if (operation == 19) {
    struct wide_record input = {
        right0, make_u64(left3, left2, left1, left0)};
    struct wide_record result = transform_record(input);
    return fold_u64(result.value) ^ result.tag;
  }

  u64 result = evaluate(make_u64(left3, left2, left1, left0),
                        make_u64(right3, right2, right1, right0), operation);
  if (operation == 10)
    return (abi_u16)result;
  if (operation >= 10 && operation <= 17) {
    volatile union {
      u64 all;
      abi_u16 word[4];
    } words = {result};
    switch (operation) {
    case 10:
    case 14: return words.word[0];
    case 11:
    case 15: return words.word[1];
    case 12:
    case 16: return words.word[2];
    default: return words.word[3];
    }
  }
  return fold_u64(result);
}
