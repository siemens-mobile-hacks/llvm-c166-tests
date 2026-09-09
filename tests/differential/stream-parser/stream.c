#include "stream.h"

void stream_reset(struct stream *s, unsigned int position) {
  unsigned int i;
  s->head = s->tail = position;
  s->queued = s->dropped = 0;
  s->state = s->length = s->received = s->checksum = 0;
  s->frames = s->payload_bytes = s->bad_length = s->bad_checksum = 0;
  s->pending_sum = s->payload_sum = 0;
  for (i = 0; i != QUEUE_SIZE; ++i) s->queue[i] = 0;
  for (i = 0; i != PAYLOAD_SIZE; ++i) s->payload[i] = 0;
}

int stream_push(struct stream *s, unsigned char value) {
  if (s->queued == QUEUE_SIZE) {
    ++s->dropped;
    return 0;
  }
  s->queue[s->head] = value;
  if (++s->head == QUEUE_SIZE) s->head = 0;
  ++s->queued;
  return 1;
}

/* Frame: A5, length (0..8), payload, XOR of length and payload bytes.
 * Only a valid checksum commits the frame's counters and payload sum. */
static void consume(struct stream *s, unsigned char value) {
  switch (s->state) {
  case 0:
    if (value == 0xa5) s->state = 1;
    break;
  case 1:
    if (value > PAYLOAD_SIZE) {
      ++s->bad_length;
      s->state = 0;
      break;
    }
    s->length = value;
    s->checksum = value;
    s->received = 0;
    s->pending_sum = 0;
    s->state = value ? 2 : 3;
    break;
  case 2:
    s->payload[s->received++] = value;
    s->checksum ^= value;
    s->pending_sum += value;
    if (s->received == s->length) s->state = 3;
    break;
  case 3:
    if (value == s->checksum) {
      ++s->frames;
      s->payload_bytes += s->length;
      s->payload_sum += s->pending_sum;
    } else {
      ++s->bad_checksum;
    }
    s->state = 0;
    break;
  }
}

void stream_drain(struct stream *s) {
  while (s->queued) {
    unsigned char value = s->queue[s->tail];
    if (++s->tail == QUEUE_SIZE) s->tail = 0;
    --s->queued;
    consume(s, value);
  }
}
