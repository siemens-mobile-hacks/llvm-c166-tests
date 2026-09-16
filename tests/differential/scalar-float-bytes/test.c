#include "c166_test.h"

typedef unsigned int u16;

extern u16 double_parameter_word(double);
extern u16 extended_parameter_word(float);
extern u16 read_first_word(const void *);
extern void store_two(void *, u16);

static const float constant_float = 1.0f;
static const double constant_double = 1.0;

/* The ABI's first word is 0x3f80 for float 1 and 0x3ff0 for double 1.
 * These exact values, not LLVM output, define the expected signatures.
 * Cases 5 and 6 observe 2.0 written as bytes by another translation unit.
 * Cases 8 and 9 exercise the width-conversion runtime in another TU.
 * Expected results: 3f80, 3f80, 3ff0, 3ff0, 3ff0, 1, 1, 3ff0, 3ff0, 8000. */
unsigned long c166_test_case(u16 case_id) {
  float single = 1.0f;
  double wide = 1.0;

  switch (case_id) {
  case 0:
    return read_first_word(&single);
  case 1:
    return read_first_word(&constant_float);
  case 2:
    return read_first_word(&wide);
  case 3:
    return read_first_word(&constant_double);
  case 4:
    return double_parameter_word(wide);
  case 5:
    store_two(&single, sizeof(single));
    return single == 2.0f;
  case 6:
    store_two(&wide, sizeof(wide));
    return wide == 2.0;
  case 7:
    return read_first_word(&wide);
	case 8:
		return extended_parameter_word(1.0f);
	default:
		return extended_parameter_word(-0.0f);
  }
}
