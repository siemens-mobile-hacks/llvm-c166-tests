#include "c166_test.h"

static volatile unsigned long unsigned_a = 0xfedcba98UL;
static volatile unsigned long unsigned_b = 0x1234UL;
static volatile signed long signed_a = -123456L;
static volatile signed long signed_b = 321L;
static volatile float float_a = 1.5F;
static volatile float float_b = 2.25F;
static volatile double double_a = 5.0;
static volatile double double_b = 2.0;

C166_TEST_NOINLINE
unsigned long runtime_variants_case(unsigned int case_id) {
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

void main(void) {
  tap_plan(5);
  tap_is_u32(runtime_variants_case(0), 0x000e0042UL,
             "unsigned 32-bit division");
  tap_is_u32(runtime_variants_case(1), 0x00000930UL,
             "unsigned 32-bit remainder");
  tap_is_u32(runtime_variants_case(2), 0xfda34dc0UL,
             "signed 32-bit multiplication");
  tap_is_u32(runtime_variants_case(3), 0x00004058UL,
             "binary32 multiplication");
  tap_is_u32(runtime_variants_case(4), 0x00004004UL,
             "binary64 division");
}
