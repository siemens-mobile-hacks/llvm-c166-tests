#include "types.h"

float tasking_mixed_observed_float;
struct abi_mixed_pair tasking_mixed_observed_pair;
abi_u16 tasking_mixed_observed_pointed;
abi_u16 tasking_mixed_observed_tail;

#pragma fragment

void tasking_mixed_observe(float value, struct abi_mixed_pair pair,
                           abi_u16 pointed, abi_u16 tail) {
  tasking_mixed_observed_float = value;
  tasking_mixed_observed_pair = pair;
  tasking_mixed_observed_pointed = pointed;
  tasking_mixed_observed_tail = tail;
}

#define ABI_MIXED_FUNCTION tasking_mixed_recursion
#define ABI_MIXED_OTHER llvm_mixed_bridge
#include "mixed-kernel.inc"
