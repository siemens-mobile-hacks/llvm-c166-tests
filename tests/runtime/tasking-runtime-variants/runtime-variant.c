typedef unsigned int probe_u16;
typedef unsigned long probe_u32;
typedef signed long probe_i32;

static volatile probe_u32 probe_unsigned_a = 0xfedcba98UL;
static volatile probe_u32 probe_unsigned_b = 0x1234UL;
static volatile probe_i32 probe_signed_a = -123456L;
static volatile probe_i32 probe_signed_b = 321L;
static volatile float probe_float_a = 1.5F;
static volatile float probe_float_b = 2.25F;
static volatile double probe_double_a = 5.0;
static volatile double probe_double_b = 2.0;

extern void *memcpy(void *, const void *, probe_u16);
extern int memcmp(const void *, const void *, probe_u16);

static probe_u16 probe_copy_check(void) {
  static const unsigned char source[6] = {1U, 3U, 5U, 7U, 9U, 11U};
  static unsigned char destination[6];

  memcpy(destination, source, sizeof(source));
  return (probe_u16)(memcmp(destination, source, sizeof(source)) == 0);
}

probe_u16 tasking_runtime_variant_probe(void) {
  probe_u16 result = 0U;
  probe_u32 quotient = probe_unsigned_a / probe_unsigned_b;
  probe_u32 remainder = probe_unsigned_a % probe_unsigned_b;
  probe_i32 product = probe_signed_a * probe_signed_b;
  float float_product = probe_float_a * probe_float_b;
  double double_quotient = probe_double_a / probe_double_b;

  if (quotient == 0x000e0042UL)
    result |= 0x0001U;
  if (remainder == 0x00000930UL)
    result |= 0x0002U;
  if (product == -39629376L)
    result |= 0x0004U;
  if (float_product == 3.375F)
    result |= 0x0008U;
  if (double_quotient == 2.5)
    result |= 0x0010U;
  if (probe_copy_check())
    result |= 0x0020U;
  return result;
}
