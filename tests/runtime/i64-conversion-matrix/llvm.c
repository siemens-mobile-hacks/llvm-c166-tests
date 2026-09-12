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

__attribute__((noinline, section(".llvm_i64_conversion_eval"))) abi_u16
llvm_i64_conversion_eval(abi_u16 word3, abi_u16 word2, abi_u16 word1,
                         abi_u16 word0, abi_u16 operation) {
  volatile u64 input = make_u64(word3, word2, word1, word0);
  volatile float single;
  volatile double binary64;

  switch (operation) {
  case 0:
    single = (float)(s64)input;
    return fold_u64((u64)(s64)single);
  case 1:
    single = (float)input;
    return fold_u64((u64)single);
  case 2:
    binary64 = (double)(s64)input;
    return fold_u64((u64)(s64)binary64);
  case 3:
    binary64 = (double)input;
    return fold_u64((u64)binary64);
  case 4:
    single = -12345.75F;
    return fold_u64((u64)(s64)single);
  case 5:
    single = 4294967296.0F;
    return fold_u64((u64)single);
  case 6:
    binary64 = -123456789.75;
    return fold_u64((u64)(s64)binary64);
  case 7:
    binary64 = 9227875636482146304.0;
    return fold_u64((u64)binary64);
  default:
    return 0;
  }
}
