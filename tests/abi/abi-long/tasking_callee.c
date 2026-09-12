typedef unsigned int u16;
typedef signed long s32;
typedef unsigned long u32;

struct long_record {
  u16 tag;
  u32 value;
};

volatile u32 tasking_long_global;
volatile s32 tasking_signed_long_global;

u32 tasking_long_mix(u16 prefix, u32 value, u16 tail)
{
  volatile struct long_record record;
  record.tag = prefix;
  record.value = value;
  tasking_long_global = record.value;
  value = tasking_long_global;
  if (record.tag != prefix)
    return 0;
  tasking_signed_long_global = (s32)value;
  if ((u16)(tasking_signed_long_global < 0) != (u16)(value >> 31))
    return 0;
  value += (u32)prefix << 16;
  value ^= ((u32)tail << 1) | prefix;
  return value;
}

#pragma fragment

struct long_record
tasking_long_record_mix(struct long_record input, u16 tail)
{
  input.tag ^= tail;
  input.value += ((u32)tail << 16) | input.tag;
  return input;
}
