#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

extern volatile unsigned int *llvm_entry_proxy(
    unsigned int tag0, unsigned int tag1, volatile unsigned int *address,
    unsigned int value);
extern volatile unsigned int *tasking_far_reference(
    unsigned int tag0, unsigned int tag1, volatile unsigned int *address,
    unsigned int value);

volatile unsigned int reference_cells[4];
volatile unsigned int actual_cells[4];

static void run_far_vector(unsigned int vector_id, unsigned int tag0,
                           unsigned int tag1, unsigned int value,
                           unsigned long golden) {
  unsigned int index = vector_id - 1;
  volatile unsigned int *reference_pointer;
  volatile unsigned int *actual_pointer;

  reference_cells[index] = 0xa55aU;
  actual_cells[index] = 0x5aa5U;
  reference_pointer = tasking_far_reference(
      tag0, tag1, &reference_cells[index], value);
  actual_pointer = llvm_entry_proxy(tag0, tag1, &actual_cells[index], value);

  c166_test_check_u32(vector_id * 4 - 3, golden, reference_cells[index]);
  c166_test_check_u32(vector_id * 4 - 2, golden, actual_cells[index]);
  c166_test_check_u32(vector_id * 4 - 1, 1,
                      reference_pointer == &reference_cells[index]);
  c166_test_check_u32(vector_id * 4, 1,
                      actual_pointer == &actual_cells[index]);
}

#define RUN_FAR(id, tag0, tag1, value, golden) \
  run_far_vector(id, tag0, tag1, value, golden);

void main(void) {
  c166_test_begin(261, 0x16600105UL);
  ABI_FAR_POINTER_VECTORS(RUN_FAR)
  c166_test_finish();
  simulator_stop();
}

