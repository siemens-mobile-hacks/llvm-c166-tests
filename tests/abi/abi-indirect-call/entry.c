#include "c166_test.h"
#include "types.h"

C166_NOINLINE
abi_u32 c166_indirect_entry(abi_u16 prefix, abi_indirect_fn target,
                            abi_u16 a, abi_u16 b, abi_u16 c, abi_u32 pair,
                            abi_u16 tail) {
  return target(a, b, c, pair, tail) ^ prefix;
}
