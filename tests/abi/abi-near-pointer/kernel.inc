typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef signed int abi_s16;
typedef unsigned long abi_u32;

static abi_u32 qualifier_kernel(
    abi_u16 seed, volatile abi_u32 C166_NEAR *near_words,
    volatile abi_u32 C166_XNEAR *xnear_words) {
  volatile abi_u8 C166_NEAR *near_bytes =
      (volatile abi_u8 C166_NEAR *)near_words;
  volatile abi_u8 C166_XNEAR *xnear_bytes =
      (volatile abi_u8 C166_XNEAR *)xnear_words;
  volatile abi_u16 C166_NEAR *near_halves =
      (volatile abi_u16 C166_NEAR *)near_words;
  volatile abi_u16 C166_XNEAR *xnear_halves =
      (volatile abi_u16 C166_XNEAR *)xnear_words;
  abi_u32 digest = 0x16604e58UL ^ seed;
  abi_u16 index;

  for (index = 0; index != 8; ++index) {
    abi_u32 low = (abi_u32)(abi_u16)(seed + index * 0x1111U);
    abi_u32 high = (abi_u32)(abi_u16)(seed ^ (index * 0x2222U));
    near_words[index] = low | (high << 16);
    xnear_words[index] = (low ^ 0xa55aUL) | ((high ^ 0x5aa5UL) << 16);
  }

  for (index = 0; index != 8; ++index) {
    abi_u16 byte_index = (abi_u16)(index * 4U);
    abi_u16 half_index = (abi_u16)(index * 2U);
    digest ^= near_words[index];
    digest = (digest << 3) | (digest >> 29);
    digest += xnear_words[index];
    digest ^= (abi_u32)near_halves[half_index];
    digest += (abi_u32)xnear_halves[half_index + 1U] << 1;
    digest ^= (abi_u32)near_bytes[byte_index + 1U] << 9;
    digest += (abi_u32)xnear_bytes[byte_index + 2U] << 17;
  }

  digest ^= (abi_u16)((near_words + 7) - (near_words + 1));
  digest ^= (abi_u32)(abi_u16)((xnear_words + 7) - (xnear_words + 1)) << 8;
  digest ^= (abi_u32)(near_words + 1 < near_words + 7) << 20;
  digest ^= (abi_u32)(xnear_words + 7 > xnear_words + 1) << 21;
  digest ^= (abi_u32)(near_words != (volatile abi_u32 C166_NEAR *)0) << 22;
  digest ^= (abi_u32)(xnear_words != (volatile abi_u32 C166_XNEAR *)0) << 23;

  {
    abi_u16 near_word = (abi_u16)near_words;
    abi_u16 xnear_word = (abi_u16)xnear_words;
    abi_u32 near_linear = (abi_u32)near_words;
    abi_u32 xnear_linear = (abi_u32)xnear_words;
    volatile abi_u32 C166_FAR *near_far =
        (volatile abi_u32 C166_FAR *)near_words;
    volatile abi_u32 C166_FAR *xnear_far =
        (volatile abi_u32 C166_FAR *)xnear_words;
    volatile abi_u32 C166_NEAR *near_from_word =
        (volatile abi_u32 C166_NEAR *)near_word;
    volatile abi_u32 C166_XNEAR *xnear_from_word =
        (volatile abi_u32 C166_XNEAR *)xnear_word;
    volatile abi_u32 C166_NEAR *near_from_long =
        (volatile abi_u32 C166_NEAR *)near_linear;
    volatile abi_u32 C166_XNEAR *xnear_from_long =
        (volatile abi_u32 C166_XNEAR *)xnear_linear;
    volatile abi_u32 C166_NEAR *near_from_far =
        (volatile abi_u32 C166_NEAR *)near_far;
    volatile abi_u32 C166_XNEAR *xnear_from_far =
        (volatile abi_u32 C166_XNEAR *)xnear_far;
    volatile abi_u32 C166_NEAR *near_cross_roundtrip =
        (volatile abi_u32 C166_NEAR *)(volatile abi_u32 C166_XNEAR *)near_words;
    volatile abi_u32 C166_XNEAR *xnear_cross_roundtrip =
        (volatile abi_u32 C166_XNEAR *)(volatile abi_u32 C166_NEAR *)xnear_words;

    digest ^= (abi_u32)(near_words + 3) - near_linear;
    digest += (abi_u32)(xnear_words + 3) - xnear_linear;
    digest ^= near_from_word[1];
    digest += xnear_from_word[2];
    digest ^= near_from_long[3];
    digest += xnear_from_long[4];
    digest ^= near_far[5];
    digest += xnear_far[6];
    digest ^= near_from_far[7];
    digest += xnear_from_far[0];
    digest ^= near_cross_roundtrip[2];
    digest += xnear_cross_roundtrip[3];
    digest ^= (abi_u32)((volatile abi_u32 C166_FAR *)
                            (volatile abi_u32 C166_NEAR *)0 ==
                        (volatile abi_u32 C166_FAR *)0)
              << 24;
    digest ^= (abi_u32)((volatile abi_u32 C166_FAR *)
                            (volatile abi_u32 C166_XNEAR *)0 ==
                        (volatile abi_u32 C166_FAR *)0)
              << 25;
  }
  return digest;
}
