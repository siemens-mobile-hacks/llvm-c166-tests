#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

extern unsigned long llvm_entry_proxy(unsigned int a, unsigned int b,
                                      unsigned int c, unsigned long pair,
                                      unsigned int tail);
extern unsigned long tasking_stop(unsigned int a, unsigned int b,
                                  unsigned int c, unsigned long pair,
                                  unsigned int tail);

static void run_reverse_vector(unsigned int vector_id, unsigned int a,
                               unsigned int b, unsigned int c,
                               unsigned long pair, unsigned int tail,
                               unsigned long golden) {
  unsigned long reference = tasking_stop(a, b, c, pair, tail);
  unsigned long actual = llvm_entry_proxy(a, b, c, pair, tail);
  c166_test_check_u32(vector_id * 2 - 1, golden, reference);
  c166_test_check_u32(vector_id * 2, golden ^ 0x13579bdfUL, actual);
}

#define RUN_REVERSE(id, a, b, c, pair, tail, golden) \
  run_reverse_vector(id, a, b, c, pair, tail, golden);

void main(void) {
  c166_test_begin(260, 0x16600104UL);
  ABI_REVERSE_STOP_VECTORS(RUN_REVERSE)
  c166_test_finish();
  simulator_stop();
}

