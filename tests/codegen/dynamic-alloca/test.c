#include "c166_test.h"
#include "kernel.h"

extern test_u16 dynamic_alloca(test_u16 count, test_u16 seed, test_u16 mode);

static test_u16 expected_result(test_u16 count, test_u16 seed,
                                test_u16 mode) {
  test_u16 first = (test_u16)(seed ^ 0x1234U);
  test_u16 second = (test_u16)(count + 0x4321U);
  test_u16 third = (test_u16)(first + second + mode);
  test_u16 inner_count = (test_u16)((count & 31U) + 5U);
  test_u16 inner_seed = (test_u16)(seed ^ 0x5a5aU);
  test_u16 result;

  result = expected_callback((test_u16)(count + 3U), seed, first, second);
  result ^= expected_callback(inner_count, inner_seed, second, third);
  result ^= expected_callback((test_u16)(count + 3U), seed, first, second);
  result ^= first;
  result ^= second;
  result ^= third;
  if (mode & 1U)
    result ^= 0x8001U;
  return result;
}

void main(void) {
  static const test_u16 counts[] = {1U, 2U, 3U, 7U, 16U,
                                    31U, 64U, 127U, 255U};
  test_u16 repetition;
  test_u16 index;

  tap_plan(36);
  for (repetition = 0; repetition != 4U; ++repetition) {
    for (index = 0; index != sizeof(counts) / sizeof(counts[0]); ++index) {
      test_u16 seed = (test_u16)(0x2107U + repetition * 0x101U + index * 17U);
      test_u16 mode = (test_u16)(repetition + index);
      tap_is_u32(dynamic_alloca(counts[index], seed, mode),
                 expected_result(counts[index], seed, mode),
                 "dynamic alloca");
    }
  }
}
