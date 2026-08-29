#include "c166-test-compat.h"

static volatile unsigned long unsigned_a = 0xfedcba98UL;
static volatile unsigned long unsigned_b = 0x1234UL;
static volatile signed long signed_a = -123456L;
static volatile signed long signed_b = 321L;
static volatile float float_a = 1.5F;
static volatile float float_b = 2.25F;
static volatile double double_a = 5.0;
static volatile double double_b = 2.0;

C166_TEST_NOINLINE
unsigned long c166_test_case(unsigned int case_id) {
  union {
    float value;
    unsigned long bits;
  } float_result;
  union {
    double value;
    unsigned long words[2];
  } double_result;

  switch (case_id) {
  case 0:
    return unsigned_a / unsigned_b;
  case 1:
    return unsigned_a % unsigned_b;
  case 2:
    return (unsigned long)(signed_a * signed_b);
  case 3:
    float_result.value = float_a * float_b;
    return float_result.bits;
  case 4:
    double_result.value = double_a / double_b;
    return double_result.words[0] ^ double_result.words[1];
  default:
    return 0;
  }
}
