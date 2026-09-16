#include "c166_test.h"
#include "types.h"

static abi_u32 rotate_left(abi_u32 value, abi_u16 count) {
  return (value << count) | (value >> (32U - count));
}

C166_NOINLINE C166_USED
abi_u32 c166_nested_leaf(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d) {
  abi_u32 mixed = b ^ ((abi_u32)a << 16) ^ (abi_u32)c;
  return rotate_left(mixed, 5) + (abi_u32)d + 0x16600001UL;
}

C166_NOINLINE C166_USED
abi_u32 c166_direct_indirect_middle(abi_chain_fn callback, abi_u16 a,
                                    abi_u32 b, abi_u16 c, abi_u16 d) {
  return callback(a ^ 0x55aaU, b + 0x01020304UL, c + 0x1111U,
                  d ^ 0x8001U) ^
         0xd1ec7001UL;
}

C166_NOINLINE C166_USED
abi_u32 c166_nested_dispatch(abi_u16 operation, abi_chain_fn callback,
                             abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d) {
  abi_u32 first;
  abi_u32 second;

  switch (operation) {
  case ABI_NESTED_DIRECT_INDIRECT:
    return c166_direct_indirect_middle(callback, a, b, c, d);
  case ABI_NESTED_INDIRECT_DIRECT:
    return callback(a, b, c, d) ^ 0x1d1ec701UL;
  case ABI_NESTED_LEAF:
    return c166_nested_leaf(a, b, c, d);
  case ABI_NESTED_DIRECT_INDIRECT_TWICE:
    first = c166_direct_indirect_middle(callback, a, b, c, d);
    second = c166_direct_indirect_middle(callback, a + 1U,
                                         b ^ 0x80000001UL, c - 1U,
                                         d + 0x0101U);
    return rotate_left(first, 7) ^ second ^ 0x2ce20002UL;
  default:
    return 0xbad00000UL | operation;
  }
}
