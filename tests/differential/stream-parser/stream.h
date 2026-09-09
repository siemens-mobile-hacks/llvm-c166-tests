#ifndef TEST_STREAM_H
#define TEST_STREAM_H

#define QUEUE_SIZE 17
#define PAYLOAD_SIZE 8

struct stream {
  unsigned char queue[QUEUE_SIZE];
  unsigned int head, tail, queued, dropped;
  unsigned int state, length, received, checksum;
  unsigned int frames, payload_bytes, bad_length, bad_checksum;
  unsigned long pending_sum, payload_sum;
  unsigned char payload[PAYLOAD_SIZE];
};

void stream_reset(struct stream *s, unsigned int position);
int stream_push(struct stream *s, unsigned char value);
void stream_drain(struct stream *s);

#endif
