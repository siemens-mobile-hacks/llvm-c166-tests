#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

extern double __adddf3(double, double);
extern double __subdf3(double, double);
extern double __muldf3(double, double);
extern double __divdf3(double, double);

C166_TEST_NOINLINE
double c166_float64_eval(abi_u16 operation, double lhs, double rhs,
                         abi_u16 tail) {
  if (tail != 0x5a5aU)
    return 0.0;

  switch (operation) {
  case 0:
    return lhs + rhs;
  case 1:
    return lhs - rhs;
  case 2:
    return lhs * rhs;
  case 3:
    return lhs / rhs;
  case 4:
    return __adddf3(lhs, rhs);
  case 5:
    return __subdf3(lhs, rhs);
  case 6:
    return __muldf3(lhs, rhs);
  default:
    return __divdf3(lhs, rhs);
  }
}

static abi_u16 words_are_nan(const abi_u16 *words) {
  return (abi_u16)((words[0] & 0x7ff0U) == 0x7ff0U &&
                   ((words[0] & 0x000fU) != 0U || words[1] != 0U ||
                    words[2] != 0U || words[3] != 0U));
}

static void run_vector(abi_u16 path, abi_u16 operation, abi_u16 l0, abi_u16 l1,
                       abi_u16 l2, abi_u16 l3, abi_u16 r0, abi_u16 r1,
                       abi_u16 r2, abi_u16 r3, abi_u16 e0, abi_u16 e1,
                       abi_u16 e2, abi_u16 e3, abi_u16 any_nan_allowed) {
  abi_u16 actual[4];
  double lhs = c166_f64_from_words(l0, l1, l2, l3);
  double rhs = c166_f64_from_words(r0, r1, r2, r3);
  double result =
      c166_float64_eval((abi_u16)(operation + path * 4U), lhs, rhs, 0x5a5aU);

  c166_f64_to_words(result, actual);
  if (any_nan_allowed != 0U && words_are_nan(actual)) {
    actual[0] = e0;
    actual[1] = e1;
    actual[2] = e2;
    actual[3] = e3;
  }
  tap_is_u32(((abi_u32)actual[0] << 16) | actual[1], ((abi_u32)e0 << 16) | e1,
             "binary64 high words");
  tap_is_u32(((abi_u32)actual[2] << 16) | actual[3], ((abi_u32)e2 << 16) | e3,
             "binary64 low words");
}

#define RUN_VECTOR(id, operation, name, l0, l1, l2, l3, r0, r1, r2, r3, e0,    \
                   e1, e2, e3, nan_ok)                                         \
  run_vector(path, operation, l0, l1, l2, l3, r0, r1, r2, r3, e0, e1, e2, e3,  \
             nan_ok);

void main(void) {
  abi_u16 path;

  tap_plan(ABI_FLOAT64_VECTOR_COUNT * ABI_FLOAT64_EVALUATION_PATH_COUNT * 2U);
  for (path = 0U; path != ABI_FLOAT64_EVALUATION_PATH_COUNT; ++path) {
    ABI_FLOAT64_VECTORS(RUN_VECTOR)
  }
}
