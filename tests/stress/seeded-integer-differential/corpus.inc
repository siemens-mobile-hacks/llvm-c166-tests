#ifndef SEEDED_INTEGER_CORPUS_INC
#define SEEDED_INTEGER_CORPUS_INC

#define ABI_SEEDED_INTEGER_SUITE_ID 284U
#define ABI_SEEDED_INTEGER_ITERATIONS 128U
#define ABI_SEEDED_INTEGER_OP_COUNT 54U
#define ABI_SEEDED_INTEGER_CHECK_COUNT 20736UL
#define ABI_SEEDED_INTEGER_REFERENCE_SIGNATURE_SEED 0x51ed0000UL
#define ABI_SEEDED_INTEGER_EXPECTED_SIGNATURE 0x0febfcb4UL
#define ABI_SEEDED_INTEGER_SEEDS(X) \
  X(1, 0x00000001UL) \
  X(2, 0x1660c166UL) \
  X(3, 0xdeadbeefUL)
#define ABI_SEEDED_INTEGER_REFERENCE_SIGNATURES(X) \
  X(0, u8_add, 0xce8b6f4cUL) \
  X(1, u8_sub, 0x81d3aa5cUL) \
  X(2, u8_xor, 0x5eedb6c3UL) \
  X(3, u8_not, 0x1b221c9dUL) \
  X(4, u8_shl, 0x02b2a7cfUL) \
  X(5, u8_shr, 0xa4790be8UL) \
  X(6, u8_lt, 0xf3bf9510UL) \
  X(7, u8_select, 0xd9a2abbaUL) \
  X(8, s8_add, 0xa0b7ed6bUL) \
  X(9, s8_sub, 0xdeddfa55UL) \
  X(10, s8_mul, 0x66e4157dUL) \
  X(11, s8_neg, 0x04de4da4UL) \
  X(12, s8_lt, 0xe438463bUL) \
  X(13, s8_ge, 0xe438463aUL) \
  X(14, s8_min, 0xdb063784UL) \
  X(15, s8_max, 0x5a4ad129UL) \
  X(16, u16_add, 0x3f1154e3UL) \
  X(17, u16_sub, 0xf6c7e82cUL) \
  X(18, u16_mul, 0x1e08ae49UL) \
  X(19, u16_and, 0x51fad349UL) \
  X(20, u16_or, 0x6e563ddfUL) \
  X(21, u16_shl, 0x457867b8UL) \
  X(22, u16_shr, 0xe284d43fUL) \
  X(23, u16_div, 0x07996389UL) \
  X(24, u16_mod, 0xb48f1db0UL) \
  X(25, u16_lt, 0x495b8108UL) \
  X(26, s16_add_wide, 0xac663861UL) \
  X(27, s16_sub_wide, 0x22d33c32UL) \
  X(28, s16_neg_wide, 0x621dd8baUL) \
  X(29, s16_div, 0x0b9c5b29UL) \
  X(30, s16_mod, 0x9dcb93edUL) \
  X(31, s16_lt, 0xc082ed4dUL) \
  X(32, s16_ge, 0xc082ed72UL) \
  X(33, s16_min, 0x78bf2105UL) \
  X(34, s16_max, 0xe465931eUL) \
  X(35, u32_add, 0x7aecda29UL) \
  X(36, u32_sub, 0x1879b77bUL) \
  X(37, u32_neg, 0xdf20e0f6UL) \
  X(38, u32_and, 0x5bda2f52UL) \
  X(39, u32_or, 0xcfb1ab8dUL) \
  X(40, u32_xor, 0xc4068576UL) \
  X(41, u32_not, 0x3a52b140UL) \
  X(42, u32_shl, 0x3c0372b7UL) \
  X(43, u32_shr, 0x993cd871UL) \
  X(44, u32_lt, 0x2995b594UL) \
  X(45, u32_select, 0x750ad030UL) \
  X(46, s32_lt, 0x7f34c54aUL) \
  X(47, s32_le, 0x7f34c54bUL) \
  X(48, s32_gt, 0x7f34c554UL) \
  X(49, s32_ge, 0x7f34c555UL) \
  X(50, s32_min, 0x64dccb31UL) \
  X(51, s32_max, 0xf0b74feeUL) \
  X(52, s32_s16_add, 0xac66384fUL) \
  X(53, s32_s16_abs, 0xe62f16d3UL)

#endif
