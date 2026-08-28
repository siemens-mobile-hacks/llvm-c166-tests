#include "types.h"

extern abi_u32 llvm_entry_proxy(abi_u16 operation, abi_chain_fn callback,
                                abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d);

volatile abi_u16 tasking_nested_call_count;

static abi_u32 rotate_left(abi_u32 value, abi_u16 count) {
  return (value << count) | (value >> (32U - count));
}

abi_u32 tasking_nested_leaf_a(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d) {
  abi_u32 mixed;
  ++tasking_nested_call_count;
  mixed = (b + ((abi_u32)a << 16) + (abi_u32)c) ^ 0xa55a5aa5UL;
  return rotate_left(mixed, 9) + (abi_u32)d + 0x01020304UL;
}

abi_u32 tasking_nested_leaf_b(abi_u16 a, abi_u32 b, abi_u16 c, abi_u16 d) {
  abi_u32 mixed;
  ++tasking_nested_call_count;
  mixed = (b ^ ((abi_u32)c << 16) ^ (abi_u32)a) + 0x13579bdfUL;
  return rotate_left(mixed, 13) ^ (abi_u32)d ^ 0x2468ace0UL;
}

abi_u32 tasking_nested_reentrant(abi_u16 a, abi_u32 b, abi_u16 c,
                                 abi_u16 d) {
  abi_u32 result;
  ++tasking_nested_call_count;
  result = llvm_entry_proxy(ABI_NESTED_LEAF, (abi_chain_fn)0, a ^ 0x0f0fU,
                            b + 0x10203040UL, c ^ 0xa5a5U, d + 0x0101U);
  return result ^ 0x71e17e11UL;
}
