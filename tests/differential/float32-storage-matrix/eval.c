#include "c166_test.h"
#include "types.h"

extern volatile float external_float_array[4];
extern volatile struct abi_float_record external_float_record_src;
extern volatile struct abi_float_record external_float_record_dst;

volatile float c166_float_array[4] = {1.0f, -2.5f, 0x1p-149f, -0.0f};

volatile struct abi_float_record c166_float_record = {0x1357U, 1.0f, 0x2468U,
                                                      -2.5f, 0x89abcdefUL};

C166_TEST_NOINLINE
float c166_float_array_load_external(unsigned int index) {
  return external_float_array[index];
}

C166_TEST_NOINLINE
void c166_float_array_store_external(unsigned int index, float value) {
  external_float_array[index] = value;
}

C166_TEST_NOINLINE
float c166_float_array_load_own(unsigned int index) {
  return c166_float_array[index];
}

C166_TEST_NOINLINE
void c166_float_array_store_own(unsigned int index, float value) {
  c166_float_array[index] = value;
}

static float select_record(volatile struct abi_float_record *record,
                           unsigned int which) {
  return which ? record->second : record->first;
}

static void store_record(volatile struct abi_float_record *record,
                         unsigned int which, float value) {
  if (which)
    record->second = value;
  else
    record->first = value;
}

C166_TEST_NOINLINE
float c166_float_record_load_external(unsigned int which) {
  return select_record(&external_float_record_src, which);
}

C166_TEST_NOINLINE
void c166_float_record_store_external(unsigned int which, float value) {
  store_record(&external_float_record_src, which, value);
}

C166_TEST_NOINLINE
float c166_float_record_load_own(unsigned int which) {
  return select_record(&c166_float_record, which);
}

C166_TEST_NOINLINE
void c166_float_record_store_own(unsigned int which, float value) {
  store_record(&c166_float_record, which, value);
}

C166_TEST_NOINLINE
float c166_float_record_copy_external(unsigned int which) {
  external_float_record_dst = external_float_record_src;
  if (external_float_record_dst.tag != 0x1357U ||
      external_float_record_dst.guard != 0x2468U ||
      external_float_record_dst.tail != 0x89abcdefUL)
    return 12345.0f;
  return select_record(&external_float_record_dst, which);
}

C166_TEST_NOINLINE
float c166_float_record_local(unsigned int which, float value) {
  volatile struct abi_float_record source;
  struct abi_float_record copy;
  union {
    float value;
    abi_u16 words[2];
  } word_view;

  word_view.value = 1.0f;
  if (word_view.words[0] != 0x3f80U || word_view.words[1] != 0x0000U)
    return 12345.0f;
  source.tag = 0x1357U;
  source.first = value;
  source.guard = 0x2468U;
  source.second = value;
  source.tail = 0x89abcdefUL;
  copy = source;
  if (copy.tag != 0x1357U || copy.guard != 0x2468U || copy.tail != 0x89abcdefUL)
    return 12345.0f;
  return which ? copy.second : copy.first;
}
