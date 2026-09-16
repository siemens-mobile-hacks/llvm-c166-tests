#include "c166_test.h"
#include "kernel.inc"

static const qualified_u16 seeds[] = {
    0x0000U, 0x0001U, 0x1234U, 0x7fffU,
    0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
};

#if C166_TEST_SMALL
static const qualified_u32 expected[] = {
    0xac4155b5UL, 0xac4155bdUL, 0xac41653dUL, 0xac42ca89UL,
    0xac4155b5UL, 0xac412771UL, 0xac40ca51UL, 0xac40ca89UL,
};
#else
static const qualified_u32 expected[] = {
    0xdeb486e5UL, 0xdeb486e6UL, 0xdeb47499UL, 0xdeb53fc8UL,
    0xdeb486e5UL, 0xdeb30587UL, 0xdeb23fbbUL, 0xdeb23fc8UL,
};
#endif

void main(void) {
  qualified_u16 index;

  tap_plan(17U);
  tap_is_u32(c166_qualified_globals(0U, 1U), 0UL,
             "qualified data initialization");
  for (index = 0; index != 8U; ++index) {
    tap_is_u32(c166_qualified_globals(seeds[index], 0U), expected[index],
               "qualified global access");
    tap_ok(c166_qualified_globals(0U, 1U) != 0UL,
           "qualified bss mutation");
  }
}
