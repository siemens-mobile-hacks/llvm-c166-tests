#include "c166_test.h"
#include "layout-hash.h"
#include "types.h"

#define ABI_C166_CALL(name) c166_##name

C166_NOINLINE
abi_u16 c166_hash_layout(abi_u16 seed, abi_u16 *components) {
  abi_u16 result;
  ABI_HASH_LAYOUT(ABI_C166_CALL, seed, components, result);
  return result;
}
