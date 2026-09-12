typedef unsigned int u16;
typedef signed long s32;
typedef unsigned long u32;

struct long_record {
  u16 tag;
  u32 value;
};

volatile u32 llvm_long_global;
volatile s32 llvm_signed_long_global;

extern u32 tasking_long_mix(u16 prefix, u32 value, u16 tail);
extern struct long_record
tasking_long_record_mix(struct long_record input, u16 tail);

__attribute__((noinline))
u32 llvm_entry(u16 prefix, u32 value, u16 tail)
{
  volatile struct long_record record;
  record.tag = prefix;
  record.value = value;
  llvm_long_global = record.value;
  value = llvm_long_global;
  if (record.tag != prefix)
    return 0;
  llvm_signed_long_global = (s32)value;
  if ((u16)(llvm_signed_long_global < 0) != (u16)(value >> 31))
    return 0;
  return tasking_long_mix(prefix, value, tail) ^ 0x13579bdfUL;
}

__attribute__((noinline, section(".llvm_long_record")))
struct long_record
llvm_long_record_entry(struct long_record input, u16 tail)
{
  return tasking_long_record_mix(input, tail);
}
