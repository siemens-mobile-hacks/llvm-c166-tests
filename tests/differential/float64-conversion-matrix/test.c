#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

extern abi_s32 __ledf2(double, double);
extern abi_s32 __gedf2(double, double);
extern abi_s32 __unorddf2(double, double);

C166_TEST_NOINLINE
abi_s32 c166_f64_to_i32(double value) { return (abi_s32)value; }

C166_TEST_NOINLINE
abi_u32 c166_f64_to_u32(double value) { return (abi_u32)value; }

C166_TEST_NOINLINE
abi_s16 c166_f64_to_i16(double value) { return (abi_s16)value; }

C166_TEST_NOINLINE
abi_u16 c166_f64_to_u16(double value) { return (abi_u16)value; }

C166_TEST_NOINLINE
double c166_i32_to_f64(abi_s32 value) { return (double)value; }

C166_TEST_NOINLINE
double c166_u32_to_f64(abi_u32 value) { return (double)value; }

C166_TEST_NOINLINE
double c166_i16_to_f64(abi_s16 value) { return (double)value; }

C166_TEST_NOINLINE
double c166_u16_to_f64(abi_u16 value) { return (double)value; }

C166_TEST_NOINLINE
abi_s32 c166_f64_compare(abi_u16 operation, double lhs, double rhs) {
  switch (operation) {
  case 0:
    return lhs == rhs;
  case 1:
    return lhs != rhs;
  case 2:
    return lhs < rhs;
  case 3:
    return lhs <= rhs;
  case 4:
    return lhs > rhs;
  case 5:
    return lhs >= rhs;
  case 6:
    return __builtin_isunordered(lhs, rhs);
  case 7:
    return __ledf2(lhs, rhs);
  case 8:
    return __gedf2(lhs, rhs);
  default:
    return __unorddf2(lhs, rhs);
  }
}

struct f64_integer_vector {
  abi_u16 words[4];
  abi_u32 expected;
};

struct s32_f64_vector {
  abi_s32 value;
  abi_u32 expected_hi;
  abi_u32 expected_lo;
};

struct u32_f64_vector {
  abi_u32 value;
  abi_u32 expected_hi;
  abi_u32 expected_lo;
};

struct s16_f64_vector {
  abi_s16 value;
  abi_u32 expected_hi;
  abi_u32 expected_lo;
};

struct u16_f64_vector {
  abi_u16 value;
  abi_u32 expected_hi;
  abi_u32 expected_lo;
};

struct f64_compare_vector {
  abi_u16 lhs[4];
  abi_u16 rhs[4];
  abi_s16 expected[ABI_F64_COMPARE_OPERATION_COUNT];
};

#define F64_INTEGER_ROW(id, w3, w2, w1, w0, expected)                          \
  {{w3, w2, w1, w0}, expected},
static const struct f64_integer_vector f64_to_i32_vectors[] = {
    ABI_F64_TO_I32_VECTORS(F64_INTEGER_ROW)};
static const struct f64_integer_vector f64_to_u32_vectors[] = {
    ABI_F64_TO_U32_VECTORS(F64_INTEGER_ROW)};
static const struct f64_integer_vector f64_to_i16_vectors[] = {
    ABI_F64_TO_I16_VECTORS(F64_INTEGER_ROW)};
static const struct f64_integer_vector f64_to_u16_vectors[] = {
    ABI_F64_TO_U16_VECTORS(F64_INTEGER_ROW)};
#undef F64_INTEGER_ROW

#define S32_F64_ROW(id, value, hi, lo) {value, hi, lo},
static const struct s32_f64_vector i32_to_f64_vectors[] = {
    ABI_I32_TO_F64_VECTORS(S32_F64_ROW)};
#undef S32_F64_ROW

#define U32_F64_ROW(id, value, hi, lo) {value, hi, lo},
static const struct u32_f64_vector u32_to_f64_vectors[] = {
    ABI_U32_TO_F64_VECTORS(U32_F64_ROW)};
#undef U32_F64_ROW

#define S16_F64_ROW(id, value, hi, lo) {value, hi, lo},
static const struct s16_f64_vector i16_to_f64_vectors[] = {
    ABI_I16_TO_F64_VECTORS(S16_F64_ROW)};
#undef S16_F64_ROW

#define U16_F64_ROW(id, value, hi, lo) {value, hi, lo},
static const struct u16_f64_vector u16_to_f64_vectors[] = {
    ABI_U16_TO_F64_VECTORS(U16_F64_ROW)};
#undef U16_F64_ROW

#define F64_COMPARE_ROW(id, lw3, lw2, lw1, lw0, rw3, rw2, rw1, rw0, eq, ne,    \
                        lt, le, gt, ge, unord, public_le, public_ge,           \
                        public_unord)                                          \
  {{lw3, lw2, lw1, lw0},                                                       \
   {rw3, rw2, rw1, rw0},                                                       \
   {eq, ne, lt, le, gt, ge, unord, public_le, public_ge, public_unord}},
static const struct f64_compare_vector compare_vectors[] = {
    ABI_F64_COMPARE_VECTORS(F64_COMPARE_ROW)};
#undef F64_COMPARE_ROW

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

static double words_to_f64(const abi_u16 *words) {
  return c166_f64_from_words(words[0], words[1], words[2], words[3]);
}

static void check_f64(double value, abi_u32 expected_hi, abi_u32 expected_lo) {
  abi_u16 words[4];
  c166_f64_to_words(value, words);
  tap_is_u32(((abi_u32)words[0] << 16) | words[1], expected_hi,
             "binary64 high words");
  tap_is_u32(((abi_u32)words[2] << 16) | words[3], expected_lo,
             "binary64 low words");
}

static void check_compare(abi_u16 operation, abi_s32 expected,
                          const struct f64_compare_vector *vector) {
  double lhs = words_to_f64(vector->lhs);
  double rhs = words_to_f64(vector->rhs);
  tap_is_u32((abi_u32)c166_f64_compare(operation, lhs, rhs), (abi_u32)expected,
             "binary64 comparison");
}

void main(void) {
  unsigned int index;
  unsigned int operation;

  tap_plan(ABI_F64_CONVERSION_CHECKS);
  for (index = 0U; index != ARRAY_COUNT(f64_to_i32_vectors); ++index)
    tap_is_u32(
        (abi_u32)c166_f64_to_i32(words_to_f64(f64_to_i32_vectors[index].words)),
        f64_to_i32_vectors[index].expected, "binary64 to signed 32");
  for (index = 0U; index != ARRAY_COUNT(f64_to_u32_vectors); ++index)
    tap_is_u32(c166_f64_to_u32(words_to_f64(f64_to_u32_vectors[index].words)),
               f64_to_u32_vectors[index].expected, "binary64 to unsigned 32");
  for (index = 0U; index != ARRAY_COUNT(f64_to_i16_vectors); ++index)
    tap_is_u32((abi_u32)(abi_s32)c166_f64_to_i16(
                   words_to_f64(f64_to_i16_vectors[index].words)),
               f64_to_i16_vectors[index].expected, "binary64 to signed 16");
  for (index = 0U; index != ARRAY_COUNT(f64_to_u16_vectors); ++index)
    tap_is_u32(
        (abi_u32)c166_f64_to_u16(words_to_f64(f64_to_u16_vectors[index].words)),
        f64_to_u16_vectors[index].expected, "binary64 to unsigned 16");
  for (index = 0U; index != ARRAY_COUNT(i32_to_f64_vectors); ++index)
    check_f64(c166_i32_to_f64(i32_to_f64_vectors[index].value),
              i32_to_f64_vectors[index].expected_hi,
              i32_to_f64_vectors[index].expected_lo);
  for (index = 0U; index != ARRAY_COUNT(u32_to_f64_vectors); ++index)
    check_f64(c166_u32_to_f64(u32_to_f64_vectors[index].value),
              u32_to_f64_vectors[index].expected_hi,
              u32_to_f64_vectors[index].expected_lo);
  for (index = 0U; index != ARRAY_COUNT(i16_to_f64_vectors); ++index)
    check_f64(c166_i16_to_f64(i16_to_f64_vectors[index].value),
              i16_to_f64_vectors[index].expected_hi,
              i16_to_f64_vectors[index].expected_lo);
  for (index = 0U; index != ARRAY_COUNT(u16_to_f64_vectors); ++index)
    check_f64(c166_u16_to_f64(u16_to_f64_vectors[index].value),
              u16_to_f64_vectors[index].expected_hi,
              u16_to_f64_vectors[index].expected_lo);
  for (index = 0U; index != ARRAY_COUNT(compare_vectors); ++index)
    for (operation = 0U; operation != ABI_F64_COMPARE_OPERATION_COUNT;
         ++operation)
      check_compare(operation, compare_vectors[index].expected[operation],
                    &compare_vectors[index]);
}
