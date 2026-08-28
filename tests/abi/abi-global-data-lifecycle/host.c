#include "c166-test-runtime.h"
#include "c166-test-result.h"

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

enum global_operation {
  GLOBAL_DATA_BYTES,
  GLOBAL_DATA_WORDS,
  GLOBAL_DATA_LONGS,
  GLOBAL_BSS_BYTES,
  GLOBAL_BSS_WORDS,
  GLOBAL_BSS_LONGS,
  GLOBAL_RO_BYTES,
  GLOBAL_RO_WORDS,
  GLOBAL_RO_LONGS,
  GLOBAL_STRING,
  GLOBAL_DATA_POINTER,
  GLOBAL_MUTATE,
  GLOBAL_DIGEST
};

struct mutation_vector {
  abi_u16 seed;
  abi_u16 byte_index;
  abi_u16 word_index;
  abi_u16 long_index;
  abi_u8 data_byte;
  abi_u16 data_word;
  abi_u32 data_long;
  abi_u8 bss_byte;
  abi_u16 bss_word;
  abi_u32 bss_long;
};

extern const void *llvm_entry_proxy(abi_u16 operation, abi_u16 index,
                                     abi_u32 seed);

static const abi_u8 initial_data_bytes[8] = {
    0x01U, 0x23U, 0x45U, 0x67U, 0x89U, 0xabU, 0xcdU, 0xefU,
};
static const abi_u16 initial_data_words[8] = {
    0x0000U, 0x0001U, 0x7fffU, 0x8000U,
    0xa55aU, 0x5aa5U, 0xfffeU, 0xffffU,
};
static const abi_u32 initial_data_longs[8] = {
    0x00000000UL, 0x00000001UL, 0x0000ffffUL, 0x00010000UL,
    0x7fffffffUL, 0x80000000UL, 0xa55a5aa5UL, 0xffffffffUL,
};
static const abi_u8 expected_ro_bytes[8] = {
    0xf0U, 0xdeU, 0xbcU, 0x9aU, 0x78U, 0x56U, 0x34U, 0x12U,
};
static const abi_u16 expected_ro_words[8] = {
    0x1357U, 0x2468U, 0x369cU, 0x48adU,
    0x5aceU, 0x6bdfU, 0x7ef0U, 0x8f01U,
};
static const abi_u32 expected_ro_longs[8] = {
    0x01234567UL, 0x89abcdefUL, 0x13579bdfUL, 0x2468ace0UL,
    0x0f1e2d3cUL, 0x4b5a6978UL, 0xdeadbeefUL, 0xc001d00dUL,
};
static const abi_u8 expected_string[17] = "C166 Classic ELF";

static const struct mutation_vector mutations[8] = {
    {0x0000U, 0, 1, 2, 0x5aU, 0x1357U, 0x0000a55aUL,
     0xa5U, 0xeca8U, 0xffff5aa5UL},
    {0x0001U, 1, 4, 7, 0x5bU, 0x1358U, 0x0001a55bUL,
     0xa4U, 0xeca7U, 0xfffe5aa4UL},
    {0x1234U, 2, 7, 4, 0x6eU, 0x258bU, 0x1234b76eUL,
     0x91U, 0xda74U, 0xedcb4891UL},
    {0x7fffU, 3, 2, 1, 0xa5U, 0x9356U, 0x7fffdaa5UL,
     0x5aU, 0x6ca9U, 0x8000255aUL},
    {0x8000U, 4, 5, 6, 0x5aU, 0x9357U, 0x8000255aUL,
     0xa5U, 0x6ca8U, 0x7fffdaa5UL},
    {0xa55aU, 5, 0, 3, 0x00U, 0xb8b1U, 0xa55a0000UL,
     0xffU, 0x474eU, 0x5aa5ffffUL},
    {0xfffeU, 6, 3, 0, 0xa4U, 0x1355U, 0xfffe5aa4UL,
     0x5bU, 0xecaaU, 0x0001a55bUL},
    {0xffffU, 7, 6, 5, 0xa5U, 0x1356U, 0xffff5aa5UL,
     0x5aU, 0xeca9U, 0x0000a55aUL},
};

static const abi_u8 final_data_bytes[8] = {
    0x5aU, 0x5bU, 0x6eU, 0xa5U, 0x5aU, 0x00U, 0xa4U, 0xa5U,
};
static const abi_u16 final_data_words[8] = {
    0xb8b1U, 0x1357U, 0x9356U, 0x1355U,
    0x1358U, 0x9357U, 0x1356U, 0x258bU,
};
static const abi_u32 final_data_longs[8] = {
    0xfffe5aa4UL, 0x7fffdaa5UL, 0x0000a55aUL, 0xa55a0000UL,
    0x1234b76eUL, 0xffff5aa5UL, 0x8000255aUL, 0x0001a55bUL,
};
static const abi_u8 final_bss_bytes[8] = {
    0xa5U, 0xa4U, 0x91U, 0x5aU, 0xa5U, 0xffU, 0x5bU, 0x5aU,
};
static const abi_u16 final_bss_words[8] = {
    0x474eU, 0xeca8U, 0x6ca9U, 0xecaaU,
    0xeca7U, 0x6ca8U, 0xeca9U, 0xda74U,
};
static const abi_u32 final_bss_longs[8] = {
    0x0001a55bUL, 0x8000255aUL, 0xffff5aa5UL, 0x5aa5ffffUL,
    0xedcb4891UL, 0x0000a55aUL, 0x7fffdaa5UL, 0xfffe5aa4UL,
};

#define CHECK_VALUE(expected, actual)                                           \
  do {                                                                          \
    c166_test_check_u32(case_id++, (abi_u32)(expected), (abi_u32)(actual));     \
  } while (0)

void main(void) {
  abi_u16 case_id = 1;
  abi_u16 index;
  abi_u16 vector;
  const void *raw;
  const abi_u8 *data_bytes;
  const abi_u16 *data_words;
  const abi_u32 *data_longs;
  const abi_u8 *bss_bytes;
  const abi_u16 *bss_words;
  const abi_u32 *bss_longs;
  const abi_u8 *ro_bytes;
  const abi_u16 *ro_words;
  const abi_u32 *ro_longs;
  const abi_u8 *string;
  const abi_u32 *digest;

  c166_test_begin(319, 0x1660013fUL);

  data_bytes = (const abi_u8 *)llvm_entry_proxy(GLOBAL_DATA_BYTES, 0, 0);
  CHECK_VALUE(1, data_bytes != (const abi_u8 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(initial_data_bytes[index], data_bytes[index]);

  data_words = (const abi_u16 *)llvm_entry_proxy(GLOBAL_DATA_WORDS, 0, 0);
  CHECK_VALUE(1, data_words != (const abi_u16 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(initial_data_words[index], data_words[index]);

  data_longs = (const abi_u32 *)llvm_entry_proxy(GLOBAL_DATA_LONGS, 0, 0);
  CHECK_VALUE(1, data_longs != (const abi_u32 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(initial_data_longs[index], data_longs[index]);

  bss_bytes = (const abi_u8 *)llvm_entry_proxy(GLOBAL_BSS_BYTES, 0, 0);
  CHECK_VALUE(1, bss_bytes != (const abi_u8 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(0, bss_bytes[index]);

  bss_words = (const abi_u16 *)llvm_entry_proxy(GLOBAL_BSS_WORDS, 0, 0);
  CHECK_VALUE(1, bss_words != (const abi_u16 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(0, bss_words[index]);

  bss_longs = (const abi_u32 *)llvm_entry_proxy(GLOBAL_BSS_LONGS, 0, 0);
  CHECK_VALUE(1, bss_longs != (const abi_u32 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(0, bss_longs[index]);

  ro_bytes = (const abi_u8 *)llvm_entry_proxy(GLOBAL_RO_BYTES, 0, 0);
  CHECK_VALUE(1, ro_bytes != (const abi_u8 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(expected_ro_bytes[index], ro_bytes[index]);

  ro_words = (const abi_u16 *)llvm_entry_proxy(GLOBAL_RO_WORDS, 0, 0);
  CHECK_VALUE(1, ro_words != (const abi_u16 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(expected_ro_words[index], ro_words[index]);

  ro_longs = (const abi_u32 *)llvm_entry_proxy(GLOBAL_RO_LONGS, 0, 0);
  CHECK_VALUE(1, ro_longs != (const abi_u32 *)0);
  for (index = 0; index != 8; ++index)
    CHECK_VALUE(expected_ro_longs[index], ro_longs[index]);

  string = (const abi_u8 *)llvm_entry_proxy(GLOBAL_STRING, 0, 0);
  CHECK_VALUE(1, string != (const abi_u8 *)0);
  for (index = 0; index != 17; ++index)
    CHECK_VALUE(expected_string[index], string[index]);

  raw = llvm_entry_proxy(GLOBAL_DATA_POINTER, 0, 0);
  CHECK_VALUE(1, raw == (const void *)data_bytes);

  for (vector = 0; vector != 8; ++vector) {
    const struct mutation_vector *mutation = &mutations[vector];
    raw = llvm_entry_proxy(GLOBAL_MUTATE, vector, mutation->seed);
    CHECK_VALUE(1, raw == (const void *)&data_bytes[mutation->byte_index]);
    CHECK_VALUE(mutation->data_byte, data_bytes[mutation->byte_index]);
    CHECK_VALUE(mutation->data_word, data_words[mutation->word_index]);
    CHECK_VALUE(mutation->data_long, data_longs[mutation->long_index]);
    CHECK_VALUE(mutation->bss_byte, bss_bytes[mutation->byte_index]);
    CHECK_VALUE(mutation->bss_word, bss_words[mutation->word_index]);
    CHECK_VALUE(mutation->bss_long, bss_longs[mutation->long_index]);
  }

  for (index = 0; index != 8; ++index) {
    CHECK_VALUE(final_data_bytes[index], data_bytes[index]);
    CHECK_VALUE(final_data_words[index], data_words[index]);
    CHECK_VALUE(final_data_longs[index], data_longs[index]);
    CHECK_VALUE(final_bss_bytes[index], bss_bytes[index]);
    CHECK_VALUE(final_bss_words[index], bss_words[index]);
    CHECK_VALUE(final_bss_longs[index], bss_longs[index]);
  }
  for (index = 0; index != 8; ++index) {
    CHECK_VALUE(expected_ro_bytes[index], ro_bytes[index]);
    CHECK_VALUE(expected_ro_words[index], ro_words[index]);
    CHECK_VALUE(expected_ro_longs[index], ro_longs[index]);
  }
  for (index = 0; index != 17; ++index)
    CHECK_VALUE(expected_string[index], string[index]);

  raw = llvm_entry_proxy(GLOBAL_DATA_POINTER, 0, 0);
  CHECK_VALUE(1, raw == (const void *)&data_bytes[7]);

  digest = (const abi_u32 *)llvm_entry_proxy(GLOBAL_DIGEST, 0, 0);
  CHECK_VALUE(1, digest != (const abi_u32 *)0);
  CHECK_VALUE(0x6fd4d1f7UL, *digest);

  c166_test_finish();
  simulator_stop();
}
