#include "c166-test-result.h"

volatile struct c166_test_result_block c166_test_result;

static unsigned long signature_step(unsigned long signature,
                                    unsigned int case_id,
                                    unsigned long actual) {
  unsigned long tag = ((unsigned long)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ actual ^ tag;
}

void c166_test_begin(unsigned int suite_id, unsigned long seed) {
  c166_test_result.status = C166_TEST_STATUS_FAIL;
  c166_test_result.suite_id = suite_id;
  c166_test_result.case_id = 0;
  c166_test_result.expected_lo = 0;
  c166_test_result.expected_hi = 0;
  c166_test_result.actual_lo = 0;
  c166_test_result.actual_hi = 0;
  c166_test_result.signature = seed;
  c166_test_result.seed = 0;
  c166_test_result.input0 = 0;
  c166_test_result.input1 = 0;
  c166_test_result.expression_id = 0;
  c166_test_result.iteration = 0;
}

void c166_test_set_context(unsigned long seed, unsigned int expression_id,
                           unsigned int iteration, unsigned long input0,
                           unsigned long input1) {
  if (c166_test_result.case_id == 0) {
    c166_test_result.seed = seed;
    c166_test_result.expression_id = expression_id;
    c166_test_result.iteration = iteration;
    c166_test_result.input0 = input0;
    c166_test_result.input1 = input1;
  }
}

void c166_test_check_u32(unsigned int case_id, unsigned long expected,
                         unsigned long actual) {
  c166_test_result.signature = signature_step(c166_test_result.signature,
                                              case_id, actual);
  if (expected != actual && c166_test_result.case_id == 0) {
    c166_test_result.case_id = case_id;
    c166_test_result.expected_lo = (unsigned int)expected;
    c166_test_result.expected_hi = (unsigned int)(expected >> 16);
    c166_test_result.actual_lo = (unsigned int)actual;
    c166_test_result.actual_hi = (unsigned int)(actual >> 16);
  }
}

void c166_test_finish(void) {
  if (c166_test_result.case_id == 0) {
    c166_test_result.seed = 0;
    c166_test_result.input0 = 0;
    c166_test_result.input1 = 0;
    c166_test_result.expression_id = 0;
    c166_test_result.iteration = 0;
    c166_test_result.status = C166_TEST_STATUS_PASS;
  }
}
