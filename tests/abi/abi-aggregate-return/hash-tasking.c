#include "types.h"
#include "hash-common.h"

abi_u16 hash_tasking_returns(abi_u16 seed) {
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
  return result;
}
