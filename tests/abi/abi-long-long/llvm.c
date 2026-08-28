typedef unsigned int u16;
typedef signed long long sll;
typedef unsigned long long ull;

struct long_long_record {
  u16 tag;
  ull value;
};

volatile ull llvm_long_long_global;
volatile sll llvm_signed_long_long_global;

extern ull tasking_long_long_mix(u16 prefix, ull value, u16 tail);
extern struct long_long_record
tasking_long_long_record_mix(struct long_long_record input, u16 tail);

__attribute__((noinline))
ull llvm_entry(u16 prefix, ull value, u16 tail)
{
  volatile struct long_long_record record;
  record.tag = prefix;
  record.value = value;
  llvm_long_long_global = record.value;
  value = llvm_long_long_global;
  if (record.tag != prefix)
    return 0;
  llvm_signed_long_long_global = (sll)value;
  if ((u16)(llvm_signed_long_long_global < 0) != (u16)(value >> 31))
    return 0;
  return tasking_long_long_mix(prefix, value, tail) ^ 0x13579bdfULL;
}

__attribute__((noinline, section(".llvm_long_long_record")))
struct long_long_record
llvm_long_long_record_entry(struct long_long_record input, u16 tail)
{
  return tasking_long_long_record_mix(input, tail);
}
