extern void size_observe4(char *, char *, char *, char *);
extern void size_barrier(void);
extern unsigned int size_consume4(char *, char *, char *, char *);

unsigned int size_frame_address_pressure(void) {
  char a[2];
  char b[2];
  char c[2];
  char d[2];
  size_observe4(a, b, c, d);
  size_barrier();
  return size_consume4(a, b, c, d);
}
