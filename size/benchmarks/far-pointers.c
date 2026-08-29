typedef unsigned short u16;

extern u16 size_consume(const char *);

u16 size_far_pointers(const char *base, u16 offset) {
  const char *selected = base != 0 ? base + offset : 0;
  if (selected == 0)
    return 0;
  return size_consume(selected);
}
