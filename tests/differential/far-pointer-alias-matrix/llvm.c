#include "types.h"

#define LLVM_FUNCTION __attribute__((noinline, used))

static abi_u16 relation_mask(volatile abi_far_u8 *lhs,
                             volatile abi_far_u8 *rhs) {
  return (abi_u16)((lhs == rhs) | ((lhs != rhs) << 1) |
                   ((lhs < rhs) << 2) | ((lhs <= rhs) << 3) |
                   ((lhs > rhs) << 4) | ((lhs >= rhs) << 5));
}

LLVM_FUNCTION
abi_u32 llvm_far_alias_entry(abi_u16 operation,
                             volatile abi_far_u8 *lhs,
                             volatile abi_far_u8 *rhs, abi_u32 value,
                             abi_u16 aux, abi_u16 tail) {
  abi_u16 index;

  switch (operation) {
  case FAR_ALIAS_BYTE_RELATIONS: {
    abi_s16 difference = lhs - rhs;
    return ((abi_u32)(abi_u16)difference << 16) |
           relation_mask(lhs, rhs);
  }
  case FAR_ALIAS_BYTE_WRITE_ORDER:
    *lhs = (abi_u8)value;
    *rhs = (abi_u8)(value >> 8);
    return ((abi_u32)*lhs << 8) | *rhs;
  case FAR_ALIAS_WORD_BYTE_OVERLAP: {
    volatile abi_far_u16 *word = (volatile abi_far_u16 *)lhs;
    *word = (abi_u16)value;
    *rhs = (abi_u8)(value >> 16);
    return ((abi_u32)*word << 16) | *rhs;
  }
  case FAR_ALIAS_LONG_BYTE_OVERLAP: {
    volatile abi_far_u32 *wide = (volatile abi_far_u32 *)lhs;
    *wide = value;
    *rhs = (abi_u8)tail;
    return *wide ^ ((abi_u32)*rhs << 24);
  }
  case FAR_ALIAS_OBSERVED_ORDER: {
    abi_u8 old_rhs = *rhs;
    abi_u8 observed_lhs;
    *lhs = (abi_u8)value;
    observed_lhs = *lhs;
    *rhs = (abi_u8)(value >> 8);
    return old_rhs | ((abi_u32)observed_lhs << 8) |
           ((abi_u32)*lhs << 16) | ((abi_u32)*rhs << 24);
  }
  case FAR_ALIAS_WORD_RELATIONS: {
    volatile abi_far_u16 *word_lhs = (volatile abi_far_u16 *)lhs;
    volatile abi_far_u16 *word_rhs = (volatile abi_far_u16 *)rhs;
    abi_s16 difference = word_lhs - word_rhs;
    return ((abi_u32)(abi_u16)difference << 16) |
           relation_mask(lhs, rhs);
  }
  case FAR_ALIAS_FORWARD_COPY:
    for (index = 0U; index != aux; ++index)
      rhs[index] = lhs[index];
    return lhs[0] | ((abi_u32)lhs[aux - 1U] << 8) |
           ((abi_u32)rhs[0] << 16) | ((abi_u32)rhs[aux - 1U] << 24);
  case FAR_ALIAS_REVERSE_COPY:
    index = aux;
    while (index != 0U) {
      --index;
      rhs[index] = lhs[index];
    }
    return lhs[0] | ((abi_u32)lhs[aux - 1U] << 8) |
           ((abi_u32)rhs[0] << 16) | ((abi_u32)rhs[aux - 1U] << 24);
  case FAR_ALIAS_POINTER_SELECT: {
    volatile abi_far_u8 *selected = (aux & 1U) != 0U ? lhs : rhs;
    abi_u8 old = *selected;
    *selected = (abi_u8)value;
    return old | ((abi_u32)*lhs << 8) | ((abi_u32)*rhs << 16) |
           ((abi_u32)(selected == lhs) << 24);
  }
  default:
    return 0xbad00000UL | operation;
  }
}
