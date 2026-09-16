#ifndef ABI_NEAR_FUNCTION_TYPES_H
#define ABI_NEAR_FUNCTION_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#include "c166_test.h"

union abi_u32_words {
  abi_u32 value;
  struct {
    abi_u16 low;
    abi_u16 high;
  } words;
};

typedef abi_u16 C166_NEAR abi_near_fn2_type(abi_u16 a, abi_u16 b);
typedef abi_near_fn2_type *abi_near_fn2;
typedef abi_u16 (*abi_far_fn2)(abi_u16 a, abi_u16 b);

abi_u16 C166_NEAR c166_near_code_base(abi_u16 a, abi_u16 b);
abi_u16 C166_NEAR c166_near_calls_near(abi_near_fn2 target, abi_u16 a,
                                       abi_u16 b);
abi_u16 C166_NEAR c166_near_calls_far(abi_far_fn2 target, abi_u16 a,
                                      abi_u16 b);
abi_u16 C166_NEAR c166_near_target(abi_u16 a, abi_u16 b);
abi_u16 c166_far_target(abi_u16 a, abi_u16 b);
abi_u16 c166_calls_near(abi_u16 a, abi_u16 b);
abi_u32 C166_NEAR c166_near_entry(abi_near_fn2 near_target,
                                  abi_far_fn2 far_target, abi_u16 a,
                                  abi_u16 b);

#endif
