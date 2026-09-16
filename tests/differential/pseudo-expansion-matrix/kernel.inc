#include "types.h"
#include "c166_test.h"

volatile abi_s8 c166_pseudo_default_s8;
volatile abi_u8 c166_pseudo_default_u8;
volatile abi_s8 C166_NEAR c166_pseudo_near_s8;
volatile abi_s8 C166_HUGE c166_pseudo_huge_s8;
volatile abi_s8 C166_NEAR *volatile c166_pseudo_near_sink;
volatile abi_s8 C166_HUGE *volatile c166_pseudo_huge_sink;

abi_u32 c166_pseudo_eval(abi_u16 seed, volatile abi_s8 C166_FAR *far_value,
                         volatile abi_s8 C166_HUGE *segmented_value,
                         abi_u32 wide) {
  abi_s32 accumulator;

  c166_pseudo_default_u8 = (abi_u8)(seed ^ 0xa5U);
  c166_pseudo_default_s8 = (abi_s8)(seed >> 1);
  c166_pseudo_near_s8 = (abi_s8)(seed + 3U);
  c166_pseudo_huge_s8 = (abi_s8)(seed - 7U);
  c166_pseudo_near_sink = &c166_pseudo_near_s8;
  c166_pseudo_huge_sink = &c166_pseudo_huge_s8;

  accumulator = (abi_s32)c166_pseudo_default_s8;
  accumulator += c166_pseudo_default_u8;
  accumulator += c166_pseudo_near_s8;
  accumulator += c166_pseudo_huge_s8;
  accumulator += *far_value;
  accumulator += *segmented_value;
  accumulator += *c166_pseudo_near_sink;
  accumulator += *c166_pseudo_huge_sink;
  return (abi_u32)accumulator ^ (wide << 7U);
}
