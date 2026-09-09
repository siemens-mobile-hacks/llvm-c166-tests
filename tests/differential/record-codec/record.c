#include "record.h"

/* Wire: 'R', version1, name length, little-endian id, big-endian counter,
 * name bytes without NUL, XOR of all preceding bytes. No alignment required. */
unsigned int record_encode(unsigned char *out, unsigned int capacity,
                           const struct record *value) {
  unsigned int n = 0, i, checksum = 0;
  while (n != sizeof(value->name) && value->name[n]) ++n;
  if (n == sizeof(value->name) || capacity < 10U + n) return 0;
  out[0] = 'R';
  out[1] = 1;
  out[2] = (unsigned char)n;
  out[3] = (unsigned char)value->id;
  out[4] = (unsigned char)(value->id >> 8);
  out[5] = (unsigned char)(value->counter >> 24);
  out[6] = (unsigned char)(value->counter >> 16);
  out[7] = (unsigned char)(value->counter >> 8);
  out[8] = (unsigned char)value->counter;
  memcpy(out + 9, value->name, n);
  for (i = 0; i != 9U + n; ++i) checksum ^= out[i];
  out[9U + n] = (unsigned char)checksum;
  return 10U + n;
}

int record_decode(struct record *out, const unsigned char *bytes,
                  unsigned int size) {
  struct record value;
  unsigned int n, i, checksum = 0;
  if (size < 10 || bytes[0] != 'R' || bytes[1] != 1) return 0;
  n = bytes[2];
  if (n > 8 || size != 10U + n) return 0;
  for (i = 0; i != size; ++i) checksum ^= bytes[i];
  if (checksum) return 0;
  for (i = 0; i != n; ++i) if (!bytes[9U + i]) return 0;
  value.id = bytes[3] | ((unsigned int)bytes[4] << 8);
  value.counter = ((unsigned long)bytes[5] << 24) |
                  ((unsigned long)bytes[6] << 16) |
                  ((unsigned long)bytes[7] << 8) | bytes[8];
  memcpy(value.name, bytes + 9, n);
  for (i = n; i != sizeof(value.name); ++i) value.name[i] = 0;
  *out = value;
  return 1;
}
