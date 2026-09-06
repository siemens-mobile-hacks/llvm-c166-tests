typedef unsigned short u16;
typedef unsigned long u32;

u32 size_long_indexed_loop(const u32 *source, u16 count) {
  u32 sum = 0;
  while (count != 0u) {
    sum += *source++;
    --count;
  }
  return sum;
}
