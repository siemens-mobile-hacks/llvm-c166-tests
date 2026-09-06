typedef unsigned short u16;

typedef struct size_flags {
  unsigned mode : 3;
  unsigned ready : 1;
  unsigned count : 6;
  unsigned error : 1;
  unsigned reserved : 5;
} size_flags;

u16 size_bitfields(size_flags *flags, u16 value) {
  u16 result = (u16)(flags->mode + flags->count);
  flags->count = (unsigned)((flags->count + value) & 0x3fu);
  flags->ready = (unsigned)(flags->error == 0u);
  return result;
}
