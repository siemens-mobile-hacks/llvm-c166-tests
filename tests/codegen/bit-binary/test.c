#include "c166_test.h"

extern unsigned long
c166_test_bit_binary(unsigned int lhs, unsigned int rhs, unsigned int flags,
                     unsigned int form) __asm__("_test_bit_binary");

static const unsigned int words[] = {0U, 0xffffU, 0x5555U, 0xaaaaU,
                                     1U, 0x8000U, 0x7fffU, 0xfffeU};

static unsigned long expected(unsigned int lhs, unsigned int rhs,
                              unsigned int form) {
  unsigned int operation = form / 5U;
  unsigned int placement = form % 5U;
  unsigned int mask = placement == 1U || placement == 4U ? 0x8000U : 1U;
  unsigned int a = (lhs & mask) != 0U;
  unsigned int b = placement == 0U || placement == 3U ? rhs >> 15 : lhs & 1U;
  unsigned int result_bit = a;
  unsigned int flags;

  switch (operation) {
  case 0:
    result_bit = b;
    break;
  case 1:
    result_bit = !b;
    break;
  case 2:
    result_bit = a & b;
    break;
  case 3:
    result_bit = a | b;
    break;
  case 4:
    result_bit = a ^ b;
    break;
  }
  if (operation < 2U)
    flags = b ? 1U : 8U;
  else
    flags = (a ^ b) | ((a & b) << 1) | ((a | b) << 2) | (!(a | b) << 3);
  return ((unsigned long)flags << 16) |
         ((lhs & ~mask) | (result_bit ? mask : 0U));
}

void main(void) {
  unsigned int form;

  tap_plan(30U);
  for (form = 0; form != 30U; ++form) {
    unsigned int left;
    unsigned int passed = 1U;

    for (left = 0; left != 8U; ++left) {
      unsigned int right;
      for (right = 0; right != 8U; ++right) {
        unsigned int flags;
        for (flags = 0; flags != 32U; ++flags) {
          unsigned long actual =
              c166_test_bit_binary(words[left], words[right], flags, form);
          if (actual != expected(words[left], words[right], form))
            passed = 0U;
        }
      }
    }
    tap_ok(passed, "bit binary form");
  }
}
