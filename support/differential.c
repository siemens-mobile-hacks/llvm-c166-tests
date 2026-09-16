#include "c166_test.h"

unsigned long c166_test_case(unsigned int case_id);

static unsigned long signature_step(unsigned long signature,
                                    unsigned int case_id,
                                    unsigned long value) {
  unsigned long tag = ((unsigned long)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

void main(void) {
  unsigned int case_id;
  unsigned long signature = C166_SEED;

  for (case_id = 0; case_id != C166_CASE_COUNT; ++case_id)
    signature = signature_step(signature, case_id + 1U,
                               c166_test_case(case_id));

  tap_plan(1);
  tap_is_u32(signature, C166_EXPECTED_SIGNATURE, "differential signature");
}
