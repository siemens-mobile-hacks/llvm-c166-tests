#include "c166_test.h"

typedef unsigned int test_u16;

#if __C166_MEMORY_MODEL__ == 4
#define INTERRUPT_DATA
#else
#define INTERRUPT_DATA C166_FAR
#endif

volatile test_u16 interrupt_result INTERRUPT_DATA
    C166_SECTION(".c166.interrupt.data");
volatile test_u16 interrupt_depth INTERRUPT_DATA;

C166_NOINLINE test_u16 interrupt_mix(test_u16 a, test_u16 b, test_u16 c,
                                     test_u16 d, test_u16 e) {
  return (test_u16)(a * b + c * d + e);
}

__attribute__((interrupt(-1), section(".c166.interrupt.text")))
void interrupt_handler(void) {
  volatile test_u16 locals[4];

  locals[0] = 3U + interrupt_depth;
  locals[1] = 5U;
  locals[2] = 7U;
  locals[3] = 11U;
  if (interrupt_depth == 0U) {
    interrupt_result = 0xdeadU;
    interrupt_depth = 1U;
    __asm__ volatile("trap #124" ::: "memory");
    if (interrupt_result != 110U) {
      interrupt_result = 0xe01bU;
      return;
    }
    interrupt_depth = 0U;
  }
  interrupt_result =
      interrupt_mix(locals[0], locals[1], locals[2], locals[3], 13U);
}

test_u16 run_interrupt_probe(void);

void main(void) {
  tap_plan(2U);
  tap_is_u32(run_interrupt_probe(), 0UL,
             "interrupt preserves architectural state");
  tap_is_u32(interrupt_result, 105UL, "nested interrupt result");
}
