#include "c166-test-result.h"
#include "vectors.inc"

typedef unsigned long (*classic_function)(unsigned int a, unsigned int b,
                                          unsigned int c, unsigned long pair,
                                          unsigned int tail);

extern unsigned long llvm_entry_proxy(unsigned int prefix,
                                      classic_function target,
                                      unsigned int a, unsigned int b,
                                      unsigned int c, unsigned long pair,
                                      unsigned int tail);
extern unsigned long tasking_indirect_target(unsigned int a, unsigned int b,
                                             unsigned int c,
                                             unsigned long pair,
                                             unsigned int tail);

void simulator_stop(void) {
  for (;;)
    ;
}

static void run_indirect_vector(unsigned int vector_id, unsigned int prefix,
                                unsigned int a, unsigned int b,
                                unsigned int c, unsigned long pair,
                                unsigned int tail, unsigned long golden) {
  unsigned long reference = tasking_indirect_target(a, b, c, pair, tail);
  unsigned long actual = llvm_entry_proxy(prefix, tasking_indirect_target,
                                          a, b, c, pair, tail);
  c166_test_check_u32(vector_id * 2 - 1, golden, reference);
  c166_test_check_u32(vector_id * 2, golden ^ prefix, actual);
}

#define RUN_INDIRECT(id, prefix, a, b, c, pair, tail, golden) \
  run_indirect_vector(id, prefix, a, b, c, pair, tail, golden);

void main(void) {
  c166_test_begin(262, 0x16600106UL);
  ABI_INDIRECT_VECTORS(RUN_INDIRECT)
  c166_test_finish();
  simulator_stop();
}

