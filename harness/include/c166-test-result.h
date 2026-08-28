#ifndef C166_TEST_RESULT_H
#define C166_TEST_RESULT_H

#define C166_TEST_STATUS_PASS 0x600dU
#define C166_TEST_STATUS_FAIL 0xdeadU

struct c166_test_result_block {
  unsigned int status;
  unsigned int suite_id;
  unsigned int case_id;
  unsigned int expected_lo;
  unsigned int expected_hi;
  unsigned int actual_lo;
  unsigned int actual_hi;
  unsigned long signature;
  unsigned long seed;
  unsigned long input0;
  unsigned long input1;
  unsigned int expression_id;
  unsigned int iteration;
};

extern volatile struct c166_test_result_block c166_test_result;

void c166_test_begin(unsigned int suite_id, unsigned long seed);
void c166_test_set_context(unsigned long seed, unsigned int expression_id,
                           unsigned int iteration, unsigned long input0,
                           unsigned long input1);
void c166_test_check_u32(unsigned int case_id, unsigned long expected,
                         unsigned long actual);
void c166_test_finish(void);

#endif
