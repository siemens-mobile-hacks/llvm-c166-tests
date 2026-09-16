#include "c166_test.h"
#include "types.h"

#if defined(C166_TEST_RELOCATED)
#define GLOBAL_TEXT C166_SECTION(".global.lifecycle.text")
#else
#define GLOBAL_TEXT
#endif

abi_u8 global_data_bytes[8] = {
    0x01U, 0x23U, 0x45U, 0x67U, 0x89U, 0xabU, 0xcdU, 0xefU,
};
abi_u16 global_data_words[8] = {
    0x0000U, 0x0001U, 0x7fffU, 0x8000U,
    0xa55aU, 0x5aa5U, 0xfffeU, 0xffffU,
};
abi_u32 global_data_longs[8] = {
    0x00000000UL, 0x00000001UL, 0x0000ffffUL, 0x00010000UL,
    0x7fffffffUL, 0x80000000UL, 0xa55a5aa5UL, 0xffffffffUL,
};

abi_u8 global_bss_bytes[8];
abi_u16 global_bss_words[8];
abi_u32 global_bss_longs[8];
abi_u32 global_digest;

const abi_u8 global_ro_bytes[8] = {
    0xf0U, 0xdeU, 0xbcU, 0x9aU, 0x78U, 0x56U, 0x34U, 0x12U,
};
const abi_u16 global_ro_words[8] = {
    0x1357U, 0x2468U, 0x369cU, 0x48adU,
    0x5aceU, 0x6bdfU, 0x7ef0U, 0x8f01U,
};
const abi_u32 global_ro_longs[8] = {
    0x01234567UL, 0x89abcdefUL, 0x13579bdfUL, 0x2468ace0UL,
    0x0f1e2d3cUL, 0x4b5a6978UL, 0xdeadbeefUL, 0xc001d00dUL,
};
const abi_u8 global_string[17] = "C166 Classic ELF";

abi_u8 *global_data_pointer = global_data_bytes;
const abi_u8 *global_string_pointer = global_string;

static abi_u32 mix(abi_u32 digest, abi_u32 value, abi_u16 tag) {
  abi_u32 tag32 = ((abi_u32)tag << 16) | tag;
  return ((digest << 5) | (digest >> 27)) ^ value ^ tag32;
}

static abi_u32 compute_digest(void) {
  abi_u32 digest = 0x1660474cUL;
  abi_u16 index;
  abi_u16 tag = 1;

  for (index = 0; index != 8; ++index) {
    digest = mix(digest, global_data_bytes[index], tag++);
    digest = mix(digest, global_data_words[index], tag++);
    digest = mix(digest, global_data_longs[index], tag++);
    digest = mix(digest, global_bss_bytes[index], tag++);
    digest = mix(digest, global_bss_words[index], tag++);
    digest = mix(digest, global_bss_longs[index], tag++);
    digest = mix(digest, global_ro_bytes[index], tag++);
    digest = mix(digest, global_ro_words[index], tag++);
    digest = mix(digest, global_ro_longs[index], tag++);
  }
  for (index = 0; index != 17; ++index)
    digest = mix(digest, global_string_pointer[index], tag++);
  return digest;
}

C166_NOINLINE GLOBAL_TEXT abi_u8 *mutate_globals(abi_u16 index,
                                                abi_u32 seed) {
  abi_u16 byte_index = index & 7U;
  abi_u16 word_index = (index * 3U + 1U) & 7U;
  abi_u16 long_index = (index * 5U + 2U) & 7U;
  abi_u8 byte_value = (abi_u8)((abi_u16)seed ^ 0x005aU);
  abi_u16 word_value = (abi_u16)seed + 0x1357U;
  abi_u32 long_value = ((abi_u32)(abi_u16)seed << 16) |
                       ((abi_u16)seed ^ 0xa55aU);

  global_data_bytes[byte_index] = byte_value;
  global_data_words[word_index] = word_value;
  global_data_longs[long_index] = long_value;
  global_bss_bytes[byte_index] = (abi_u8)(byte_value ^ 0xffU);
  global_bss_words[word_index] = word_value ^ 0xffffU;
  global_bss_longs[long_index] = long_value ^ 0xffffffffUL;
  global_data_pointer = &global_data_bytes[byte_index];
  return global_data_pointer;
}

C166_NOINLINE GLOBAL_TEXT const abi_u32 *update_global_digest(void) {
  global_digest = compute_digest();
  return &global_digest;
}
