#include "c166_test.h"
#include "types.h"

extern volatile double external_double_array[4];
extern volatile struct abi_double_record external_double_record_src;
extern volatile struct abi_double_record external_double_record_dst;

typedef union {
  double value;
  abi_u16 words[4];
} abi_double_word_view;

volatile double c166_double_array[4] = {1.0, -2.5, 0x1p-1074, -0.0};

volatile struct abi_double_record c166_double_record = {0x1357U, 1.0, 0x2468U,
                                                        -2.5, 0x89abcdefUL};

static double select_record(volatile struct abi_double_record *record,
                            abi_u16 which) {
  return which ? record->second : record->first;
}

static void store_record(volatile struct abi_double_record *record,
                         abi_u16 which, double value) {
  if (which)
    record->second = value;
  else
    record->first = value;
}

static double copy_external_record(abi_u16 which) {
  external_double_record_dst = external_double_record_src;
  if (external_double_record_dst.tag != 0x1357U ||
      external_double_record_dst.guard != 0x2468U ||
      external_double_record_dst.tail != 0x89abcdefUL)
    return 12345.0;
  return select_record(&external_double_record_dst, which);
}

static double copy_local_record(abi_u16 which, double value) {
  volatile struct abi_double_record source;
  struct abi_double_record copy;
  abi_double_word_view word_view;

  word_view.value = 1.0;
  if (word_view.words[0] != 0x3ff0U || word_view.words[1] != 0x0000U ||
      word_view.words[2] != 0x0000U || word_view.words[3] != 0x0000U)
    return 12345.0;
  source.tag = 0x1357U;
  source.first = value;
  source.guard = 0x2468U;
  source.second = value;
  source.tail = 0x89abcdefUL;
  copy = source;
  if (copy.tag != 0x1357U || copy.guard != 0x2468U || copy.tail != 0x89abcdefUL)
    return 12345.0;
  return which ? copy.second : copy.first;
}

C166_TEST_NOINLINE
double c166_float64_storage_eval(abi_u16 operation, abi_u16 index, double value,
                                 abi_u16 tail) {
  if (tail != 0x5a5aU)
    return 0.0;

  switch (operation) {
  case 0:
    return external_double_array[index];
  case 1:
    external_double_array[index] = value;
    return external_double_array[index];
  case 2:
    return c166_double_array[index];
  case 3:
    c166_double_array[index] = value;
    return c166_double_array[index];
  case 4:
    return select_record(&external_double_record_src, index);
  case 5:
    store_record(&external_double_record_src, index, value);
    return select_record(&external_double_record_src, index);
  case 6:
    return select_record(&c166_double_record, index);
  case 7:
    store_record(&c166_double_record, index, value);
    return select_record(&c166_double_record, index);
  case 8:
    return copy_external_record(index);
  default:
    return copy_local_record(index, value);
  }
}
