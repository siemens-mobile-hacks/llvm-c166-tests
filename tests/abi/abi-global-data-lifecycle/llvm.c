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

abi_u8 llvm_data_bytes[8] = {
    0x01U, 0x23U, 0x45U, 0x67U, 0x89U, 0xabU, 0xcdU, 0xefU,
};
abi_u16 llvm_data_words[8] = {
    0x0000U, 0x0001U, 0x7fffU, 0x8000U,
    0xa55aU, 0x5aa5U, 0xfffeU, 0xffffU,
};
abi_u32 llvm_data_longs[8] = {
    0x00000000UL, 0x00000001UL, 0x0000ffffUL, 0x00010000UL,
    0x7fffffffUL, 0x80000000UL, 0xa55a5aa5UL, 0xffffffffUL,
};

abi_u8 llvm_bss_bytes[8];
abi_u16 llvm_bss_words[8];
abi_u32 llvm_bss_longs[8];
abi_u32 llvm_digest;

const abi_u8 llvm_ro_bytes[8] = {
    0xf0U, 0xdeU, 0xbcU, 0x9aU, 0x78U, 0x56U, 0x34U, 0x12U,
};
const abi_u16 llvm_ro_words[8] = {
    0x1357U, 0x2468U, 0x369cU, 0x48adU,
    0x5aceU, 0x6bdfU, 0x7ef0U, 0x8f01U,
};
const abi_u32 llvm_ro_longs[8] = {
    0x01234567UL, 0x89abcdefUL, 0x13579bdfUL, 0x2468ace0UL,
    0x0f1e2d3cUL, 0x4b5a6978UL, 0xdeadbeefUL, 0xc001d00dUL,
};

abi_u8 *llvm_data_pointer = llvm_data_bytes;
const abi_u8 *llvm_string_pointer = (const abi_u8 *)"C166 Classic ELF";

static abi_u32 mix(abi_u32 digest, abi_u32 value, abi_u16 tag) {
  abi_u32 tag32 = ((abi_u32)tag << 16) | tag;
  return ((digest << 5) | (digest >> 27)) ^ value ^ tag32;
}

static abi_u32 compute_digest(void) {
  abi_u32 digest = 0x1660474cUL;
  abi_u16 index;
  abi_u16 tag = 1;

  for (index = 0; index != 8; ++index) {
    digest = mix(digest, llvm_data_bytes[index], tag++);
    digest = mix(digest, llvm_data_words[index], tag++);
    digest = mix(digest, llvm_data_longs[index], tag++);
    digest = mix(digest, llvm_bss_bytes[index], tag++);
    digest = mix(digest, llvm_bss_words[index], tag++);
    digest = mix(digest, llvm_bss_longs[index], tag++);
    digest = mix(digest, llvm_ro_bytes[index], tag++);
    digest = mix(digest, llvm_ro_words[index], tag++);
    digest = mix(digest, llvm_ro_longs[index], tag++);
  }
  for (index = 0; index != 17; ++index)
    digest = mix(digest, llvm_string_pointer[index], tag++);
  return digest;
}

__attribute__((noinline))
const void *llvm_global_lifecycle(abi_u16 operation, abi_u16 index,
                                  abi_u32 seed) {
  abi_u16 byte_index = index & 7U;
  abi_u16 word_index = (index * 3U + 1U) & 7U;
  abi_u16 long_index = (index * 5U + 2U) & 7U;
  abi_u8 byte_value = (abi_u8)((abi_u16)seed ^ 0x005aU);
  abi_u16 word_value = (abi_u16)seed + 0x1357U;
  abi_u32 long_value = ((abi_u32)(abi_u16)seed << 16) |
                       ((abi_u16)seed ^ 0xa55aU);

  switch (operation) {
  case GLOBAL_DATA_BYTES:
    return llvm_data_bytes;
  case GLOBAL_DATA_WORDS:
    return llvm_data_words;
  case GLOBAL_DATA_LONGS:
    return llvm_data_longs;
  case GLOBAL_BSS_BYTES:
    return llvm_bss_bytes;
  case GLOBAL_BSS_WORDS:
    return llvm_bss_words;
  case GLOBAL_BSS_LONGS:
    return llvm_bss_longs;
  case GLOBAL_RO_BYTES:
    return llvm_ro_bytes;
  case GLOBAL_RO_WORDS:
    return llvm_ro_words;
  case GLOBAL_RO_LONGS:
    return llvm_ro_longs;
  case GLOBAL_STRING:
    return llvm_string_pointer;
  case GLOBAL_DATA_POINTER:
    return llvm_data_pointer;
  case GLOBAL_MUTATE:
    llvm_data_bytes[byte_index] = byte_value;
    llvm_data_words[word_index] = word_value;
    llvm_data_longs[long_index] = long_value;
    llvm_bss_bytes[byte_index] = (abi_u8)(byte_value ^ 0xffU);
    llvm_bss_words[word_index] = word_value ^ 0xffffU;
    llvm_bss_longs[long_index] = long_value ^ 0xffffffffUL;
    llvm_data_pointer = &llvm_data_bytes[byte_index];
    return llvm_data_pointer;
  case GLOBAL_DIGEST:
    llvm_digest = compute_digest();
    return &llvm_digest;
  default:
    return (const void *)0;
  }
}
