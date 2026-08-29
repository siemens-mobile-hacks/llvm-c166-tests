#include "c166-address-spaces.h"

typedef unsigned char qualified_u8;
typedef unsigned int qualified_u16;
typedef unsigned long qualified_u32;

volatile qualified_u8 _near c166_near_byte;
volatile qualified_u16 _near c166_near_word;
volatile qualified_u32 _near c166_near_long;
#if !TASKING_MODEL_IS_SMALL
volatile qualified_u8 _xnear c166_xnear_byte;
volatile qualified_u16 _xnear c166_xnear_word;
volatile qualified_u32 _xnear c166_xnear_long;
#endif
volatile qualified_u8 _far c166_far_byte;
volatile qualified_u16 _far c166_far_word;
volatile qualified_u32 _far c166_far_long;
volatile qualified_u8 _huge c166_huge_byte;
volatile qualified_u16 _huge c166_huge_word;
volatile qualified_u32 _huge c166_huge_long;
volatile qualified_u8 _shuge c166_shuge_byte;
volatile qualified_u16 _shuge c166_shuge_word;
volatile qualified_u32 _shuge c166_shuge_long;

qualified_u32 c166_qualified_globals(qualified_u16 seed) {
  qualified_u32 wide;
  qualified_u32 digest;

  wide = 0x12340000UL + seed;
  c166_near_byte = (qualified_u8)(seed ^ 0x5aU);
  c166_near_word = (qualified_u16)(seed + 0x1234U);
  c166_near_long = wide ^ 0x13579bdfUL;
#if !TASKING_MODEL_IS_SMALL
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
#if !TASKING_MODEL_IS_SMALL
  digest ^= c166_xnear_long;
#endif
  digest += c166_far_long;
  digest ^= c166_huge_long;
  digest += c166_shuge_long;
  digest += c166_near_word;
#if !TASKING_MODEL_IS_SMALL
  digest ^= c166_xnear_word;
#endif
  digest += c166_far_word;
  digest ^= c166_huge_word;
  digest += c166_shuge_word;
  digest += c166_near_byte;
#if !TASKING_MODEL_IS_SMALL
  digest ^= c166_xnear_byte;
#endif
  digest += c166_far_byte;
  digest ^= c166_huge_byte;
  digest += c166_shuge_byte;

  return digest;
}
