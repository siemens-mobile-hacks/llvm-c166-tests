#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

extern volatile double external_double_array[4];
extern volatile struct abi_double_record external_double_record_src;
extern volatile struct abi_double_record external_double_record_dst;
extern volatile double c166_double_array[4];
extern volatile struct abi_double_record c166_double_record;

extern double c166_float64_storage_eval(abi_u16, abi_u16, double, abi_u16);

struct storage_vector {
  abi_u16 words[4];
};

#define STORAGE_ROW(id, w3, w2, w1, w0) {{w3, w2, w1, w0}},
static const struct storage_vector storage_vectors[] = {
    ABI_FLOAT64_STORAGE_VECTORS(STORAGE_ROW)};
#undef STORAGE_ROW

static void check_words(const volatile double *value, const abi_u16 *expected) {
  abi_u16 actual[4];
  abi_u16 index;

  c166_f64_load_words(value, actual);
  for (index = 0U; index != 4U; ++index)
    tap_is_u32(actual[index], expected[index], "binary64 storage word");
}

static void call_and_check(abi_u16 operation, abi_u16 index,
                           const abi_u16 *words) {
  double value = c166_f64_from_words(words[0], words[1], words[2], words[3]);
  double result = c166_float64_storage_eval(operation, index, value, 0x5a5aU);
  abi_u16 actual[4];
  abi_u16 word;

  c166_f64_to_words(result, actual);
  for (word = 0U; word != 4U; ++word)
    tap_is_u32(actual[word], words[word], "binary64 result word");
}

static void check_initializers(void) {
  static const abi_u16 expected[6][4] = {
      {0x3ff0U, 0x0000U, 0x0000U, 0x0000U},
      {0xc004U, 0x0000U, 0x0000U, 0x0000U},
      {0x0000U, 0x0000U, 0x0000U, 0x0001U},
      {0x8000U, 0x0000U, 0x0000U, 0x0000U},
      {0x3ff0U, 0x0000U, 0x0000U, 0x0000U},
      {0xc004U, 0x0000U, 0x0000U, 0x0000U},
  };
  abi_u16 index;

  for (index = 0U; index != 4U; ++index) {
    call_and_check(2U, index, expected[index]);
    check_words(&c166_double_array[index], expected[index]);
  }
  call_and_check(6U, 0U, expected[4]);
  check_words(&c166_double_record.first, expected[4]);
  call_and_check(6U, 1U, expected[5]);
  check_words(&c166_double_record.second, expected[5]);
}

static void
check_record_metadata(const volatile struct abi_double_record *record) {
  tap_is_u32(record->tag, 0x1357U, "record tag");
  tap_is_u32(record->guard, 0x2468U, "record guard");
  tap_is_u32((abi_u16)record->tail, 0xcdefU, "record tail low word");
  tap_is_u32((abi_u16)(record->tail >> 16), 0x89abU, "record tail high word");
}

static void run_storage_vector(abi_u16 vector_id, const abi_u16 *words) {
  abi_u16 index = (abi_u16)(vector_id & 3U);
  abi_u16 which = (abi_u16)(vector_id & 1U);
  volatile double *field;

  c166_f64_store_words(&external_double_array[index], words);
  call_and_check(0U, index, words);
  call_and_check(1U, index, words);
  check_words(&external_double_array[index], words);

  c166_f64_store_words(&c166_double_array[index], words);
  call_and_check(2U, index, words);
  call_and_check(3U, index, words);
  check_words(&c166_double_array[index], words);

  field = which ? &external_double_record_src.second
                : &external_double_record_src.first;
  c166_f64_store_words(field, words);
  call_and_check(4U, which, words);
  call_and_check(5U, which, words);
  check_words(field, words);

  field = which ? &c166_double_record.second : &c166_double_record.first;
  c166_f64_store_words(field, words);
  call_and_check(6U, which, words);
  call_and_check(7U, which, words);
  check_words(field, words);

  external_double_record_src.tag = 0x1357U;
  c166_f64_store_words(&external_double_record_src.first, words);
  external_double_record_src.guard = 0x2468U;
  c166_f64_store_words(&external_double_record_src.second, words);
  external_double_record_src.tail = 0x89abcdefUL;
  call_and_check(8U, 0U, words);
  call_and_check(8U, 1U, words);
  check_words(&external_double_record_dst.first, words);
  check_words(&external_double_record_dst.second, words);
  check_record_metadata(&external_double_record_dst);

  call_and_check(9U, 0U, words);
  call_and_check(9U, 1U, words);
}

void main(void) {
  abi_u16 index;

  tap_plan(ABI_FLOAT64_STORAGE_CHECK_COUNT);
  check_initializers();
  for (index = 0U; index != ABI_FLOAT64_STORAGE_VECTOR_COUNT; ++index)
    run_storage_vector(index, storage_vectors[index].words);
}
