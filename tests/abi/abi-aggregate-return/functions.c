#include "types.h"
#include "callback-state.inc"
#include "hash-common.h"
#include "indirect.inc"

abi_aggregate_callback volatile aggregate_callback;
volatile struct abi_live_state aggregate_live;

#define DEFINE_RETURN(size)                                                    \
  C166_NOINLINE C166_SECTION(".aggregate_return" #size)                        \
  struct shape##size aggregate_return##size(abi_u16 seed) {                    \
    struct shape##size value;                                                  \
    abi_u16 index;                                                             \
    for (index = 0; index < size; ++index)                                     \
      value.bytes[index] =                                                     \
          (abi_u8)(seed + (abi_u16)size * 0x11U + index * 0x23U);              \
    return value;                                                              \
  }

DEFINE_RETURN(1)
DEFINE_RETURN(2)

C166_NOINLINE C166_SECTION(".aggregate_return3")
struct shape3 aggregate_return3(abi_u16 seed) {
  struct shape3 value;
  abi_u16 index;

  mutate_callback_state();
  for (index = 0; index < 3; ++index)
    value.bytes[index] = (abi_u8)(seed + 0x33U + index * 0x23U);
  return value;
}

DEFINE_RETURN(4)
DEFINE_RETURN(5)
DEFINE_RETURN(6)
DEFINE_RETURN(7)
DEFINE_RETURN(8)

C166_NOINLINE C166_SECTION(".aggregate_return_tail")
struct shape8 aggregate_return_tail(
    abi_u16 first, abi_u16 second, abi_u16 third, abi_u16 fourth,
    abi_u16 fifth, abi_u16 sixth, abi_u16 seventh, abi_u16 eighth) {
#include "tail.inc"
}

C166_NOINLINE
abi_u16 aggregate_return_hash(abi_u16 seed) {
  struct shape1 value1 = aggregate_return1(seed);
  struct shape2 value2 = aggregate_return2(seed);
  struct shape3 value3 = aggregate_return3(seed);
  struct shape4 value4 = aggregate_return4(seed);
  struct shape5 value5 = aggregate_return5(seed);
  struct shape6 value6 = aggregate_return6(seed);
  struct shape7 value7 = aggregate_return7(seed);
  struct shape8 value8 = aggregate_return8(seed);
  struct shape8 tail;
  abi_u16 result = 0x4a39U;
  abi_u16 index;

  aggregate_callback = aggregate_return3;
  tail = aggregate_return_tail(TAIL_ARGUMENTS(seed));

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

C166_NOINLINE
abi_u16 aggregate_return_indirect_ok(abi_u16 seed) {
  struct shape3 value3 = aggregate_return3(seed);
  struct shape8 tail;

  aggregate_callback = aggregate_return3;
  tail = aggregate_return_tail(TAIL_ARGUMENTS(seed));
  return check_indirect_returns(seed, &value3, &tail, aggregate_return3,
                                aggregate_return_tail);
}
