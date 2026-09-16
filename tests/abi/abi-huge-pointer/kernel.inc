#include "c166_test.h"

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef signed long abi_s32;
typedef unsigned long abi_u32;

static abi_u32 canonical_direct_address(abi_u32 address) {
  return (address & 0x3fffUL) | 0x4000UL;
}

static abi_u16 huge_relation_mask(volatile abi_u8 C166_HUGE *a,
                                  volatile abi_u8 C166_HUGE *b) {
  return (abi_u16)((a == b) | ((a != b) << 1) | ((a < b) << 2) |
                   ((a <= b) << 3) | ((a > b) << 4) | ((a >= b) << 5));
}

static abi_u16 shuge_relation_mask(volatile abi_u8 C166_SHUGE *a,
                                   volatile abi_u8 C166_SHUGE *b) {
  return (abi_u16)((a == b) | ((a != b) << 1) | ((a < b) << 2) |
                   ((a <= b) << 3) | ((a > b) << 4) | ((a >= b) << 5));
}

C166_NOINLINE abi_u32 huge_pointer_kernel(
    abi_u16 seed, volatile abi_u32 C166_HUGE *huge_words,
    volatile abi_u32 C166_SHUGE *shuge_words) {
  volatile abi_u8 C166_HUGE *huge_bytes =
      (volatile abi_u8 C166_HUGE *)huge_words;
  volatile abi_u8 C166_SHUGE *shuge_bytes =
      (volatile abi_u8 C166_SHUGE *)shuge_words;
  abi_u32 digest = 0x16606875UL ^ seed;
  abi_u16 index;

  for (index = 0; index != 8; ++index) {
    abi_u32 low = (abi_u32)(abi_u16)(seed + index * 0x1111U);
    abi_u32 high = (abi_u32)(abi_u16)(seed ^ (index * 0x2222U));
    huge_words[index] = low | (high << 16);
    shuge_words[index] = (low ^ 0xa55aUL) | ((high ^ 0x5aa5UL) << 16);
  }

  huge_bytes[1] = (abi_u8)(seed ^ 0x5aU);
  *(volatile abi_u16 C166_HUGE *)(huge_bytes + 2U) =
      (abi_u16)(seed ^ 0xa55aU);
  shuge_bytes[1] = (abi_u8)(seed ^ 0xa5U);
  *(volatile abi_u16 C166_SHUGE *)(shuge_bytes + 2U) =
      (abi_u16)(seed ^ 0x5aa5U);
  digest ^= huge_bytes[1];
  digest += *(volatile abi_u16 C166_HUGE *)(huge_bytes + 2U);
  digest ^= shuge_bytes[1];
  digest += *(volatile abi_u16 C166_SHUGE *)(shuge_bytes + 2U);

  {
    volatile abi_u32 C166_HUGE *crossing =
        (volatile abi_u32 C166_HUGE *)(huge_bytes + 14U);
    *crossing = 0xa55a0000UL | seed;
    digest ^= *crossing;
  }
  huge_words[8] = digest;

  for (index = 0; index != 8; ++index) {
    digest += huge_words[index];
    digest = (digest << 3) | (digest >> 29);
    digest ^= shuge_words[index];
  }
  huge_words[9] = digest;

  {
    volatile abi_u8 C166_HUGE *huge_edge =
        (volatile abi_u8 C166_HUGE *)(((abi_u32)huge_bytes & 0xffff0000UL) |
                                     0xfff0UL);
    volatile abi_u8 C166_HUGE *huge_next = huge_edge + 0x20U;
    volatile abi_u8 C166_SHUGE *shuge_edge =
        (volatile abi_u8 C166_SHUGE *)(((abi_u32)shuge_bytes & 0xffff0000UL) |
                                      0xfff0UL);
    volatile abi_u8 C166_SHUGE *shuge_wrap = shuge_edge + 0x20U;

    digest ^= (abi_u32)huge_next - (abi_u32)huge_edge;
    digest += (abi_u32)shuge_wrap - (abi_u32)shuge_edge;
    digest ^= (abi_u32)(abi_s32)(huge_next - huge_edge);
    digest += (abi_u32)(abi_s32)(shuge_wrap - shuge_edge);
    digest ^= (abi_u32)huge_relation_mask(huge_next, huge_edge) << 9;
    digest ^= (abi_u32)shuge_relation_mask(shuge_wrap, shuge_edge) << 15;
    digest ^= (abi_u32)(huge_edge != (volatile abi_u8 C166_HUGE *)0) << 11;
    digest ^= (abi_u32)(shuge_edge != (volatile abi_u8 C166_SHUGE *)0) << 12;
    digest ^= (abi_u32)((volatile abi_u8 C166_HUGE *)(abi_u32)huge_next ==
                        huge_next) << 13;
    digest ^= (abi_u32)((volatile abi_u8 C166_SHUGE *)(abi_u32)shuge_wrap ==
                        shuge_wrap) << 14;
  }
  huge_words[10] = digest;

  {
    abi_u16 near_raw = (abi_u16)(0x8010U + (seed & 0x000fU));
    abi_u16 xnear_raw = (abi_u16)(0x4010U + (seed & 0x000fU));
    volatile abi_u8 C166_NEAR *near_value =
        (volatile abi_u8 C166_NEAR *)near_raw;
    volatile abi_u8 C166_XNEAR *xnear_value =
        (volatile abi_u8 C166_XNEAR *)xnear_raw;
    volatile abi_u8 C166_HUGE *near_huge =
        (volatile abi_u8 C166_HUGE *)near_value;
    volatile abi_u8 C166_SHUGE *near_shuge =
        (volatile abi_u8 C166_SHUGE *)near_value;
    volatile abi_u8 C166_HUGE *xnear_huge =
        (volatile abi_u8 C166_HUGE *)xnear_value;
    volatile abi_u8 C166_SHUGE *xnear_shuge =
        (volatile abi_u8 C166_SHUGE *)xnear_value;
    abi_u16 near_from_huge =
        (abi_u16)(volatile abi_u8 C166_NEAR *)near_huge;
    abi_u16 near_from_shuge =
        (abi_u16)(volatile abi_u8 C166_NEAR *)near_shuge;
    abi_u16 xnear_from_huge =
        (abi_u16)(volatile abi_u8 C166_XNEAR *)xnear_huge;
    abi_u16 xnear_from_shuge =
        (abi_u16)(volatile abi_u8 C166_XNEAR *)xnear_shuge;

    digest ^= canonical_direct_address((abi_u32)near_huge);
    digest += canonical_direct_address((abi_u32)near_shuge);
    digest ^= canonical_direct_address((abi_u32)xnear_huge);
    digest += canonical_direct_address((abi_u32)xnear_shuge);
    digest ^= canonical_direct_address(near_from_huge);
    digest += canonical_direct_address(near_from_shuge);
    digest ^= canonical_direct_address(xnear_from_huge);
    digest += canonical_direct_address(xnear_from_shuge);
    huge_words[11] = (abi_u32)near_huge;
    huge_words[12] = (abi_u32)near_shuge;
    huge_words[13] = (abi_u32)xnear_huge;
    huge_words[14] = (abi_u32)xnear_shuge;
    huge_words[15] = near_from_huge;
    huge_words[16] = near_from_shuge;
    huge_words[17] = xnear_from_huge;
    huge_words[18] = xnear_from_shuge;
  }

  {
    volatile abi_u32 C166_HUGE *huge_far_roundtrip =
        (volatile abi_u32 C166_HUGE *)(volatile abi_u32 C166_FAR *)(huge_words + 5);
    volatile abi_u32 C166_HUGE *huge_shuge_roundtrip =
        (volatile abi_u32 C166_HUGE *)(volatile abi_u32 C166_SHUGE *)(huge_words + 6);
    volatile abi_u32 C166_SHUGE *shuge_huge_roundtrip =
        (volatile abi_u32 C166_SHUGE *)(volatile abi_u32 C166_HUGE *)(shuge_words + 7);

    digest ^= *huge_far_roundtrip;
    digest += *huge_shuge_roundtrip;
    digest ^= *shuge_huge_roundtrip;
    digest += (abi_u32)(volatile abi_u32 C166_HUGE *)0;
    digest ^= (abi_u32)(volatile abi_u32 C166_SHUGE *)0;
  }
  huge_words[19] = digest;

  return digest;
}
