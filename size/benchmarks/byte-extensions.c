typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned short u16;

s16 size_signed_byte_load(const s8 *source, s16 bias) {
  return (s16)(*source + bias);
}

u16 size_unsigned_byte_load(const u8 *source, u16 mask) {
  return (u16)(*source & mask);
}
