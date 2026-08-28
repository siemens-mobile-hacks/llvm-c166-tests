#ifndef FLOAT_VOLATILE_MATRIX_VECTORS_INC
#define FLOAT_VOLATILE_MATRIX_VECTORS_INC

#define ABI_VOLATILE_F32_RAW_VECTORS(X)                                    \
  X(1, 0x00000000UL)                                                       \
  X(2, 0x80000000UL)                                                       \
  X(3, 0x00000001UL)                                                       \
  X(4, 0x007fffffUL)                                                       \
  X(5, 0x00800000UL)                                                       \
  X(6, 0x3f800000UL)                                                       \
  X(7, 0xc0200000UL)                                                       \
  X(8, 0x7f7fffffUL)                                                       \
  X(9, 0x7f800000UL)                                                       \
  X(10, 0xff800000UL)                                                      \
  X(11, 0x7fc12345UL)                                                      \
  X(12, 0x7fa00001UL)

#define ABI_VOLATILE_F64_RAW_VECTORS(X)                                    \
  X(1, 0x0000U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(2, 0x8000U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(3, 0x0000U, 0x0000U, 0x0000U, 0x0001U)                               \
  X(4, 0x000fU, 0xffffU, 0xffffU, 0xffffU)                               \
  X(5, 0x0010U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(6, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(7, 0xc004U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(8, 0x7fefU, 0xffffU, 0xffffU, 0xffffU)                               \
  X(9, 0x7ff0U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(10, 0xfff0U, 0x0000U, 0x0000U, 0x0000U)                              \
  X(11, 0x7ff8U, 0x1234U, 0x5678U, 0x9abcU)                              \
  X(12, 0x7ff0U, 0x1234U, 0x5678U, 0x9abcU)

#define ABI_VOLATILE_F32_COMPOUND_VECTORS(X)                               \
  X(1, 0x3f800000UL, 0x40000000UL, 0x40400000UL, 0x40000000UL)            \
  X(2, 0xc0200000UL, 0x3f000000UL, 0xc0000000UL, 0xbfa00000UL)            \
  X(3, 0x00000001UL, 0x00000001UL, 0x00000002UL, 0x00000000UL)            \
  X(4, 0x7f7fffffUL, 0x7f7fffffUL, 0x7f800000UL, 0x7f800000UL)            \
  X(5, 0x00000000UL, 0x80000000UL, 0x00000000UL, 0x80000000UL)            \
  X(6, 0x40000000UL, 0xc0400000UL, 0xbf800000UL, 0xc0c00000UL)

#define ABI_VOLATILE_F64_COMPOUND_VECTORS(X)                               \
  X(1, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x4000U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x4008U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x4000U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(2, 0xc004U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x3fe0U, 0x0000U, 0x0000U, 0x0000U,                               \
       0xc000U, 0x0000U, 0x0000U, 0x0000U,                               \
       0xbff4U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(3, 0x0000U, 0x0000U, 0x0000U, 0x0001U,                               \
       0x0000U, 0x0000U, 0x0000U, 0x0001U,                               \
       0x0000U, 0x0000U, 0x0000U, 0x0002U,                               \
       0x0000U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(4, 0x7fefU, 0xffffU, 0xffffU, 0xffffU,                               \
       0x7fefU, 0xffffU, 0xffffU, 0xffffU,                               \
       0x7ff0U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x7ff0U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(5, 0x0000U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x8000U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x0000U, 0x0000U, 0x0000U, 0x0000U,                               \
       0x8000U, 0x0000U, 0x0000U, 0x0000U)                               \
  X(6, 0x4000U, 0x0000U, 0x0000U, 0x0000U,                               \
       0xc008U, 0x0000U, 0x0000U, 0x0000U,                               \
       0xbff0U, 0x0000U, 0x0000U, 0x0000U,                               \
       0xc018U, 0x0000U, 0x0000U, 0x0000U)

#endif
