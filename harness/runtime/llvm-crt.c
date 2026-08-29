typedef unsigned char __attribute__((c166_huge)) c166_crt_byte;

struct c166_zero_range {
  c166_crt_byte *begin;
  c166_crt_byte *end;
};

#define DECLARE_RANGE(name)                                                    \
  extern c166_crt_byte c166_##name##_bss_start[];                              \
  extern c166_crt_byte c166_##name##_bss_end[]

DECLARE_RANGE(near);
DECLARE_RANGE(xnear);
DECLARE_RANGE(small);
DECLARE_RANGE(far);
DECLARE_RANGE(huge);
DECLARE_RANGE(shuge);
DECLARE_RANGE(default);

#define ZERO_RANGE(name)                                                       \
  { c166_##name##_bss_start, c166_##name##_bss_end }

static const struct c166_zero_range zero_ranges[] = {
    ZERO_RANGE(near),  ZERO_RANGE(xnear), ZERO_RANGE(small),
    ZERO_RANGE(far),   ZERO_RANGE(huge),  ZERO_RANGE(shuge),
    ZERO_RANGE(default),
};

void c166_crt_init(void) {
  unsigned int range_index;

  for (range_index = 0;
       range_index != sizeof(zero_ranges) / sizeof(zero_ranges[0]);
       ++range_index) {
    c166_crt_byte *cursor = zero_ranges[range_index].begin;
    c166_crt_byte *end = zero_ranges[range_index].end;

    while (cursor != end)
      *cursor++ = 0;
  }
}
