#define ABI_FAR_MEMORY_FORWARD_OFFSETS(X) \
  X(1, 0U) \
  X(2, 1U) \
  X(3, 44U) \
  X(4, 46U) \
  X(5, 47U) \
  X(6, 48U)
#define ABI_FAR_MEMORY_RETREAT_OFFSETS(X) \
  X(1, 0U) \
  X(2, 1U) \
  X(3, 2U) \
  X(4, 4U) \
  X(5, 48U)
#define ABI_FAR_MEMORY_FORWARD_CASES 18U
#define ABI_FAR_MEMORY_CASES_PER_PAGE 33U
#define ABI_FAR_MEMORY_BOUNDARY_CASES 66U
#define ABI_FAR_MEMORY_VECTORS(X) \
  X(1, 0x0000U, 0x9b6def21UL) \
  X(2, 0x1234U, 0xafa2aa41UL) \
  X(3, 0x8000U, 0x9b6c6f21UL) \
  X(4, 0xffffU, 0x666ac23fUL)
#define ABI_FAR_MEMORY_OFFSETS(X) \
  X(1, 0U) \
  X(2, 1U) \
  X(3, 4U) \
  X(4, 6U) \
  X(5, 12U) \
  X(6, 43U) \
  X(7, 44U) \
  X(8, 47U)
#define ABI_FAR_MEMORY_CASES_PER_VECTOR 55U
#define ABI_FAR_MEMORY_SIGNATURE 0x0c3bec48UL
