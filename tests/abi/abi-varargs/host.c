#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

extern unsigned long llvm_entry_proxy(unsigned int fixed0,
                                      unsigned int fixed1, ...);
extern unsigned long tasking_varargs_reference(unsigned int fixed0,
                                               unsigned int fixed1, ...);

volatile unsigned int cells[4];

static void run_varargs_vector(unsigned int vector_id, unsigned int fixed0,
                               unsigned int fixed1, signed char signed_value,
                               unsigned int unsigned_value,
                               unsigned long long_value,
                               unsigned int pointed_value,
                               unsigned long golden) {
  unsigned int index = vector_id - 1;
  unsigned long reference;
  unsigned long actual;

  cells[index] = pointed_value;
  reference = tasking_varargs_reference(
      fixed0, fixed1, signed_value, unsigned_value, long_value, &cells[index]);
  actual = llvm_entry_proxy(fixed0, fixed1, signed_value, unsigned_value,
                            long_value, &cells[index]);
  c166_test_check_u32(vector_id * 2 - 1, golden, reference);
  c166_test_check_u32(vector_id * 2, golden, actual);
}

#define RUN_VARARGS(id, fixed0, fixed1, signed_value, unsigned_value, \
                    long_value, pointed_value, golden) \
  run_varargs_vector(id, fixed0, fixed1, signed_value, unsigned_value, \
                     long_value, pointed_value, golden);

void main(void) {
  c166_test_begin(263, 0x16600107UL);
  ABI_VARARGS_VECTORS(RUN_VARARGS)
  c166_test_finish();
  simulator_stop();
}

