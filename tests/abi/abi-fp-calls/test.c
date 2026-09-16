#include "types.h"

static const abi_u8 vectors[4][12] = {
    {0x80, 0x3f, 0, 0, 4, 0xc0, 0, 0, 0, 0, 0, 0},
    {0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0, 0, 0},
    {0x91, 0x3f, 0x45, 0x23, 0x23, 0x40, 0x67, 0x45, 0xab, 0x89, 0xef,
     0xcd},
    {0x80, 0, 1, 0, 0x10, 0, 1, 0, 2, 0, 3, 0}};

static const char *path_name(abi_u16 path) {
  switch (path) {
  case 0:
    return "direct FP call";
  case 1:
    return "indirect FP call";
  case 2:
    return "direct nested FP call";
  default:
    return "indirect nested FP call";
  }
}

static void run_vector(abi_u16 vector) {
  abi_float_fn volatile float_target = abi_fp_float;
  abi_double_fn volatile double_target = abi_fp_double;
  abi_u8 actual[46];
  abi_u8 expected[46];
  float single;
  float float_result;
  double wide;
  double double_result;
  abi_u16 path;
  abi_u16 index;

  for (index = 0U; index != 4U; ++index) {
    ((abi_u8 *)&single)[index] = vectors[vector][index];
    expected[9U + index] = vectors[vector][index];
    expected[17U + index] = vectors[vector][index];
    expected[21U + index] = vectors[vector][index];
  }
  for (index = 0U; index != 8U; ++index) {
    ((abi_u8 *)&wide)[index] = vectors[vector][4U + index];
    expected[1U + index] = vectors[vector][4U + index];
    expected[25U + index] = vectors[vector][4U + index];
    expected[37U + index] = vectors[vector][4U + index];
  }
  expected[0] = 0xa5;
  expected[13] = 0x57;
  expected[14] = 0x13;
  expected[15] = 0xdf;
  expected[16] = 0x9b;
  expected[33] = 0x57;
  expected[34] = 0x13;
  expected[35] = 0xdf;
  expected[36] = 0x9b;
  expected[45] = 0x5a;

  for (path = 0U; path != 4U; ++path) {
    for (index = 0U; index != 46U; ++index)
      actual[index] = 0xcc;
    actual[0] = 0xa5;
    actual[45] = 0x5a;

    if (path == 0U) {
      float_result =
          abi_fp_float(0x1357U, wide, single, 0x9bdfU, actual + 1U);
      double_result =
          abi_fp_double(0x1357U, single, wide, 0x9bdfU, actual + 21U);
    } else if (path == 1U) {
      float_result =
          float_target(0x1357U, wide, single, 0x9bdfU, actual + 1U);
      double_result =
          double_target(0x1357U, single, wide, 0x9bdfU, actual + 21U);
    } else {
      abi_fp_reverse(path - 2U, single, wide, actual + 1U);
    }
    if (path < 2U) {
      for (index = 0U; index != 4U; ++index)
        actual[17U + index] = ((abi_u8 *)&float_result)[index];
      for (index = 0U; index != 8U; ++index)
        actual[37U + index] = ((abi_u8 *)&double_result)[index];
    }
    for (index = 0U; index != 46U; ++index)
      tap_is_u32(actual[index], expected[index], path_name(path));
  }
}

void main(void) {
  abi_u16 vector;

  tap_plan(736U);
  for (vector = 0U; vector != 4U; ++vector)
    run_vector(vector);
}
