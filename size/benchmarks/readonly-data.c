typedef unsigned short u16;

extern u16 size_consume(const char *);

u16 size_readonly_data(void) {
  static const char message[] = "C166 code size";
  return size_consume(message);
}
