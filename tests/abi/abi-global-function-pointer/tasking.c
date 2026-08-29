#include "types.h"

/* External arrays use the default Large CFAR object class. Element zero is
   the writable global function-pointer slot under test. */
abi_fn volatile tasking_global_slot[4];

#pragma fragment

abi_u16 tasking_global_roundtrip(abi_fn target, abi_u16 a, abi_u16 b) {
  abi_fn loaded;
  abi_u16 result;

  tasking_global_slot[0] = target;
  loaded = tasking_global_slot[0];
  result = loaded(a, b);
  if (loaded != target)
    result ^= 0xffffU;
  return result;
}

abi_fn tasking_global_exchange(abi_fn target) {
  tasking_global_slot[0] = target;
  return tasking_global_slot[0];
}
