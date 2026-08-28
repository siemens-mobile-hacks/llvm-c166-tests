#include "types.h"
#include "hash-common.h"

#define DECLARE_PROXY(size) \
  extern struct shape##size llvm_return##size##_proxy(abi_u16 seed)

DECLARE_PROXY(1);
DECLARE_PROXY(2);
DECLARE_PROXY(3);
DECLARE_PROXY(4);
DECLARE_PROXY(5);
DECLARE_PROXY(6);
DECLARE_PROXY(7);
DECLARE_PROXY(8);

extern struct shape8 llvm_return_tail_proxy(
    abi_u16 first, abi_u16 second, abi_u16 third, abi_u16 fourth,
    abi_u16 fifth, abi_u16 sixth, abi_u16 seventh, abi_u16 eighth);

abi_u16 hash_llvm_returns(abi_u16 seed) {
  struct shape1 value1 = llvm_return1_proxy(seed);
  struct shape2 value2 = llvm_return2_proxy(seed);
  struct shape3 value3 = llvm_return3_proxy(seed);
  struct shape4 value4 = llvm_return4_proxy(seed);
  struct shape5 value5 = llvm_return5_proxy(seed);
  struct shape6 value6 = llvm_return6_proxy(seed);
  struct shape7 value7 = llvm_return7_proxy(seed);
  struct shape8 value8 = llvm_return8_proxy(seed);
  struct shape8 tail = llvm_return_tail_proxy(TAIL_ARGUMENTS(seed));
  abi_u16 result = 0x4a39U;
  abi_u16 index;

  MIX_VALUE(value1, 1);
  MIX_VALUE(value2, 2);
  MIX_VALUE(value3, 3);
  MIX_VALUE(value4, 4);
  MIX_VALUE(value5, 5);
  MIX_VALUE(value6, 6);
  MIX_VALUE(value7, 7);
  MIX_VALUE(value8, 8);
  MIX_VALUE(tail, 8);
  return result;
}
