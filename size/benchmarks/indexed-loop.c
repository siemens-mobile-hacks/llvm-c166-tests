typedef unsigned short u16;

u16 size_indexed_loop(const u16 *source, u16 *destination, u16 count) {
  u16 sum = 0;
  while (count != 0) {
    u16 value = *source++;
    *destination++ = (u16)(value + 3u);
    sum = (u16)(sum + value);
    --count;
  }
  return sum;
}
