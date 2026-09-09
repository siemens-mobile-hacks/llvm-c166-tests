#ifndef TEST_RECORD_H
#define TEST_RECORD_H

struct record {
  unsigned int id;
  unsigned long counter;
  char name[9];
};

unsigned int record_encode(unsigned char *, unsigned int, const struct record *);
int record_decode(struct record *, const unsigned char *, unsigned int);
extern void *memcpy(void *, const void *, unsigned int);
extern int strcmp(const char *, const char *);

#endif
