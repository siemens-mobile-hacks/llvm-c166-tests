typedef unsigned char u8;
typedef unsigned short u16;

u16 size_byte_indexed_loop(const u8 *source, u8 *destination, u16 count) {
  u16 sum = 0;
  while (count != 0u) {
    u8 value = *source++;
    *destination++ = (u8)(value ^ 0x5au);
    sum = (u16)(sum + value);
    --count;
  }
  return sum;
}
