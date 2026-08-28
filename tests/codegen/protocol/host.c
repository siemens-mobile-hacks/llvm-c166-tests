#include "c166-test-runtime.h"
#include "c166-test-result.h"

volatile unsigned int protocol_word;
volatile unsigned long protocol_long;
volatile signed char protocol_byte;

extern unsigned long llvm_entry_proxy(unsigned int a, unsigned long b,
                                      signed char c);
extern unsigned long tasking_mix_reference(unsigned int a, unsigned long b,
                                           signed char c);

void main(void) {
  unsigned long golden = 0x373facdbUL;
  unsigned long expected;
  unsigned long actual;

  /* _CSTART deliberately bypasses the TASKING CRT and its CINIT copy. */
  protocol_word = 0x1357U;
  protocol_long = 0x2468ace0UL;
  protocol_byte = -5;
  expected = tasking_mix_reference(protocol_word, protocol_long, protocol_byte);
  actual = llvm_entry_proxy(protocol_word, protocol_long, protocol_byte);

  c166_test_result.status = C166_TEST_STATUS_FAIL;
  c166_test_result.suite_id = 1;
  c166_test_result.case_id = 1;
  c166_test_result.expected_lo = (unsigned int)expected;
  c166_test_result.expected_hi = (unsigned int)(expected >> 16);
  c166_test_result.actual_lo = (unsigned int)actual;
  c166_test_result.actual_hi = (unsigned int)(actual >> 16);
  c166_test_result.signature = actual ^ 0xa5a55a5aUL;
  if (expected != golden) {
    c166_test_result.case_id = 2;
  } else if (actual == expected) {
    c166_test_result.case_id = 0;
    c166_test_result.status = C166_TEST_STATUS_PASS;
  }
  simulator_stop();
}

