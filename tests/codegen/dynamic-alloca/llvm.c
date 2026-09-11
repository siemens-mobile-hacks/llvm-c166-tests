#include "kernel.h"

extern test_u16 tasking_alloca_callback(const test_u8 *, test_u16, test_u16,
                                        test_u16, test_u16);

__attribute__((noinline, used))
test_u16 llvm_dynamic_alloca(test_u16 count, test_u16 seed, test_u16 mode) {
  volatile test_u16 fixed[3];
  test_u16 index;
  test_u16 result;
  test_u16 outer_count = (test_u16)(count + 3U);
  test_u8 *outer = __builtin_alloca_with_align(outer_count, 32);

  fixed[0] = (test_u16)(seed ^ 0x1234U);
  fixed[1] = (test_u16)(count + 0x4321U);
  fixed[2] = (test_u16)(fixed[0] + fixed[1] + mode);

  if (((unsigned long)outer & 3UL) != 0)
    return 0xa001U;

  for (index = 0; index != outer_count; ++index)
    outer[index] = dynamic_byte(seed, index);

  result = tasking_alloca_callback(outer, outer_count, seed, fixed[0],
                                   fixed[1]);

  {
    test_u16 scratch_count = (test_u16)((count & 15U) + 1U);
    test_u16 scratch_seed = (test_u16)(seed + 0x1111U);
    test_u8 *scratch = __builtin_alloca(scratch_count);

    for (index = 0; index != scratch_count; ++index)
      scratch[index] = dynamic_byte(scratch_seed, index);
    if (tasking_alloca_callback(scratch, scratch_count, scratch_seed,
                                fixed[2], fixed[0]) !=
        expected_callback(scratch_count, scratch_seed, fixed[2], fixed[0]))
      return 0xa002U;
  }

  {
    test_u16 inner_count = (test_u16)((count & 31U) + 5U);
    test_u16 inner_seed = (test_u16)(seed ^ 0x5a5aU);
    test_u8 inner[inner_count];

    for (index = 0; index != inner_count; ++index)
      inner[index] = dynamic_byte(inner_seed, index);
    result ^= tasking_alloca_callback(inner, inner_count, inner_seed,
                                      fixed[1], fixed[2]);
  }

  result ^= tasking_alloca_callback(outer, outer_count, seed, fixed[0],
                                    fixed[1]);
  result ^= fixed[0];
  result ^= fixed[1];
  result ^= fixed[2];
  if (mode & 1U)
    return (test_u16)(result ^ 0x8001U);
  return result;
}
