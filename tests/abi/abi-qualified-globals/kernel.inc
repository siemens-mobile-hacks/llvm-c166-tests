typedef unsigned char qualified_u8;
typedef unsigned int qualified_u16;
typedef unsigned long qualified_u32;

struct qualified_data {
	qualified_u8 byte_value;
	qualified_u16 word_value;
	qualified_u32 long_value;
};

volatile struct qualified_data C166_NEAR c166_near_data = {
    0xa5U, 0x1357U, 0x89abcdefUL};
#if !C166_TEST_SMALL
volatile struct qualified_data C166_XNEAR c166_xnear_data = {
    0x5aU, 0x2468U, 0x76543210UL};
#endif
volatile struct qualified_data C166_FAR c166_far_data = {
    0xc3U, 0x8001U, 0xfedcba98UL};
volatile struct qualified_data C166_HUGE c166_huge_data = {
    0x3cU, 0xfffeU, 0x01234567UL};
volatile struct qualified_data C166_SHUGE c166_shuge_data = {
    0x96U, 0x7fffU, 0xa55a5aa5UL};

volatile qualified_u8 C166_NEAR c166_near_byte;
volatile qualified_u16 C166_NEAR c166_near_word;
volatile qualified_u32 C166_NEAR c166_near_long;
#if !C166_TEST_SMALL
volatile qualified_u8 C166_XNEAR c166_xnear_byte;
volatile qualified_u16 C166_XNEAR c166_xnear_word;
volatile qualified_u32 C166_XNEAR c166_xnear_long;
#endif
volatile qualified_u8 C166_FAR c166_far_byte;
volatile qualified_u16 C166_FAR c166_far_word;
volatile qualified_u32 C166_FAR c166_far_long;
volatile qualified_u8 C166_HUGE c166_huge_byte;
volatile qualified_u16 C166_HUGE c166_huge_word;
volatile qualified_u32 C166_HUGE c166_huge_long;
volatile qualified_u8 C166_SHUGE c166_shuge_byte;
volatile qualified_u16 C166_SHUGE c166_shuge_word;
volatile qualified_u32 C166_SHUGE c166_shuge_long;

qualified_u32 c166_qualified_globals(qualified_u16 seed, qualified_u16 read_only) {
  qualified_u32 wide;
  qualified_u32 digest;

	/* OR every field so nonzero bytes cannot cancel each other in a digest. */
	if (read_only) {
		digest = c166_near_byte | c166_near_word | c166_near_long;
#if !C166_TEST_SMALL
		digest |= c166_xnear_byte | c166_xnear_word | c166_xnear_long;
#endif
		digest |= c166_far_byte | c166_far_word | c166_far_long;
		digest |= c166_huge_byte | c166_huge_word | c166_huge_long;
		digest |= c166_shuge_byte | c166_shuge_word | c166_shuge_long;
		digest |= c166_near_data.byte_value ^ 0xa5U;
		digest |= c166_near_data.word_value ^ 0x1357U;
		digest |= c166_near_data.long_value ^ 0x89abcdefUL;
#if !C166_TEST_SMALL
		digest |= c166_xnear_data.byte_value ^ 0x5aU;
		digest |= c166_xnear_data.word_value ^ 0x2468U;
		digest |= c166_xnear_data.long_value ^ 0x76543210UL;
#endif
		digest |= c166_far_data.byte_value ^ 0xc3U;
		digest |= c166_far_data.word_value ^ 0x8001U;
		digest |= c166_far_data.long_value ^ 0xfedcba98UL;
		digest |= c166_huge_data.byte_value ^ 0x3cU;
		digest |= c166_huge_data.word_value ^ 0xfffeU;
		digest |= c166_huge_data.long_value ^ 0x01234567UL;
		digest |= c166_shuge_data.byte_value ^ 0x96U;
		digest |= c166_shuge_data.word_value ^ 0x7fffU;
		digest |= c166_shuge_data.long_value ^ 0xa55a5aa5UL;
		return digest;
	}

  wide = 0x12340000UL + seed;
  c166_near_byte = (qualified_u8)(seed ^ 0x5aU);
  c166_near_word = (qualified_u16)(seed + 0x1234U);
  c166_near_long = wide ^ 0x13579bdfUL;
#if !C166_TEST_SMALL
  c166_xnear_byte = (qualified_u8)(seed + 0xa5U);
  c166_xnear_word = (qualified_u16)(seed ^ 0x5aa5U);
  c166_xnear_long = wide + 0x2468ace0UL;
#endif
  c166_far_byte = (qualified_u8)(seed ^ 0xc3U);
  c166_far_word = (qualified_u16)(seed + 0x369cU);
  c166_far_long = wide ^ 0x89abcdefUL;
  c166_huge_byte = (qualified_u8)(seed + 0x3cU);
  c166_huge_word = (qualified_u16)(seed ^ 0xa55aU);
  c166_huge_long = wide + 0x10203040UL;
  c166_shuge_byte = (qualified_u8)(seed ^ 0x96U);
  c166_shuge_word = (qualified_u16)(seed + 0x5a5aU);
  c166_shuge_long = wide ^ 0xfedcba98UL;

  digest = c166_near_long;
#if !C166_TEST_SMALL
  digest ^= c166_xnear_long;
#endif
  digest += c166_far_long;
  digest ^= c166_huge_long;
  digest += c166_shuge_long;
  digest += c166_near_word;
#if !C166_TEST_SMALL
  digest ^= c166_xnear_word;
#endif
  digest += c166_far_word;
  digest ^= c166_huge_word;
  digest += c166_shuge_word;
  digest += c166_near_byte;
#if !C166_TEST_SMALL
  digest ^= c166_xnear_byte;
#endif
  digest += c166_far_byte;
  digest ^= c166_huge_byte;
  digest += c166_shuge_byte;

  return digest;
}
