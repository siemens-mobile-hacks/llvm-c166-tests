#include "c166_test.h"

extern unsigned int c166_saved_probe(unsigned int a, unsigned int b,
                                     unsigned int c, unsigned int d);

static unsigned int helper(unsigned int a, unsigned int b, unsigned int c,
                           unsigned int d) C166_NOINLINE;

C166_NOINLINE
unsigned int c166_saved_entry(unsigned int a, unsigned int b, unsigned int c,
                              unsigned int d) {
  unsigned int x0 = a + 10U;
  unsigned int x1 = b + 20U;
  unsigned int x2 = c + 30U;
  unsigned int x3 = d + 40U;
  unsigned int x4 = a ^ 0x55U;
  unsigned int x5 = b ^ 0xaaU;
  unsigned int x6 = c + 5U;
  unsigned int x7 = d + 6U;
  unsigned int middle = helper(x0, x1, x2, x3);

  return middle + x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7;
}

static unsigned int helper(unsigned int a, unsigned int b, unsigned int c,
                           unsigned int d) {
  return a + 2U * b + 3U * c + 4U * d;
}

void main(void) {
  tap_plan(1U);
  tap_is_u32(c166_saved_probe(1U, 2U, 3U, 4U), 710U, "callee-saved registers");
}
