#include "c166_test.h"
#include "types.h"

void main(void) {
  unsigned int repetition;

  tap_plan(4U);
  for (repetition = 0; repetition != 2U; ++repetition) {
    tap_is_u32(stack_page_limit(), 0x1234U,
               "16 KiB automatic object");
    tap_is_u32(stack_escape_store(), 0x3456U,
               "pointer into large automatic object");
  }
}
