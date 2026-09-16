#include "c166_test.h"

typedef unsigned char u8;

volatile u8 C166_NEAR near_source[8];
volatile u8 C166_NEAR near_destination[8];
#if __C166_MEMORY_MODEL__ != 4
volatile u8 C166_FAR far_source[8];
volatile u8 C166_FAR far_destination[8];
#endif

extern void helper_load_store_near(void);
#if __C166_MEMORY_MODEL__ != 4
extern void helper_load_store_far(void);
#endif
extern void helper_ldnoal_near(void);
#if __C166_MEMORY_MODEL__ != 4
extern void helper_ldnoal_far(void);
#endif
extern void helper_cpnnb(void);
extern void helper_cpnnw(void);
#if __C166_MEMORY_MODEL__ != 4
extern void helper_cpnfb(void);
extern void helper_cpnfw(void);
extern void helper_cpfnb(void);
extern void helper_cpfnw(void);
#endif

#define PREPARE(source, destination, seed)                                    \
  do {                                                                        \
    unsigned int index;                                                       \
    for (index = 0; index != 8U; ++index) {                                   \
      (source)[index] = (u8)((seed) + (u8)(index * 17U));                     \
      (destination)[index] = 0U;                                              \
    }                                                                         \
  } while (0)

#define CHECK(destination, seed, name)                                        \
  do {                                                                        \
    unsigned int index;                                                       \
    for (index = 0; index != 8U; ++index)                                    \
      tap_is_u32((destination)[index],                                        \
                 (u8)((seed) + (u8)(index * 17U)), name);                    \
  } while (0)

void main(void) {
#if __C166_MEMORY_MODEL__ == 4
  tap_plan(32);
#else
  tap_plan(80);
#endif

  PREPARE(near_source, near_destination, 0x11U);
  helper_load_store_near();
  CHECK(near_destination, 0x11U, "near load/store helper");

#if __C166_MEMORY_MODEL__ != 4
  PREPARE(far_source, far_destination, 0x22U);
  helper_load_store_far();
  CHECK(far_destination, 0x22U, "far load/store helper");
#endif

  PREPARE(near_source, near_destination, 0x33U);
  helper_ldnoal_near();
  CHECK(near_destination, 0x33U, "near unaligned-load helper");

#if __C166_MEMORY_MODEL__ != 4
  PREPARE(far_source, near_destination, 0x44U);
  helper_ldnoal_far();
  CHECK(near_destination, 0x44U, "far unaligned-load helper");
#endif

  PREPARE(near_source, near_destination, 0x55U);
  helper_cpnnb();
  CHECK(near_destination, 0x55U, "near-to-near byte copy helper");

  PREPARE(near_source, near_destination, 0x66U);
  helper_cpnnw();
  CHECK(near_destination, 0x66U, "near-to-near word copy helper");

#if __C166_MEMORY_MODEL__ != 4
  PREPARE(near_source, far_destination, 0x77U);
  helper_cpnfb();
  CHECK(far_destination, 0x77U, "near-to-far byte copy helper");

  PREPARE(near_source, far_destination, 0x88U);
  helper_cpnfw();
  CHECK(far_destination, 0x88U, "near-to-far word copy helper");

  PREPARE(far_source, near_destination, 0x99U);
  helper_cpfnb();
  CHECK(near_destination, 0x99U, "far-to-near byte copy helper");

  PREPARE(far_source, near_destination, 0xaaU);
  helper_cpfnw();
  CHECK(near_destination, 0xaaU, "far-to-near word copy helper");
#endif
}
