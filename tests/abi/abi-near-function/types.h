#ifndef ABI_NEAR_FUNCTION_TYPES_H
#define ABI_NEAR_FUNCTION_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#ifdef __clang__
#define ABI_NEAR_FUNCTION __attribute__((c166_near))
#else
#define ABI_NEAR_FUNCTION _near
#endif

union abi_u32_words {
  abi_u32 value;
  struct {
    abi_u16 low;
    abi_u16 high;
  } words;
};

typedef abi_u16 ABI_NEAR_FUNCTION abi_near_fn2_type(abi_u16 a, abi_u16 b);
typedef abi_near_fn2_type *abi_near_fn2;
typedef abi_u16 (*abi_far_fn2)(abi_u16 a, abi_u16 b);

abi_u16 ABI_NEAR_FUNCTION tasking_near_target(abi_u16 a, abi_u16 b);
abi_u16 tasking_far_target(abi_u16 a, abi_u16 b);
abi_u16 tasking_calls_llvm_near(abi_u16 a, abi_u16 b);

#endif
