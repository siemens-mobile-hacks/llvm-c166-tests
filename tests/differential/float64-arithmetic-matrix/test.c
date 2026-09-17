#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

#if defined(C166_TEST_LLVM)
extern double __adddf3(double, double);
extern double __subdf3(double, double);
extern double __muldf3(double, double);
extern double __divdf3(double, double);
#define EVALUATION_PATH_COUNT 2U
#else
#define EVALUATION_PATH_COUNT 1U
#endif

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
#if defined(C166_TEST_LLVM)
  case 4:
    return __adddf3(lhs, rhs);
  case 5:
    return __subdf3(lhs, rhs);
  case 6:
    return __muldf3(lhs, rhs);
  default:
    return __divdf3(lhs, rhs);
#else
  default:
    return 0.0;
#endif
  }
}

struct arithmetic_vector {
  abi_u16 lhs[4];
  abi_u16 rhs[4];
  abi_u16 expected[4];
  abi_u16 any_quiet_nan_allowed;
};

#if defined(C166_TEST_LLVM) && __C166_MEMORY_MODEL__ != 3 &&               \
    __C166_MEMORY_MODEL__ != 4
#define TABLE_PAGE_ALIGNED __attribute__((aligned(0x4000)))
#else
#define TABLE_PAGE_ALIGNED
#endif

#define ARITHMETIC_ROW(id, l0, l1, l2, l3, r0, r1, r2, r3, e0, e1, e2, e3,  \
                       nan_ok)                                                 \
  {{l0, l1, l2, l3}, {r0, r1, r2, r3}, {e0, e1, e2, e3}, nan_ok},
#if C166_IEEE_OPERATION == 0
static const struct arithmetic_vector
    arithmetic_vectors[] TABLE_PAGE_ALIGNED = {
    ABI_FLOAT64_ADD_VECTORS(ARITHMETIC_ROW)};
#define ARITHMETIC_VECTOR_COUNT ABI_FLOAT64_ADD_COUNT
#elif C166_IEEE_OPERATION == 1
static const struct arithmetic_vector
    arithmetic_vectors[] TABLE_PAGE_ALIGNED = {
    ABI_FLOAT64_SUB_VECTORS(ARITHMETIC_ROW)};
#define ARITHMETIC_VECTOR_COUNT ABI_FLOAT64_SUB_COUNT
#elif C166_IEEE_OPERATION == 2
static const struct arithmetic_vector
    arithmetic_vectors[] TABLE_PAGE_ALIGNED = {
    ABI_FLOAT64_MUL_VECTORS(ARITHMETIC_ROW)};
#define ARITHMETIC_VECTOR_COUNT ABI_FLOAT64_MUL_COUNT
#elif C166_IEEE_OPERATION == 3
static const struct arithmetic_vector
    arithmetic_vectors[] TABLE_PAGE_ALIGNED = {
    ABI_FLOAT64_DIV_VECTORS(ARITHMETIC_ROW)};
#define ARITHMETIC_VECTOR_COUNT ABI_FLOAT64_DIV_COUNT
#else
#error C166_IEEE_OPERATION must select add, subtract, multiply, or divide
#endif
#undef ARITHMETIC_ROW
#undef TABLE_PAGE_ALIGNED

static abi_u16 words_are_quiet_nan(const abi_u16 *words) {
  return (abi_u16)((words[0] & 0x7ff0U) == 0x7ff0U &&
                   (words[0] & 0x0008U) != 0U);
}

static void run_vector(abi_u16 path, abi_u16 operation,
                       const struct arithmetic_vector *vector) {
  abi_u16 actual[4];
  double lhs = c166_f64_from_words(vector->lhs[0], vector->lhs[1],
                                    vector->lhs[2], vector->lhs[3]);
  double rhs = c166_f64_from_words(vector->rhs[0], vector->rhs[1],
                                    vector->rhs[2], vector->rhs[3]);
  double result =
      c166_float64_eval((abi_u16)(operation + path * 4U), lhs, rhs, 0x5a5aU);

  c166_f64_to_words(result, actual);
  if (vector->any_quiet_nan_allowed != 0U && words_are_quiet_nan(actual)) {
    actual[0] = vector->expected[0];
    actual[1] = vector->expected[1];
    actual[2] = vector->expected[2];
    actual[3] = vector->expected[3];
  }
  tap_is_u32(((abi_u32)actual[0] << 16) | actual[1],
             ((abi_u32)vector->expected[0] << 16) | vector->expected[1],
             "binary64 high words");
  tap_is_u32(((abi_u32)actual[2] << 16) | actual[3],
             ((abi_u32)vector->expected[2] << 16) | vector->expected[3],
             "binary64 low words");
}

static void run_vectors(abi_u16 path, abi_u16 operation,
                        const struct arithmetic_vector *vectors,
                        abi_u16 count) {
  abi_u16 index;
  for (index = 0U; index != count; ++index)
    run_vector(path, operation, &vectors[index]);
}

void main(void) {
  abi_u16 path;

  tap_plan(ARITHMETIC_VECTOR_COUNT * EVALUATION_PATH_COUNT * 2U);
  for (path = 0U; path != EVALUATION_PATH_COUNT; ++path) {
    run_vectors(path, C166_IEEE_OPERATION, arithmetic_vectors,
                ARITHMETIC_VECTOR_COUNT);
  }
}
