#ifndef ABI_PRESSURE_WORD_FUNCTION
#error ABI_PRESSURE_WORD_FUNCTION must name the word-pressure function
#endif
#ifndef ABI_PRESSURE_LONG_FUNCTION
#error ABI_PRESSURE_LONG_FUNCTION must name the long-pressure function
#endif
#ifndef ABI_PRESSURE_SELECTOR
#error ABI_PRESSURE_SELECTOR must name the selector callback
#endif
#ifndef ABI_PRESSURE_ATTRIBUTE
#define ABI_PRESSURE_ATTRIBUTE
#endif
#ifndef ABI_PRESSURE_WORD_ATTRIBUTE
#define ABI_PRESSURE_WORD_ATTRIBUTE ABI_PRESSURE_ATTRIBUTE
#endif
#ifndef ABI_PRESSURE_LONG_ATTRIBUTE
#define ABI_PRESSURE_LONG_ATTRIBUTE ABI_PRESSURE_ATTRIBUTE
#endif

ABI_PRESSURE_WORD_ATTRIBUTE
abi_u16 ABI_PRESSURE_WORD_FUNCTION(
    abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
    abi_u16 a6, abi_u16 a7, abi_u16 selector) {
  abi_u16 v0 = a0 + 0x0101U;
  abi_u16 v1 = a1 + 0x0203U;
  abi_u16 v2 = a2 + 0x0305U;
  abi_u16 v3 = a3 + 0x0407U;
  abi_u16 v4 = a4 + 0x0509U;
  abi_u16 v5 = a5 + 0x060bU;
  abi_u16 v6 = a6 + 0x070dU;
  abi_u16 v7 = a7 + 0x080fU;

  switch (ABI_PRESSURE_SELECTOR(selector) & 7U) {
  case 0: return v0;
  case 1: return v1;
  case 2: return v2;
  case 3: return v3;
  case 4: return v4;
  case 5: return v5;
  case 6: return v6;
  default: return v7;
  }
}

ABI_PRESSURE_LONG_ATTRIBUTE
abi_u32 ABI_PRESSURE_LONG_FUNCTION(abi_u32 a0, abi_u32 a1, abi_u32 a2,
                                   abi_u32 a3, abi_u32 a4, abi_u32 a5,
                                   abi_u16 selector) {
  abi_u32 v0 = a0 + 0x00010001UL;
  abi_u32 v1 = a1 + 0x01020304UL;
  abi_u32 v2 = a2 + 0x10203040UL;
  abi_u32 v3 = a3 + 0x7fff0001UL;
  abi_u32 v4 = a4 + 0x8000ffffUL;
  abi_u32 v5 = a5 + 0xfedcba98UL;

  switch (ABI_PRESSURE_SELECTOR(selector)) {
  case 0: return v0;
  case 1: return v1;
  case 2: return v2;
  case 3: return v3;
  case 4: return v4;
  default: return v5;
  }
}

#undef ABI_PRESSURE_ATTRIBUTE
#undef ABI_PRESSURE_LONG_ATTRIBUTE
#undef ABI_PRESSURE_WORD_ATTRIBUTE
#undef ABI_PRESSURE_SELECTOR
#undef ABI_PRESSURE_LONG_FUNCTION
#undef ABI_PRESSURE_WORD_FUNCTION
