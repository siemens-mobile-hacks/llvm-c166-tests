#include "c166_test.h"

typedef unsigned int test_u16;

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
#define TEST_FUNCTION C166_NEAR
#else
#define TEST_FUNCTION C166_HUGE
#endif

test_u16 TEST_FUNCTION relax_probe(test_u16 value);

void main(void) {
  tap_plan(2U);
  tap_is_u32(relax_probe(0U), 0x1234UL, "relaxed zero branch");
  tap_is_u32(relax_probe(1U), 0x5678UL, "relaxed nonzero branch");
}
