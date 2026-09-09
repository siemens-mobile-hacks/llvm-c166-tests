#include "types.h"
#include "hash-common.h"
#include "indirect.inc"
#include "callback-state.inc"

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
  struct shape8 tail;
  abi_u16 result = 0x4a39U;
  abi_u16 index;

	aggregate_callback = llvm_return3;
	tail = tasking_return_tail(TAIL_ARGUMENTS(seed));

  MIX_VALUE(value1, 1);
  MIX_VALUE(value2, 2);
  MIX_VALUE(value3, 3);
  MIX_VALUE(value4, 4);
  MIX_VALUE(value5, 5);
  MIX_VALUE(value6, 6);
  MIX_VALUE(value7, 7);
  MIX_VALUE(value8, 8);
  MIX_VALUE(tail, 8);
	if (!check_indirect_returns(seed, &value3, &tail, tasking_return3, tasking_return_tail))
		return result ^ 0xffffU;

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
__attribute__((noinline, section(".llvm_return3")))
struct shape3 llvm_return3(abi_u16 seed) {
	struct shape3 value;
	abi_u16 index;

	mutate_callback_state();
	for (index = 0; index < 3; ++index)
		value.bytes[index] = (abi_u8)(seed + 0x33U + index * 0x23U);
	return value;
}

DEFINE_LLVM_RETURN(4)
DEFINE_LLVM_RETURN(5)
DEFINE_LLVM_RETURN(6)
DEFINE_LLVM_RETURN(7)
DEFINE_LLVM_RETURN(8)

__attribute__((noinline, section(".llvm_return_tail")))
struct shape8 llvm_return_tail(
    abi_u16 first, abi_u16 second, abi_u16 third, abi_u16 fourth,
    abi_u16 fifth, abi_u16 sixth, abi_u16 seventh, abi_u16 eighth) {
#include "tail.inc"
}
