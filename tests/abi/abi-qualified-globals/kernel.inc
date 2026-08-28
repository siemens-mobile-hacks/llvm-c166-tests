#include "c166-address-spaces.h"

#ifndef QUALIFIED_PREFIX
#error QUALIFIED_PREFIX must name this translation unit's globals
#endif

#define QUALIFIED_JOIN_INNER(a, b) a##b
#define QUALIFIED_JOIN(a, b) QUALIFIED_JOIN_INNER(a, b)
#define QUALIFIED_NAME(name) QUALIFIED_JOIN(QUALIFIED_PREFIX, name)

typedef unsigned char qualified_u8;
typedef unsigned int qualified_u16;
typedef unsigned long qualified_u32;

volatile qualified_u8 _near QUALIFIED_NAME(_near_byte);
volatile qualified_u16 _near QUALIFIED_NAME(_near_word);
volatile qualified_u32 _near QUALIFIED_NAME(_near_long);
#if !TASKING_MODEL_IS_SMALL
volatile qualified_u8 _xnear QUALIFIED_NAME(_xnear_byte);
volatile qualified_u16 _xnear QUALIFIED_NAME(_xnear_word);
volatile qualified_u32 _xnear QUALIFIED_NAME(_xnear_long);
#endif
volatile qualified_u8 _far QUALIFIED_NAME(_far_byte);
volatile qualified_u16 _far QUALIFIED_NAME(_far_word);
volatile qualified_u32 _far QUALIFIED_NAME(_far_long);
volatile qualified_u8 _huge QUALIFIED_NAME(_huge_byte);
volatile qualified_u16 _huge QUALIFIED_NAME(_huge_word);
volatile qualified_u32 _huge QUALIFIED_NAME(_huge_long);
volatile qualified_u8 _shuge QUALIFIED_NAME(_shuge_byte);
volatile qualified_u16 _shuge QUALIFIED_NAME(_shuge_word);
volatile qualified_u32 _shuge QUALIFIED_NAME(_shuge_long);

qualified_u32 QUALIFIED_NAME(_qualified_globals)(qualified_u16 seed) {
  qualified_u32 wide;
  qualified_u32 digest;

  wide = 0x12340000UL + seed;
  QUALIFIED_NAME(_near_byte) = (qualified_u8)(seed ^ 0x5aU);
  QUALIFIED_NAME(_near_word) = (qualified_u16)(seed + 0x1234U);
  QUALIFIED_NAME(_near_long) = wide ^ 0x13579bdfUL;
#if !TASKING_MODEL_IS_SMALL
  QUALIFIED_NAME(_xnear_byte) = (qualified_u8)(seed + 0xa5U);
  QUALIFIED_NAME(_xnear_word) = (qualified_u16)(seed ^ 0x5aa5U);
  QUALIFIED_NAME(_xnear_long) = wide + 0x2468ace0UL;
#endif
  QUALIFIED_NAME(_far_byte) = (qualified_u8)(seed ^ 0xc3U);
  QUALIFIED_NAME(_far_word) = (qualified_u16)(seed + 0x369cU);
  QUALIFIED_NAME(_far_long) = wide ^ 0x89abcdefUL;
  QUALIFIED_NAME(_huge_byte) = (qualified_u8)(seed + 0x3cU);
  QUALIFIED_NAME(_huge_word) = (qualified_u16)(seed ^ 0xa55aU);
  QUALIFIED_NAME(_huge_long) = wide + 0x10203040UL;
  QUALIFIED_NAME(_shuge_byte) = (qualified_u8)(seed ^ 0x96U);
  QUALIFIED_NAME(_shuge_word) = (qualified_u16)(seed + 0x5a5aU);
  QUALIFIED_NAME(_shuge_long) = wide ^ 0xfedcba98UL;

  digest = QUALIFIED_NAME(_near_long);
#if !TASKING_MODEL_IS_SMALL
  digest ^= QUALIFIED_NAME(_xnear_long);
#endif
  digest += QUALIFIED_NAME(_far_long);
  digest ^= QUALIFIED_NAME(_huge_long);
  digest += QUALIFIED_NAME(_shuge_long);
  digest += QUALIFIED_NAME(_near_word);
#if !TASKING_MODEL_IS_SMALL
  digest ^= QUALIFIED_NAME(_xnear_word);
#endif
  digest += QUALIFIED_NAME(_far_word);
  digest ^= QUALIFIED_NAME(_huge_word);
  digest += QUALIFIED_NAME(_shuge_word);
  digest += QUALIFIED_NAME(_near_byte);
#if !TASKING_MODEL_IS_SMALL
  digest ^= QUALIFIED_NAME(_xnear_byte);
#endif
  digest += QUALIFIED_NAME(_far_byte);
  digest ^= QUALIFIED_NAME(_huge_byte);
  digest += QUALIFIED_NAME(_shuge_byte);

  return digest;
}

#undef QUALIFIED_NAME
#undef QUALIFIED_JOIN
#undef QUALIFIED_JOIN_INNER
