#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

extern volatile float external_float32_slot;
extern volatile float c166_float32_slot;

extern float c166_float32_eval(abi_u16, float, float, abi_u16);
extern float c166_float32_reverse(float);
extern float c166_float32_load_external(void);
extern void c166_float32_store_external(float);
extern float c166_float32_load_own(void);
extern void c166_float32_store_own(float);
extern float c166_float32_slot0(float, abi_u16);
extern float c166_float32_slot1(abi_u16, float, abi_u16);
extern float c166_float32_slot2(abi_u16, abi_u16, float, abi_u16);
extern float c166_float32_slot3(abi_u16, abi_u16, abi_u16, float, abi_u16);
extern float c166_float32_slot4(abi_u16, abi_u16, abi_u16, abi_u16, float,
                                abi_u16);
extern float c166_float32_call_slot3(abi_u16, abi_u16, abi_u16, float, abi_u16);
extern float c166_float32_icall_slot3(abi_float_slot3_fn, abi_u16, abi_u16,
                                      abi_u16, float, abi_u16);
extern float c166_float32_cross_tu(abi_u16, abi_u16, abi_u16, float, abi_u16);
extern float c166_float32_nested_slot3(abi_u16, abi_u16, abi_u16, float,
                                       abi_u16);

struct arithmetic_vector {
  abi_u16 operation;
  abi_u32 lhs;
  abi_u32 rhs;
  abi_u32 expected;
  abi_u16 any_nan_allowed;
};

#define ARITHMETIC_ROW(id, operation, name, lhs, rhs, expected, nan_ok)        \
  {operation, lhs, rhs, expected, nan_ok},
static const struct arithmetic_vector arithmetic_vectors[] = {
    ABI_FLOAT32_VECTORS(ARITHMETIC_ROW)};
#undef ARITHMETIC_ROW

#define IDENTITY_ROW(id, bits) bits,
static const abi_u32 identity_vectors[] = {
    ABI_FLOAT32_IDENTITY_VECTORS(IDENTITY_ROW)};
#undef IDENTITY_ROW

#define CALL_ROW(id, bits) bits,
static const abi_u32 call_vectors[] = {ABI_FLOAT32_CALL_VECTORS(CALL_ROW)};
#undef CALL_ROW

static abi_u16 is_nan_bits(abi_u32 bits) {
  return (abi_u16)((bits & 0x7f800000UL) == 0x7f800000UL &&
                   (bits & 0x007fffffUL) != 0UL);
}

static void check_float(float value, abi_u32 expected, abi_u16 any_nan_allowed,
                        const char *name) {
  abi_u32 actual = c166_f32_to_bits(value);

  if (any_nan_allowed != 0U && is_nan_bits(actual))
    actual = expected;
  tap_is_u32(actual, expected, name);
}

static void run_arithmetic_vector(const struct arithmetic_vector *vector) {
  float lhs = c166_f32_from_bits(vector->lhs);
  float rhs = c166_f32_from_bits(vector->rhs);
  float result = c166_float32_eval(vector->operation, lhs, rhs, 0x5a5aU);

  check_float(result, vector->expected, vector->any_nan_allowed,
              "binary32 arithmetic");
}

static void run_storage_vector(abi_u32 bits) {
  float value = c166_f32_from_bits(bits);

  c166_f32_store_bits(&external_float32_slot, bits);
  check_float(c166_float32_load_external(), bits, 0U, "external float load");
  c166_float32_store_external(value);
  tap_is_u32(c166_f32_load_bits(&external_float32_slot), bits,
             "external float store");

  c166_f32_store_bits(&c166_float32_slot, bits);
  check_float(c166_float32_load_own(), bits, 0U, "own float load");
  c166_float32_store_own(value);
  tap_is_u32(c166_f32_load_bits(&c166_float32_slot), bits, "own float store");
}

static void run_call_vector(abi_u32 bits) {
  float value = c166_f32_from_bits(bits);
  abi_float_slot3_fn indirect = c166_float32_slot3;

  check_float(c166_float32_slot0(value, 0x5a5aU), bits, 0U, "float slot 0");
  check_float(c166_float32_slot1(0x1111U, value, 0x5a5aU), bits, 0U,
              "float slot 1");
  check_float(c166_float32_slot2(0x1111U, 0x2222U, value, 0x5a5aU), bits, 0U,
              "float slot 2");
  check_float(c166_float32_slot3(0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU),
              bits, 0U, "float slot 3");
  check_float(
      c166_float32_slot4(0x1111U, 0x2222U, 0x3333U, 0x4444U, value, 0x5a5aU),
      bits, 0U, "float slot 4");
  check_float(indirect(0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU), bits, 0U,
              "indirect float call");
  check_float(
      c166_float32_call_slot3(0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU), bits,
      0U, "direct cross-tu float call");
  check_float(c166_float32_icall_slot3(indirect, 0x1111U, 0x2222U, 0x3333U,
                                       value, 0x5a5aU),
              bits, 0U, "wrapped indirect float call");
  check_float(c166_float32_cross_tu(0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU),
              bits, 0U, "cross-tu float return");
  check_float(
      c166_float32_nested_slot3(0x1111U, 0x2222U, 0x3333U, value, 0x5a5aU),
      bits, 0U, "nested float call");
}

void main(void) {
  abi_u16 index;

  tap_plan(ABI_FLOAT32_VECTOR_COUNT + 1U + ABI_FLOAT32_IDENTITY_COUNT * 5U +
           ABI_FLOAT32_CALL_COUNT * 10U);
  for (index = 0U; index != ABI_FLOAT32_VECTOR_COUNT; ++index)
    run_arithmetic_vector(&arithmetic_vectors[index]);

  check_float(c166_float32_load_own(), 0x3f800000UL, 0U,
              "initialized float global");
  for (index = 0U; index != ABI_FLOAT32_IDENTITY_COUNT; ++index) {
    abi_u32 bits = identity_vectors[index];
    check_float(c166_float32_reverse(c166_f32_from_bits(bits)), bits, 0U,
                "float roundtrip");
    run_storage_vector(bits);
  }
  for (index = 0U; index != ABI_FLOAT32_CALL_COUNT; ++index)
    run_call_vector(call_vectors[index]);
}
