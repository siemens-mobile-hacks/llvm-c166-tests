#include "record.h"

static const struct record records[] = {
  {0,0,""}, {0x1234,0x89abcdefUL,"abc"}, {0xffff,0xffffffffUL,"12345678"}
};
static const unsigned int sizes[] = {10,13,18};
/* Fixed wire fixtures, not bytes produced by record_encode. */
static const unsigned char wire[3][18] = {
  {0x52,1,0,0,0,0,0,0,0,0x53},
  {0x52,1,3,0x34,0x12,0x89,0xab,0xcd,0xef,0x61,0x62,0x63,0x16},
  {0x52,1,8,0xff,0xff,0xff,0xff,0xff,0xff,
   0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x53}
};
static const char saved_name[9] = "keep";

static void initialize(struct record *value) {
  value->id = 0x7654;
  value->counter = 0x01234567UL;
  memcpy(value->name, saved_name, sizeof(saved_name));
}

static int unchanged(const struct record *value) {
  unsigned int i;
  if (value->id != 0x7654 || value->counter != 0x01234567UL) return 0;
  for (i = 0; i != sizeof(saved_name); ++i)
    if (value->name[i] != saved_name[i]) return 0;
  return 1;
}

unsigned long c166_test_case(unsigned int case_id) {
  unsigned char buffer[24];
  unsigned int offset = 1U + (case_id & 1U);
  unsigned char *bytes = buffer + offset;
  struct {
    unsigned int before;
    struct record value;
    unsigned int after;
  } guarded;
  unsigned int index = (case_id % 6U) / 2U;
  unsigned int size = sizes[index], i, limit, variant, checksum, written;
  unsigned long failure = 0;

  guarded.before = 0xa55a;
  guarded.after = 0x5aa5;
  initialize(&guarded.value);
  for (i = 0; i != sizeof(buffer); ++i) buffer[i] = 0xcc;
  if (case_id < 6) {
    written = record_encode(bytes, size, &records[index]);
    if (written != size) failure |= 2UL;
    for (i = 0; i != size; ++i)
      if (bytes[i] != wire[index][i]) failure |= 4UL;
    for (i = 0; i != sizeof(buffer); ++i)
      if ((i < offset || i >= offset + size) && buffer[i] != 0xcc) failure |= 8UL;
  } else if (case_id < 12) {
    for (limit = 0; limit != size; ++limit) {
      if (record_encode(bytes, limit, &records[index])) failure |= 16UL;
      for (i = 0; i != sizeof(buffer); ++i)
        if (buffer[i] != 0xcc) failure |= 32UL;
    }
  } else if (case_id < 18) {
    memcpy(bytes, wire[index], size);
    if (!record_decode(&guarded.value, bytes, size) ||
        guarded.value.id != records[index].id ||
        guarded.value.counter != records[index].counter ||
        strcmp(guarded.value.name, records[index].name)) failure |= 64UL;
    for (i = 0; i != size; ++i)
      if (bytes[i] != wire[index][i]) failure |= 128UL;
  } else if (case_id < 24) {
    memcpy(bytes, wire[index], size);
    for (limit = 0; limit != size; ++limit)
      if (record_decode(&guarded.value, bytes, limit) || !unchanged(&guarded.value))
        failure |= 256UL;
  } else if (case_id < 26) {
    /* Malformed fields keep a valid checksum where possible, so checksum
     * rejection alone cannot satisfy these cases. */
    for (variant = 0; variant != 6; ++variant) {
      memcpy(bytes, wire[1], 13);
      if (variant == 0) bytes[0] = 'X';
      if (variant == 1) bytes[1] = 2;
      if (variant == 2) bytes[2] = 9;
      if (variant == 3) bytes[12] ^= 1;
      if (variant == 4) bytes[10] = 0;
      if (variant != 3) {
        checksum = 0;
        for (i = 0; i != 12; ++i) checksum ^= bytes[i];
        bytes[12] = (unsigned char)checksum;
      }
      if (record_decode(&guarded.value, bytes, variant == 5 ? 14 : 13) ||
          !unchanged(&guarded.value)) failure |= 512UL;
    }
  } else {
    /* No terminating NUL within the source field: reject without a write. */
    for (i = 0; i != sizeof(guarded.value.name); ++i) guarded.value.name[i] = 'x';
    if (record_encode(bytes, 20, &guarded.value)) failure |= 1024UL;
    for (i = 0; i != sizeof(buffer); ++i)
      if (buffer[i] != 0xcc) failure |= 2048UL;
  }
  if (guarded.before != 0xa55a || guarded.after != 0x5aa5) failure |= 4096UL;
  return 1UL | failure;
}
