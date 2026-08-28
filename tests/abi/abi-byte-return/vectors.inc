#define ABI_BYTE_RETURN_VECTORS(X) \
  X(1, (signed char)0x00, 0x00UL) \
  X(2, (signed char)0x01, 0x01UL) \
  X(3, (signed char)0x7f, 0x7fUL) \
  X(4, (signed char)0x80, 0x80UL) \
  X(5, (signed char)0xff, 0xffUL)
