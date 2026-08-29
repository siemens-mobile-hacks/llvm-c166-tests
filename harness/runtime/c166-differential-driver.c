#include "c166-test-result.h"
#include "c166-test-runtime.h"

extern unsigned long c166_test_case(unsigned int case_id);
extern unsigned long llvm_entry_proxy(unsigned int case_id);

void main(void) {
  unsigned int case_id;
  unsigned long reference_signature;

  c166_test_begin(C166_TEST_SUITE_ID, C166_TEST_SEED);
  for (case_id = 0; case_id != C166_TEST_CASE_COUNT; ++case_id) {
    unsigned long value = c166_test_case(case_id);
    c166_test_check_u32(case_id + 1U, value, value);
  }

  reference_signature = c166_test_result.signature;
  c166_test_check_u32(0x1000U, C166_TEST_REFERENCE_SIGNATURE,
                      reference_signature);

  for (case_id = 0; case_id != C166_TEST_CASE_COUNT; ++case_id) {
    unsigned long expected = c166_test_case(case_id);
    unsigned long actual = llvm_entry_proxy(case_id);
    c166_test_check_u32(0x2000U + case_id, expected, actual);
  }

  c166_test_finish();
  simulator_stop();
}
