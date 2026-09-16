#ifndef SEEDED_INTEGER_CORPUS_INC
#define SEEDED_INTEGER_CORPUS_INC

#define ABI_SEEDED_INTEGER_SUITE_ID 284U
#define ABI_SEEDED_INTEGER_ITERATIONS 640U
#define ABI_SEEDED_INTEGER_OP_COUNT 54U
#define ABI_SEEDED_INTEGER_CHECK_COUNT 103680UL
#define ABI_SEEDED_INTEGER_REFERENCE_SIGNATURE_SEED 0x51ed0000UL
#define ABI_SEEDED_INTEGER_EXPECTED_SIGNATURE 0x8cc4a0e2UL
#define ABI_SEEDED_INTEGER_SEEDS(X) \
  X(1, 0x00000001UL) \
  X(2, 0x1660c166UL) \
  X(3, 0xdeadbeefUL)
#define ABI_SEEDED_INTEGER_REFERENCE_SIGNATURES(X) \
  X(0, u8_add, 0x2dc170a5UL) \
  X(1, u8_sub, 0x0d10a2daUL) \
  X(2, u8_xor, 0xce5898deUL) \
  X(3, u8_not, 0x40a6462fUL) \
  X(4, u8_shl, 0xa6513f32UL) \
  X(5, u8_shr, 0xa590b97fUL) \
  X(6, u8_lt, 0x6862b24dUL) \
  X(7, u8_select, 0xb693da46UL) \
  X(8, s8_add, 0x688e22f5UL) \
  X(9, s8_sub, 0x4c42dd41UL) \
  X(10, s8_mul, 0x3c17604fUL) \
  X(11, s8_neg, 0x6a2b43caUL) \
  X(12, s8_lt, 0x738b171cUL) \
  X(13, s8_ge, 0x738b171dUL) \
  X(14, s8_min, 0x04dca83aUL) \
  X(15, s8_max, 0xcd2d62a7UL) \
  X(16, u16_add, 0xecb2b4aeUL) \
  X(17, u16_sub, 0x22aea915UL) \
  X(18, u16_mul, 0x1e8201bbUL) \
  X(19, u16_and, 0xe20a8e0fUL) \
  X(20, u16_or, 0x756a1e26UL) \
  X(21, u16_shl, 0x22871a96UL) \
  X(22, u16_shr, 0x64947812UL) \
  X(23, u16_div, 0x25c48046UL) \
  X(24, u16_mod, 0x8e859cd9UL) \
  X(25, u16_lt, 0xed2cfbeaUL) \
  X(26, s16_add_wide, 0xacd5f4c3UL) \
  X(27, s16_sub_wide, 0x0c5f7811UL) \
  X(28, s16_neg_wide, 0xe2513c69UL) \
  X(29, s16_div, 0xb5f1a595UL) \
  X(30, s16_mod, 0xbee57bfdUL) \
  X(31, s16_lt, 0xb85a9abaUL) \
  X(32, s16_ge, 0xb85a9a85UL) \
  X(33, s16_min, 0x7c48c748UL) \
  X(34, s16_max, 0x0737bb7aUL) \
  X(35, u32_add, 0x68fef7d4UL) \
  X(36, u32_sub, 0x9d3daa01UL) \
  X(37, u32_neg, 0xc2755418UL) \
  X(38, u32_and, 0x3ee1d2cdUL) \
  X(39, u32_or, 0x8d4c249eUL) \
  X(40, u32_xor, 0xe5c0f1faUL) \
  X(41, u32_not, 0xa0b84dc2UL) \
  X(42, u32_shl, 0x5fbb266eUL) \
  X(43, u32_shr, 0xddcbc87bUL) \
  X(44, u32_lt, 0x2186cb40UL) \
  X(45, u32_select, 0xdc0cb452UL) \
  X(46, s32_lt, 0x9fc5110bUL) \
  X(47, s32_le, 0x9fc5110aUL) \
  X(48, s32_gt, 0x9fc51115UL) \
  X(49, s32_ge, 0x9fc51114UL) \
  X(50, s32_min, 0x85d8e750UL) \
  X(51, s32_max, 0x36751103UL) \
  X(52, s32_s16_add, 0xacd5f4edUL) \
  X(53, s32_s16_abs, 0x9b7d0c26UL)

#endif
