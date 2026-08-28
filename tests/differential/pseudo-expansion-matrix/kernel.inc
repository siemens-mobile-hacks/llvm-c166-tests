#include "types.h"
#include "c166-address-spaces.h"

#ifndef PSEUDO_NAME
#error PSEUDO_NAME must be defined
#endif

#define PSEUDO_CAT_INNER(a, b) a##b
#define PSEUDO_CAT(a, b) PSEUDO_CAT_INNER(a, b)
#define PSEUDO_SYMBOL(name) PSEUDO_CAT(PSEUDO_NAME, name)

volatile abi_s8 PSEUDO_SYMBOL(_default_s8);
volatile abi_u8 PSEUDO_SYMBOL(_default_u8);
volatile abi_s8 _near PSEUDO_SYMBOL(_near_s8);
volatile abi_s8 _huge PSEUDO_SYMBOL(_huge_s8);
volatile abi_s8 _near *volatile PSEUDO_SYMBOL(_near_sink);
volatile abi_s8 _huge *volatile PSEUDO_SYMBOL(_huge_sink);

abi_u32 PSEUDO_SYMBOL(_eval)(abi_u16 seed,
    volatile abi_s8 _far *far_value,
    volatile abi_s8 _huge *segmented_value, abi_u32 wide) {
  abi_s32 accumulator;

  PSEUDO_SYMBOL(_default_u8) = (abi_u8)(seed ^ 0xa5U);
  PSEUDO_SYMBOL(_default_s8) = (abi_s8)(seed >> 1);
  PSEUDO_SYMBOL(_near_s8) = (abi_s8)(seed + 3U);
  PSEUDO_SYMBOL(_huge_s8) = (abi_s8)(seed - 7U);
  PSEUDO_SYMBOL(_near_sink) = &PSEUDO_SYMBOL(_near_s8);
  PSEUDO_SYMBOL(_huge_sink) = &PSEUDO_SYMBOL(_huge_s8);

  accumulator = (abi_s32)PSEUDO_SYMBOL(_default_s8);
  accumulator += PSEUDO_SYMBOL(_default_u8);
  accumulator += PSEUDO_SYMBOL(_near_s8);
  accumulator += PSEUDO_SYMBOL(_huge_s8);
  accumulator += *far_value;
  accumulator += *segmented_value;
  accumulator += *PSEUDO_SYMBOL(_near_sink);
  accumulator += *PSEUDO_SYMBOL(_huge_sink);
  return (abi_u32)accumulator ^ (wide << 7U);
}

#undef PSEUDO_SYMBOL
#undef PSEUDO_CAT
#undef PSEUDO_CAT_INNER
