#include "types.h"
#include "hash-common.h"

__attribute__((noinline))
abi_u16 llvm_entry(abi_u16 seed) {
  volatile abi_u16 guard0 = seed ^ 0xa55aU;
  volatile abi_u16 guard1 = seed + 0x1357U;
  struct shape1 value1 = tasking_return1(seed);
  struct shape2 value2 = tasking_return2(seed);
  struct shape3 value3 = tasking_return3(seed);
  struct shape4 value4 = tasking_return4(seed);
  struct shape5 value5 = tasking_return5(seed);
  struct shape6 value6 = tasking_return6(seed);
  struct shape7 value7 = tasking_return7(seed);
  struct shape8 value8 = tasking_return8(seed);
  struct shape8 tail = tasking_return_tail(TAIL_ARGUMENTS(seed));
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

  if (guard0 != (abi_u16)(seed ^ 0xa55aU) ||
      guard1 != (abi_u16)(seed + 0x1357U))
    return result ^ 0xffffU;
  return result;
}

#define DEFINE_LLVM_RETURN(size) \
  __attribute__((noinline, section(".llvm_return" #size))) \
  struct shape##size llvm_return##size(abi_u16 seed) { \
    struct shape##size value; \
    abi_u16 index; \
    for (index = 0; index < size; ++index) \
      value.bytes[index] = \
          (abi_u8)(seed + (abi_u16)size * 0x11U + index * 0x23U); \
    return value; \
  }

DEFINE_LLVM_RETURN(1)
DEFINE_LLVM_RETURN(2)
DEFINE_LLVM_RETURN(3)
DEFINE_LLVM_RETURN(4)
DEFINE_LLVM_RETURN(5)
DEFINE_LLVM_RETURN(6)
DEFINE_LLVM_RETURN(7)
DEFINE_LLVM_RETURN(8)

__attribute__((noinline, section(".llvm_return_tail")))
struct shape8 llvm_return_tail(
    abi_u16 first, abi_u16 second, abi_u16 third, abi_u16 fourth,
    abi_u16 fifth, abi_u16 sixth, abi_u16 seventh, abi_u16 eighth) {
  struct shape8 value;
  abi_u16 word0 = first + fifth;
  abi_u16 word1 = second + sixth;
  abi_u16 word2 = third + seventh;
  abi_u16 word3 = fourth + eighth;
  value.bytes[0] = (abi_u8)word0;
  value.bytes[1] = (abi_u8)(word0 >> 8);
  value.bytes[2] = (abi_u8)word1;
  value.bytes[3] = (abi_u8)(word1 >> 8);
  value.bytes[4] = (abi_u8)word2;
  value.bytes[5] = (abi_u8)(word2 >> 8);
  value.bytes[6] = (abi_u8)word3;
  value.bytes[7] = (abi_u8)(word3 >> 8);
  return value;
}
