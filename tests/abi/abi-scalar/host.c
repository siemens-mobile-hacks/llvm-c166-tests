#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

extern unsigned long llvm_entry_proxy(unsigned int head, unsigned long pair,
                                      signed char register_tail,
                                      unsigned int stack0,
                                      unsigned long stack1,
                                      unsigned int stack2);
extern unsigned long tasking_scalar_reference(
    unsigned int head, unsigned long pair, signed char register_tail,
    unsigned int stack0, unsigned long stack1, unsigned int stack2);

static void run_scalar_vector(unsigned int vector_id, unsigned int head,
                              unsigned long pair, signed char register_tail,
                              unsigned int stack0, unsigned long stack1,
                              unsigned int stack2, unsigned long golden) {
  unsigned long reference = tasking_scalar_reference(
      head, pair, register_tail, stack0, stack1, stack2);
  unsigned long actual = llvm_entry_proxy(head, pair, register_tail, stack0,
                                          stack1, stack2);
  c166_test_check_u32(vector_id * 2 - 1, golden, reference);
  c166_test_check_u32(vector_id * 2, golden, actual);
}

#define RUN_SCALAR(id, head, pair, reg_tail, stack0, stack1, stack2, golden) \
  run_scalar_vector(id, head, pair, reg_tail, stack0, stack1, stack2, golden);

void main(void) {
  c166_test_begin(257, 0x16600101UL);
  ABI_SCALAR_VECTORS(RUN_SCALAR)
  c166_test_finish();
  simulator_stop();
}

