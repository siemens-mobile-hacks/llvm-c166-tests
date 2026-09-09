#include "stream.h"

static const unsigned char wire[] = {
  0, 0x55,
  0xa5, 0, 0,
  0xa5, 3, 1, 2, 3, 3,
  0xa5, 9, 0x10, 0x20,
  0xa5, 2, 0xa5, 0, 0xa7,
  0xa5, 1, 7, 0,
  0xa5, 8, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x88
};
static const unsigned int chunks[] = {1, 2, 3, 5, 8, 13, 16, 17};

unsigned long c166_test_case(unsigned int case_id) {
  struct {
    unsigned int before;
    struct stream s;
    unsigned int after;
  } guarded;
  struct stream *s = &guarded.s;
  unsigned int position = (case_id & 1U) ? 16 : 0;
  unsigned int i, chunk, pending = 0;
  unsigned long failure = 0;

  guarded.before = 0xa55a;
  guarded.after = 0x5aa5;
  stream_reset(s, position);
  if (case_id < 16) {
    chunk = chunks[case_id / 2];
    for (i = 0; i != sizeof(wire); ++i) {
      if (!stream_push(s, wire[i])) failure |= 2UL;
      if (++pending == chunk) {
        stream_drain(s);
        pending = 0;
      }
    }
    stream_drain(s);
    /* Four good frames: lengths0+3+2+8, sum6+165+576=747.
     * The malformed length and bad checksum each occur exactly once. */
    if (s->frames != 4 || s->payload_bytes != 13 || s->payload_sum != 747UL)
      failure |= 4UL;
    if (s->bad_length != 1 || s->bad_checksum != 1 || s->dropped != 0)
      failure |= 8UL;
    for (i = 0; i != PAYLOAD_SIZE; ++i)
      if (s->payload[i] != (i + 1U) * 16U) failure |= 16UL;
  } else if (case_id < 18) {
    for (i = 0; i != QUEUE_SIZE; ++i)
      if (!stream_push(s, 0x55)) failure |= 2UL;
    if (stream_push(s, 0xa5) || s->queued != QUEUE_SIZE || s->dropped != 1)
      failure |= 32UL;
    stream_drain(s);
    stream_push(s, 0xa5);
    stream_push(s, 1);
    stream_push(s, 42);
    stream_push(s, 43);
    stream_drain(s);
    if (s->frames != 1 || s->payload_bytes != 1 || s->payload_sum != 42UL ||
        s->bad_length || s->bad_checksum || s->dropped != 1)
      failure |= 64UL;
  } else {
    stream_push(s, 0xa5);
    stream_push(s, 3);
    stream_push(s, 1);
    stream_drain(s);
    if (s->frames || s->state != 2 || s->received != 1 || s->payload_sum)
      failure |= 128UL;
    stream_push(s, 2);
    stream_push(s, 3);
    stream_push(s, 3);
    stream_drain(s);
    if (s->frames != 1 || s->payload_bytes != 3 || s->payload_sum != 6UL ||
        s->bad_length || s->bad_checksum || s->dropped)
      failure |= 256UL;
  }
  if (guarded.before != 0xa55a || guarded.after != 0x5aa5)
    failure |= 512UL;
  if (s->state || s->queued || s->head != s->tail || s->head >= QUEUE_SIZE)
    failure |= 1024UL;
  return 1UL | failure;
}
