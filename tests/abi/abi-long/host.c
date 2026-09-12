#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

typedef unsigned int u16;
typedef unsigned long u32;

struct long_record {
  u16 tag;
  u32 value;
};

extern u32 llvm_entry_proxy(u16 prefix, u32 value, u16 tail);
extern u32 tasking_long_mix(u16 prefix, u32 value, u16 tail);
extern struct long_record
llvm_long_record_proxy(struct long_record input, u16 tail);
extern struct long_record
tasking_long_record_mix(struct long_record input, u16 tail);

static void run_vector(u16 vector_id, u16 prefix, u32 value, u16 tail,
                       unsigned long golden)
{
  u32 reference = tasking_long_mix(prefix, value, tail);
  u32 actual = llvm_entry_proxy(prefix, value, tail);
  c166_test_check_u32(vector_id * 2 - 1, golden, (unsigned long)reference);
  c166_test_check_u32(vector_id * 2, golden ^ 0x13579bdfUL,
                      (unsigned long)actual);
}

#define RUN_VECTOR(id, prefix, value, tail, golden, record_tag, record_value) \
  run_vector(id, prefix, (u32)value, tail, golden);

static void run_record_vector(u16 vector_id, u16 prefix, u32 value, u16 tail,
                              u16 golden_tag, unsigned long golden_value)
{
  struct long_record input;
  struct long_record reference;
  struct long_record actual;
  u16 base = 100 + (vector_id - 1) * 4;

  input.tag = prefix;
  input.value = value;
  reference = tasking_long_record_mix(input, tail);
  actual = llvm_long_record_proxy(input, tail);
  c166_test_check_u32(base + 1, golden_tag, reference.tag);
  c166_test_check_u32(base + 2, golden_value,
                      (unsigned long)reference.value);
  c166_test_check_u32(base + 3, golden_tag, actual.tag);
  c166_test_check_u32(base + 4, golden_value, (unsigned long)actual.value);
}

#define RUN_RECORD(id, prefix, value, tail, golden, record_tag, record_value) \
  run_record_vector(id, prefix, (u32)value, tail, record_tag, record_value);

void main(void)
{
  c166_test_begin(306, 0x16600132UL);
  ABI_LONG_VECTORS(RUN_VECTOR)
  ABI_LONG_VECTORS(RUN_RECORD)
  c166_test_finish();
  simulator_stop();
}

