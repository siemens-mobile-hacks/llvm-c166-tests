#if __C166_MEMORY_MODEL__ == 3 || __C166_MEMORY_MODEL__ == 4
typedef unsigned char __attribute__((c166_near)) crt_default_u8;
typedef unsigned char __attribute__((c166_near)) crt_rom_u8;
#else
typedef unsigned char __attribute__((c166_huge)) crt_default_u8;
typedef unsigned char __attribute__((c166_huge)) crt_rom_u8;
#endif

#define DECLARE_RANGE(name, type)                                              \
  extern crt_rom_u8 name##_data_load_start[];                                 \
  extern type name##_data_start[];                                             \
  extern type name##_data_end[];                                               \
  extern type name##_bss_start[];                                              \
  extern type name##_bss_end[]

DECLARE_RANGE(c166, crt_default_u8);

#if __C166_MEMORY_MODEL__ == 3
typedef unsigned char __attribute__((c166_far)) crt_far_u8;
typedef unsigned char __attribute__((c166_huge)) crt_huge_u8;
typedef unsigned char __attribute__((c166_shuge)) crt_shuge_u8;
DECLARE_RANGE(c166_far, crt_far_u8);
DECLARE_RANGE(c166_huge, crt_huge_u8);
DECLARE_RANGE(c166_shuge, crt_shuge_u8);
#elif __C166_MEMORY_MODEL__ != 4
typedef unsigned char __attribute__((c166_near)) crt_near_u8;
typedef unsigned char __attribute__((c166_xnear)) crt_xnear_u8;
DECLARE_RANGE(c166_near, crt_near_u8);
DECLARE_RANGE(c166_xnear, crt_xnear_u8);
#endif

#define INITIALIZE_RANGE(name, type)                                           \
  do {                                                                         \
    crt_rom_u8 *source = name##_data_load_start;                               \
    type *destination = name##_data_start;                                     \
    while (destination != name##_data_end)                                     \
      *destination++ = *source++;                                              \
    destination = name##_bss_start;                                            \
    while (destination != name##_bss_end)                                      \
      *destination++ = 0;                                                      \
  } while (0)

__attribute__((weak)) void c166_test_preinit(void) {}

void c166_crt_init(void) {
  INITIALIZE_RANGE(c166, crt_default_u8);

#if __C166_MEMORY_MODEL__ == 3
  INITIALIZE_RANGE(c166_far, crt_far_u8);
  INITIALIZE_RANGE(c166_huge, crt_huge_u8);
  INITIALIZE_RANGE(c166_shuge, crt_shuge_u8);
#elif __C166_MEMORY_MODEL__ != 4
  INITIALIZE_RANGE(c166_near, crt_near_u8);
  INITIALIZE_RANGE(c166_xnear, crt_xnear_u8);
#endif
}
