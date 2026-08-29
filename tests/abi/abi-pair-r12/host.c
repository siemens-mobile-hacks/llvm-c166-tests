#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

extern unsigned int llvm_entry_proxy(unsigned long pair,
                                     unsigned int word14,
                                     signed char byte15,
                                     unsigned int stack0,
                                     unsigned long stack1);
extern unsigned int c166_test_entry(
    unsigned long pair, unsigned int word14, signed char byte15,
    unsigned int stack0, unsigned long stack1);

static void run_pair_r12_vector(unsigned int vector_id, unsigned long pair,
                                unsigned int word14, signed char byte15,
                                unsigned int stack0, unsigned long stack1,
                                unsigned long golden) {
  unsigned int reference = c166_test_entry(
      pair, word14, byte15, stack0, stack1);
  unsigned int actual = llvm_entry_proxy(pair, word14, byte15, stack0, stack1);
  c166_test_check_u32(vector_id * 2 - 1, golden, reference);
  c166_test_check_u32(vector_id * 2, golden, actual);
}

#define RUN_PAIR_R12(id, pair, word14, byte15, stack0, stack1, golden) \
  run_pair_r12_vector(id, pair, word14, byte15, stack0, stack1, golden);

void main(void) {
  c166_test_begin(264, 0x16600108UL);
  ABI_PAIR_R12_VECTORS(RUN_PAIR_R12)
  c166_test_finish();
  simulator_stop();
}
