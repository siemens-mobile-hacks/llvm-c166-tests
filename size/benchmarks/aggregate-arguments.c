typedef unsigned short u16;
typedef unsigned long u32;

typedef struct size_pair {
  u16 low;
  u16 high;
} size_pair;

extern u32 size_consume_pair(size_pair, u16, u16, u16);

u32 size_aggregate_arguments(size_pair pair, u16 a, u16 b, u16 c) {
  pair.low = (u16)(pair.low + a);
  pair.high = (u16)(pair.high ^ b);
  return size_consume_pair(pair, a, b, c);
}
