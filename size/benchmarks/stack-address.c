typedef unsigned short u16;

extern char *size_copy(char *, const char *);
extern u16 size_consume(const char *);

u16 size_stack_address(const char *source, u16 offset) {
  char buffer[128];
  size_copy(buffer, source);
  size_copy(buffer + offset, source);
  return size_consume(buffer);
}
