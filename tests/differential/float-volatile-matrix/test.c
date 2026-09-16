#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

extern volatile float volatile_f32_a[4];
extern volatile float volatile_f32_b[4];
extern volatile double volatile_f64_a[4];
extern volatile double volatile_f64_b[4];

extern float c166_volatile_f32_eval(abi_u16, abi_u16);
extern double c166_volatile_f64_eval(abi_u16, abi_u16);

struct f64_words {
  abi_u16 words[4];
};

struct f32_compound_vector {
  abi_u32 a;
  abi_u32 b;
  abi_u32 sum;
  abi_u32 product;
};

struct f64_compound_vector {
  struct f64_words a;
  struct f64_words b;
  struct f64_words sum;
  struct f64_words product;
};

#define F32_RAW_ROW(id, bits) bits,
static const abi_u32 f32_raw_vectors[] = {
    ABI_VOLATILE_F32_RAW_VECTORS(F32_RAW_ROW)};
#undef F32_RAW_ROW

#define F64_RAW_ROW(id, w3, w2, w1, w0) {{w3, w2, w1, w0}},
static const struct f64_words f64_raw_vectors[] = {
    ABI_VOLATILE_F64_RAW_VECTORS(F64_RAW_ROW)};
#undef F64_RAW_ROW

#define F32_COMPOUND_ROW(id, a, b, sum, product) {a, b, sum, product},
static const struct f32_compound_vector f32_compound_vectors[] = {
    ABI_VOLATILE_F32_COMPOUND_VECTORS(F32_COMPOUND_ROW)};
#undef F32_COMPOUND_ROW

#define F64_COMPOUND_ROW(id, a3, a2, a1, a0, b3, b2, b1, b0, s3, s2, s1, s0,   \
                         p3, p2, p1, p0)                                       \
  {{{a3, a2, a1, a0}},                                                         \
   {{b3, b2, b1, b0}},                                                         \
   {{s3, s2, s1, s0}},                                                         \
   {{p3, p2, p1, p0}}},
static const struct f64_compound_vector f64_compound_vectors[] = {
    ABI_VOLATILE_F64_COMPOUND_VECTORS(F64_COMPOUND_ROW)};
#undef F64_COMPOUND_ROW

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

static void check_f32(float value, abi_u32 expected, const char *name) {
  tap_is_u32(c166_f32_to_bits(value), expected, name);
}

static void check_f64(double value, const struct f64_words *expected,
                      const char *name) {
  abi_u16 actual[4];

  c166_f64_to_words(value, actual);
  tap_is_u32(((abi_u32)actual[0] << 16) | actual[1],
             ((abi_u32)expected->words[0] << 16) | expected->words[1], name);
  tap_is_u32(((abi_u32)actual[2] << 16) | actual[3],
             ((abi_u32)expected->words[2] << 16) | expected->words[3], name);
}

static void store_f64(volatile double *destination,
                      const struct f64_words *value) {
  c166_f64_store_words(destination, value->words);
}

static void check_stored_f64(const volatile double *value,
                             const struct f64_words *expected,
                             const char *name) {
  abi_u16 actual[4];

  c166_f64_load_words(value, actual);
  tap_is_u32(((abi_u32)actual[0] << 16) | actual[1],
             ((abi_u32)expected->words[0] << 16) | expected->words[1], name);
  tap_is_u32(((abi_u32)actual[2] << 16) | actual[3],
             ((abi_u32)expected->words[2] << 16) | expected->words[3], name);
}

static void run_f32_raw(abi_u16 vector_id, abi_u32 bits) {
  abi_u16 index = (abi_u16)(vector_id & 3U);

  c166_f32_store_bits(&volatile_f32_a[index], bits);
  c166_f32_store_bits(&volatile_f32_b[index], 0x5a5aa5a5UL);
  check_f32(c166_volatile_f32_eval(0U, index), bits, "volatile float load");
  check_f32(c166_volatile_f32_eval(1U, index), bits, "volatile float copy");
  tap_is_u32(c166_f32_load_bits(&volatile_f32_b[index]), bits,
             "volatile float store");
}

static void run_f64_raw(abi_u16 vector_id, const struct f64_words *words) {
  static const struct f64_words fill = {{0x5a5aU, 0xa5a5U, 0x1357U, 0x2468U}};
  abi_u16 index = (abi_u16)(vector_id & 3U);

  store_f64(&volatile_f64_a[index], words);
  store_f64(&volatile_f64_b[index], &fill);
  check_f64(c166_volatile_f64_eval(0U, index), words, "volatile double load");
  check_f64(c166_volatile_f64_eval(1U, index), words, "volatile double copy");
  check_stored_f64(&volatile_f64_b[index], words, "volatile double store");
}

static void reset_f32(abi_u16 index, const struct f32_compound_vector *vector) {
  c166_f32_store_bits(&volatile_f32_a[index], vector->a);
  c166_f32_store_bits(&volatile_f32_b[index], vector->b);
}

static void run_f32_compound(abi_u16 vector_id,
                             const struct f32_compound_vector *vector) {
  abi_u16 index = (abi_u16)(vector_id & 3U);

  reset_f32(index, vector);
  check_f32(c166_volatile_f32_eval(2U, index), vector->sum,
            "volatile float compound add");
  tap_is_u32(c166_f32_load_bits(&volatile_f32_b[index]), vector->sum,
             "volatile float compound add store");

  reset_f32(index, vector);
  check_f32(c166_volatile_f32_eval(3U, index), vector->product,
            "volatile float compound multiply");
  tap_is_u32(c166_f32_load_bits(&volatile_f32_b[index]), vector->product,
             "volatile float compound multiply store");

  reset_f32(index, vector);
  check_f32(c166_volatile_f32_eval(4U, index), vector->sum,
            "volatile float local add");
  tap_is_u32(c166_f32_load_bits(&volatile_f32_a[index]), vector->a,
             "volatile float add source a");
  tap_is_u32(c166_f32_load_bits(&volatile_f32_b[index]), vector->b,
             "volatile float add source b");

  reset_f32(index, vector);
  check_f32(c166_volatile_f32_eval(5U, index), vector->product,
            "volatile float local multiply");
  tap_is_u32(c166_f32_load_bits(&volatile_f32_a[index]), vector->a,
             "volatile float multiply source a");
  tap_is_u32(c166_f32_load_bits(&volatile_f32_b[index]), vector->b,
             "volatile float multiply source b");
}

static void reset_f64(abi_u16 index, const struct f64_compound_vector *vector) {
  store_f64(&volatile_f64_a[index], &vector->a);
  store_f64(&volatile_f64_b[index], &vector->b);
}

static void run_f64_compound(abi_u16 vector_id,
                             const struct f64_compound_vector *vector) {
  abi_u16 index = (abi_u16)(vector_id & 3U);

  reset_f64(index, vector);
  check_f64(c166_volatile_f64_eval(2U, index), &vector->sum,
            "volatile double compound add");
  check_stored_f64(&volatile_f64_b[index], &vector->sum,
                   "volatile double compound add store");

  reset_f64(index, vector);
  check_f64(c166_volatile_f64_eval(3U, index), &vector->product,
            "volatile double compound multiply");
  check_stored_f64(&volatile_f64_b[index], &vector->product,
                   "volatile double compound multiply store");

  reset_f64(index, vector);
  check_f64(c166_volatile_f64_eval(4U, index), &vector->sum,
            "volatile double local add");
  check_stored_f64(&volatile_f64_a[index], &vector->a,
                   "volatile double add source a");
  check_stored_f64(&volatile_f64_b[index], &vector->b,
                   "volatile double add source b");

  reset_f64(index, vector);
  check_f64(c166_volatile_f64_eval(5U, index), &vector->product,
            "volatile double local multiply");
  check_stored_f64(&volatile_f64_a[index], &vector->a,
                   "volatile double multiply source a");
  check_stored_f64(&volatile_f64_b[index], &vector->b,
                   "volatile double multiply source b");
}

void main(void) {
  abi_u16 index;

  tap_plan((abi_u16)(ARRAY_COUNT(f32_raw_vectors) * 3U +
                     ARRAY_COUNT(f64_raw_vectors) * 6U +
                     ARRAY_COUNT(f32_compound_vectors) * 10U +
                     ARRAY_COUNT(f64_compound_vectors) * 20U + 2U));
  for (index = 0U; index != ARRAY_COUNT(f32_raw_vectors); ++index)
    run_f32_raw(index, f32_raw_vectors[index]);
  for (index = 0U; index != ARRAY_COUNT(f64_raw_vectors); ++index)
    run_f64_raw(index, &f64_raw_vectors[index]);
  for (index = 0U; index != ARRAY_COUNT(f32_compound_vectors); ++index)
    run_f32_compound(index, &f32_compound_vectors[index]);
  for (index = 0U; index != ARRAY_COUNT(f64_compound_vectors); ++index)
    run_f64_compound(index, &f64_compound_vectors[index]);
  check_f32(c166_volatile_f32_eval(6U, 0U), 0x00000000UL,
            "32-bit atomics are not lock free");
  check_f32(c166_volatile_f32_eval(6U, 1U), 0x00000000UL,
            "64-bit atomics are not lock free");
}
