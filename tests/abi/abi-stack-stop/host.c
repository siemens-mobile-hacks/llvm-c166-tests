#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

extern unsigned long llvm_entry_proxy(unsigned int a, unsigned int b,
                                      unsigned int c, unsigned long pair,
                                      unsigned int tail);
extern unsigned long c166_test_entry(unsigned int a, unsigned int b,
                                     unsigned int c, unsigned long pair,
                                     unsigned int tail);

static void run_stop_vector(unsigned int vector_id, unsigned int a,
                            unsigned int b, unsigned int c,
                            unsigned long pair, unsigned int tail,
                            unsigned long golden) {
  unsigned long reference = c166_test_entry(a, b, c, pair, tail);
  unsigned long actual = llvm_entry_proxy(a, b, c, pair, tail);
  c166_test_check_u32(vector_id * 2 - 1, golden, reference);
  c166_test_check_u32(vector_id * 2, golden, actual);
}

#define RUN_STOP(id, a, b, c, pair, tail, golden) \
  run_stop_vector(id, a, b, c, pair, tail, golden);

void main(void) {
  c166_test_begin(258, 0x16600102UL);
  ABI_STOP_VECTORS(RUN_STOP)
  c166_test_finish();
  simulator_stop();
}
