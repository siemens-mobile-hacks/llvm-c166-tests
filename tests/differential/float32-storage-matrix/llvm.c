#include "types.h"

extern volatile float tasking_float_array[4];
extern volatile struct abi_float_record tasking_float_record_src;
extern volatile struct abi_float_record tasking_float_record_dst;

__attribute__((section(".llvm_float_storage_data")))
volatile float llvm_float_array[4] = {1.0f, -2.5f, 0x1p-149f, -0.0f};

__attribute__((section(".llvm_float_storage_data")))
volatile struct abi_float_record llvm_float_record = {
    0x1357U, 1.0f, 0x2468U, -2.5f, 0x89abcdefUL};

#define LLVM_FN(section_name) __attribute__((noinline, section(section_name)))

typedef union {
  float value;
  abi_u16 words[2];
} abi_float_word_view;

LLVM_FN(".llvm_float_array_load_external")
float llvm_float_array_load_external(abi_u16 index) {
  return tasking_float_array[index];
}

LLVM_FN(".llvm_float_array_store_external")
void llvm_float_array_store_external(abi_u16 index, float value) {
  tasking_float_array[index] = value;
}

LLVM_FN(".llvm_float_array_load_own")
float llvm_float_array_load_own(abi_u16 index) {
  return llvm_float_array[index];
}

LLVM_FN(".llvm_float_array_store_own")
void llvm_float_array_store_own(abi_u16 index, float value) {
  llvm_float_array[index] = value;
}

static float select_record(volatile struct abi_float_record *record,
                           abi_u16 which) {
  return which ? record->second : record->first;
}

static void store_record(volatile struct abi_float_record *record,
                         abi_u16 which, float value) {
  if (which)
    record->second = value;
  else
    record->first = value;
}

LLVM_FN(".llvm_float_record_load_external")
float llvm_float_record_load_external(abi_u16 which) {
  return select_record(&tasking_float_record_src, which);
}

LLVM_FN(".llvm_float_record_store_external")
void llvm_float_record_store_external(abi_u16 which, float value) {
  store_record(&tasking_float_record_src, which, value);
}

LLVM_FN(".llvm_float_record_load_own")
float llvm_float_record_load_own(abi_u16 which) {
  return select_record(&llvm_float_record, which);
}

LLVM_FN(".llvm_float_record_store_own")
void llvm_float_record_store_own(abi_u16 which, float value) {
  store_record(&llvm_float_record, which, value);
}

LLVM_FN(".llvm_float_record_copy_external")
float llvm_float_record_copy_external(abi_u16 which) {
  tasking_float_record_dst = tasking_float_record_src;
  if (tasking_float_record_dst.tag != 0x1357U ||
      tasking_float_record_dst.guard != 0x2468U ||
      tasking_float_record_dst.tail != 0x89abcdefUL)
    return 12345.0f;
  return select_record(&tasking_float_record_dst, which);
}

LLVM_FN(".llvm_float_record_local")
float llvm_float_record_local(abi_u16 which, float value) {
  volatile struct abi_float_record source;
  struct abi_float_record copy;
  abi_float_word_view word_view;
  word_view.value = 1.0f;
  if (word_view.words[0] != 0x3f80U || word_view.words[1] != 0x0000U)
    return 12345.0f;
  source.tag = 0x1357U;
  source.first = value;
  source.guard = 0x2468U;
  source.second = value;
  source.tail = 0x89abcdefUL;
  copy = source;
  if (copy.tag != 0x1357U || copy.guard != 0x2468U ||
      copy.tail != 0x89abcdefUL)
    return 12345.0f;
  return which ? copy.second : copy.first;
}
