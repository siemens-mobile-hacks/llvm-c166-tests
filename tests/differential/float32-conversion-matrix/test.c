#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

extern abi_s32 c166_f32_to_i32(float);
extern abi_u32 c166_f32_to_u32(float);
extern abi_s16 c166_f32_to_i16(float);
extern abi_u16 c166_f32_to_u16(float);
extern float c166_i32_to_f32(abi_s32);
extern float c166_u32_to_f32(abi_u32);
extern float c166_i16_to_f32(abi_s16);
extern float c166_u16_to_f32(abi_u16);
extern abi_s16 c166_f32_compare(abi_u16, float, float);
extern abi_s32 c166_f32_compare_runtime(abi_u16, abi_u32, abi_u32);

struct f32_integer_vector {
  abi_u32 bits;
  abi_u32 expected;
};

struct s32_f32_vector {
  abi_s32 value;
  abi_u32 expected;
};

struct u32_f32_vector {
  abi_u32 value;
  abi_u32 expected;
};

struct s16_f32_vector {
  abi_s16 value;
  abi_u32 expected;
};

struct u16_f32_vector {
  abi_u16 value;
  abi_u32 expected;
};

struct f32_compare_vector {
  abi_u32 lhs;
  abi_u32 rhs;
  abi_s16 expected[7];
  abi_s32 runtime_expected[3];
};

#define F32_INTEGER_ROW(id, pool, bits, expected) {bits, expected},
static const struct f32_integer_vector f32_to_i32_vectors[] = {
    ABI_F32_TO_I32_VECTORS(F32_INTEGER_ROW)};
static const struct f32_integer_vector f32_to_u32_vectors[] = {
    ABI_F32_TO_U32_VECTORS(F32_INTEGER_ROW)};
static const struct f32_integer_vector f32_to_i16_vectors[] = {
    ABI_F32_TO_I16_VECTORS(F32_INTEGER_ROW)};
static const struct f32_integer_vector f32_to_u16_vectors[] = {
    ABI_F32_TO_U16_VECTORS(F32_INTEGER_ROW)};
#undef F32_INTEGER_ROW

#define S32_F32_ROW(id, value, expected) {value, expected},
static const struct s32_f32_vector i32_to_f32_vectors[] = {
    ABI_I32_TO_F32_VECTORS(S32_F32_ROW)};
#undef S32_F32_ROW

#define U32_F32_ROW(id, value, expected) {value, expected},
static const struct u32_f32_vector u32_to_f32_vectors[] = {
    ABI_U32_TO_F32_VECTORS(U32_F32_ROW)};
#undef U32_F32_ROW

#define S16_F32_ROW(id, value, expected) {value, expected},
static const struct s16_f32_vector i16_to_f32_vectors[] = {
    ABI_I16_TO_F32_VECTORS(S16_F32_ROW)};
#undef S16_F32_ROW

#define U16_F32_ROW(id, value, expected) {value, expected},
static const struct u16_f32_vector u16_to_f32_vectors[] = {
    ABI_U16_TO_F32_VECTORS(U16_F32_ROW)};
#undef U16_F32_ROW

#define F32_COMPARE_ROW(id, lhs_pool, rhs_pool, lhs_bits, rhs_bits, eq, ne,    \
                        lt, le, gt, ge, unord)                                 \
  {lhs_bits,                                                                   \
   rhs_bits,                                                                   \
   {eq, ne, lt, le, gt, ge, unord},                                            \
   {(lt)   ? -1L                                                               \
    : (eq) ? 0L                                                                \
           : 1L,                                                               \
    ((lt) || (unord)) ? -1L                                                    \
    : (eq)            ? 0L                                                     \
                      : 1L,                                                    \
    unord}},
static const struct f32_compare_vector compare_vectors[] = {
    ABI_F32_COMPARE_VECTORS(F32_COMPARE_ROW)};
#undef F32_COMPARE_ROW

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

static void run_f32_integer_vectors(void) {
  abi_u16 index;
  float value;

  for (index = 0U; index != ARRAY_COUNT(f32_to_i32_vectors); ++index) {
    value = c166_f32_from_bits(f32_to_i32_vectors[index].bits);
    tap_is_u32((abi_u32)c166_f32_to_i32(value),
               f32_to_i32_vectors[index].expected, "binary32 to signed 32");
  }
  for (index = 0U; index != ARRAY_COUNT(f32_to_u32_vectors); ++index) {
    value = c166_f32_from_bits(f32_to_u32_vectors[index].bits);
    tap_is_u32(c166_f32_to_u32(value), f32_to_u32_vectors[index].expected,
               "binary32 to unsigned 32");
  }
  for (index = 0U; index != ARRAY_COUNT(f32_to_i16_vectors); ++index) {
    value = c166_f32_from_bits(f32_to_i16_vectors[index].bits);
    tap_is_u32((abi_u32)(abi_s32)c166_f32_to_i16(value),
               f32_to_i16_vectors[index].expected, "binary32 to signed 16");
  }
  for (index = 0U; index != ARRAY_COUNT(f32_to_u16_vectors); ++index) {
    value = c166_f32_from_bits(f32_to_u16_vectors[index].bits);
    tap_is_u32((abi_u32)c166_f32_to_u16(value),
               f32_to_u16_vectors[index].expected, "binary32 to unsigned 16");
  }
}

static void run_integer_f32_vectors(void) {
  abi_u16 index;

  for (index = 0U; index != ARRAY_COUNT(i32_to_f32_vectors); ++index)
    tap_is_u32(
        c166_f32_to_bits(c166_i32_to_f32(i32_to_f32_vectors[index].value)),
        i32_to_f32_vectors[index].expected, "signed 32 to binary32");
  for (index = 0U; index != ARRAY_COUNT(u32_to_f32_vectors); ++index)
    tap_is_u32(
        c166_f32_to_bits(c166_u32_to_f32(u32_to_f32_vectors[index].value)),
        u32_to_f32_vectors[index].expected, "unsigned 32 to binary32");
  for (index = 0U; index != ARRAY_COUNT(i16_to_f32_vectors); ++index)
    tap_is_u32(
        c166_f32_to_bits(c166_i16_to_f32(i16_to_f32_vectors[index].value)),
        i16_to_f32_vectors[index].expected, "signed 16 to binary32");
  for (index = 0U; index != ARRAY_COUNT(u16_to_f32_vectors); ++index)
    tap_is_u32(
        c166_f32_to_bits(c166_u16_to_f32(u16_to_f32_vectors[index].value)),
        u16_to_f32_vectors[index].expected, "unsigned 16 to binary32");
}

static void run_comparisons(void) {
  abi_u16 index;
  abi_u16 operation;

  for (index = 0U; index != ARRAY_COUNT(compare_vectors); ++index) {
    float lhs = c166_f32_from_bits(compare_vectors[index].lhs);
    float rhs = c166_f32_from_bits(compare_vectors[index].rhs);
    for (operation = 0U; operation != 7U; ++operation)
      tap_is_u32((abi_u32)(abi_s32)c166_f32_compare(operation, lhs, rhs),
                 (abi_u32)(abi_s32)compare_vectors[index].expected[operation],
                 "binary32 comparison");
    for (operation = 0U; operation != 3U; ++operation)
      tap_is_u32((abi_u32)c166_f32_compare_runtime(operation,
                                                   compare_vectors[index].lhs,
                                                   compare_vectors[index].rhs),
                 (abi_u32)compare_vectors[index].runtime_expected[operation],
                 "binary32 runtime comparison");
  }
}

void main(void) {
  const abi_u16 conversion_count =
      ARRAY_COUNT(f32_to_i32_vectors) + ARRAY_COUNT(f32_to_u32_vectors) +
      ARRAY_COUNT(f32_to_i16_vectors) + ARRAY_COUNT(f32_to_u16_vectors) +
      ARRAY_COUNT(i32_to_f32_vectors) + ARRAY_COUNT(u32_to_f32_vectors) +
      ARRAY_COUNT(i16_to_f32_vectors) + ARRAY_COUNT(u16_to_f32_vectors);
  const abi_u16 comparison_count = ARRAY_COUNT(compare_vectors) * 10U;

  tap_plan(conversion_count + comparison_count);
  run_f32_integer_vectors();
  run_integer_f32_vectors();
  run_comparisons();
}
