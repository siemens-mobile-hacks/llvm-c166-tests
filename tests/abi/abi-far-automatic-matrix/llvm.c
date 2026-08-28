#include "types.h"

#define LLVM_SECTION(name) __attribute__((noinline, section(name)))

LLVM_SECTION(".llvm_stack_apply")
abi_u32 llvm_stack_apply(abi_u16 seed,
                         volatile struct abi_stack_arena *arena) {
  volatile abi_u8 *bytes = (volatile abi_u8 *)arena;
  abi_u16 dynamic = (seed >> 4) & 0x007fU;
  abi_u16 low = 0x4a39U;
  abi_u16 high = 0x6d2bU;
  abi_u16 index;

  for (index = 0; index != sizeof(*arena); ++index)
    bytes[index] = (abi_u8)(seed + index * 0x25U + 0x5bU);

  arena->byte0 = (abi_u8)seed;
  arena->byte1 = (abi_u8)(seed >> 8);
  arena->word2 = seed ^ 0xa55aU;
  bytes[3] ^= (abi_u8)((seed >> 3) | 0x21U);
  arena->long14 = 0x13579bdfUL ^ ((abi_u32)seed << 8);
  arena->middle[dynamic] = (abi_u8)(seed * 13U + 0x5bU);
  arena->middle[237] = (abi_u8)((seed >> 8) ^ 0xc3U);
  arena->word256 = seed + 0x2468U;
  arena->long258 = 0x89abcdefUL + seed;

  for (index = 0; index != sizeof(*arena); ++index) {
    low = low * 33U + bytes[index] + index;
    high = high * 257U;
    high ^= ((abi_u16)bytes[index] << 8) | index;
  }
  return ((abi_u32)high << 16) | low;
}

LLVM_SECTION(".llvm_stack_owner_local")
abi_u32 llvm_stack_owner_local(abi_u16 seed) {
  volatile struct abi_stack_wrapper local;
  abi_u32 result;

  local.before = 0x13579bdfUL;
  local.after = 0x89abcdefUL;
  result = llvm_stack_apply(seed, &local.arena);
  if (local.before != 0x13579bdfUL || local.after != 0x89abcdefUL)
    return 0xdeadc0deUL;
  return result;
}

LLVM_SECTION(".llvm_stack_owner_tasking")
abi_u32 llvm_stack_owner_tasking(abi_u16 seed) {
  volatile struct abi_stack_wrapper local;
  abi_u32 result;

  local.before = 0x13579bdfUL;
  local.after = 0x89abcdefUL;
  result = tasking_stack_apply(seed, &local.arena);
  if (local.before != 0x13579bdfUL || local.after != 0x89abcdefUL)
    return 0xdeadc0deUL;
  return result;
}

LLVM_SECTION(".llvm_stack_reverse_apply")
abi_u32 llvm_stack_reverse_apply(
    abi_u16 seed, volatile struct abi_stack_arena *arena) {
  return tasking_stack_apply(seed, arena);
}
