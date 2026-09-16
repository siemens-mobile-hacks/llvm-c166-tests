#include "c166_test.h"
#include "types.h"

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

#if defined(C166_TEST_LLVM)
static volatile abi_u8 C166_SECTION(".noinit") preinit_guard;

void c166_test_preinit(void) {
  abi_u16 index;

  for (index = 0; index != 8; ++index) {
    ((volatile abi_u8 *)global_bss_bytes)[index] = 0xa5U;
    ((volatile abi_u16 *)global_bss_words)[index] = 0x5aa5U;
    ((volatile abi_u32 *)global_bss_longs)[index] = 0xdeadbeefUL;
  }
  global_digest = 0xc001d00dUL;
  preinit_guard = 0x69U;
}
#endif

void main(void) {
  abi_u16 index;
  abi_u16 vector;
  const abi_u32 *digest;

#if defined(C166_TEST_LLVM)
  tap_plan(242U);
  tap_is_u32(preinit_guard, 0x69U, ".bss end preserved");
#else
  tap_plan(241U);
#endif

  for (index = 0; index != 8; ++index) {
    tap_is_u32(global_data_bytes[index], initial_data_bytes[index],
               ".data byte initialized");
    tap_is_u32(global_data_words[index], initial_data_words[index],
               ".data word initialized");
    tap_is_u32(global_data_longs[index], initial_data_longs[index],
               ".data long initialized");
    tap_is_u32(global_bss_bytes[index], 0, ".bss byte zeroed");
    tap_is_u32(global_bss_words[index], 0, ".bss word zeroed");
    tap_is_u32(global_bss_longs[index], 0, ".bss long zeroed");
    tap_is_u32(global_ro_bytes[index], expected_ro_bytes[index],
               ".rodata byte initialized");
    tap_is_u32(global_ro_words[index], expected_ro_words[index],
               ".rodata word initialized");
    tap_is_u32(global_ro_longs[index], expected_ro_longs[index],
               ".rodata long initialized");
  }
  tap_is_u32(global_digest, 0, ".bss digest zeroed");
  for (index = 0; index != 17; ++index)
    tap_is_u32(global_string[index], expected_string[index],
               "string initialized");
  tap_ok(global_data_pointer == global_data_bytes,
         "initialized data pointer");
  tap_ok(global_string_pointer == global_string,
         "initialized const pointer");

  for (vector = 0; vector != 8; ++vector) {
    const struct mutation_vector *mutation = &mutations[vector];
    abi_u8 *result = mutate_globals(vector, mutation->seed);

    tap_ok(result == &global_data_bytes[mutation->byte_index],
           "mutation return pointer");
    tap_is_u32(global_data_bytes[mutation->byte_index], mutation->data_byte,
               "mutated data byte");
    tap_is_u32(global_data_words[mutation->word_index], mutation->data_word,
               "mutated data word");
    tap_is_u32(global_data_longs[mutation->long_index], mutation->data_long,
               "mutated data long");
    tap_is_u32(global_bss_bytes[mutation->byte_index], mutation->bss_byte,
               "mutated bss byte");
    tap_is_u32(global_bss_words[mutation->word_index], mutation->bss_word,
               "mutated bss word");
    tap_is_u32(global_bss_longs[mutation->long_index], mutation->bss_long,
               "mutated bss long");
  }

  for (index = 0; index != 8; ++index) {
    tap_is_u32(global_data_bytes[index], final_data_bytes[index],
               "final data byte");
    tap_is_u32(global_data_words[index], final_data_words[index],
               "final data word");
    tap_is_u32(global_data_longs[index], final_data_longs[index],
               "final data long");
    tap_is_u32(global_bss_bytes[index], final_bss_bytes[index],
               "final bss byte");
    tap_is_u32(global_bss_words[index], final_bss_words[index],
               "final bss word");
    tap_is_u32(global_bss_longs[index], final_bss_longs[index],
               "final bss long");
    tap_is_u32(global_ro_bytes[index], expected_ro_bytes[index],
               "unchanged rodata byte");
    tap_is_u32(global_ro_words[index], expected_ro_words[index],
               "unchanged rodata word");
    tap_is_u32(global_ro_longs[index], expected_ro_longs[index],
               "unchanged rodata long");
  }
  for (index = 0; index != 17; ++index)
    tap_is_u32(global_string[index], expected_string[index],
               "unchanged string");

  tap_ok(global_string_pointer == global_string,
         "unchanged const pointer");
  tap_ok(global_data_pointer == &global_data_bytes[7],
         "final data pointer");
  digest = update_global_digest();
  tap_ok(digest == &global_digest, "digest pointer");
  tap_is_u32(*digest, 0x6fd4d1f7UL, "digest value");
}
