typedef unsigned short u16;

typedef struct size_triple {
  u16 first;
  u16 second;
  u16 third;
} size_triple;

size_triple size_aggregate_return(u16 a, u16 b, u16 c) {
  size_triple result;
  result.first = (u16)(a + b);
  result.second = (u16)(b ^ c);
  result.third = (u16)(c - a);
  return result;
}
