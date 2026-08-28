typedef unsigned int u16;
typedef signed long long sll;
typedef unsigned long long ull;

struct long_long_record {
  u16 tag;
  ull value;
};

volatile ull tasking_long_long_global;
volatile sll tasking_signed_long_long_global;

ull tasking_long_long_mix(u16 prefix, ull value, u16 tail)
{
  volatile struct long_long_record record;
  record.tag = prefix;
  record.value = value;
  tasking_long_long_global = record.value;
  value = tasking_long_long_global;
  if (record.tag != prefix)
    return 0;
  tasking_signed_long_long_global = (sll)value;
  if ((u16)(tasking_signed_long_long_global < 0) != (u16)(value >> 31))
    return 0;
  value += (ull)prefix << 16;
  value ^= ((ull)tail << 1) | prefix;
  return value;
}

#pragma fragment

struct long_long_record
tasking_long_long_record_mix(struct long_long_record input, u16 tail)
{
  input.tag ^= tail;
  input.value += ((ull)tail << 16) | input.tag;
  return input;
}
