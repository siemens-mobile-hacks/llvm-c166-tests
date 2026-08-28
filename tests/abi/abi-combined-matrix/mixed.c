#include "types.h"

abi_u16 mixed_probe(abi_s8 signed_byte, abi_u8 unsigned_byte,
                    abi_u32 long_value, abi_u16 stack_word,
                    abi_u16 stack_tail) {
  if (signed_byte != (abi_s8)-5)
    return 1U;
  if (unsigned_byte != (abi_u8)250)
    return 2U;
  if (long_value != 0x12345678UL)
    return 3U;
  if (stack_word != 0x9abcU || stack_tail != 0xdef0U)
    return 4U;
  return 42U;
}
